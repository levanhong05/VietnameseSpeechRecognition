#include "dictionarycreator.h"

#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QApplication>

#include "workcase.h"

using namespace VietnameseSpeechRecognition;

DictionaryCreator::DictionaryCreator(QObject *parent) :
    QThread(parent)
{
    m_working = false;
    m_abort = false;
}

DictionaryCreator::~DictionaryCreator()
{
    mutex.lock();
    m_abort = true;
    mutex.unlock();

    wait();
}

void DictionaryCreator::execute(QString path)
{
    QMutexLocker locker(&mutex);

    _path = path;

    if (!isRunning()) {
        start(LowPriority);
    } else {
        m_working = true;
    }
}

void DictionaryCreator::abort()
{
    mutex.lock();
    if (isRunning()) {
        m_abort = true;
    }
    mutex.unlock();

    wait();
}

void DictionaryCreator::create()
{
    QFile file(_path);

    if (!file.open(QIODevice::ReadOnly)) {
        return;
    }

    QTextStream in(&file);

    QString line = "";

    QStringList words;

    while (!in.atEnd()) {
        line = in.readLine();

        if (line.startsWith("//")) {
            continue;
        }

        QStringList values = line.split(" ", QString::SkipEmptyParts);
        values.removeFirst();

        foreach (QString str, values) {
            if (!words.contains(str)) {
                words.append(str);
            }
        }
    }

    qSort(words.begin(), words.end());

    //Write grammar file
    QFile grammarFile(WorkCase::currentCase()->getWorkspace() + "/text/grammar.txt");

    grammarFile.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate);

    QTextStream out(&grammarFile);

    out << "$word = " << words.join(" | ") << ";" << endl << endl;
    out << "( SENT-START ( <$word>) SENT-END )" << endl;

    grammarFile.close();

    //Read Vietnamese phonemes
    QStringList phonemes;

    QFile phonemeFile(WorkCase::currentCase()->getWorkspace() + "/text/phoneme.txt");

    if (!phonemeFile.open(QIODevice::ReadOnly)) {
        return;
    }

    QTextStream inPhone(&phonemeFile);

    while (!inPhone.atEnd()) {
        line = inPhone.readLine();

        phonemes.append(line);
    }

    phonemeFile.close();

    words.append("SENT-START");
    words.append("SENT-END");
    words.append("silence");

    qSort(words.begin(), words.end());

    //Write dictionary file
    QFile dictFile(WorkCase::currentCase()->getWorkspace() + "/text/dict.dct");

    dictFile.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate);

    QTextStream outDic(&dictFile);

    foreach (QString word, words) {
        if (word == "SENT-START" || word == "SENT-END" || word == "silence") {
            outDic << word << "\t" << "[] sil" << endl;
        } else {
            outDic << word << "\t";

            QString phone = "";

            while (word != "") {
                if (word.length() == 1) {
                    outDic << phone << word << "\t";

                    word = "";
                } else {
                    phone += word.mid(0, 1);

                    word = word.mid(1);

                    if (phonemes.contains(phone, Qt::CaseInsensitive)) {
                        if (!phonemes.contains(phone + word.mid(0, 1), Qt::CaseInsensitive)) {
                            outDic << phone << "\t";

                            phone = "";
                        }
                    } else {
                        outDic << phone << "\t";

                        phone = "";
                    }
                }
            }

            outDic << "sp" << endl;
        }
    }

    dictFile.close();
}

void DictionaryCreator::run()
{
    mutex.lock();
    create();
    mutex.unlock();

    if (!m_working) {
        //emit ;
    }

    mutex.lock();
    m_working = false;
    mutex.unlock();

    exit();
}

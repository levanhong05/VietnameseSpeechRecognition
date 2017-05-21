#include "dictionarycreator.h"

#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QApplication>

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

        if (line.startsWith("#")) {
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

    QDir dir(QApplication::applicationDirPath() + "/text");

    if (!dir.exists()) {
        dir.mkpath(QApplication::applicationDirPath() + "/text");
    }

    QFile grammarFile(QApplication::applicationDirPath() + "/text/grammar.txt");

    grammarFile.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate);

    QTextStream out(&grammarFile);

    out << "#grammar.txt" << endl << endl;
    out << "$word = " << words.join(" | ") << endl << endl;
    out << "(<$word>)" << endl;

    QFile dictFile(QApplication::applicationDirPath() + "/text/dict.dct");

    dictFile.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate);

    QTextStream outDic(&dictFile);

    outDic << "#dict.dct" << endl << endl;

    QStringList phonemes;

    QFile phonemeFile(QApplication::applicationDirPath() + "/text/phoneme.txt");

    if (!phonemeFile.open(QIODevice::ReadOnly)) {
        return;
    }

    QTextStream inPhone(&phonemeFile);

    while (!inPhone.atEnd()) {
        line = inPhone.readLine();

        phonemes.append(line);
    }

    foreach (QString word, words) {
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

    outDic << "silence" << "\t" << "sil" << endl;
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

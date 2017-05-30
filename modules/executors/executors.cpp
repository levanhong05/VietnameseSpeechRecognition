#include <QtGui>
#include <QLabel>
#include <QLayout>
#include <QVariant>
#include <QHBoxLayout>
#include <QApplication>

#include <QDebug>

#include "executors.h"
#include "loglistmodel.h"

namespace VietnameseSpeechRecognition {

Executors executors;

#ifdef Q_OS_WIN
#include <windows.h>
QString Executors::shortPathName(const QString &file) // returns 8.3 file format from long path
{
    wchar_t *input = new wchar_t[file.size() + 1];
    file.toWCharArray(input);
    input[file.size()] = L'\0'; // terminate string

    long length = GetShortPathName(input, NULL, 0);
    wchar_t *output = new wchar_t[length];

    GetShortPathName(input, output, length);
    QString ret = QString::fromWCharArray(output, length - 1); // discard

    delete [] input;
    delete [] output;
    return ret;
}
#else
QString Executors::shortPathName(const QString &file)
{
    return file;
}
#endif

ExecutingJob ::ExecutingJob(QString title, QWidget *parent) :
    title(title)
{
    this->dock = new QDockWidget(title, parent);
    this->logWidget = new RunLogWidget(0, this->dock);
    this->exec = new Executor(WorkCase::currentCase()->getWorkspace());

    this->dock->setWidget(this->logWidget);
    this->logWidget->logView()->setModel(this->exec->logModel());
    this->exec->setLogWidget(this->logWidget->logView());
}

void Executors::onErrorLogging(QString log)
{
    console.logError(log);
}

void Executors::onResultLogging(QString result)
{
    console.logInfo(result);
}

bool Executors::removeDir(QString dirName)
{
    bool result = true;
    QDir dir(dirName);

    if (dir.exists(dirName)) {
        Q_FOREACH (QFileInfo info, dir.entryInfoList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden | QDir::AllDirs | QDir::Files, QDir::DirsFirst)) {
            if (info.isDir()) {
                result = removeDir(info.absoluteFilePath());
            } else if (info.isFile()) {
                result = QFile::remove(info.absoluteFilePath());
            }

            if (!result) {
                return result;
            }
        }

        result = dir.rmdir(dirName);
    }

    return result;
}

void Executors::scanDir(QString dirName, QString dst)
{
    QDir dir(dirName);

    dir.setNameFilters(QStringList("*.mfc"));
    dir.setFilter(QDir::Files | QDir::NoDotAndDotDot | QDir::NoSymLinks);

    QStringList fileList = dir.entryList();

    for (int i = 0; i < fileList.count(); i++) {
        QFile::copy(dirName + "/" + fileList[i], WorkCase::currentCase()->getWorkspace() + "/" + dst + "/" + fileList[i]);

        QFile::remove(dirName + "/" + fileList[i]);
    }

    dir.setFilter(QDir::AllDirs | QDir::NoDotAndDotDot | QDir::NoSymLinks);
    QStringList dirList = dir.entryList();

    for (int i = 0; i < dirList.size(); ++i) {
        QString newPath = QString("%1/%2").arg(dir.absolutePath()).arg(dirList.at(i));

        scanDir(newPath);
    }
}

void Executors::execWordNet(QString grammar, QString wordnet)
{
    ExecutingJob *job = new ExecutingJob(tr("WordNet"));
    this->_jobs.append(job);

    job->exec->setName("createWordNet");

    WaitingDialog *wait = new WaitingDialog(tr("Create WordNet..."));

    QIcon icon(":/speech/images/chat.png");
    wait->setWindowIcon(icon);

    connect(job->exec, SIGNAL(finished(int)), wait, SLOT(close()));
    connect(job->exec, SIGNAL(error()), wait, SLOT(close()));

    job->exec->setUseCustomErrorHandler(true);

    connect(job->exec, SIGNAL(customErrorHandler(QString)), this, SLOT(onErrorLogging(QString)));

    job->exec->start();

    wait->show();

#ifdef Q_OS_LINUX
    job->exec->directExecute("source /opt/htk341/etc/bashrc");
#else
    job->exec->directExecute("call " + shortPathName(QApplication::applicationDirPath()) + "\\HTK\\setvars.bat");
#endif
    job->exec->waitForFinished();

    job->exec->execute("HParse " + WorkCase::currentCase()->getWorkspace() + "/" + grammar +
                       " " + WorkCase::currentCase()->getWorkspace() + "/" + wordnet);

    job->exec->waitForFinished();

    if (job->exec->lastExitCode() != 0) {
        console.logError(tr("Errors occurred while create word network."));
    } else {
        console.logSuccess(tr("Create word network successfull."));
    }
}

void Executors::execMonophones(QString prompts, QString wlist,
                               QString mphones,
                               QString dstDict, QString srcDict,
                               QString mphones0, QString mphones1)
{
    ExecutingJob *job = new ExecutingJob(tr("Monophones"));
    this->_jobs.append(job);

    job->exec->setName("createMonophones");

    WaitingDialog *wait = new WaitingDialog(tr("Create Monophones..."));

    QIcon icon(":/speech/images/chat.png");
    wait->setWindowIcon(icon);

    connect(job->exec, SIGNAL(finished(int)), wait, SLOT(close()));
    connect(job->exec, SIGNAL(error()), wait, SLOT(close()));

    job->exec->setUseCustomErrorHandler(true);

    connect(job->exec, SIGNAL(customErrorHandler(QString)), this, SLOT(onErrorLogging(QString)));

    job->exec->start();

    wait->show();

#ifdef Q_OS_LINUX
    job->exec->directExecute("source /opt/htk341/etc/bashrc");
#else
    job->exec->directExecute("call " + shortPathName(QApplication::applicationDirPath()) + "\\HTK\\setvars.bat");
#endif
    job->exec->waitForFinished();

    job->exec->execute("perl " + QApplication::applicationDirPath() + "/perl/prompts2wlist.pl " +
                       WorkCase::currentCase()->getWorkspace() + "/" + prompts + " " +
                       WorkCase::currentCase()->getWorkspace() + "/" + wlist);

    job->exec->waitForFinished();

    job->exec->execute("HDMan -A -D -T 1 -m -w " +
                       WorkCase::currentCase()->getWorkspace() + "/" + wlist + " -n " +
                       WorkCase::currentCase()->getWorkspace() + "/" + mphones + " -i " +
                       WorkCase::currentCase()->getWorkspace() + "/" + dstDict + " " +
                       WorkCase::currentCase()->getWorkspace() + "/" + srcDict);

    job->exec->waitForFinished();

    job->exec->execute("perl " + QApplication::applicationDirPath() + "/perl/mkMonophones.pl " +
                       WorkCase::currentCase()->getWorkspace() + "/" + mphones + " " +
                       WorkCase::currentCase()->getWorkspace() + "/" + mphones0 + " " +
                       WorkCase::currentCase()->getWorkspace() + "/" + mphones1);

    job->exec->waitForFinished();

    if (job->exec->lastExitCode() != 0) {
        console.logError(tr("Errors occurred while create monophones."));
    } else {
        console.logSuccess(tr("Create monophones successfull."));
    }
}

void Executors::execTranscription(QString prompts, QString mlfwords, QString dict,
                                  QString mlfphone0, QString mkphones0,
                                  QString mlfphone1, QString mkphones1)
{
    ExecutingJob *job = new ExecutingJob(tr("Transcription"));
    this->_jobs.append(job);

    job->exec->setName("createTranscription");

    WaitingDialog *wait = new WaitingDialog(tr("Create Transcription..."));

    QIcon icon(":/speech/images/chat.png");
    wait->setWindowIcon(icon);

    connect(job->exec, SIGNAL(finished(int)), wait, SLOT(close()));
    connect(job->exec, SIGNAL(error()), wait, SLOT(close()));

    job->exec->setUseCustomErrorHandler(true);

    connect(job->exec, SIGNAL(customErrorHandler(QString)), this, SLOT(onErrorLogging(QString)));

    job->exec->start();

    wait->show();

#ifdef Q_OS_LINUX
    job->exec->directExecute("source /opt/htk341/etc/bashrc");
#else
    job->exec->directExecute("call " + shortPathName(QApplication::applicationDirPath()) + "\\HTK\\setvars.bat");
#endif
    job->exec->waitForFinished();

    job->exec->execute("perl " + QApplication::applicationDirPath() + "/perl/prompts2mlf.pl " +
                       WorkCase::currentCase()->getWorkspace() + "/" + mlfwords + " " +
                       WorkCase::currentCase()->getWorkspace() + "/" + prompts);

    job->exec->waitForFinished();

    job->exec->execute("HLEd -A -D -T 1 -l * -d " +
                       WorkCase::currentCase()->getWorkspace() + "/" + dict + " -i " +
                       WorkCase::currentCase()->getWorkspace() + "/" + mlfphone0 + " " +
                       WorkCase::currentCase()->getWorkspace() + "/" + mkphones0 + " " +
                       WorkCase::currentCase()->getWorkspace() + "/" + mlfwords);

    job->exec->waitForFinished();

    job->exec->execute("HLEd -A -D -T 1 -l * -d " +
                       WorkCase::currentCase()->getWorkspace() + "/" + dict + " -i " +
                       WorkCase::currentCase()->getWorkspace() + "/" + mlfphone1 + " " +
                       WorkCase::currentCase()->getWorkspace() + "/" + mkphones1 + " " +
                       WorkCase::currentCase()->getWorkspace() + "/" + mlfwords);

    job->exec->waitForFinished();

    if (job->exec->lastExitCode() != 0) {
        console.logError(tr("Errors occurred while create transcription."));
    } else {
        console.logSuccess(tr("Create transcription successfull."));
    }
}

void Executors::execMFCC(QString path, QString hcopyCFG)
{
    QDir dir(path);

    dir.setFilter(QDir::Dirs | QDir::NoDotAndDotDot);

    QFileInfoList listFiles = dir.entryInfoList();

    ExecutingJob *job = new ExecutingJob(tr("MFCC"));
    this->_jobs.append(job);

    job->exec->setName("createMFCC");

    WaitingDialog *wait = new WaitingDialog(tr("Create MFCC..."));

    QIcon icon(":/speech/images/chat.png");
    wait->setWindowIcon(icon);

    connect(job->exec, SIGNAL(error()), wait, SLOT(close()));

    job->exec->setUseCustomErrorHandler(true);

    connect(job->exec, SIGNAL(customErrorHandler(QString)), this, SLOT(onErrorLogging(QString)));

    job->exec->start();

    wait->show();

#ifdef Q_OS_LINUX
    job->exec->directExecute("source /opt/htk341/etc/bashrc");
#else
    job->exec->directExecute("call " + shortPathName(QApplication::applicationDirPath()) + "\\HTK\\setvars.bat");
#endif
    job->exec->waitForFinished();

    foreach (QFileInfo info, listFiles) {
        if (info.isDir()) {
            QString path = info.absoluteFilePath();

            job->exec->execute("perl " + QApplication::applicationDirPath() + "/perl/listwavmfc.pl " +
                               path + " " + path + "/listwavmfc");

            job->exec->waitForFinished();

            job->exec->execute("HCopy -A -D -T 1  -C " +
                               QApplication::applicationDirPath() + "/" + hcopyCFG + " -S " +
                               path + "/listwavmfc");

            job->exec->waitForFinished();

            scanDir(path);

            QFile::remove(path + "/listwavmfc");
        }
    }

    if (job->exec->lastExitCode() != 0) {
        console.logError(tr("Errors occurred while create MFCC."));
    } else {
        console.logSuccess(tr("Create MFCC successfull."));
    }

    wait->close();
}

void Executors::execProto(QString wave, QString train, QString hcomvCFG)
{
    ExecutingJob *job = new ExecutingJob(tr("Proto"));
    this->_jobs.append(job);

    job->exec->setName("createProto");

    WaitingDialog *wait = new WaitingDialog(tr("Create Proto..."));

    QIcon icon(":/speech/images/chat.png");
    wait->setWindowIcon(icon);

    connect(job->exec, SIGNAL(error()), wait, SLOT(close()));

    job->exec->setUseCustomErrorHandler(true);

    connect(job->exec, SIGNAL(customErrorHandler(QString)), this, SLOT(onErrorLogging(QString)));

    job->exec->start();

    wait->show();

#ifdef Q_OS_LINUX
    job->exec->directExecute("source /opt/htk341/etc/bashrc");
#else
    job->exec->directExecute("call " + shortPathName(QApplication::applicationDirPath()) + "\\HTK\\setvars.bat");
#endif
    job->exec->waitForFinished();

    job->exec->execute("perl " + QApplication::applicationDirPath() + "/perl/mkTrainFile.pl " +
                       WorkCase::currentCase()->getWorkspace() + "/" + wave + " " +
                       WorkCase::currentCase()->getWorkspace() + "/" + train);

    job->exec->waitForFinished();

    job->exec->execute("HCompV -A -D -T 1 -C " +
                       QApplication::applicationDirPath() + "/" + hcomvCFG + " -f 0.01 -m -S " +
                       WorkCase::currentCase()->getWorkspace() + "/" + train + " -M " +
                       WorkCase::currentCase()->getWorkspace() + "/hmm0 " +
                       WorkCase::currentCase()->getWorkspace() + "/hmm/proto");

    job->exec->waitForFinished();

    job->exec->execute("perl " + QApplication::applicationDirPath() + "/perl/mkMacrosFile.pl " +
                       WorkCase::currentCase()->getWorkspace() + "/hmm0/vFloors" + " " +
                       WorkCase::currentCase()->getWorkspace() + "/hmm0/macros");

    job->exec->waitForFinished();

    job->exec->execute("perl " + QApplication::applicationDirPath() + "/perl/mkHmmdefsFile.pl " +
                       WorkCase::currentCase()->getWorkspace() + "/hmm0/proto" + " " +
                       WorkCase::currentCase()->getWorkspace() + "/phones/monophones0" + " " +
                       WorkCase::currentCase()->getWorkspace() + "/hmm0/hmmdefs");

    job->exec->waitForFinished();

    if (job->exec->lastExitCode() != 0) {
        console.logError(tr("Errors occurred while create proto."));
    } else {
        console.logSuccess(tr("Create proto successfull."));
    }

    wait->close();
}

void Executors::execHRest(QString train, QString hrestCFG)
{
    ExecutingJob *job = new ExecutingJob(tr("HRest"));
    this->_jobs.append(job);

    job->exec->setName("runHRest");

    WaitingDialog *wait = new WaitingDialog(tr("Run HRest..."));

    QIcon icon(":/speech/images/chat.png");
    wait->setWindowIcon(icon);

    connect(job->exec, SIGNAL(error()), wait, SLOT(close()));

    job->exec->setUseCustomErrorHandler(true);

    connect(job->exec, SIGNAL(customErrorHandler(QString)), this, SLOT(onErrorLogging(QString)));

    job->exec->start();

    wait->show();

#ifdef Q_OS_LINUX
    job->exec->directExecute("source /opt/htk341/etc/bashrc");
#else
    job->exec->directExecute("call " + shortPathName(QApplication::applicationDirPath()) + "\\HTK\\setvars.bat");
#endif
    job->exec->waitForFinished();

    for (int i = 1; i <= 3; i++) {
        job->exec->execute("HERest -A -D -T 1 -C " +
                           QApplication::applicationDirPath() + "/" + hrestCFG + " -I " +
                           WorkCase::currentCase()->getWorkspace() + "/mlf/phones0.mlf" + " -t 250.0 150.0 1000.0 -S " +
                           WorkCase::currentCase()->getWorkspace() + "/" + train + " -H " +
                           WorkCase::currentCase()->getWorkspace() + "/hmm" + QString::number(i - 1) + "/macros" + " -H " +
                           WorkCase::currentCase()->getWorkspace() + "/hmm" + QString::number(i - 1) + "/hmmdefs" + " -M " +
                           WorkCase::currentCase()->getWorkspace() + "/hmm" + QString::number(i) + " " +
                           WorkCase::currentCase()->getWorkspace() + "/phones/monophones0");

        job->exec->waitForFinished();
    }

    if (job->exec->lastExitCode() != 0) {
        console.logError(tr("Errors occurred while create HRest training."));
    } else {
        console.logSuccess(tr("Create HRest training successfull."));
    }

    wait->close();
}

void Executors::execHHEd(QString monophones1, QString silinstr, QString train, QString hrestCFG)
{
    ExecutingJob *job = new ExecutingJob(tr("HHEd"));
    this->_jobs.append(job);

    job->exec->setName("runHHEd");

    WaitingDialog *wait = new WaitingDialog(tr("Run HHEd..."));

    QIcon icon(":/speech/images/chat.png");
    wait->setWindowIcon(icon);

    connect(job->exec, SIGNAL(error()), wait, SLOT(close()));

    job->exec->setUseCustomErrorHandler(true);

    connect(job->exec, SIGNAL(customErrorHandler(QString)), this, SLOT(onErrorLogging(QString)));

    job->exec->start();

    wait->show();

#ifdef Q_OS_LINUX
    job->exec->directExecute("source /opt/htk341/etc/bashrc");
#else
    job->exec->directExecute("call " + shortPathName(QApplication::applicationDirPath()) + "\\HTK\\setvars.bat");
#endif
    job->exec->waitForFinished();

    job->exec->execute("perl " + QApplication::applicationDirPath() + "/perl/makesp.pl " +
                       WorkCase::currentCase()->getWorkspace() + "/hmm3/hmmdefs" + " " +
                       WorkCase::currentCase()->getWorkspace() + "/hmm4/hmmdefs" + " " +
                       WorkCase::currentCase()->getWorkspace() + "/hmm3/macros" + " " +
                       WorkCase::currentCase()->getWorkspace() + "/hmm4/macros");

    job->exec->waitForFinished();

    job->exec->execute("HHEd -A -D -T 1 -H " +
                       WorkCase::currentCase()->getWorkspace() + "/hmm4/macros" + " -H " +
                       WorkCase::currentCase()->getWorkspace() + "/hmm4/hmmdefs" + " -M " +
                       WorkCase::currentCase()->getWorkspace() + "/hmm5 " +
                       WorkCase::currentCase()->getWorkspace() + "/" + silinstr + " " +
                       WorkCase::currentCase()->getWorkspace() + "/" + monophones1);

    job->exec->waitForFinished();

    for (int i = 6; i <= 7; i++) {
        job->exec->execute("HERest -A -D -T 1 -C " +
                           QApplication::applicationDirPath() + "/" + hrestCFG + " -I " +
                           WorkCase::currentCase()->getWorkspace() + "/mlf/phones1.mlf" + " -t 250.0 150.0 1000.0 -S " +
                           WorkCase::currentCase()->getWorkspace() + "/" + train + " -H " +
                           WorkCase::currentCase()->getWorkspace() + "/hmm" + QString::number(i - 1) + "/macros" + " -H " +
                           WorkCase::currentCase()->getWorkspace() + "/hmm" + QString::number(i - 1) + "/hmmdefs" + " -M " +
                           WorkCase::currentCase()->getWorkspace() + "/hmm" + QString::number(i) + " " +
                           WorkCase::currentCase()->getWorkspace() + "/phones/monophones1");

        job->exec->waitForFinished();
    }

    if (job->exec->lastExitCode() != 0) {
        console.logError(tr("Errors occurred while create fixing silence."));
    } else {
        console.logSuccess(tr("Fixing silence successfull."));
    }

    wait->close();
}

void Executors::execHVite(QString monophones1, QString dict, QString train, QString hrestCFG, QString hviteCFG)
{
    ExecutingJob *job = new ExecutingJob(tr("HVite"));
    this->_jobs.append(job);

    job->exec->setName("runHVite");

    WaitingDialog *wait = new WaitingDialog(tr("Run HVite..."));

    QIcon icon(":/speech/images/chat.png");
    wait->setWindowIcon(icon);

    connect(job->exec, SIGNAL(error()), wait, SLOT(close()));

    job->exec->setUseCustomErrorHandler(true);

    connect(job->exec, SIGNAL(customErrorHandler(QString)), this, SLOT(onErrorLogging(QString)));

    job->exec->start();

    wait->show();

#ifdef Q_OS_LINUX
    job->exec->directExecute("source /opt/htk341/etc/bashrc");
#else
    job->exec->directExecute("call " + shortPathName(QApplication::applicationDirPath()) + "\\HTK\\setvars.bat");
#endif
    job->exec->waitForFinished();

    job->exec->execute("HVite -A -D -T 1 -l * -o SWT -b silence -C " +
                       QApplication::applicationDirPath() + "/" + hviteCFG + " -H " +
                       WorkCase::currentCase()->getWorkspace() + "/hmm7/macros -H " +
                       WorkCase::currentCase()->getWorkspace() + "/hmm7/hmmdefs -i " +
                       WorkCase::currentCase()->getWorkspace() + "/mlf/aligned.mlf -m -t 250.0 150.0 1000.0 -y lab -a -I " +
                       WorkCase::currentCase()->getWorkspace() + "/mlf/words.mlf -S " +
                       WorkCase::currentCase()->getWorkspace() + "/" + train + " " +
                       WorkCase::currentCase()->getWorkspace() + "/" + dict + " " +
                       WorkCase::currentCase()->getWorkspace() + "/" + monophones1);

    job->exec->waitForFinished();

    wait->setTitle(tr("Running HERest..."));

    for (int i = 8; i <= 9; i++) {
        job->exec->execute("HERest -A -D -T 1 -C " +
                           QApplication::applicationDirPath() + "/" + hrestCFG + " -I " +
                           WorkCase::currentCase()->getWorkspace() + "/mlf/aligned.mlf -t 250.0 150.0 1000.0 -s stats -S " +
                           WorkCase::currentCase()->getWorkspace() + "/" + train + " -H " +
                           WorkCase::currentCase()->getWorkspace() + "/hmm" + QString::number(i - 1) + "/macros" + " -H " +
                           WorkCase::currentCase()->getWorkspace() + "/hmm" + QString::number(i - 1) + "/hmmdefs" + " -M " +
                           WorkCase::currentCase()->getWorkspace() + "/hmm" + QString::number(i) + " " +
                           WorkCase::currentCase()->getWorkspace() + "/" + monophones1);

        job->exec->waitForFinished();
    }

    if (job->exec->lastExitCode() != 0) {
        console.logError(tr("Errors occurred while create optimize data."));
    } else {
        console.logSuccess(tr("Optimize data successfull."));
    }

    wait->close();
}

void Executors::execTriphones(QString train, QString triphones1, QString hrestCFG)
{
    ExecutingJob *job = new ExecutingJob(tr("Triphones"));
    this->_jobs.append(job);

    job->exec->setName("createTriphones");

    WaitingDialog *wait = new WaitingDialog(tr("Create Triphones..."));

    QIcon icon(":/speech/images/chat.png");
    wait->setWindowIcon(icon);

    connect(job->exec, SIGNAL(error()), wait, SLOT(close()));

    job->exec->setUseCustomErrorHandler(true);

    connect(job->exec, SIGNAL(customErrorHandler(QString)), this, SLOT(onErrorLogging(QString)));

    job->exec->start();

    wait->show();

#ifdef Q_OS_LINUX
    job->exec->directExecute("source /opt/htk341/etc/bashrc");
#else
    job->exec->directExecute("call " + shortPathName(QApplication::applicationDirPath()) + "\\HTK\\setvars.bat");
#endif
    job->exec->waitForFinished();

    job->exec->execute("HLEd -A -D -T 1 -n " +
                       WorkCase::currentCase()->getWorkspace() + "/phones/triphones1 -l * -i " +
                       WorkCase::currentCase()->getWorkspace() + "/mlf/wintri.mlf " +
                       WorkCase::currentCase()->getWorkspace() + "/instruction/mktri.led " +
                       WorkCase::currentCase()->getWorkspace() + "/mlf/aligned.mlf");

    job->exec->waitForFinished();

    job->exec->execute("perl " + QApplication::applicationDirPath() + "/perl/mkTriHed.pl " +
                       WorkCase::currentCase()->getWorkspace() + "/phones/monophones1 " +
                       WorkCase::currentCase()->getWorkspace() + "/phones/triphones1 " +
                       WorkCase::currentCase()->getWorkspace() + "/instruction/mktri.hed");

    job->exec->waitForFinished();

    job->exec->execute("HHEd -A -D -T 1 -H " +
                       WorkCase::currentCase()->getWorkspace() + "/hmm9/macros -H " +
                       WorkCase::currentCase()->getWorkspace() + "/hmm9/hmmdefs -M " +
                       WorkCase::currentCase()->getWorkspace() + "/hmm10 " +
                       WorkCase::currentCase()->getWorkspace() + "/instruction/mktri.hed " +
                       WorkCase::currentCase()->getWorkspace() + "/phones/monophones1");;

    job->exec->waitForFinished();

    for (int i = 11; i <= 12; i++) {
        job->exec->execute("HERest -A -D -T 1 -C " +
                           QApplication::applicationDirPath() + "/" + hrestCFG + " -I " +
                           WorkCase::currentCase()->getWorkspace() + "/mlf/wintri.mlf -t 250.0 150.0 3000.0 -s stats -S " +
                           WorkCase::currentCase()->getWorkspace() + "/" + train + " -H " +
                           WorkCase::currentCase()->getWorkspace() + "/hmm" + QString::number(i - 1) + "/macros" + " -H " +
                           WorkCase::currentCase()->getWorkspace() + "/hmm" + QString::number(i - 1) + "/hmmdefs" + " -M " +
                           WorkCase::currentCase()->getWorkspace() + "/hmm" + QString::number(i) + " " +
                           WorkCase::currentCase()->getWorkspace() + "/" + triphones1);

        job->exec->waitForFinished();
    }

    if (job->exec->lastExitCode() != 0) {
        console.logError(tr("Errors occurred while create triphones."));
    } else {
        console.logSuccess(tr("Create triphones successfull."));
    }

    wait->close();
}

void Executors::execTiedTriphone()
{
    ExecutingJob *job = new ExecutingJob(tr("TiedTriphones"));
    this->_jobs.append(job);

    job->exec->setName("createTiedTriphones");

    WaitingDialog *wait = new WaitingDialog(tr("Preparing Tied Triphones..."));

    QIcon icon(":/speech/images/chat.png");
    wait->setWindowIcon(icon);

    connect(job->exec, SIGNAL(error()), wait, SLOT(close()));

    job->exec->start();

    wait->show();

#ifdef Q_OS_LINUX
    job->exec->directExecute("source /opt/htk341/etc/bashrc");
#else
    job->exec->directExecute("call " + shortPathName(QApplication::applicationDirPath()) + "\\HTK\\setvars.bat");
#endif
    job->exec->waitForFinished();

    job->exec->execute("HDMan -A -D -T 1 -b sp -n " +
                       WorkCase::currentCase()->getWorkspace() + "/fulllist -g " +
                       WorkCase::currentCase()->getWorkspace() + "/instruction/maketriphones.ded " +
                       WorkCase::currentCase()->getWorkspace() + "/text/dict-tri " +
                       WorkCase::currentCase()->getWorkspace() + "/text/dict.dct");

    job->exec->waitForFinished();

    job->exec->execute("type " +
                       QString(WorkCase::currentCase()->getWorkspace() + "/fulllist ").replace("/", "\\") +
                       QString(WorkCase::currentCase()->getWorkspace() + "/phones/triphones1 > ").replace("/", "\\") +
                       QString(WorkCase::currentCase()->getWorkspace() + "/fulllist1").replace("/", "\\"));

    job->exec->waitForFinished();

    job->exec->execute("perl " +
                       QApplication::applicationDirPath() + "/perl/fixfulllist.pl " +
                       WorkCase::currentCase()->getWorkspace() + "/fulllist1 " +
                       WorkCase::currentCase()->getWorkspace() + "/fulllist");

    job->exec->waitForFinished();

    job->exec->execute("perl " +
                       QApplication::applicationDirPath() + "/perl/mkclscript.prl TB 350 " +
                       WorkCase::currentCase()->getWorkspace() + "/phones/monophones0 " +
                       WorkCase::currentCase()->getWorkspace() + "/instruction/tree2.hed");

    job->exec->waitForFinished();

    job->exec->execute("type " +
                       QString(WorkCase::currentCase()->getWorkspace() + "/instruction/tree1.hed ").replace("/", "\\") +
                       QString(WorkCase::currentCase()->getWorkspace() + "/instruction/tree2.hed > ").replace("/", "\\") +
                       QString(WorkCase::currentCase()->getWorkspace() + "/instruction/tree.hed").replace("/", "\\"));

    job->exec->waitForFinished();

    wait->close();
}

void Executors::execTiedTriphones(QString wintri, QString train, QString triphones1, QString hrestCFG)
{
    ExecutingJob *job = new ExecutingJob(tr("TiedTriphones"));
    this->_jobs.append(job);

    job->exec->setName("createTiedTriphones");

    WaitingDialog *wait = new WaitingDialog(tr("Create Tied Triphones..."));

    QIcon icon(":/speech/images/chat.png");
    wait->setWindowIcon(icon);

    connect(job->exec, SIGNAL(error()), wait, SLOT(close()));

    job->exec->setUseCustomErrorHandler(true);

    connect(job->exec, SIGNAL(customErrorHandler(QString)), this, SLOT(onErrorLogging(QString)));

    job->exec->start();

    wait->show();

#ifdef Q_OS_LINUX
    job->exec->directExecute("source /opt/htk341/etc/bashrc");
#else
    job->exec->directExecute("call " + shortPathName(QApplication::applicationDirPath()) + "\\HTK\\setvars.bat");
#endif
    job->exec->waitForFinished();

//    job->exec->execute("perl " + QApplication::applicationDirPath() + "/perl/mkFullList.pl " +
//                       WorkCase::currentCase()->getWorkspace() + "/phones/monophones0 " +
//                       WorkCase::currentCase()->getWorkspace() + "/fulllist");

//    job->exec->waitForFinished();

//    job->exec->execute("perl " + QApplication::applicationDirPath() + "/perl/mkTree.pl 40 " +
//                       WorkCase::currentCase()->getWorkspace() + "/phones/monophones0 " +
//                       WorkCase::currentCase()->getWorkspace() + "/instruction/tree.hed");

//    job->exec->waitForFinished();

    job->exec->execute("HHEd -A -D -T 1 -H " +
                       WorkCase::currentCase()->getWorkspace() + "/hmm12/macros -H " +
                       WorkCase::currentCase()->getWorkspace() + "/hmm12/hmmdefs -M " +
                       WorkCase::currentCase()->getWorkspace() + "/hmm13 " +
                       WorkCase::currentCase()->getWorkspace() + "/instruction/tree.hed " +
                       WorkCase::currentCase()->getWorkspace() + "/" + triphones1);

    job->exec->waitForFinished();

    for (int i = 14; i <= 15; i++) {
        job->exec->execute("HERest -A -D -T 1 -T 1 -C " +
                           QApplication::applicationDirPath() + "/" + hrestCFG + " -I " +
                           WorkCase::currentCase()->getWorkspace() + "/" + wintri + " -t 250.0 150.0 3000.0 -s stats -S " +
                           WorkCase::currentCase()->getWorkspace() + "/" + train + " -H " +
                           WorkCase::currentCase()->getWorkspace() + "/hmm" + QString::number(i - 1) + "/macros" + " -H " +
                           WorkCase::currentCase()->getWorkspace() + "/hmm" + QString::number(i - 1) + "/hmmdefs" + " -M " +
                           WorkCase::currentCase()->getWorkspace() + "/hmm" + QString::number(i) + " " +
                           WorkCase::currentCase()->getWorkspace() + "/tiedlist");

        job->exec->waitForFinished();
    }

    if (job->exec->lastExitCode() != 0) {
        console.logError(tr("Errors occurred while create tied state triphones."));
    } else {
        console.logSuccess(tr("Create tied state triphones successfull."));
    }

    wait->close();
}

void Executors::execTest(QString hviteCFG, QString test, QString recout, QString wordnet, QString dict)
{
    ExecutingJob *job = new ExecutingJob(tr("Testing"));
    this->_jobs.append(job);

    job->exec->setName("testing");

    WaitingDialog *wait = new WaitingDialog(tr("Testing..."));

    QIcon icon(":/speech/images/chat.png");
    wait->setWindowIcon(icon);

    connect(job->exec, SIGNAL(error()), wait, SLOT(close()));

    job->exec->setUseCustomErrorHandler(true);

    connect(job->exec, SIGNAL(customErrorHandler(QString)), this, SLOT(onErrorLogging(QString)));

    job->exec->start();

    wait->show();

#ifdef Q_OS_LINUX
    job->exec->directExecute("source /opt/htk341/etc/bashrc");
#else
    job->exec->directExecute("call " + shortPathName(QApplication::applicationDirPath()) + "\\HTK\\setvars.bat");
#endif
    job->exec->waitForFinished();

    job->exec->execute("HVite -A -D -T 1 -C " +
                       QApplication::applicationDirPath() + "/" + hviteCFG + " -H " +
                       WorkCase::currentCase()->getWorkspace() + "/hmm15/macros -H " +
                       WorkCase::currentCase()->getWorkspace() + "/hmm15/hmmdefs -S " +
                       WorkCase::currentCase()->getWorkspace() + "/" + test + " -i " +
                       WorkCase::currentCase()->getWorkspace() + "/" + recout + " -w " +
                       WorkCase::currentCase()->getWorkspace() + "/" + wordnet + " " +
                       WorkCase::currentCase()->getWorkspace() + "/" + dict + " " +
                       WorkCase::currentCase()->getWorkspace() + "/tiedlist");

    job->exec->waitForFinished();

    if (job->exec->lastExitCode() != 0) {
        console.logError(tr("Errors occurred while testing."));
    } else {
        console.logSuccess(tr("Testing successfull."));
    }

    wait->close();
}

void Executors::execPreparingDataTest(QString waveTestPath, QString hcopyCFG, QString test)
{
    ExecutingJob *job = new ExecutingJob(tr("Testing"));
    this->_jobs.append(job);

    job->exec->setName("preparingDataTesting");

    WaitingDialog *wait = new WaitingDialog(tr("Preparing Data Testing..."));

    QIcon icon(":/speech/images/chat.png");
    wait->setWindowIcon(icon);

    connect(job->exec, SIGNAL(error()), wait, SLOT(close()));

    job->exec->setUseCustomErrorHandler(true);

    connect(job->exec, SIGNAL(customErrorHandler(QString)), this, SLOT(onErrorLogging(QString)));

    job->exec->start();

    wait->show();

#ifdef Q_OS_LINUX
    job->exec->directExecute("source /opt/htk341/etc/bashrc");
#else
    job->exec->directExecute("call " + shortPathName(QApplication::applicationDirPath()) + "\\HTK\\setvars.bat");
#endif
    job->exec->waitForFinished();

    QDir dir(waveTestPath);

    dir.setFilter(QDir::Dirs | QDir::NoDotAndDotDot);

    QFileInfoList listFiles = dir.entryInfoList();

    if (!dir.exists(WorkCase::currentCase()->getWorkspace() + "/test/wave")) {
        dir.mkpath(WorkCase::currentCase()->getWorkspace() + "/test/wave");
    }

    foreach (QFileInfo info, listFiles) {
        if (info.isDir()) {
            QString path = info.absoluteFilePath();

            job->exec->execute("perl " + QApplication::applicationDirPath() + "/perl/listwavmfc.pl " +
                               path + " " + path + "/listwavmfc");

            job->exec->waitForFinished();

            job->exec->execute("HCopy -A -D -T 1  -C " +
                               QApplication::applicationDirPath() + "/" + hcopyCFG + " -S " +
                               path + "/listwavmfc");

            job->exec->waitForFinished();

            scanDir(path, "test/wave");

            QFile::remove(path + "/listwavmfc");
        }
    }

    job->exec->execute("perl " + QApplication::applicationDirPath() + "/perl/mkTrainFile.pl " +
                       WorkCase::currentCase()->getWorkspace() + "/test/wave " +
                       WorkCase::currentCase()->getWorkspace() + "/" + test);

    job->exec->waitForFinished();

    if (job->exec->lastExitCode() != 0) {
        console.logError(tr("Errors occurred while preparing data testing."));
    } else {
        console.logSuccess(tr("Preparing data testing successfull."));
    }

    wait->close();
}

void Executors::execShowResult(QString recout)
{
    ExecutingJob *job = new ExecutingJob(tr("Testing"));
    this->_jobs.append(job);

    job->exec->setName("testing");

    WaitingDialog *wait = new WaitingDialog(tr("Testing..."));

    QIcon icon(":/speech/images/chat.png");
    wait->setWindowIcon(icon);

    connect(job->exec, SIGNAL(error()), wait, SLOT(close()));

    job->exec->setUseCustomErrorHandler(true);

    connect(job->exec, SIGNAL(customErrorHandler(QString)), this, SLOT(onErrorLogging(QString)));
    connect(job->exec, SIGNAL(customLogHandler(QString)), this, SLOT(onResultLogging(QString)));

    job->exec->start();

    wait->show();

#ifdef Q_OS_LINUX
    job->exec->directExecute("source /opt/htk341/etc/bashrc");
#else
    job->exec->directExecute("call " + shortPathName(QApplication::applicationDirPath()) + "\\HTK\\setvars.bat");
#endif
    job->exec->waitForFinished();

    job->exec->execute("HResults -I " +
                       WorkCase::currentCase()->getWorkspace() + "/mlf/words.mlf " +
                       WorkCase::currentCase()->getWorkspace() + "/tiedlist " +
                       WorkCase::currentCase()->getWorkspace() + "/" + recout + " " +
                       "> test/result");

    job->exec->waitForFinished();

    if (job->exec->lastExitCode() != 0) {
        console.logError(tr("Errors occurred while show result."));
    } else {
        console.logSuccess(tr("Show result successfull."));
    }

    wait->close();
}

}

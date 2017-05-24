#include <QtGui>
#include <QLabel>
#include <QLayout>
#include <QVariant>
#include <QHBoxLayout>
#include <QApplication>

#include <QDebug>

#include "executors.h"
#include "loglistmodel.h"

using namespace VietnameseSpeechRecognition;

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

void Executors::scanDir(QString dirName)
{
    QDir dir(dirName);

    dir.setNameFilters(QStringList("*.mfc"));
    dir.setFilter(QDir::Files | QDir::NoDotAndDotDot | QDir::NoSymLinks);

    QStringList fileList = dir.entryList();

    for (int i = 0; i < fileList.count(); i++) {
        QFile::copy(dirName + "/" + fileList[i], WorkCase::currentCase()->getWorkspace() + "/wave" + "/" + fileList[i]);

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
    wait->setUsingPerCent(true);

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
    wait->setUsingPerCent(true);

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

    job->exec->execute("HDMan -m -w " +
                       WorkCase::currentCase()->getWorkspace() + "/" + wlist + " -n " +
                       WorkCase::currentCase()->getWorkspace() + "/" + mphones + " -l dlog " +
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
    wait->setUsingPerCent(true);

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

    job->exec->execute("HLEd -l * -d " +
                       WorkCase::currentCase()->getWorkspace() + "/" + dict + " -i " +
                       WorkCase::currentCase()->getWorkspace() + "/" + mlfphone0 + " " +
                       WorkCase::currentCase()->getWorkspace() + "/" + mkphones0 + " " +
                       WorkCase::currentCase()->getWorkspace() + "/" + mlfwords);

    job->exec->waitForFinished();

    job->exec->execute("HLEd -l * -d " +
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
    wait->setUsingPerCent(true);

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

//            QDir dir(path);

//            dir.setNameFilters(QStringList("*.wav"));
//            dir.setFilter(QDir::Files | QDir::NoDotAndDotDot | QDir::NoSymLinks);

//            QStringList fileList = dir.entryList();

//            QFile listMFC(path + "/listwavmfc");

//            listMFC.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate);

//            QTextStream in(&listMFC);

//            for (int i = 0; i < fileList.count(); i++) {
//                in << path << "/" << fileList[i] << " " << path << "/" << fileList[i].left(fileList[i].length() - 3) << "mfc" << endl;
//            }

//            listMFC.close();

            job->exec->execute("HCopy -T 1 -C " +
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
    wait->setUsingPerCent(true);

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

    job->exec->execute("HCompV -C " +
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
    wait->setUsingPerCent(true);

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

    for (int i = 1; i < 4; i++) {
        job->exec->execute("HERest -C " +
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
        console.logError(tr("Errors occurred while create HRest."));
    } else {
        console.logSuccess(tr("Create HRest successfull."));
    }

    wait->close();
}

void Executors::execHHEd(QString monophones1, QString silinstr, QString train, QString hrestCFG)
{
    ExecutingJob *job = new ExecutingJob(tr("HHEd"));
    this->_jobs.append(job);

    job->exec->setName("runHHEd");

    WaitingDialog *wait = new WaitingDialog(tr("Run HHEd..."));
    wait->setUsingPerCent(true);

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

    job->exec->execute("HHEd -H " +
                       WorkCase::currentCase()->getWorkspace() + "/hmm4/macros" + " -H " +
                       WorkCase::currentCase()->getWorkspace() + "/hmm4/hmmdefs" + " -M " +
                       WorkCase::currentCase()->getWorkspace() + "/hmm5 " +
                       WorkCase::currentCase()->getWorkspace() + "/" + silinstr + " " +
                       WorkCase::currentCase()->getWorkspace() + "/" + monophones1);

    job->exec->waitForFinished();

    for (int i = 6; i <= 7; i++) {
        job->exec->execute("HERest -C " +
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
        console.logError(tr("Errors occurred while create HHEd."));
    } else {
        console.logSuccess(tr("Create HHEd successfull."));
    }

    wait->close();
}

void Executors::execHVite(QString monophones1, QString dict, QString train, QString hrestCFG)
{
    ExecutingJob *job = new ExecutingJob(tr("HVite"));
    this->_jobs.append(job);

    job->exec->setName("runHVite");

    WaitingDialog *wait = new WaitingDialog(tr("Run HVite..."));
    wait->setUsingPerCent(true);

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

    job->exec->execute("HVite -l * -o SWT -b silence -a -H " +
                       WorkCase::currentCase()->getWorkspace() + "/hmm7/macros -H " +
                       WorkCase::currentCase()->getWorkspace() + "/hmm7/hmmdefs -i " +
                       WorkCase::currentCase()->getWorkspace() + "/mlf/aligned.mlf -m -t 250.0 -y lab -I " +
                       WorkCase::currentCase()->getWorkspace() + "/mlf/words.mlf -S " +
                       WorkCase::currentCase()->getWorkspace() + "/" + train + " " +
                       WorkCase::currentCase()->getWorkspace() + "/" + dict + " " +
                       WorkCase::currentCase()->getWorkspace() + "/" + monophones1);

    job->exec->waitForFinished();

    for (int i = 8; i <= 9; i++) {
        job->exec->execute("HERest -B -C " +
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
        console.logError(tr("Errors occurred while create HVite."));
    } else {
        console.logSuccess(tr("Create HVite successfull."));
    }

    wait->close();
}

void Executors::execTriphones(QString train, QString triphones1)
{
    ExecutingJob *job = new ExecutingJob(tr("Triphones"));
    this->_jobs.append(job);

    job->exec->setName("createTriphones");

    WaitingDialog *wait = new WaitingDialog(tr("Create Triphones..."));
    wait->setUsingPerCent(true);

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

    job->exec->execute("HLEd -n " +
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

    job->exec->execute("HHEd -B -H " +
                       WorkCase::currentCase()->getWorkspace() + "/hmm9/macros -H " +
                       WorkCase::currentCase()->getWorkspace() + "/hmm9/hmmdefs -M " +
                       WorkCase::currentCase()->getWorkspace() + "/hmm10 " +
                       WorkCase::currentCase()->getWorkspace() + "/instruction/mktri.hed " +
                       WorkCase::currentCase()->getWorkspace() + "/phones/monophones1");;

    job->exec->waitForFinished();

    for (int i = 11; i <= 12; i++) {
        job->exec->execute("HERest -B -C " +
                           QApplication::applicationDirPath() + "/config/HERest.cfg -I " +
                           WorkCase::currentCase()->getWorkspace() + "/mlf/wintri.mlf -t 250.0 150.0 1000.0 -s stats -S " +
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

void Executors::execTiedTriphones(QString wintri, QString train, QString triphones1)
{
    ExecutingJob *job = new ExecutingJob(tr("TiedTriphones"));
    this->_jobs.append(job);

    job->exec->setName("createTiedTriphones");

    WaitingDialog *wait = new WaitingDialog(tr("Create Tied Triphones..."));
    wait->setUsingPerCent(true);

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

    job->exec->execute("perl " + QApplication::applicationDirPath() + "/perl/mkFullList.pl " +
                       WorkCase::currentCase()->getWorkspace() + "/phones/monophones0");

    job->exec->waitForFinished();

    job->exec->execute("perl " + QApplication::applicationDirPath() + "/perl/mkTree.pl 40 " +
                       WorkCase::currentCase()->getWorkspace() + "/phones/monophones0 " +
                       WorkCase::currentCase()->getWorkspace() + "/instruction/tree.hed");

    job->exec->waitForFinished();

    job->exec->execute("HHEd -B -H " +
                       WorkCase::currentCase()->getWorkspace() + "/hmm12/macros -H " +
                       WorkCase::currentCase()->getWorkspace() + "/hmm12/hmmdefs -M " +
                       WorkCase::currentCase()->getWorkspace() + "/hmm13 " +
                       WorkCase::currentCase()->getWorkspace() + "/instruction/tree.hed " +
                       WorkCase::currentCase()->getWorkspace() + "/" + triphones1 + " > log");

    job->exec->waitForFinished();

    for (int i = 14; i <= 15; i++) {
        job->exec->execute("HERest -B -C " +
                           QApplication::applicationDirPath() + "/config/HERest.cfg -I " +
                           WorkCase::currentCase()->getWorkspace() + "/" + wintri + " -t 250.0 150.0 1000.0 -s stats -S " +
                           WorkCase::currentCase()->getWorkspace() + "/" + train + " -H " +
                           WorkCase::currentCase()->getWorkspace() + "/hmm" + QString::number(i - 1) + "/macros" + " -H " +
                           WorkCase::currentCase()->getWorkspace() + "/hmm" + QString::number(i - 1) + "/hmmdefs" + " -M " +
                           WorkCase::currentCase()->getWorkspace() + "/hmm" + QString::number(i) + " " +
                           WorkCase::currentCase()->getWorkspace() + "/" + triphones1);

        job->exec->waitForFinished();
    }

    if (job->exec->lastExitCode() != 0) {
        console.logError(tr("Errors occurred while create tied state triphones."));
    } else {
        console.logSuccess(tr("Create tied state triphones successfull."));
    }

    wait->close();
}

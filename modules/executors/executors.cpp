#include <QtGui>
#include <QLabel>
#include <QLayout>
#include <QVariant>
#include <QHBoxLayout>
#include <QInputDialog>
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

    //job->exec->execute("HParse " + shortPathName(WorkCase::currentCase()->getWorkspace() + "/" + grammar) +
    //                   " " + shortPathName(WorkCase::currentCase()->getWorkspace() + "/" + wordnet));

    job->exec->execute("HParse " + WorkCase::currentCase()->getWorkspace() + "/" + grammar +
                       " " + WorkCase::currentCase()->getWorkspace() + "/" + wordnet);

    job->exec->waitForFinished();

    if (job->exec->lastExitCode() != 0) {
        console.logError(tr("Errors occurred while create word network."));
    } else {
        console.logSuccess(tr("Create word network successfull."));
    }
}

void Executors::execMonophones(QString prompts, QString wlist, QString monophones, QString dstDict, QString srcDict)
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

    QString cmd = "perl " + QApplication::applicationDirPath() + "/perl/prompts2wlist.pl " +
                  WorkCase::currentCase()->getWorkspace() + "/" + prompts + " " +
                  WorkCase::currentCase()->getWorkspace() + "/" + wlist;

    qDebug() << cmd;

    job->exec->execute("perl " + QApplication::applicationDirPath() + "/perl/prompts2wlist.pl " +
                       WorkCase::currentCase()->getWorkspace() + "/" + prompts + " " +
                       WorkCase::currentCase()->getWorkspace() + "/" + wlist);

    job->exec->waitForFinished();

    job->exec->execute("HDMan -m -w " +
                       WorkCase::currentCase()->getWorkspace() + "/" + wlist + " -n " +
                       WorkCase::currentCase()->getWorkspace() + "/" + monophones + " -l dlog " +
                       WorkCase::currentCase()->getWorkspace() + "/" + dstDict + " " +
                       WorkCase::currentCase()->getWorkspace() + "/" + srcDict);

    job->exec->waitForFinished();

    job->exec->execute("perl " + QApplication::applicationDirPath() + "perl/mkMonophones.pl " +
                       WorkCase::currentCase()->getWorkspace() + "/" + monophones + " " +
                       WorkCase::currentCase()->getWorkspace() + "/phones/monophones0 " +
                       WorkCase::currentCase()->getWorkspace() + "/phones/monophones1");

    job->exec->waitForFinished();

    if (job->exec->lastExitCode() != 0) {
        console.logError(tr("Errors occurred while create monophones."));
    } else {
        console.logSuccess(tr("Create monophones successfull."));
    }
}

void Executors::onErrorLogging(QString log)
{
    console.logError(log);
}

void Executors::abort()
{
    _isAbort = true;
}

bool Executors::removeDir(QString dirName)
{
    bool result = true;
    QDir dir(dirName);

    if (dir.exists(dirName)) {
        Q_FOREACH (QFileInfo info, dir.entryInfoList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden  | QDir::AllDirs | QDir::Files, QDir::DirsFirst)) {
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

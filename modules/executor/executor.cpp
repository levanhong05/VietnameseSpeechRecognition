#include "executor.h"

#include <QApplication>
#include <QTemporaryFile>
#include <QFileInfo>

#include "logger.h"

using namespace VietnameseSpeechRecognition;

#ifdef Q_OS_WIN

#ifndef UNICODE
#define UNICODE
#endif

#include <windows.h>
QString Executor::shortPathName(const QString &file) // returns 8.3 file format from long path
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
QString Executor::shortPathName(const QString &file)
{
    return file;
}
#endif

Executor::Executor(QString path, QObject *parent) :
    QObject(parent), m_path(shortPathName(path)), m_isRunning(false)
{
    this->m_thread = new QThread();
    this->m_thread->start();

    this->m_logger = new Logger();

    this->m_logModel = this->m_logger->getModel();
    this->m_logModel->moveToThread(this->m_thread);
    this->m_logger->moveToThread(this->m_thread);

    this->m_lastExitCode = 0;
    this->m_useCustomErrorHandler = false;
    this->m_useCustomLogHandler = false;

    this->m_process = new QProcess();
    this->m_logWidget = 0;

    this->m_process->setWorkingDirectory(m_path);
#ifdef Q_OS_WIN
    this->m_process->setProgram("cmd.exe");
#else
    this->m_process->setProgram("/bin/bash");
#endif

    connect(this->m_process, SIGNAL(readyRead()), this, SLOT(readyRead()));
    connect(this->m_process, SIGNAL(readyReadStandardError()), this, SLOT(readyReadError()));
    connect(this, SIGNAL(finished(int)), this, SLOT(afterFinished(int)));
}

Executor::~Executor()
{
    this->PIDs.clear();
}

LogListModel *Executor::logModel() const
{
    return this->m_logModel;
}

void Executor::execute()
{
    QTemporaryFile *tmpFile = new QTemporaryFile(this->m_path + QDir::separator() + "run");
    QString cmd = this->m_argument.join(" ");

    if (tmpFile->open()) {
#ifdef Q_OS_WIN
        this->m_tmpBatch = tmpFile->fileName() + ".bat";
#else
        this->m_tmpBatch = tmpFile->fileName() + ".sh";
#endif

        tmpFile->close();
        QFileInfo tmpInfo(this->m_tmpBatch);
        this->m_processToken = tmpInfo.fileName().split('.').at(1);

        QFile tmpBatchFile(this->m_tmpBatch);
        tmpBatchFile.open(QIODevice::WriteOnly);

#ifdef Q_OS_WIN
        tmpBatchFile.write("@echo off\r\n");
        tmpBatchFile.write((cmd + "\r\n").toUtf8());
        tmpBatchFile.write("set EXITCODE=%ERRORLEVEL%\r\n");
        tmpBatchFile.write(QString("echo TERMINATED " + this->m_processToken + " ? %EXITCODE%\r\n").toUtf8());
        tmpBatchFile.write("Exit /b");
#else
        tmpBatchFile.write("#!/bin/bash\n");
        tmpBatchFile.write((cmd + "\n").toUtf8());
        tmpBatchFile.write("EXITCODE=$?\n");
        tmpBatchFile.write(QString("echo \"TERMINATED " + this->m_processToken + " ? $EXITCODE\"\n").toUtf8());
#endif

        tmpBatchFile.close();

#ifdef Q_OS_WIN
        this->m_process->write((this->m_tmpBatch + "\n").toLatin1());
#else
        this->m_process->write(("chmod +x " + this->m_tmpBatch + "\n").toLatin1());
        this->m_process->write((this->m_tmpBatch + "\n").toLatin1());
#endif

        this->m_isRunning = true;
    }
}

void Executor::execute(QString cmd)
{
    QTemporaryFile *tmpFile = new QTemporaryFile(this->m_path + QDir::separator() + "run");
    //tmpFile->autoRemove();
    if (tmpFile->open()) {
#ifdef Q_OS_WIN
        this->m_tmpBatch = tmpFile->fileName() + ".bat";
#else
        this->m_tmpBatch = tmpFile->fileName() + ".sh";
#endif
        tmpFile->close();
        QFileInfo tmpInfo(this->m_tmpBatch);
        this->m_processToken = tmpInfo.fileName().split('.').at(1);

        QFile tmpBatchFile(this->m_tmpBatch);
        tmpBatchFile.open(QIODevice::WriteOnly);

#ifdef Q_OS_WIN
        tmpBatchFile.write("@echo off\r\n");
        tmpBatchFile.write( cmd.toUtf8() );
        tmpBatchFile.write("\r\n");
        tmpBatchFile.write("set EXITCODE=%ERRORLEVEL%\r\n");
        tmpBatchFile.write(QString("echo TERMINATED " + this->m_processToken + " ? %EXITCODE%\r\n").toUtf8());
#else
        tmpBatchFile.write("#!/bin/bash\n");
        tmpBatchFile.write(cmd.toUtf8());
        tmpBatchFile.write("\n");
        tmpBatchFile.write("EXITCODE=$?\n");
        tmpBatchFile.write(QString("echo \"TERMINATED " + this->m_processToken + " ? $EXITCODE\"\n").toUtf8());
#endif

        tmpBatchFile.close();

#ifdef Q_OS_WIN
        this->m_process->write((this->m_tmpBatch + "\n").toLatin1());
#else
        this->m_process->write(("chmod +x " + this->m_tmpBatch + "\n").toLatin1());
        this->m_process->write((this->m_tmpBatch + "\n").toLatin1());
        delete tmpFile;
#endif

        this->m_isRunning = true;
    }
}

void Executor::directExecute(QString cmd)
{
    this->m_process->write((cmd + "\n").toUtf8());
}

void Executor::start()
{
    this->m_process->start(QIODevice::ReadWrite);

    lineNumber = 0;
}

void Executor::readyRead()
{
    while (this->m_process->canReadLine()) {
        QString line = QString::fromUtf8(this->m_process->readLine());
        line.remove('\r');
        line.chop(1);

        if (!line.startsWith("TERMINATED")) {
            if (!this->m_useCustomLogHandler) {
                this->m_logModel->appendLog(line, LogListModel::LOG_LOG, QDateTime());
            } else {
                emit customLogHandler(line.toUtf8());
            }

            if (this->m_name == "Solving") {
                emit messageChange(line);
            } else if (this->m_name == "Forcing") {
                emit messageForceChange(line);
            } else if (this->m_name == "Integrating") {
                emit messageIntegralChange(line);
            }

            if (line.left(3) == "PID") {
                QStringList temps = line.trimmed().split(":", QString::SkipEmptyParts);

                if (temps.size() == 2) {
                    this->PIDs.append(temps[1].toInt());
                }
            }

            lineNumber++;

            emit lineChange(lineNumber);
        } else {
            if (line.contains('?') && line.contains(m_processToken)) {
                line.remove("TERMINATED ");
                line.remove(' ');
                QStringList parts = line.split('?');
                bool ok = false;
                this->m_isRunning = false;
                this->m_lastExitCode = parts.at(1).toInt(&ok);
                emit finished(this->m_lastExitCode);
            }
        }
    }

    if (this->m_process->bytesAvailable() > 0) {
        if (!this->m_useCustomLogHandler) {
            if( this->m_logModel != NULL ) this->m_logModel->appendLog(this->m_process->readAll(), LogListModel::LOG_LOG, QDateTime());
        } else {
            emit customLogHandler(this->m_process->readAll());
        }
    }

    if (!this->m_useCustomLogHandler) {
        if( this->m_logWidget != NULL ) this->m_logWidget->scrollToBottom();
    }
}

void Executor::readyReadError()
{
    QString all = QString::fromUtf8(this->m_process->readAllStandardError());
    all.remove('\r');
    QStringList lines = all.split('\n',QString::SkipEmptyParts);

    foreach (QString line, lines) {
        line = line.trimmed();
        line.remove("-->");
        if (!line.isEmpty() && !line.startsWith("From function") && !line.startsWith("in file")) {
            if (!this->m_useCustomErrorHandler) {
                if( this->m_logModel != NULL ) this->m_logModel->appendLog(line, LogListModel::LOG_ERROR, QDateTime());
            } else {
                emit customErrorHandler(line.toUtf8());
            }
        }
    }

    if (!this->m_useCustomErrorHandler) {
       if( this->m_logWidget != NULL ) this->m_logWidget->scrollToBottom();
    }

    emit error();
}

void Executor::waitForFinished()
{
    while (this->m_isRunning) {
        QApplication::processEvents();
        QThread::currentThread()->msleep(1);
    }
}

void Executor::terminate()
{
    if (this->m_process->pid() <= 0) {
        return;
    }

    QProcess *taskKill;
    taskKill = new QProcess();
    QString command;

    QString cmdPID = "";
    foreach (int iPID, this->PIDs) {
#ifdef Q_OS_WIN
        cmdPID = "/PID " + QString::number(iPID) + " ";
#else
        cmdPID = QString::number(iPID) + " ";
#endif
    }

#ifdef Q_OS_WIN
    command = "taskkill " + cmdPID + " /F /T";
    taskKill->start(command, QIODevice::ReadOnly);
    this->m_process->kill();
#else
    command = "kill -2 " + cmdPID;
    system(command.toLatin1());
#endif

    //if (!this->m_useCustomLogHandler) {
    //    this->m_logModel->appendLog(tr("Process has been stopped!"), LogListModel::LOG_WARN, QDateTime());
    //} else {
    //    emit customLogHandler(tr("Process has been stopped!"));
    //}

    this->PIDs.clear();
}

bool Executor::useCustomErrorHandler() const
{
    return this->m_useCustomErrorHandler;
}

void Executor::setUseCustomErrorHandler(bool useCustomErrorHandler)
{
    this->m_useCustomErrorHandler = useCustomErrorHandler;
}

bool Executor::useCustomLogHandler() const
{
    return this->m_useCustomLogHandler;
}

void Executor::setUseCustomLogHandler(bool useCustomLogHandler)
{
    this->m_useCustomLogHandler = useCustomLogHandler;
}

int Executor::lastExitCode() const
{
    return this->m_lastExitCode;
}

void Executor::afterFinished(int)
{
    emit ready();
}

QTextEdit *Executor::textEdit() const
{
    return m_textEdit;
}

void Executor::setTextEdit(QTextEdit *textEdit)
{
    m_textEdit = textEdit;
}

QString Executor::name() const
{
    return this->m_name;
}

void Executor::setName(const QString &name)
{
    this->m_name = name;
}

QThread *Executor::thread() const
{
    return this->m_thread;
}

void Executor::addArgument(QString argument)
{
    this->m_argument.append(argument);
}

void Executor::clearArguments()
{
    this->m_argument.clear();
}

bool Executor::isRunning() const
{
    return this->m_isRunning;
}

void Executor::setRunning(bool isRunning)
{
    this->m_isRunning = isRunning;
}

QString Executor::path() const
{
    return this->m_path;
}

void Executor::setPath(const QString &path)
{
    this->m_path = path;
}

QListView *Executor::logWidget() const
{
    return this->m_logWidget;
}

void Executor::setLogWidget(QListView *logWidget)
{
    this->m_logWidget = logWidget;
}

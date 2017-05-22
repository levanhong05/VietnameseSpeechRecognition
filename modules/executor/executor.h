#ifndef EXECUTOR_H
#define EXECUTOR_H

#include <QDir>
#include <QThread>
#include <QProcess>
#include <QListView>
#include <QTextEdit>

#include "executor_global.h"

#include "logger.h"

namespace VietnameseSpeechRecognition {

class EXECUTORSHARED_EXPORT Executor : public QObject
{
    Q_OBJECT
public:
    Executor(QString path, QObject *parent = 0);
    ~Executor();

    QString name() const;
    void setName(const QString &name);

    LogListModel *logModel() const;

    QListView *logWidget() const;
    void setLogWidget(QListView *logWidget);

    QString path() const;
    void setPath(const QString &path);

    bool isRunning() const;
    void setRunning(bool isRunning);

    int lastExitCode() const;

    bool useCustomLogHandler() const;
    void setUseCustomLogHandler(bool useCustomLogHandler);

    bool useCustomErrorHandler() const;
    void setUseCustomErrorHandler(bool useCustomErrorHandler);

    QThread *thread() const;

    void addArgument(QString argument);
    void clearArguments();

    QTextEdit *textEdit() const;
    void setTextEdit(QTextEdit *textEdit);

protected:
    static QString shortPathName(const QString &file);

public slots:
    void execute();
    void execute(QString cmd);
    void directExecute(QString cmd);
    void start();
    void readyRead();
    void readyReadError();
    void waitForFinished();

    void terminate();

private slots:
    void afterFinished(int);

signals:
    void ready();
    void finished(int);
    void customLogHandler(QString);
    void customErrorHandler(QString);
    void messageChange(QString);
    void messageForceChange(QString);
    void messageIntegralChange(QString);
    void error();

    void lineChange(unsigned int number);

protected:
    QProcess *m_process;
    QThread *m_thread;
    QListView *m_logWidget;
    QTextEdit *m_textEdit;
    Logger *m_logger;
    LogListModel *m_logModel;

    QString m_processToken;
    QString m_tmpBatch;
    QString m_path;

    QString m_name;
    QStringList m_argument;

    bool m_isRunning;
    int m_lastExitCode;
    bool m_useCustomLogHandler;
    bool m_useCustomErrorHandler;

    QList<int> PIDs;

    unsigned int lineNumber;
};

}

#endif // EXECUTOR_H

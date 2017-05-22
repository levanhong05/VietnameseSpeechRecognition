#ifndef EXECUTORS_H
#define EXECUTORS_H

#include <QList>
#include <QObject>
#include <QDockWidget>
#include <QDirIterator>

#include <iostream>

#include "executors_global.h"

#include "executors.h"
#include "runlogwidget.h"

#include "console.h"
#include "executor.h"
#include "workcase.h"

#include "waitingdialog.h"

namespace VietnameseSpeechRecognition {

typedef struct ExecutingJob{
    ExecutingJob(QString title, QWidget *parent = 0);
    QString title;
    QDockWidget *dock;
    RunLogWidget *logWidget;

    Executor *exec;
    bool beginWrite;
} ExecutingJob;

class EXECUTORSSHARED_EXPORT Executors : public QObject
{
    Q_OBJECT
private:
    QString shortPathName(const QString &file);

    bool removeDir(QString dirName);

public slots:
    void abort();

    void execWordNet(QString grammar = "text/grammar.txt", QString wordnet = "text/wdnet.txt");

    void execMonophones(QString prompts = "text/prompts.txt", QString wlist = "text/wlist",
                        QString monophones = "phones/monophones",
                        QString dstDict = "text/dict", QString srcDict = "text/dict.dct");

private slots:
    void onErrorLogging(QString);

private:
    QList<ExecutingJob *> _jobs;

    WaitingDialog *_wait;
    bool _isAbort;

};

}

#endif // EXECUTORS_H

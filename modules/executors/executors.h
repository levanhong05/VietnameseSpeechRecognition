#ifndef EXECUTORS_H
#define EXECUTORS_H

#include <QList>
#include <QObject>
#include <QDockWidget>
#include <QDirIterator>

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
public slots:
    void execWordNet(QString grammar = "text/grammar.txt", QString wordnet = "text/wdnet.txt");

    void execMonophones(QString prompts = "text/prompts.txt", QString wlist = "text/wlist",
                        QString mphones = "phones/monophones",
                        QString dstDict = "text/dict", QString srcDict = "text/dict.dct",
                        QString mphones0 = "phones/monophones0", QString mphones1 = "phones/monophones1");

    void execTranscription(QString prompts = "text/prompts.txt", QString mlfwords = "mlf/words.mlf",
                           QString dict = "text/dict.dct",
                           QString mlfphone0 = "mlf/phones0.mlf", QString mkphones0 = "instruction/mkphones0.led",
                           QString mlfphone1 = "mlf/phones1.mlf", QString mkphones1 = "instruction/mkphones1.led");

    void execMFCC(QString path, QString hcopyCFG = "config/HCopy.cfg");

    void execProto(QString wave = "wave", QString train = "text/train.scp",
                   QString hcomvCFG = "config/HCompV.cfg");

    void execHRest(QString train = "text/train.scp", QString hrestCFG = "config/HERest.cfg");

    void execHHEd(QString monophones1 = "phones/monophones1", QString silinstr = "instruction/sil.hed",
                  QString train = "text/train.scp", QString hrestCFG = "config/HERest.cfg");

    void execHVite(QString monophones1 = "phones/monophones1", QString dict = "text/dict.dct",
                  QString train = "text/train.scp", QString hrestCFG = "config/HERest.cfg");

    void execTriphones(QString train = "text/train.scp", QString triphones1 = "phones/triphones1");

    void execTiedTriphones(QString wintri = "mlf/wintri.mlf", QString train = "text/train.scp",
                           QString triphones1 = "phones/triphones1");

private slots:
    void onErrorLogging(QString);

private:
    QString shortPathName(const QString &file);

    bool removeDir(QString dirName);

    void scanDir(QString dirName);

private:
    QList<ExecutingJob *> _jobs;

    WaitingDialog *_wait;
    bool _isAbort;
};

}

#endif // EXECUTORS_H

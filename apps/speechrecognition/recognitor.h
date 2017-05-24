#ifndef RECOGNITOR_H
#define RECOGNITOR_H

#include <QObject>

#include "executors.h"

namespace VietnameseSpeechRecognition {

class Recognitor : public QObject
{
    Q_OBJECT
public:
    explicit Recognitor(QObject *parent = 0);

    void execute(QString path);

    void executeWordNet();

    void executeMonophones();

    void executeTranscription();

    void executeMFCC(QString path);

    void executeProto();

    void executeHRest();

    void executeHHEd();

    void executeHVite();

    void executeTriphones();

    void executeTiedTriphones();

};

}

#endif // RECOGNITOR_H

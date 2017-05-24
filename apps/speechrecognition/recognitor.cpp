#include "recognitor.h"

using namespace VietnameseSpeechRecognition;

Recognitor::Recognitor(QObject *parent) :
    QObject(parent)
{
}

void Recognitor::execute(QString path)
{
    Executors *executors = new Executors();

    executors->execWordNet();

    executors->execMonophones();

    executors->execTranscription();

    executors->execMFCC(path);

    executors->execProto();

    executors->execHRest();

    executors->execHHEd();

    executors->execHVite();

    executors->execTriphones();

    executors->execTiedTriphones();
}

void Recognitor::executeWordNet()
{
    Executors *executors = new Executors();

    executors->execWordNet();
}

void Recognitor::executeMonophones()
{
    Executors *executors = new Executors();

    executors->execMonophones();
}

void Recognitor::executeTranscription()
{
    Executors *executors = new Executors();

    executors->execTranscription();
}

void Recognitor::executeMFCC(QString path)
{
    Executors *executors = new Executors();

    executors->execMFCC(path);
}

void Recognitor::executeProto()
{
    Executors *executors = new Executors();

    executors->execProto();
}

void Recognitor::executeHRest()
{
    Executors *executors = new Executors();

    executors->execHRest();
}

void Recognitor::executeHHEd()
{
    Executors *executors = new Executors();

    executors->execHHEd();
}

void Recognitor::executeHVite()
{
    Executors *executors = new Executors();

    executors->execHVite();
}

void Recognitor::executeTriphones()
{
    Executors *executors = new Executors();

    executors->execTriphones();
}

void Recognitor::executeTiedTriphones()
{
    Executors *executors = new Executors();

    executors->execTiedTriphones();
}

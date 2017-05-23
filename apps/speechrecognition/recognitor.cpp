#include "recognitor.h"

using namespace VietnameseSpeechRecognition;

Recognitor::Recognitor(QObject *parent) :
    QObject(parent)
{
}

void Recognitor::execute()
{
    Executors *executors = new Executors();

    executors->execWordNet();

    executors->execMonophones();

    executors->execTranscription();
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

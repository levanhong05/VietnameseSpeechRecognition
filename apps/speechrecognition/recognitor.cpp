#include "recognitor.h"

using namespace VietnameseSpeechRecognition;

Recognitor::Recognitor(QObject *parent) :
    QObject(parent)
{
}

void Recognitor::executeWordNet()
{
    Executors *executors = new Executors();

    executors->execWordNet("text/grammar.txt", "text/wdnet.txt");
}

void Recognitor::executeMonophones()
{
    Executors *executors = new Executors();

    executors->execMonophones("text/prompts.txt", "text/wlist", "phones/monophones", "text/dict", "text/dict.dct");
}

#include "application.h"
#include "workcase.h"

using namespace VietnameseSpeechRecognition;

Application::Application(int &argc, char **argv)
    : QApplication(argc, argv)
{
    this->mainWindow = new SpeechRecognition();

    VietnameseSpeechRecognition::WorkCase::currentCase();
}

void Application::showMainWindow()
{
    mainWindow->showMaximized();
}

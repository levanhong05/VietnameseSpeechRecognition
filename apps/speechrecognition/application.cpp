#include "application.h"

#include "workcase.h"

Application::Application(int &argc, char **argv)
    : QApplication(argc, argv)
{
    _mainWindow = new SpeechRecognition();

    VietnameseSpeechRecognition::WorkCase::currentCase();
}

void Application::showMainWindow()
{
    _mainWindow->showMaximized();
}

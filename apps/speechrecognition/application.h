#ifndef APPLICATION_H
#define APPLICATION_H

#include <QApplication>

#include "speechrecognition.h"

using namespace VietnameseSpeechRecognition;

class Application : public QApplication
{
public:
    Application(int &argc, char **argv);
    ~Application() {}

public slots:
    void showMainWindow();

private:
    SpeechRecognition *mainWindow;
};

#endif // APPLICATION_H

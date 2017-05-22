#ifndef ABOUTSCREEN_H
#define ABOUTSCREEN_H

#include <QDialog>

namespace Ui {
class AboutScreen;
}

namespace VietnameseSpeechRecognition {

class AboutScreen : public QDialog
{
    Q_OBJECT
public:
    explicit AboutScreen(QWidget *parent = 0);
    ~AboutScreen();

private:
    Ui::AboutScreen *ui;
};

}

#endif // ABOUTSCREEN_H

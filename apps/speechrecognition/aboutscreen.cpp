#include "aboutscreen.h"
#include "ui_aboutscreen.h"

#include <QDesktopWidget>

AboutScreen::AboutScreen(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutScreen)
{
    ui->setupUi(this);

    //Move the dialog away from the center
    setGeometry(0, 0, this->width(), this->height());

    int i = 0;

    if (qApp->desktop()->screenCount() > 1) {
        i = qApp->desktop()->screenNumber(this) ;
    }

    //Put the dialog in the screen center
    const QRect screen = qApp->desktop()->screenGeometry(i);

    move(screen.center() - rect().center());

    setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint);
}

AboutScreen::~AboutScreen()
{
    delete ui;
}

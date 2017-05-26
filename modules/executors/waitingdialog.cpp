#include "waitingdialog.h"
#include "ui_waitingdialog.h"

#include <QDesktopWidget>

WaitingDialog::WaitingDialog(QString title, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::WaitingDialog),
    _movie(":/executors/animation/loader.gif")
{
    ui->setupUi(this);

    Qt::WindowFlags flags = 0;
    flags = Qt::Tool;
    flags |= Qt::CustomizeWindowHint;
    flags |= Qt::WindowTitleHint;
    setWindowFlags(flags);

    ui->loader->clear();
    ui->loader->setMovie(&_movie);

    _movie.start();

    //Move the dialog away from the center
    setGeometry(0, 0, this->width(), this->height());

    int i = 0;

    if (qApp->desktop()->screenCount() > 1) {
        i = qApp->desktop()->screenNumber(this) ;
    }

    //Put the dialog in the screen center
    const QRect screen = qApp->desktop()->screenGeometry(i);
    move(screen.center() - rect().center());

    ui->lblMessage->setText(tr("Processing..."));

    setWindowTitle(title);
}

WaitingDialog::~WaitingDialog()
{
    delete ui;
}

void WaitingDialog::setTitle(QString title)
{
    setWindowTitle(title);
}

void WaitingDialog::closeEvent(QCloseEvent *event)
{
    event->setAccepted(!event->spontaneous());
}

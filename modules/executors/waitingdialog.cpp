#include "waitingdialog.h"
#include "ui_waitingdialog.h"

#include <QDesktopWidget>

WaitingDialog::WaitingDialog(QString message, QWidget *parent) :
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

    ui->frameAbort->setVisible(false);

    flag = true;

    setWindowTitle(message);

    this->isUsingPercent = false;
}

WaitingDialog::~WaitingDialog()
{
    delete ui;
}

void WaitingDialog::setEnabledButton(bool enable)
{
    ui->frameAbort->setVisible(enable);
}

void WaitingDialog::setUsingPerCent(bool enabled)
{
    this->isUsingPercent = enabled;
}

void WaitingDialog::updatePercent(unsigned int number)
{
    if (!isUsingPercent) {
        ui->lblMessage->setText(tr("Processing..."));
        return;
    }

    unsigned int percent = 0;

    if (number <= 2) {
        percent = 2;
    } else if (number <= 5) {
        percent = 5;
    } else if (number <= 10) {
        percent = 10;
    } else if (number <= 15) {
        percent = 12;
    } else if (number <= 20) {
        percent = 15;
    } else if (number <= 30) {
        percent = 20;
    } else if (number <= 40) {
        percent = 23;
    } else if (number <= 50) {
        percent = 27;
    } else if (number <= 60) {
        percent = 30;
    } else if (number <= 70) {
        percent = 35;
    } else if (number <= 80) {
        percent = 37;
    } else if (number <= 90) {
        percent = 40;
    } else if (number <= 100) {
        percent = 45;
    } else if (number <= 150) {
        percent = 50;
    } else if (number <= 200) {
        percent = 55;
    } else if (number <= 250) {
        percent = 60;
    } else if (number <= 300) {
        percent = 62;
    } else if (number <= 350) {
        percent = 65;
    } else if (number <= 400) {
        percent = 70;
    } else if (number <= 450) {
        percent = 75;
    } else if (number <= 500) {
        percent = 80;
    } else if (number <= 550) {
        percent = 85;
    } else if (number <= 600) {
        percent = 90;
    } else if (number <= 700) {
        percent = 92;
    } else if (number <= 800) {
        percent = 93;
    } else if (number <= 900) {
        percent = 94;
    } else if (number <= 1000) {
        percent = 95;
    } else {
        percent = 96;
    }

    if (percent <= 100) {
        ui->lblMessage->setText(tr("Processing... %1%").arg(percent));
    }
}

void WaitingDialog::closeEvent(QCloseEvent *event)
{
    event->setAccepted(!event->spontaneous());

    if (event->isAccepted()) {
        flag = false;

        this->close();
    }
}

void WaitingDialog::on_btnAbort_clicked()
{
    flag = false;
    emit abort();

    ui->lblMessage->setText("Abort...");
}

#ifndef MESSAGEDIALOG_H
#define MESSAGEDIALOG_H

#include <QMovie>
#include <QDialog>
#include <QCloseEvent>

namespace Ui {
class WaitingDialog;
}

class WaitingDialog : public QDialog
{
    Q_OBJECT
public:
    explicit WaitingDialog(QString message, QWidget *parent = 0);
    ~WaitingDialog();

private slots:
    void closeEvent(QCloseEvent *event);

private:
    Ui::WaitingDialog *ui;

    QMovie _movie;

};

#endif // MESSAGEDIALOG_H

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

    bool flag;
    void setEnabledButton(bool enable = true);
    void setUsingPerCent(bool enabled);

public slots:
    void updatePercent(unsigned int number);

private slots:
    void on_btnAbort_clicked();
    void closeEvent(QCloseEvent *event);

signals:
    void abort();

private:
    Ui::WaitingDialog *ui;

    QMovie _movie;

    bool isUsingPercent;

};

#endif // MESSAGEDIALOG_H

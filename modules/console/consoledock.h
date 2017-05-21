#ifndef CONSOLEDOCK_H
#define CONSOLEDOCK_H

#include "console_global.h"

#include <QEvent>
#include <QStack>
#include <QObject>
#include <QListView>
#include <QDockWidget>
#include <QAbstractItemModel>

namespace Ui {
class ConsoleDock;
}

namespace VietnameseSpeechRecognition {

class CONSOLESHARED_EXPORT ConsoleDock : public QDockWidget
{
    Q_OBJECT
public:
    explicit ConsoleDock(QWidget *parent = 0);
    ~ConsoleDock();

    void setModel(QAbstractItemModel * model);

    QListView * logViewer();

    bool clear();

public slots:
    void scrollToBottom();

signals:
    void commandEntered(QString);

protected:
    bool eventFilter(QObject *object, QEvent *event);

private slots:
    void on_txtConsoleCmd_returnPressed();
    void on_btnClearLog_clicked();

private:
    Ui::ConsoleDock *ui;

    QStack<QString> stackCommandUp;
    QStack<QString> stackCommandDown;

};

}
#endif // CONSOLEDOCK_H

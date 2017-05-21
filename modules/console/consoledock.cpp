#include "consoledock.h"
#include "ui_consoledock.h"

#include <QHBoxLayout>

#include "htmldelegate.h"

using namespace VietnameseSpeechRecognition;

ConsoleDock::ConsoleDock(QWidget *parent) :
    QDockWidget(parent),
    ui(new Ui::ConsoleDock)
{
    ui->setupUi(this);
    //    HtmlDelegate* delegate = new HtmlDelegate();
    //    ui->logviewer->setItemDelegate(delegate);
    qApp->installEventFilter(this);
}

ConsoleDock::~ConsoleDock()
{
    delete ui;
}

void ConsoleDock::setModel(QAbstractItemModel *model)
{
    ui->logviewer->setModel(model);
}

QListView *ConsoleDock::logViewer()
{
    return ui->logviewer;
}

bool ConsoleDock::clear()
{
    ui->logviewer->model()->removeRows(0, ui->logviewer->model()->rowCount());

    return true;
}

void ConsoleDock::scrollToBottom()
{
    ui->logviewer->scrollToBottom();
}

bool ConsoleDock::eventFilter(QObject *object, QEvent *event)
{
    if (object == ui->txtConsoleCmd && event->type() == QEvent::KeyRelease) {
        QKeyEvent *key = static_cast<QKeyEvent *>(event);

        if (key->key() == Qt::Key_Up) {
            if (!stackCommandUp.isEmpty()) {
                QString command = stackCommandUp.pop();

                ui->txtConsoleCmd->setText(command);
                stackCommandDown.push(command);
            }
        } else if (key->key() == Qt::Key_Down) {
            if (!stackCommandDown.isEmpty()) {
                QString command = stackCommandDown.pop();

                ui->txtConsoleCmd->setText(command);
                stackCommandUp.push(command);
            }
        }
    }

    return QObject::eventFilter(object, event);
}

void ConsoleDock::on_txtConsoleCmd_returnPressed()
{
    QString command = ui->txtConsoleCmd->text().toLower();

    if (command.length() == 0) {
        return;
    }

    emit commandEntered(command);

    if (command.trimmed() != "") {
        stackCommandUp.push(command);
    }

    ui->txtConsoleCmd->setText("");
}

void ConsoleDock::on_btnClearLog_clicked()
{
    clear();
}

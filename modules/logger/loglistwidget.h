#ifndef LOGLISTWIDGET_H
#define LOGLISTWIDGET_H

#include <QWidget>
#include <QStringList>
#include <QStringListModel>
#include "loglistmodel.h"

namespace Ui {
class LogListWidget;
}

namespace VietnameseSpeechRecognition {

class LogListWidget : public QWidget
{
    Q_OBJECT
public:
    explicit LogListWidget(QWidget *parent = 0);
    ~LogListWidget();

    void setModel(QAbstractItemModel *model);

private:
    Ui::LogListWidget *ui;
    LogListModel *model;
};

}
#endif // LOGLISTWIDGET_H

#include "loglistwidget.h"
#include "ui_loglistwidget.h"

using namespace VietnameseSpeechRecognition;

LogListWidget::LogListWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LogListWidget)
{
    ui->setupUi(this);
}

LogListWidget::~LogListWidget()
{
    delete ui;
}

void LogListWidget::setModel(QAbstractItemModel *model)
{
    ui->listView->setModel(model);
}

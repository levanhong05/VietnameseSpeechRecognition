#include "runlogwidget.h"
#include "ui_runlogwidget.h"

RunLogWidget::RunLogWidget(int key, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RunLogWidget)
{
    Q_UNUSED(key);
    ui->setupUi(this);
    this->m_logView = ui->logviewer;
}

RunLogWidget::~RunLogWidget()
{
    delete ui;
}

QListView *RunLogWidget::logView() const
{
    return m_logView;
}

void RunLogWidget::setLogView(QListView *logView)
{
    m_logView = logView;
}

void RunLogWidget::on_btnClearLog_clicked()
{
    emit clear();
}

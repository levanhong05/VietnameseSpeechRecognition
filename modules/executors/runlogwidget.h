#ifndef RUNLOGWIDGET_H
#define RUNLOGWIDGET_H

#include <QWidget>
#include <QListView>

namespace Ui {
class RunLogWidget;
}

class RunLogWidget : public QWidget
{
    Q_OBJECT
public:
    explicit RunLogWidget(int key,QWidget *parent = 0);
    ~RunLogWidget();

    QListView *logView() const;
    void setLogView(QListView *logView);

private slots:
    void on_btnClearLog_clicked();

signals:
    void clear();

private:
    Ui::RunLogWidget *ui;
    QListView * m_logView;
};

#endif // RUNLOGWIDGET_H

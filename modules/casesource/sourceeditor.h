#ifndef SOURCEEDITOR_H
#define SOURCEEDITOR_H

#include <QWidget>

class QsciScintilla;

namespace Ui {
class SourceEditor;
}

class SourceEditor : public QWidget
{
    Q_OBJECT
public:
    explicit SourceEditor(QWidget *parent = 0);
    ~SourceEditor();

public slots:
    void closeEvent(QCloseEvent * event);
    void openFile(const QString &filepath);
    void reloadFileContent();

signals:
    void requestCloseEditor(SourceEditor*);

private:
    Ui::SourceEditor *ui;
    QsciScintilla *m_qsciWidget;
    QString filepath;
};

#endif // SOURCEEDITOR_H

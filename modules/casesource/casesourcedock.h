#ifndef CASESOURCEDOCK_H
#define CASESOURCEDOCK_H

#include <QDockWidget>
#include <QFileSystemModel>
#include <QFileSystemWatcher>
#include <QPersistentModelIndex>

#include <QList>
#include <QMetaType>
#include <QTreeView>
#include <QListWidgetItem>
#include <QDesktopWidget>

#include "workcase.h"
#include "console.h"
#include "sourceeditor.h"

namespace Ui {
class CaseSourceDock;
}

class CaseSourceDock : public QDockWidget
{
    Q_OBJECT
public:
    explicit CaseSourceDock(QWidget *parent = 0);
    ~CaseSourceDock();

public:
    void setupCaseSourceTree();

private slots:
    void on_treeView_doubleClicked(const QModelIndex &index);
    void onEditorClosed(SourceEditor *editor);
    void onFileChanged(QString filename);

    void on_listWidget_currentRowChanged(int currentRow);

    void onCurrentTabChanged(QWidget *w);

private:
    Ui::CaseSourceDock *ui;
    QFileSystemModel * model;

    typedef struct OpenDocument {
        OpenDocument(SourceEditor *srcEditor, QListWidgetItem *listItem, QString filename);
        SourceEditor *srcEditor;
        QListWidgetItem *listItem;
        QString filename;
    } OpenDocument;

    QList<OpenDocument*> openDocuments;

    QFileSystemWatcher * fsWatcher;
};

#endif // CASESOURCEDOCK_H

#include "casesourcedock.h"
#include "ui_casesourcedock.h"

using namespace VietnameseSpeechRecognition;

CaseSourceDock::CaseSourceDock(QWidget *parent) :
    QDockWidget(parent),
    ui(new Ui::CaseSourceDock)
{
    this->setTitleBarWidget(new QWidget());

    ui->setupUi(this);

    model = new QFileSystemModel();
    model->setRootPath("");

    fsWatcher = new QFileSystemWatcher();

    connect(fsWatcher, SIGNAL(fileChanged(QString)), this, SLOT(onFileChanged(QString)));

    //connect(Plugin::PluginApi::getInstance(), SIGNAL(currentTabChanged(QWidget *)),
    //        this, SLOT(onCurrentTabChanged(QWidget *)));
}

CaseSourceDock::~CaseSourceDock()
{
    delete ui;
}

void CaseSourceDock::setupCaseSourceTree()
{
    ui->treeView->setModel(model);

    ui->treeView->hideColumn(1);
    ui->treeView->hideColumn(2);
    ui->treeView->hideColumn(3);
    ui->treeView->hideColumn(4);

    const QModelIndex rootIndex = model->index(QDir::cleanPath(WorkCase::currentCase()->getWorkspace()));

    if (rootIndex.isValid()) {
        ui->treeView->setRootIndex(rootIndex);
    }

    ui->treeView->setAnimated(true);
    ui->treeView->setIndentation(20);
    ui->treeView->setSortingEnabled(true);
    ui->treeView->sortByColumn(0, Qt::AscendingOrder);
}

void CaseSourceDock::on_treeView_doubleClicked(const QModelIndex &index)
{
    QFileInfo finfo(model->filePath(index));

    if (finfo.isDir()) {
        return;
    }

    foreach (OpenDocument *each, this->openDocuments) {
        if (each->filename == finfo.absoluteFilePath()) {
            //Plugin::PluginApi::getInstance()->raiseTab((QWidget *)each->srcEditor);
            return;
        }
    }

    //SourceEditor *sed = new SourceEditor(Plugin::PluginApi::getInstance()->mainWindow());
    SourceEditor *sed = new SourceEditor();
    console.logDebug(model->filePath(index));

    sed->openFile(model->filePath(index));

    QListWidgetItem *listItem = new QListWidgetItem(finfo.fileName(), ui->listWidget);
    ui->listWidget->addItem(listItem);
    fsWatcher->addPath(finfo.absoluteFilePath());

    this->openDocuments.append(new OpenDocument(sed, listItem, finfo.absoluteFilePath()));

    connect(sed, SIGNAL(requestCloseEditor(SourceEditor *)), this, SLOT(onEditorClosed(SourceEditor *)));

    //Plugin::PluginApi::getInstance()->addTab(finfo.fileName(), sed, QIcon(":/numix/actions/stock_notes.svg"), true, true);
}

void CaseSourceDock::onEditorClosed(SourceEditor *editor)
{
    console.logDebug("slot triggered: on editor closed");

    foreach (OpenDocument *each, this->openDocuments) {
        if (each->srcEditor == editor) {
            ui->listWidget->removeItemWidget(each->listItem);
            delete each->listItem;

            openDocuments.removeOne(each);
            delete each;
            break;
        }
    }
}

void CaseSourceDock::onFileChanged(QString filename)
{
    foreach (OpenDocument *each, this->openDocuments) {
        if (each->filename == filename) {
            each->srcEditor->reloadFileContent();
            break;
        }
    }
}


CaseSourceDock::OpenDocument::OpenDocument(SourceEditor *srcEditor, QListWidgetItem *listItem, QString filename)
    : srcEditor(srcEditor), listItem(listItem), filename(filename)
{
    //initialize new open document
}

void CaseSourceDock::on_listWidget_currentRowChanged(int currentRow)
{
    if (ui->listWidget->count() > 0 && currentRow >= 0) {
        //Plugin::PluginApi::getInstance()->raiseTab((QWidget *)this->openDocuments.at(currentRow)->srcEditor);
    }
}

void CaseSourceDock::onCurrentTabChanged(QWidget *w)
{
    int index = -1;
    bool found = false;

    foreach (OpenDocument *each, this->openDocuments) {
        index++;

        if (each->srcEditor == w) {
            ui->listWidget->setCurrentRow(index);
            found = true;
            break;
        }
    }

    if (!found) {
        if (ui->listWidget->selectedItems().count() > 0) {
            ui->listWidget->clearSelection();
        }
    }
}

#include "speechrecognition.h"
#include "ui_speechrecognition.h"

#include <QAction>
#include <QFileDialog>
#include <QMessageBox>
#include <QApplication>

#include "console.h"
#include "workcase.h"

#include "recognitor.h"
#include "typingconverter.h"
#include "dictionarycreator.h"

#include "aboutscreen.h"

#include "waitingdialog.h"

using namespace VietnameseSpeechRecognition;

SpeechRecognition::SpeechRecognition(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SpeechRecognition)
{
    ui->setupUi(this);

    this->addDockWidget(Qt::BottomDockWidgetArea, console.dock());

    console.logInfo(tr("Vietnamese Speech Recognition Program !!!"));

    preparingData();

    QAction *actionTraining = ui->toolBar->addAction(QIcon(":/speech/images/train.png"), tr("Training"));
    actionTraining->setShortcut(Qt::Key_F5 | Qt::CTRL);

    QAction *actionTesting = ui->toolBar->addAction(QIcon(":/speech/images/test.png"), tr("Testing"));
    actionTraining->setShortcut(Qt::Key_T | Qt::CTRL);

    connect(actionTraining, SIGNAL(triggered(bool)), this, SLOT(startTrainingData()));
    connect(actionTesting, SIGNAL(triggered(bool)), this, SLOT(startTestingData()));

    QWidget::showMaximized();
}

SpeechRecognition::~SpeechRecognition()
{
    delete ui;
}

QMenu *SpeechRecognition::addMenu(const QString &title)
{
    return ui->menuBar->addMenu(title);
}

void SpeechRecognition::startTrainingData()
{
    if (ui->txtPromtPath->text().isEmpty()) {
        QMessageBox::critical(this, tr("Missing Data"), tr("Please select promts data."), QMessageBox::Ok);
        return;
    }

    if (ui->txtWavePath->text().isEmpty()) {
        QMessageBox::critical(this, tr("Missing Data"), tr("Please select wave data."), QMessageBox::Ok);
        return;
    }

    DictionaryCreator *creator = new DictionaryCreator();

    WaitingDialog *wait = new WaitingDialog(tr("Create Dictionary..."));

    connect(creator, SIGNAL(finished()), this, SLOT(onTrainingDictionaryFinished()));
    connect(creator, SIGNAL(finished()), wait, SLOT(close()));

    wait->show();
    creator->execute(ui->txtWavePath->text());
}

void SpeechRecognition::startTestingData()
{
    if (ui->txtWaveTest->text().isEmpty()) {
        QMessageBox::critical(this, tr("Missing Data"), tr("Please select test data."), QMessageBox::Ok);
        return;
    }
}

void SpeechRecognition::onCreateDictionaryFinished()
{
    console.logSuccess(tr("Create dictionary successfully."));
}

void SpeechRecognition::onTrainingDictionaryFinished()
{
    console.logSuccess(tr("Create dictionary successfully."));

    Recognitor *recognitor = new Recognitor();

    recognitor->execute(ui->txtWavePath->text());
}

void SpeechRecognition::on_actionConvert_Typing_triggered()
{
    TypingConverter *converter = new TypingConverter();

    converter->show();
}

void SpeechRecognition::on_actionAbout_triggered()
{
    AboutScreen *screen = new AboutScreen();

    screen->show();
}

void SpeechRecognition::on_btnPromtBrowse_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(this, tr("Open Promts Data"), QString(QApplication::applicationDirPath()), tr("Text File (*.txt)"));

    if (filePath.isEmpty()) {
        return;
    }

    if (QFile::exists(filePath)) {
        ui->txtPromtPath->setText(filePath);

        QFile::copy(filePath, WorkCase::currentCase()->getWorkspace() + "/text/prompts.txt");
    }
}

void SpeechRecognition::on_btnCreateDictionary_clicked()
{
    QString path = ui->txtPromtPath->text();

    if (!path.isEmpty()) {
        DictionaryCreator *creator = new DictionaryCreator();

        WaitingDialog *wait = new WaitingDialog(tr("Create Dictionary..."));

        connect(creator, SIGNAL(finished()), this, SLOT(onCreateDictionaryFinished()));
        connect(creator, SIGNAL(finished()), wait, SLOT(close()));

        wait->show();
        creator->execute(path);
    }
}

void SpeechRecognition::on_actionQuit_triggered()
{
    QApplication::quit();
}

void SpeechRecognition::preparingData()
{
    QDir data(QApplication::applicationDirPath() + "/text");

    if (data.exists()) {
        copyPath(QApplication::applicationDirPath() + "/text", WorkCase::currentCase()->getWorkspace() + "/text");
    } else {
        data.mkpath(WorkCase::currentCase()->getWorkspace() + "/text");
    }

    QDir data1(QApplication::applicationDirPath() + "/instruction");

    if (data1.exists()) {
        copyPath(QApplication::applicationDirPath() + "/instruction", WorkCase::currentCase()->getWorkspace() + "/instruction");
    } else {
        data1.mkpath(WorkCase::currentCase()->getWorkspace() + "/instruction");
    }

    QDir data2(QApplication::applicationDirPath() + "/hmm");

    if (data2.exists()) {
        copyPath(QApplication::applicationDirPath() + "/hmm", WorkCase::currentCase()->getWorkspace() + "/hmm");
    } else {
        data2.mkpath(WorkCase::currentCase()->getWorkspace() + "/hmm");
    }

    data.mkpath(WorkCase::currentCase()->getWorkspace() + "/mlf");
    data.mkpath(WorkCase::currentCase()->getWorkspace() + "/phones");
    data.mkpath(WorkCase::currentCase()->getWorkspace() + "/wave");

    for (int i = 0; i < 16; i++) {
        data.mkpath(WorkCase::currentCase()->getWorkspace() + "/hmm" + QString::number(i));
    }
}

void SpeechRecognition::copyPath(QString src, QString dst)
{
    QDir dir(src);

    if (!dir.exists()) {
        return;
    }

    dir.mkpath(dst);

    foreach (QString d, dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot)) {
        QString dstPath = dst + QDir::separator() + d;
        dir.mkpath(dstPath);
        copyPath(src+ QDir::separator() + d, dstPath);
    }

    foreach (QString f, dir.entryList(QDir::Files)) {
        QFile::copy(src + QDir::separator() + f, dst + QDir::separator() + f);
    }
}

void SpeechRecognition::showEvent(QShowEvent *)
{
    QDir workspace(WorkCase::currentCase()->getWorkspace());

    workspace.setFilter(QDir::Files | QDir::NoDotAndDotDot | QDir::NoSymLinks);

    QFileInfoList infos = workspace.entryInfoList();

    foreach (QFileInfo info, infos) {
        if (info.fileName().startsWith("run")) {
            QFile::remove(info.absoluteFilePath());
        }
    }
}

void SpeechRecognition::on_btnCreateWordNet_clicked()
{
    Recognitor *recognitor = new Recognitor();

    recognitor->executeWordNet();
}

void SpeechRecognition::on_btnCreateMonophone_clicked()
{
    Recognitor *recognitor = new Recognitor();

    recognitor->executeMonophones();
}

void SpeechRecognition::on_btnCreateTranscription_clicked()
{
    Recognitor *recognitor = new Recognitor();

    recognitor->executeTranscription();
}

void SpeechRecognition::on_btnWaveBrowse_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Select Wave Directory"),
                                                 QString(QApplication::applicationDirPath()),
                                                 QFileDialog::ShowDirsOnly
                                                 | QFileDialog::DontResolveSymlinks);

    if (!dir.isEmpty()) {
        ui->txtWavePath->setText(dir);
    }
}

void SpeechRecognition::on_btnCreateMFCC_clicked()
{
    QString path = ui->txtWavePath->text();

    if (!path.isEmpty()) {
        Recognitor *recognitor = new Recognitor();

        recognitor->executeMFCC(path);
    }
}

void SpeechRecognition::on_btnCreateProto_clicked()
{
    Recognitor *recognitor = new Recognitor();

    recognitor->executeProto();
}

void SpeechRecognition::on_btnHRest_clicked()
{
    Recognitor *recognitor = new Recognitor();

    recognitor->executeHRest();
}

void SpeechRecognition::on_btnFixingSilence_clicked()
{
    Recognitor *recognitor = new Recognitor();

    recognitor->executeHHEd();
}

void SpeechRecognition::on_btnOptimizeData_clicked()
{
    Recognitor *recognitor = new Recognitor();

    recognitor->executeHVite();
}

void SpeechRecognition::on_btnCreateTriphones_clicked()
{
    Recognitor *recognitor = new Recognitor();

    recognitor->executeTriphones();
}

void SpeechRecognition::on_btnTiedTriphones_clicked()
{
    Recognitor *recognitor = new Recognitor();

    recognitor->executeTiedTriphones();
}

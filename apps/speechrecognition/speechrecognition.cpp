#include "speechrecognition.h"
#include "ui_speechrecognition.h"

#include <QAction>
#include <QFileDialog>
#include <QMessageBox>
#include <QApplication>

#include "console.h"
#include "workcase.h"
#include "executors.h"

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
        QMessageBox::critical(this, tr("Missing Data"), tr("Please select promts data for training."), QMessageBox::Ok);
        return;
    }

    if (ui->txtWavePath->text().isEmpty()) {
        QMessageBox::critical(this, tr("Missing Data"), tr("Please select wave data for training."), QMessageBox::Ok);
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
    if (ui->txtPromtTest->text().isEmpty()) {
        QMessageBox::critical(this, tr("Missing Data"), tr("Please select promts data for testing."), QMessageBox::Ok);
        return;
    }

    if (ui->txtWaveTest->text().isEmpty()) {
        QMessageBox::critical(this, tr("Missing Data"), tr("Please select wave data for testing."), QMessageBox::Ok);
        return;
    }

    executors.execPreparingDataTest(ui->txtWaveTest->text());

    executors.execTest();

    executors.execShowResult();
}

void SpeechRecognition::onCreateDictionaryFinished()
{
    console.logSuccess(tr("Create dictionary successfully."));
}

void SpeechRecognition::onTrainingDictionaryFinished()
{
    console.logSuccess(tr("Create dictionary successfully."));

    executors.execWordNet();

    executors.execMonophones();

    executors.execTranscription();

    executors.execMFCC(ui->txtWavePath->text());

    executors.execProto();

    executors.execHRest();

    executors.execHHEd();

    executors.execHVite();

    executors.execTriphones();

    executors.execTiedTriphone();

    executors.execTiedTriphones();

    console.logSuccess(tr("Training successfull!!!"));
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

        if (QFile::exists(WorkCase::currentCase()->getWorkspace() + "/text/prompts.txt")) {
            QFile::remove(WorkCase::currentCase()->getWorkspace() + "/text/prompts.txt");
        }

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

    if (!dir.exists(dst)) {
        dir.mkpath(dst);
    }

    foreach (QString dirName, dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot)) {
        QString dstPath = dst + QDir::separator() + dirName;
        dir.mkpath(dstPath);
        copyPath(src+ QDir::separator() + dirName, dstPath);
    }

    foreach (QString fileName, dir.entryList(QDir::Files)) {
        if (QFile::exists(dst + QDir::separator() + fileName)) {
            QFile::remove(dst + QDir::separator() + fileName);
        }

        QFile::copy(src + QDir::separator() + fileName, dst + QDir::separator() + fileName);
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
    executors.execWordNet();
}

void SpeechRecognition::on_btnCreateMonophone_clicked()
{
    executors.execMonophones();
}

void SpeechRecognition::on_btnCreateTranscription_clicked()
{
    executors.execTranscription();
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
        executors.execMFCC(path);
    }
}

void SpeechRecognition::on_btnCreateProto_clicked()
{
    executors.execProto();
}

void SpeechRecognition::on_btnHRest_clicked()
{
    executors.execHRest();
}

void SpeechRecognition::on_btnFixingSilence_clicked()
{
    executors.execHHEd();
}

void SpeechRecognition::on_btnOptimizeData_clicked()
{
    executors.execHVite();
}

void SpeechRecognition::on_btnCreateTriphones_clicked()
{
    executors.execTriphones();
}

void SpeechRecognition::on_btnTiedTriphones_clicked()
{
    executors.execTiedTriphone();

    executors.execTiedTriphones();
}

void SpeechRecognition::on_groupBoxTrainingStep_toggled(bool toggled)
{
    ui->frameTraining->setVisible(toggled);
}

void SpeechRecognition::on_btnPromtTest_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(this, tr("Open Promts Data"), QString(QApplication::applicationDirPath()), tr("Text File (*.txt)"));

    if (filePath.isEmpty()) {
        return;
    }

    if (QFile::exists(filePath)) {
        ui->txtPromtTest->setText(filePath);

        QDir dir(WorkCase::currentCase()->getWorkspace());

        if (!dir.exists(WorkCase::currentCase()->getWorkspace() + "/test")) {
            dir.mkpath(WorkCase::currentCase()->getWorkspace() + "/test");
        }

        if (QFile::exists(WorkCase::currentCase()->getWorkspace() + "/test/prompts.txt")) {
            QFile::remove(WorkCase::currentCase()->getWorkspace() + "/test/prompts.txt");
        }

        QFile::copy(filePath, WorkCase::currentCase()->getWorkspace() + "/test/prompts.txt");
    }
}

void SpeechRecognition::on_btnTest_clicked()
{
    executors.execTest();
}

void SpeechRecognition::on_btnWaveTest_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Select Wave Directory"),
                                                 QString(QApplication::applicationDirPath()),
                                                 QFileDialog::ShowDirsOnly
                                                 | QFileDialog::DontResolveSymlinks);

    if (!dir.isEmpty()) {
        ui->txtWaveTest->setText(dir);
    }
}

void SpeechRecognition::on_btnTestData_clicked()
{
    if (ui->txtPromtTest->text().isEmpty()) {
        QMessageBox::critical(this, tr("Missing Data"), tr("Please select promts data for testing."), QMessageBox::Ok);
        return;
    }

    if (ui->txtWaveTest->text().isEmpty()) {
        QMessageBox::critical(this, tr("Missing Data"), tr("Please select wave data for testing."), QMessageBox::Ok);
        return;
    }

    executors.execPreparingDataTest(ui->txtWaveTest->text());
}

void SpeechRecognition::on_btnResult_clicked()
{
    executors.execShowResult();
}

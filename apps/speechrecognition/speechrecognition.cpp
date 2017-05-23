#include "speechrecognition.h"
#include "ui_speechrecognition.h"

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

    QWidget::showMaximized();

    preparingData();
}

SpeechRecognition::~SpeechRecognition()
{
    delete ui;
}

QMenu *SpeechRecognition::addMenu(const QString &title)
{
    return ui->menuBar->addMenu(title);
}

void SpeechRecognition::onCreateDictionaryFinished()
{
    console.logSuccess(tr("Create dictionary successfully."));
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

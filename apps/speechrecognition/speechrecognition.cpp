#include "speechrecognition.h"
#include "ui_speechrecognition.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QApplication>

#include "console.h"
#include "typingconverter.h"
#include "aboutscreen.h"

#include "dictionarycreator.h"

using namespace VietnameseSpeechRecognition;

SpeechRecognition::SpeechRecognition(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SpeechRecognition)
{
    ui->setupUi(this);

    this->addDockWidget(Qt::BottomDockWidgetArea, console.dock());

    console.logInfo(tr("Vietnamese Speech Recognition Program !!!"));

    QWidget::showMaximized();
}

SpeechRecognition::~SpeechRecognition()
{
    delete ui;
}

void SpeechRecognition::onCreateDictionaryFinished()
{
    QMessageBox::information(this, tr("Create Dictionary"), tr("Create dictionary successfully."));

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

    if (filePath == "") {
        return;
    }

    if (QFile::exists(filePath)) {
        ui->txtPromtPath->setText(filePath);
    }
}

void SpeechRecognition::on_btnCreateDictionary_clicked()
{
    QString path = ui->txtPromtPath->text();

    if (path != "") {
        DictionaryCreator *creator = new DictionaryCreator();

        connect(creator, SIGNAL(finished()), this, SLOT(onCreateDictionaryFinished()));

        creator->execute(path);
    }
}

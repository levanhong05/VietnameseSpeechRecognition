#include "speechrecognition.h"
#include "ui_speechrecognition.h"

#include "console.h"
#include "typingconverter.h"

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

void SpeechRecognition::on_actionConvert_Typing_triggered()
{
    TypingConverter *converter = new TypingConverter();

    converter->show();
}

void SpeechRecognition::on_actionAbout_triggered()
{

}

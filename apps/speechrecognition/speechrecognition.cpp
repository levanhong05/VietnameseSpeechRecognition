#include "speechrecognition.h"
#include "ui_speechrecognition.h"

#include "typingconverter.h"

SpeechRecognition::SpeechRecognition(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::SpeechRecognition)
{
  ui->setupUi(this);
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

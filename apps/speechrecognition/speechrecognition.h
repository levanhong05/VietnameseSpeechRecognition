#ifndef SPEECHRECOGNITION_H
#define SPEECHRECOGNITION_H

#include <QMainWindow>

namespace Ui {
  class SpeechRecognition;
}

class SpeechRecognition : public QMainWindow
{
  Q_OBJECT
public:
  explicit SpeechRecognition(QWidget *parent = 0);
  ~SpeechRecognition();

private slots:
    void on_actionConvert_Typing_triggered();

private:
  Ui::SpeechRecognition *ui;
};

#endif // SPEECHRECOGNITION_H

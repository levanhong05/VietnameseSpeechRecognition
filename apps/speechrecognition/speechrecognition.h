#ifndef SPEECHRECOGNITION_H
#define SPEECHRECOGNITION_H

#include <QMainWindow>

namespace Ui {
  class SpeechRecognition;
}

namespace VietnameseSpeechRecognition {

class SpeechRecognition : public QMainWindow
{
  Q_OBJECT
public:
  explicit SpeechRecognition(QWidget *parent = 0);
  ~SpeechRecognition();

public slots:
    void onCreateDictionaryFinished();

    QMenu *addMenu(const QString &title);

private slots:
    void on_actionConvert_Typing_triggered();

    void on_actionAbout_triggered();

    void on_btnPromtBrowse_clicked();

    void on_btnCreateDictionary_clicked();

    void on_actionQuit_triggered();

    void on_btnCreateWordNet_clicked();

    void on_btnCreateMonophone_clicked();

private:
    void preparingData();

    void copyPath(QString src, QString dst);

private:
    Ui::SpeechRecognition *ui;

};

}

#endif // SPEECHRECOGNITION_H

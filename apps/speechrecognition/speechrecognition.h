#ifndef SPEECHRECOGNITION_H
#define SPEECHRECOGNITION_H

#include <QShowEvent>
#include <QMainWindow>

#include "converter.h"

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

    void onTrainingDictionaryFinished();

    QMenu *addMenu(const QString &title);

    void onSentenceDetected(QString sentence);

private slots:
    void startTrainingData();

    void startTestingData();

    void on_actionConvert_Typing_triggered();

    void on_actionAbout_triggered();

    void on_btnPromtBrowse_clicked();

    void on_btnCreateDictionary_clicked();

    void on_actionQuit_triggered();

    void on_btnCreateWordNet_clicked();

    void on_btnCreateMonophone_clicked();

    void on_btnCreateTranscription_clicked();

    void on_btnWaveBrowse_clicked();

    void on_btnCreateMFCC_clicked();

    void on_btnCreateProto_clicked();

    void on_btnHRest_clicked();

    void on_btnFixingSilence_clicked();

    void on_btnOptimizeData_clicked();

    void on_btnCreateTriphones_clicked();

    void on_btnTiedTriphones_clicked();

    void on_groupBoxTrainingStep_toggled(bool toggled);

    void on_btnPromtTest_clicked();

    void on_btnTest_clicked();

    void on_btnWaveTest_clicked();

    void on_btnTestData_clicked();

    void on_btnResult_clicked();

    void on_groupBoxTestStep_toggled(bool toggled);

    void on_rbnPhoneticModel_toggled(bool checked);

    void on_rbnLanguagModel_toggled(bool checked);

    void on_rbnBiGram_toggled(bool checked);

    void on_rbnTriGram_toggled(bool checked);

    void on_btnBuildLM_clicked();

    void on_btnRunPerplexity_clicked();

    void on_btnLMTestBrowse_clicked();

    void on_btnLMTrainBrowse_clicked();

    void on_btnIncreaseGaussian_clicked();

private:
    void preparingData();

    void copyPath(QString src, QString dst);

    void showEvent(QShowEvent *);

private:
    Ui::SpeechRecognition *ui;

    Converter *_converter;

    bool _isLanguageModel;

    bool _isTriGram;

};

}

#endif // SPEECHRECOGNITION_H

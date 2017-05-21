#ifndef TYPINGCONVERTER_H
#define TYPINGCONVERTER_H

#include <QObject>
#include <QMainWindow>

namespace Ui
{
class TypingConverter;
}

class TypingConverter : public QMainWindow
{
    Q_OBJECT

public:
    explicit TypingConverter(QWidget *parent = 0);
    ~TypingConverter();

private slots:
    void on_btnUni2Telex_clicked();

    void on_btnUni2VNI_clicked();

    void on_btnTelex2VNI_clicked();

    void on_btnTelex2Uni_clicked();

    void on_btnVNI2Uni_clicked();

    void on_btnVNI2Telex_clicked();

    void on_btnBrowse_clicked();

private:
    Ui::TypingConverter *ui;
};

#endif // TYPINGCONVERTER_H

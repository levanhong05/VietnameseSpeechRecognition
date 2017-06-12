#include "typingconverter.h"
#include "ui_typingconverter.h"

#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>

#include "converter.h"

using namespace VietnameseSpeechRecognition;

TypingConverter::TypingConverter(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::TypingConverter)
{
    ui->setupUi(this);
}

TypingConverter::~TypingConverter()
{
    delete ui;
}

void TypingConverter::on_btnUni2Telex_clicked()
{
    QString path = ui->txtFileName->text();

    if (path != "") {
        QFile file(path);

        if (!file.open(QIODevice::ReadOnly)) {
            return;
        }

        QTextStream in(&file);
        in.setCodec("UTF-8");

        QFile outFile(path.mid(0, path.lastIndexOf("/") + 1) + path.mid(path.lastIndexOf("/") + 1, path.length() - 4) + "_uni2telex.txt");
        outFile.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate);

        QTextStream out(&outFile);
        out.setCodec("UTF-8");

        QString line = "";
        Converter *convert = new Converter();

        ui->plainTextEdit->clear();

        while (!in.atEnd()) {
            line = in.readLine();

            QString result = convert->convertUnicode2Telex(line);

            out << result << endl;
            ui->plainTextEdit->appendPlainText(result + "\n");
        }

        QMessageBox::information(this, tr("Convert Data"), tr("Convert data successfully."));
    }
}

void TypingConverter::on_btnUni2VNI_clicked()
{
    QString path = ui->txtFileName->text();

    if (path != "") {
        QFile file(path);

        if (!file.open(QIODevice::ReadOnly)) {
            return;
        }

        QTextStream in(&file);
        in.setCodec("UTF-8");

        QFile outFile(path.mid(0, path.lastIndexOf("/") + 1) + path.mid(path.lastIndexOf("/") + 1, path.length() - 4) + "_uni2vni.txt");
        outFile.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate);

        QTextStream out(&outFile);
        out.setCodec("UTF-8");

        QString line = "";
        Converter *convert = new Converter();

        ui->plainTextEdit->clear();

        while (!in.atEnd()) {
            line = in.readLine();

            QString result = convert->convertUnicode2VNI(line);

            out << result << endl;
            ui->plainTextEdit->appendPlainText(result + "\n");
        }

        QMessageBox::information(this, tr("Convert Data"), tr("Convert data successfully."));
    }
}

void TypingConverter::on_btnTelex2VNI_clicked()
{
    QString path = ui->txtFileName->text();

    if (path != "") {
        QFile file(path);

        if (!file.open(QIODevice::ReadOnly)) {
            return;
        }

        QTextStream in(&file);
        in.setCodec("UTF-8");

        QFile outFile(path.mid(0, path.lastIndexOf("/") + 1) + path.mid(path.lastIndexOf("/") + 1, path.length() - 4) + "_telex2vni.txt");
        outFile.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate);

        QTextStream out(&outFile);
        out.setCodec("UTF-8");

        QString line = "";
        Converter *convert = new Converter();

        ui->plainTextEdit->clear();

        while (!in.atEnd()) {
            line = in.readLine();

            QString result = convert->convertTelex2VNI(line);

            out << result << endl;
            ui->plainTextEdit->appendPlainText(result + "\n");
        }

        QMessageBox::information(this, tr("Convert Data"), tr("Convert data successfully."));
    }
}

void TypingConverter::on_btnTelex2Uni_clicked()
{
    QString path = ui->txtFileName->text();

    if (path != "") {
        QFile file(path);

        if (!file.open(QIODevice::ReadOnly)) {
            return;
        }

        QTextStream in(&file);
        in.setCodec("UTF-8");

        QFile outFile(path.mid(0, path.lastIndexOf("/") + 1) + path.mid(path.lastIndexOf("/") + 1, path.length() - 4) + "_telex2uni.txt");
        outFile.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate);

        QTextStream out(&outFile);
        out.setCodec("UTF-8");

        QString line = "";
        Converter *convert = new Converter();

        ui->plainTextEdit->clear();

        while (!in.atEnd()) {
            line = in.readLine();

            QString result = convert->convertTelex2Unicode(line);

            out << result << endl;
            ui->plainTextEdit->appendPlainText(result + "\n");
        }

        QMessageBox::information(this, tr("Convert Data"), tr("Convert data successfully."));
    }
}

void TypingConverter::on_btnVNI2Uni_clicked()
{
    QString path = ui->txtFileName->text();

    if (path != "") {
        QFile file(path);

        if (!file.open(QIODevice::ReadOnly)) {
            return;
        }

        QTextStream in(&file);
        in.setCodec("UTF-8");

        QFile outFile(path.mid(0, path.lastIndexOf("/") + 1) + path.mid(path.lastIndexOf("/") + 1, path.length() - 4) + "_vni2uni.txt");
        outFile.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate);

        QTextStream out(&outFile);
        out.setCodec("UTF-8");

        QString line = "";
        Converter *convert = new Converter();

        ui->plainTextEdit->clear();

        while (!in.atEnd()) {
            line = in.readLine();

            QString result = convert->convertVNI2Unicode(line);

            out << result << endl;
            ui->plainTextEdit->appendPlainText(result + "\n");
        }

        QMessageBox::information(this, tr("Convert Data"), tr("Convert data successfully."));
    }
}

void TypingConverter::on_btnVNI2Telex_clicked()
{
    QString path = ui->txtFileName->text();

    if (path != "") {
        QFile file(path);

        if (!file.open(QIODevice::ReadOnly)) {
            return;
        }

        QTextStream in(&file);
        in.setCodec("UTF-8");

        QFile outFile(path.mid(0, path.lastIndexOf("/") + 1) + path.mid(path.lastIndexOf("/") + 1, path.length() - 4) + "_vni2telex.txt");
        outFile.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate);

        QTextStream out(&outFile);
        out.setCodec("UTF-8");

        QString line = "";
        Converter *convert = new Converter();

        ui->plainTextEdit->clear();

        while (!in.atEnd()) {
            line = in.readLine();

            QString result = convert->convertVNI2Telex(line);

            out << result << endl;
            ui->plainTextEdit->appendPlainText(result + "\n");
        }

        QMessageBox::information(this, tr("Convert Data"), tr("Convert data successfully."));
    }
}

void TypingConverter::on_btnBrowse_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(this, tr("Open Promts Data"), QString("D:\\Pratice\\SLP\\Thuc_hanh_2\\vivos"), tr("Text File (*.txt)"));

    if (filePath == "") {
        return;
    }

    if (QFile::exists(filePath)) {
        ui->txtFileName->setText(filePath);
    }
}

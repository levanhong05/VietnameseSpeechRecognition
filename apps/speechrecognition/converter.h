#ifndef CONVERTER_H
#define CONVERTER_H

#include <QObject>

namespace VietnameseSpeechRecognition {

class Converter
{
public:
    Converter();

    QString convertUnicode2Telex(QString string);
    QString convertVNI2Unicode(QString string);
    QString convertTelex2Unicode(QString string);
    QString convertUnicode2VNI(QString string);

    QString convertTelex2VNI(QString string);
    QString convertVNI2Telex(QString string);

private:
    QStringList _unicodeText;
    QStringList _telexText;
    QStringList _vniText;

};

}

#endif // CONVERTER_H

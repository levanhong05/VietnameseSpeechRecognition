#include "converter.h"

using namespace VietnameseSpeechRecognition;

Converter::Converter()
{
    _unicodeText = QStringList() << "ấ" << "ầ" << "ẩ" << "ẫ" << "ậ" << "â" << "á" << "à" << "ả" << "ã" << "ạ" << "ắ" << "ằ" <<
                                    "ẳ" << "ẵ" << "ặ" << "ă" << "ú" << "ù" << "ủ" << "ũ" << "ụ" << "ứ" << "ừ" << "ử" << "ữ" <<
                                    "ự" << "ư" << "ế" << "ề" << "ể" << "ễ" << "ệ" << "ê" << "é" << "è" << "ẻ" << "ẽ" << "ẹ" <<
                                    "ố" << "ồ" << "ổ" << "ỗ" << "ộ" << "ô" << "ó" << "ò" << "ỏ" << "õ" << "ọ" << "ớ" << "ờ" <<
                                    "ở" << "ỡ" << "ợ" << "ơ" << "í" << "ì" << "ỉ" << "ĩ" << "ị" << "ý" << "ỳ" << "ỷ" << "ỹ" <<
                                    "ỵ" << "đ" << "Ấ" << "Ầ" << "Ẩ" << "Ẫ" << "Ậ" << "Â" << "Á" << "À" << "Ả" << "Ã" << "Ạ" <<
                                    "Ắ" << "Ằ" << "Ẳ" << "Ẵ" << "Ặ" << "Ă" << "Ú" << "Ù" << "Ủ" << "Ũ" << "Ụ" << "Ứ" << "Ừ" <<
                                    "Ử" << "Ữ" << "Ự" << "Ư" << "Ế" << "Ề" << "Ể" << "Ễ" << "Ệ" << "Ê" << "É" << "È" << "Ẻ" <<
                                    "Ẽ" << "Ẹ" << "Ố" << "Ồ" << "Ổ" << "Ỗ" << "Ộ" << "Ô" << "Ó" << "Ò" << "Ỏ" << "Õ" << "Ọ" <<
                                    "Ớ" << "Ờ" << "Ở" << "Ỡ" << "Ợ" << "Ơ" << "Í" << "Ì" << "Ỉ" << "Ĩ" << "Ị" << "Ý" << "Ỳ" <<
                                    "Ỷ" << "Ỹ" << "Ỵ" << "Đ";

    _telexText = QStringList() << "aas" << "aaf" << "aar" << "aax" << "aaj" << "aa" << "as" << "af" << "ar" << "ax" << "aj" << "aws" << "awf" <<
                                  "awr" << "awx" << "awj" << "aw" << "us" << "uf" << "ur" << "ux" << "uj" << "uws" << "uwf" << "uwr" << "uwx" <<
                                  "uwj" << "uw" << "ees" << "eef" << "eer" << "eex" << "eej" << "ee" << "es" << "ef" << "er" << "ex" << "ej" <<
                                  "oos" << "oof" << "oor" << "oox" << "ooj" << "oo" << "os" << "of" << "or" << "ox" << "oj" << "ows" << "owf" <<
                                  "owr" << "owx" << "owj" << "ow" << "is" << "if" << "ir" << "ix" << "ij" << "ys" << "yf" << "yr" << "yx" << "yj" <<
                                  "dd" << "AAS" << "AAF" << "AAR" << "AAX" << "AAJ" << "AA" << "AS" << "AF" << "AR" << "AX" <<
                                  "AJ" << "AWS" << "AWF" << "AWR" << "AWX" << "AWJ" << "AW" << "US" << "UF" << "UR" << "UX" <<
                                  "UJ" << "UWS" << "UWF" << "UWR" << "UWX" << "UWJ" << "UW" << "EES" << "EEF" << "EER" <<
                                  "EEX" << "EEJ" << "EE" << "ES" << "EF" << "ER" << "EX" << "EJ" << "OOS" << "OOF" << "OOR" <<
                                  "OOX" << "OOJ" << "OO" << "OS" << "OF" << "OR" << "OX" << "OJ" << "OWS" << "OWF" << "OWR" <<
                                  "OWX" << "OWJ" << "OW" << "IS" << "IF" << "IR" << "IX" << "IJ" << "YS" << "YF" << "YR" << "YX" <<
                                  "YJ" << "DD";

    _vniText = QStringList() << "a61" << "a62" << "a63" << "a64" << "a65" << "a6" << "a1" << "a2" << "a3" << "a4" << "a5" << "a81" << "a82" <<
                                "a83" << "a84" << "a85" << "a8" << "u1" << "u2" << "u3" << "u4" << "u5" << "u71" << "u72" << "u73" << "u74" <<
                                "u75" << "u7" << "e61" << "e62" << "e63" << "e64" << "e65" << "e6" << "e1" << "e2" << "e3" << "e4" << "e5" <<
                                "o61" << "o62" << "o63" << "o64" << "o65" << "o6" << "o1" << "o2" << "o3" << "o4" << "o5" << "o71" << "o72" <<
                                "o73" << "o74" << "o75" << "o7" << "i1" << "i2" << "i3" << "i4" << "i5" << "y1" << "y2" << "y3" << "y4" << "y5" <<
                                "d9" << "A61" << "A62" << "A63" << "A64" << "A65" << "A6" << "A1" << "A2" << "A3" << "A4" << "A5" <<
                                "A81" << "A82" << "A83" << "A84" << "A85" << "A8" << "U1" << "U2" << "U3" << "U4" << "U5" << "U71" <<
                                "U72" << "U73" << "U74" << "U75" << "U7" << "E61" << "E62" << "E63" << "E64" << "E65" << "E6" << "E1" <<
                                "E2" << "E3" << "E4" << "E5" << "O61" << "O62" << "O63" << "O64" << "O65" << "O6" << "O1" << "O2" <<
                                "O3" << "O4" << "O5" << "O71" << "O72" << "O73" << "O74" << "O75" << "O7" << "I1" << "I2" << "I3" << "I4" <<
                                "I5" << "Y1" << "Y2" << "Y3" << "Y4" << "Y5" << "D9";
}

QString Converter::convertTelex2Unicode(QString string)
{
    for (int i = 0; i <= 133; i++) {
        string = string.replace(_telexText.at(i), _unicodeText.at(i), Qt::CaseSensitive);
    }

    return string;
}

QString Converter::convertVNI2Unicode(QString string)
{
    for (int i = 0; i <= 133; i++) {
        string = string.replace(_vniText.at(i), _unicodeText.at(i), Qt::CaseSensitive);
    }

    return string;
}

QString Converter::convertUnicode2Telex(QString string)
{
    for (int i = 0; i <= 133; i++) {
        string = string.replace(_unicodeText.at(i), _telexText.at(i), Qt::CaseSensitive);
    }

    return string;
}

QString Converter::convertUnicode2VNI(QString string)
{
    for (int i = 0; i <= 133; i++) {
        string = string.replace(_unicodeText.at(i), _vniText.at(i), Qt::CaseSensitive);
    }

    return string;
}

QString Converter::convertTelex2VNI(QString string)
{
    for (int i = 0; i <= 133; i++) {
        string = string.replace(_telexText.at(i), _vniText.at(i), Qt::CaseSensitive);
    }

    return string;
}

QString Converter::convertVNI2Telex(QString string)
{
    for (int i = 0; i <= 133; i++) {
        string = string.replace(_vniText.at(i), _telexText.at(i), Qt::CaseSensitive);
    }

    return string;
}

QT +=  gui widgets core script scripttools

unix: TARGET = foamsource
win32: TARGET = ../../../dist/bin/casesource

TEMPLATE = lib
CONFIG += plugin

SOURCES +=  casesource.cpp \
            casesourcedock.cpp \
            sourceeditor.cpp

HEADERS +=  casesource.h \
            casesourcedock.h \
            sourceeditor.h

unix: {
    INSTALLS += target
}

FORMS += \
            casesourcedock.ui \
            sourceeditor.ui

win32:LIBS += -L$$OUT_PWD/../../dist/bin -lqscintilla2
unix: LIBS += -L$$OUT_PWD/../../3rdparty/qscintilla2/Qt4Qt5 -lqscintilla2

INCLUDEPATH += ../../3rdparty/qscintilla2/Qt4Qt5
DEPENDPATH  += ../../3rdparty/qscintilla2/Qt4Qt5

win32:LIBS += -L$$OUT_PWD/../../dist/bin -lconsole
unix: LIBS += -L$$OUT_PWD/../console -lconsole

INCLUDEPATH += $$PWD/../console
DEPENDPATH += $$PWD/../console

win32:LIBS += -L$$OUT_PWD/../../dist/bin -llogger
unix: LIBS += -L$$OUT_PWD/../logger -llogger

INCLUDEPATH += $$PWD/../logger
DEPENDPATH += $$PWD/../logger

win32:LIBS += -L$$OUT_PWD/../../dist/bin -lworkspace
unix: LIBS += -L$$OUT_PWD/../workspace -lworkspace

INCLUDEPATH += $$PWD/../workspace
DEPENDPATH += $$PWD/../workspace


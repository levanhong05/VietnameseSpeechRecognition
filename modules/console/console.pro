#-------------------------------------------------
#
# Project created by QtCreator 2015-08-22T17:21:15
#
#-------------------------------------------------

QT       += gui widgets script scripttools

unix:TARGET = console
win32:TARGET = ../../../dist/bin/console

TEMPLATE = lib

DEFINES += CONSOLE_LIBRARY

SOURCES += \
    console.cpp \
    consoledock.cpp \
    htmldelegate.cpp

HEADERS +=\
    console_global.h \
    console.h \
    consoledock.h \
    htmldelegate.h

FORMS += consoledock.ui

unix {
    target.path = /usr/lib
    INSTALLS += target
}

win32: LIBS += -L$$OUT_PWD/../../dist/bin/ -llogger
unix: LIBS += -L$$OUT_PWD/../logger/ -llogger

INCLUDEPATH += $$PWD/../logger
DEPENDPATH += $$PWD/../logger

RESOURCES += \
    console_res.qrc

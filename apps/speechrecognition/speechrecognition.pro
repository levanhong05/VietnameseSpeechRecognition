#-------------------------------------------------
#
# Project created by QtCreator 2017-05-21T08:05:48
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

unix:TARGET = ../../bin/SpeechRecognition
win32:TARGET = ../../../dist/bin/SpeechRecognition

TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES +=  main.cpp\
            speechrecognition.cpp \
            converter.cpp \
            typingconverter.cpp

HEADERS  += speechrecognition.h \
            converter.h \
            typingconverter.h

FORMS    += speechrecognition.ui \
            typingconverter.ui

win32:LIBS += -L$$OUT_PWD/../../dist/bin/ -llogger
unix:LIBS += -L$$OUT_PWD/../../modules/logger/ -llogger

INCLUDEPATH += $$PWD/../../modules/logger
DEPENDPATH += $$PWD/../../modules/logger

unix:LIBS += -L$$OUT_PWD/../../modules/console -lconsole
win32:LIBS += -L$$OUT_PWD/../../dist/bin -lconsole

INCLUDEPATH += $$PWD/../../modules/console
DEPENDPATH += $$PWD/../../modules/console

win32:RC_FILE = speechrecognition.rc

RESOURCES += \
    speech_res.qrc

win32 {
    SOURCEPATH1 = $$PWD/config
    SOURCEPATH2 = $$PWD/perl
    DESTPATH1 = $$OUT_PWD/../../dist/bin/config
    DESTPATH2 = $$OUT_PWD/../../dist/bin/perl
    copydata.commands = "$(COPY_DIR) $$replace(SOURCEPATH1,/,\\) $$replace(DESTPATH1,/,\\) && $(COPY_DIR) $$replace(SOURCEPATH2,/,\\) $$replace(DESTPATH2,/,\\)"
}

unix {
    copydata.commands = "$(COPY_DIR) $$PWD/data $$OUT_PWD/../bin/"
}

first.depends = $(first) copydata
export(first.depends)
export(copydata.commands)

QMAKE_EXTRA_TARGETS += first copydata


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
            typingconverter.cpp \
            dictionarycreator.cpp \
            aboutscreen.cpp \
            application.cpp \
    waitingdialog.cpp \
    recognitor.cpp

HEADERS  += speechrecognition.h \
            converter.h \
            typingconverter.h \
            dictionarycreator.h \
            aboutscreen.h \
            application.h \
    waitingdialog.h \
    recognitor.h

FORMS    += speechrecognition.ui \
            typingconverter.ui \
            aboutscreen.ui \
    waitingdialog.ui

win32:LIBS += -L$$OUT_PWD/../../dist/bin/ -llogger
unix:LIBS += -L$$OUT_PWD/../../modules/logger/ -llogger

INCLUDEPATH += $$PWD/../../modules/logger
DEPENDPATH += $$PWD/../../modules/logger

unix:LIBS += -L$$OUT_PWD/../../modules/console -lconsole
win32:LIBS += -L$$OUT_PWD/../../dist/bin -lconsole

INCLUDEPATH += $$PWD/../../modules/console
DEPENDPATH += $$PWD/../../modules/console

win32:LIBS += -L$$OUT_PWD/../../dist/bin/ -lexecutor
unix: LIBS += -L$$OUT_PWD/../../modules/executor/ -lexecutor

INCLUDEPATH += $$PWD/../../modules/executor
DEPENDPATH += $$PWD/../../modules/executor

win32:LIBS += -L$$OUT_PWD/../../dist/bin/ -lexecutors
unix: LIBS += -L$$OUT_PWD/../../modules/executors/ -lexecutors

INCLUDEPATH += $$PWD/../../modules/executors
DEPENDPATH += $$PWD/../../modules/executors

win32:LIBS += -L$$OUT_PWD/../../dist/bin/ -lworkspace
unix: LIBS += -L$$OUT_PWD/../../modules/workspace/ -lworkspace

INCLUDEPATH += $$PWD/../../modules/workspace
DEPENDPATH += $$PWD/../../modules/workspace

win32:RC_FILE = speechrecognition.rc

RESOURCES += \
    speech_res.qrc

win32 {
    SOURCEPATH1 = $$PWD/config
    SOURCEPATH2 = $$PWD/perl
    SOURCEPATH3 = $$PWD/HTK
    SOURCEPATH4 = $$PWD/text
    SOURCEPATH5 = $$PWD/instruction
    DESTPATH1 = $$OUT_PWD/../../dist/bin/config
    DESTPATH2 = $$OUT_PWD/../../dist/bin/perl
    DESTPATH3 = $$OUT_PWD/../../dist/bin/HTK
    DESTPATH4 = $$OUT_PWD/../../dist/bin/text
    DESTPATH5 = $$OUT_PWD/../../dist/bin/instruction
    copydata.commands = "$(COPY_DIR) $$replace(SOURCEPATH1,/,\\) $$replace(DESTPATH1,/,\\) && $(COPY_DIR) $$replace(SOURCEPATH2,/,\\) $$replace(DESTPATH2,/,\\) && $(COPY_DIR) $$replace(SOURCEPATH3,/,\\) $$replace(DESTPATH3,/,\\) && $(COPY_DIR) $$replace(SOURCEPATH4,/,\\) $$replace(DESTPATH4,/,\\) && $(COPY_DIR) $$replace(SOURCEPATH5,/,\\) $$replace(DESTPATH5,/,\\)"
}

unix {
    copydata.commands = "$(COPY_DIR) $$PWD/config $$OUT_PWD/../bin/ && $(COPY_DIR) $$PWD/perl $$OUT_PWD/../bin/ && $(COPY_DIR) $$PWD/HTK $$OUT_PWD/../bin/ && $(COPY_DIR) $$PWD/text $$OUT_PWD/../bin/ && $(COPY_DIR) $$PWD/instruction $$OUT_PWD/../bin/"
}

first.depends = $(first) copydata
export(first.depends)
export(copydata.commands)

QMAKE_EXTRA_TARGETS += first copydata


QT +=  gui widgets core script scripttools

unix:TARGET = executors
win32:TARGET = ../../../dist/bin/executors

TEMPLATE = lib

DEFINES +=  EXECUTORS_LIBRARY

SOURCES +=  executors.cpp \
            waitingdialog.cpp \
    runlogwidget.cpp

HEADERS +=  executors.h \
            executors_global.h \
            waitingdialog.h \
    runlogwidget.h

FORMS += \
            waitingdialog.ui \
    runlogwidget.ui

win32:LIBS += -L$$OUT_PWD/../../dist/bin -lexecutor
unix: LIBS += -L$$OUT_PWD/../../modules/executor -lexecutor

INCLUDEPATH += $$PWD/../executor
DEPENDPATH += $$PWD/../executor

win32:LIBS += -L$$OUT_PWD/../../dist/bin -llogger
unix: LIBS += -L$$OUT_PWD/../../modules/logger -llogger

INCLUDEPATH += $$PWD/../logger
DEPENDPATH += $$PWD/../logger

win32:LIBS += -L$$OUT_PWD/../../dist/bin -lconsole
unix: LIBS += -L$$OUT_PWD/../../modules/console -lconsole

INCLUDEPATH += $$PWD/../console
DEPENDPATH += $$PWD/../console

win32:LIBS += -L$$OUT_PWD/../../dist/bin -lworkspace
unix: LIBS += -L$$OUT_PWD/../../modules/workspace -lworkspace

INCLUDEPATH += $$PWD/../workspace
DEPENDPATH += $$PWD/../workspace

RESOURCES += \
    executors_res.qrc


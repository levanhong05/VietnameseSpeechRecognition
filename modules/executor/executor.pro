#-------------------------------------------------
#
# Project created by QtCreator 2015-08-11T14:31:31
#
#-------------------------------------------------

QT       += gui core widgets script scripttools

unix:TARGET = executor
win32:TARGET = ../../../dist/bin/executor

TEMPLATE = lib

DEFINES +=  EXECUTOR_LIBRARY

SOURCES +=  executor.cpp

HEADERS +=  executor.h\
            executor_global.h

symbian {
    MMP_RULES += EXPORTUNFROZEN
    TARGET.UID3 = 0xE35E42D4
    TARGET.CAPABILITY =
    TARGET.EPOCALLOWDLLDATA = 1
    addFiles.sources = executor.dll
    addFiles.path = !:/sys/bin
    DEPLOYMENT += addFiles
}

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}

win32: LIBS += -L$$OUT_PWD/../../dist/bin/ -llogger
else:unix: LIBS += -L$$OUT_PWD/../logger/ -llogger

INCLUDEPATH += $$PWD/../logger
DEPENDPATH += $$PWD/../logger

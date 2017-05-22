QT       += core gui widgets script scripttools

unix:TARGET = workspace
win32:TARGET = ../../../dist/bin/workspace

TEMPLATE = lib

DEFINES += WORKSPACE_LIBRARY

HEADERS += \
    workspace_global.h \
    workcase.h

SOURCES += \
    workcase.cpp

unix: QMAKE_CXXFLAGS += -std=c++0x


win32: LIBS += -L$$OUT_PWD/../../dist/bin/ -llogger
unix: LIBS += -L$$OUT_PWD/../logger/ -llogger

INCLUDEPATH += $$PWD/../logger
DEPENDPATH += $$PWD/../logger

win32: LIBS += -L$$OUT_PWD/../../dist/bin/ -lconsole
unix: LIBS += -L$$OUT_PWD/../console/ -lconsole

INCLUDEPATH += $$PWD/../console
DEPENDPATH += $$PWD/../console

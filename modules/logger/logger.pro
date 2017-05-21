#-------------------------------------------------
#
# Project created by QtCreator 2015-08-11T14:36:16
#
#-------------------------------------------------

unix:TARGET = logger
win32:TARGET = ../../../dist/bin/logger

TEMPLATE = lib

DEFINES += LOGGER_LIBRARY

QT += core gui widgets

SOURCES += \
    logger.cpp \
    loglistmodel.cpp \
    loglistwidget.cpp

HEADERS +=\
    logger_global.h \
    logger.h \
    loglistmodel.h \
    loglistwidget.h


#LIBS += -L$$OUT_PWD/../../3rdparty/log4cpp/src/.libs -llog4cpp
unix:LIBS += -Wl,--whole-archive $$OUT_PWD/../../3rdparty/log4cpp/src/.libs/liblog4cpp.a -Wl,--no-whole-archive
#win32:LIBS += -Wl,--whole-archive $$OUT_PWD/../../3rdparty/log4cpp/msvc12/log4cpp/x64/Debug/log4cpp.lib -Wl,--no-whole-archive -lws2_32
win32:LIBS += -L$$OUT_PWD/../../dist/bin/ -llog4cpp

INCLUDEPATH += $$OUT_PWD/../../3rdparty/log4cpp/include
DEPENDPATH += $$OUT_PWD/../../3rdparty/log4cpp/include

symbian {
    MMP_RULES += EXPORTUNFROZEN
    TARGET.UID3 = 0xE4AA4BBF
    TARGET.CAPABILITY =
    TARGET.EPOCALLOWDLLDATA = 1
    addFiles.sources = logger.dll
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

FORMS += \
    loglistwidget.ui

RESOURCES += \
    logger_res.qrc


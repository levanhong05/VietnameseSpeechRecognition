TEMPLATE = aux

CONFIG -= debug_and_release

unix {
    liblog4cpp.targets = src/.libs/liblog4cpp.a
    liblog4cpp.commands = echo Compiling log4cpp 3rdparty library && cp -rn $$PWD/* . && cd $$OUT_PWD && ./autogen.sh && ./configure --with-pic --with-gnu-ld && make
    liblog4cpp.depends = Makefile

    QMAKE_CLEAN += src/.libs/liblog4cpp.a
    QMAKE_EXTRA_TARGETS += liblog4cpp
    TARGET = liblog4cpp
}

win32 {
    SOURCEPATH=$$PWD

CONFIG(release, debug|release){
    liblog4cpp.commands = "$(COPY_DIR) $$replace(SOURCEPATH,/,\\) && call build_release.bat"
    liblog4cpp.depends = Makefile

    first.targets = msvc12/log4cpp/x64/Release/log4cpp.dll
    first.depends = liblog4cpp

    QMAKE_CLEAN += msvc12/log4cpp/x64/Release/log4cpp.dll
} else {
    liblog4cpp.commands = "$(COPY_DIR) $$replace(SOURCEPATH,/,\\) && call build.bat"
    liblog4cpp.depends = Makefile

    first.targets = msvc12/log4cpp/x64/Debug/log4cpp.dll
    first.depends = liblog4cpp

    QMAKE_CLEAN += msvc12/log4cpp/x64/Debug/log4cpp.dll
}
    QMAKE_EXTRA_TARGETS += first liblog4cpp
    TARGET = first liblog4cpp
}

OTHER_FILES += \
    build.bat

DISTFILES += \
    build_release.bat

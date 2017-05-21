TEMPLATE = aux

CONFIG -= debug_and_release

win32 {
    SOURCEPATH=$$PWD

    htk.commands = "$(COPY_DIR) $$replace(SOURCEPATH,/,\\) && call build.bat"
    htk.depends = Makefile

    first.depends = htk

    QMAKE_EXTRA_TARGETS += first htk
    TARGET = first htk
}

OTHER_FILES += \
    build.bat

DISTFILES +=

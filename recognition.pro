TEMPLATE = subdirs

# CONFIG += ordered

CONFIG += USING_HTK_BUILD_TOOLS

USING_HTK_LIBS_TOOLS {
    SUBDIRS += \
    3rdparty \
}

SUBDIRS += \
    apps

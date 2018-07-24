CONFIG += ordered
TEMPLATE = subdirs

SUBDIRS += \
    $$PWD/apps \
    $$PWD/share

OTHER_FILES += \
        $$PWD/README.md \
        $$PWD/LICENSE

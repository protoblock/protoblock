CONFIG += ordered
TEMPLATE = subdirs

message(p $$CONFIG )

SUBDIRS += \
    $$PWD/apps \
    $$PWD/share

OTHER_FILES += \
        $$PWD/README.md \
        $$PWD/LICENSE

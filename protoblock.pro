CONFIG += ordered
TEMPLATE = subdirs

SUBDIRS += \
    $$PWD/apps 
    $$PWD/share


OTHER_FILES += \
        $$PWD/README.md \
        $$PWD/.gitignore \
        $$PWD/TODO \
        $$PWD/pri/defs.pri \
        $$PWD/pri/deps.pri \
        $$PWD/pri/core.pri \
        $$PWD/pri/genproto.pri \
        $$PWD/pri/protos.pri

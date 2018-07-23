TEMPLATE = subdirs
CONFIG += ordered
SUBDIRS += \
    $$PWD/protoblockd
#    $$PWD/protoblock-qt


OTHER_FILES += \
    $$PWD/protoblockd/deps.pri \
    $$PWD/defs.pri


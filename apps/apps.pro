TEMPLATE = subdirs
CONFIG += ordered
SUBDIRS += \
    $$PWD/protoblockd \
    $$PWD/protoblock-qt \

#    $$PWD/fantasybit-server \
#    $$PWD/dataagent-qt \
#    $$PWD/wsserver \
#    $$PWD/fantasybit-lite


OTHER_FILES += \
    $$PWD/protoblockd/deps.pri \
    $$PWD/defs.pri

macx {
#    message (apps.pro 10.14)
#    QMAKE_MAC_SDK = macosx10.14
}

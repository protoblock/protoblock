include($$PWD/../../pri/protos.pri)
include($$PWD/../../pri/deps.pri)
include($$PWD/../../pri/macrosAndModels.pri)

win32 {
    include($$PWD/../../pri/core.pri)
}

android {
    include($$PWD/../../pri/core.pri)
}

ios{
    CXX_MODULE=qml
    QMAKE_MOC_OPTIONS += -Muri=ProRotoQml.Protoblock
}


macx {
    include($$PWD/../../pri/core.pri)

//LIBS += -L$$OUT_PWD/../protoblock-core -lprotoblock-core
}
#INCLUDEPATH += $$PWD/../protoblock-core
#DEPENDPATH += $$PWD/../protoblock-core
#INCLUDEPATH += $$PWD/../protoblock-core/bitcoin-core-base58


TEMPLATE = lib
TARGETPATH=ProRotoQml/Protoblock
TARGET = ProRotoQml.Protoblock

uri = ProRotoQml.Protoblock

QT += qml quick network websockets core
CONFIG += qt plugin c++11 warn_off
TARGET = $$qtLibraryTarget($$TARGET)
uri = ProRotoQml.Protoblock

DEFINES += USE_NUM_NONE
DEFINES += USE_FIELD_10X26
DEFINES += USE_FIELD_INV_BUILTIN
DEFINES += USE_SCALAR_8X32
DEFINES += USE_SCALAR_INV_BUILTIN
#DEFINES += __cplusplus
DEFINES += TRACE

SOURCES += \
#    $$PWD/getuserinfo.cpp \
    $$PWD/plugin.cpp \
    $$PWD/socketclient.cpp \
    $$PWD/mediator.cpp \
    $$PWD/playerquoteslicemodel.cpp \
    depthmarketmodel.cpp

HEADERS += \
#    $$PWD/getuserinfo.h \
    $$PWD/plugin.h \
    $$PWD/socketclient.h \
    $$PWD/mediator.h \
    playerquoteslicemodel.h \
    depthmarketmodel.h \
    fantasynamemodel.h \
    openordersmodel.h


DISTFILES = qmldir

!equals(_PRO_FILE_PWD_, $$OUT_PWD) {
    copy_qmldir.target = $$OUT_PWD/qmldir
    copy_qmldir.depends = $$_PRO_FILE_PWD_/qmldir
    copy_qmldir.commands = $(COPY_FILE) \"$$replace(copy_qmldir.depends, /, $$QMAKE_DIR_SEP)\" \"$$replace(copy_qmldir.target, /, $$QMAKE_DIR_SEP)\"
    QMAKE_EXTRA_TARGETS += copy_qmldir
    PRE_TARGETDEPS += $$copy_qmldir.target
}

qmldir.files = qmldir

installPath = $$[QT_INSTALL_QML]/$$replace(uri, \\., /)
qmldir.path = $$installPath
target.path = $$installPath
INSTALLS += target qmldir

#QOIL = $$[QT_INSTALL_LIBS]
#win32:{
#message(forlder  $$QOIL protoblock-core)
#    LIBS += -L$$[QT_INSTALL_LIBS]/ -lprotoblock-core
#        LIBS += -L./../../protoblock-core/release \
#              -lprotoblock-core
#         INCLUDEPATH += ./../../protoblock-core
#LIBS += -LD:\Qt\5.6\msvc2013_64\lib\protoblock-core.dll
#}

#INCLUDEPATH += $$PWD/bitcoin-core-base58

#else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../protoblock-core/debug/ -lprotoblock-core
















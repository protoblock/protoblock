include($$PWD/../../pri/protos.pri)
include($$PWD/../../pri/deps.pri)
include($$PWD/../../pri/macrosAndModels.pri)
ios{
    CXX_MODULE=qml
    QMAKE_MOC_OPTIONS += -Muri=ProRotoQml.Protoblock
}

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
    $$PWD/utils/utils.cpp \
    $$PWD/appsettings.cpp \
    $$PWD/FantasyName.cpp \
    $$PWD/FantasyAgent.cpp \
    $$PWD/mnemonic.cpp \
    $$PWD/Commissioner.cpp \
    $$PWD/platform.cpp \
    $$PWD/getuserinfo.cpp \
    $$PWD/bitcoin-core-base58/base58.cpp \
    $$PWD/city.cpp \
    $$PWD/crc.cpp \
    $$PWD/plugin.cpp \
    $$PWD/socketclient.cpp \
    $$PWD/mediator.cpp



HEADERS += \
    $$PWD/FantasyAgent.h \
    $$PWD/Commissioner.h \
    $$PWD/getuserinfo.h \
    $$PWD/appsettings.h \
    $$PWD/DataPersist.h \
    $$PWD/FantasyName.h \
    $$PWD/fbutils.h \
    $$PWD/genericsingleton.h \
    $$PWD/globals.h \
    $$PWD/mnemonic.h \
    $$PWD/platform.h \
    $$PWD/bitcoin-core-base58/allocators.h \
    $$PWD/bitcoin-core-base58/base58.h \
    $$PWD/bitcoin-core-base58/hash.h \
    $$PWD/utils/utils.h \
    $$PWD/city.hpp \
    $$PWD/optional.hpp \
    $$PWD/uint128.hpp \
    $$PWD/utility.hpp \
    $$PWD/plugin.h \
    $$PWD/socketclient.h \
    $$PWD/mediator.h

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

## Extra Includes
INCLUDEPATH += $$PWD/bitcoin-core-base58
#                                  $$PWD/../../../PhoneApp/TradingFootball2015


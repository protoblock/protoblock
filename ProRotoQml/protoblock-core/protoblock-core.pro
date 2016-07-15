include($$PWD/../../pri/protos.pri)
include($$PWD/../../pri/deps.pri)

QT  += network sql
CONFIG +=  c++11 warn_off

TARGET = protoblock-core
TEMPLATE = lib

DEFINES += USE_NUM_NONE
DEFINES += USE_FIELD_10X26
DEFINES += USE_FIELD_INV_BUILTIN
DEFINES += USE_SCALAR_8X32
DEFINES += USE_SCALAR_INV_BUILTIN
#DEFINES += __cplusplus
DEFINES += TRACE

DEFINES += PROTOBLOCKCORE_LIBRARY
INCLUDEPATH += $$PWD/bitcoin-core-base58

SOURCES += protoblockcore.cpp \
    FantasyAgent.cpp \
    appsettings.cpp \
    city.cpp \
    Commissioner.cpp \
    crc.cpp \
    FantasyName.cpp \
    mnemonic.cpp \
    platform.cpp \
    bitcoin-core-base58/base58.cpp \
    utils/utils.cpp

HEADERS += protoblockcore.h\
        protoblock-core_global.h \
    appsettings.h \
    city.hpp \
    Commissioner.h \
    DataPersist.h \
    FantasyAgent.h \
    FantasyName.h \
    fbutils.h \
    genericsingleton.h \
    globals.h \
    mnemonic.h \
    optional.hpp \
    platform.h \
    uint128.hpp \
    utility.hpp \
    bitcoin-core-base58/allocators.h \
    bitcoin-core-base58/base58.h \
    bitcoin-core-base58/hash.h \
    utils/utils.h

#unix {
#    target.path = /usr/lib
#    INSTALLS += target
#}

include($$PWD/../../pri/protos.pri)
include($$PWD/../../pri/deps.pri)
include($$PWD/../../pri/macrosAndModels.pri)
QT += core websockets network
QT -= gui

CONFIG += c++11 warn_off
TARGET = WSServer
CONFIG += console
CONFIG -= app_bundle
TEMPLATE = app
OTHER_FILES += $$PWD/sslechoclient.html

DEFINES += USE_NUM_NONE
DEFINES += USE_FIELD_10X26
DEFINES += USE_FIELD_INV_BUILTIN
DEFINES += USE_SCALAR_8X32
DEFINES += USE_SCALAR_INV_BUILTIN

INCLUDEPATH  += $$PWD/../../ProRotoQml/Protoblock
INCLUDEPATH  += $$PWD/../../ProRotoQml/Protoblock/bitcoin-core-base58

SOURCES   += \
#    $$PWD/../../ProRotoQml/Backend/src/ldbhashreadertool.cpp \
#    $$PWD/../../ProRotoQml/Backend/src/maps/ldbhashreadermap.cpp \
#    $$PWD/../../ProRotoQml/Backend/src/tempapi.cpp \
    $$PWD/main.cpp \
    $$PWD/server.cpp \
    $$PWD/../../ProRotoQml/Protoblock/bitcoin-core-base58/base58.cpp \
    $$PWD/../../ProRotoQml/Protoblock/utils/utils.cpp \
    $$PWD/../../ProRotoQml/Protoblock/appsettings.cpp \
    $$PWD/../../ProRotoQml/Protoblock/city.cpp \
    $$PWD/../../ProRotoQml/Protoblock/crc.cpp \
    $$PWD/../../ProRotoQml/Protoblock/FantasyAgent.cpp \
    $$PWD/../../ProRotoQml/Protoblock/FantasyName.cpp \
    $$PWD/../../ProRotoQml/Protoblock/mnemonic.cpp \
    $$PWD/../../ProRotoQml/Protoblock/platform.cpp \
    $$PWD/../../ProRotoQml/Protoblock/Commissioner.cpp \
    $$PWD/NameData.cpp

HEADERS += \
#    $$PWD/../../ProRotoQml/Backend/src/ldbhashreadertool.h \
#    $$PWD/../../ProRotoQml/Backend/src/maps/ldbhashreadermap.h \
#    $$PWD/../../ProRotoQml/Backend/src/tempapi.h \
#    $$PWD/../../ProRotoQml/Backend/src/models/playernewmodel.h \
    $$PWD/server.h \
    $$PWD/../../ProRotoQml/Protoblock/bitcoin-core-base58/allocators.h \
    $$PWD/../../ProRotoQml/Protoblock/bitcoin-core-base58/base58.h \
    $$PWD/../../ProRotoQml/Protoblock/bitcoin-core-base58/hash.h \
    $$PWD/../../ProRotoQml/Protoblock/utils/utils.h \
    $$PWD/../../ProRotoQml/Protoblock/appsettings.h \
    $$PWD/../../ProRotoQml/Protoblock/city.hpp \
    $$PWD/../../ProRotoQml/Protoblock/DataPersist.h \
    $$PWD/../../ProRotoQml/Protoblock/FantasyAgent.h \
    $$PWD/../../ProRotoQml/Protoblock/FantasyName.h \
    $$PWD/../../ProRotoQml/Protoblock/fbutils.h \
    $$PWD/../../ProRotoQml/Protoblock/genericsingleton.h \
    $$PWD/../../ProRotoQml/Protoblock/globals.h \
    $$PWD/../../ProRotoQml/Protoblock/mnemonic.h \
    $$PWD/../../ProRotoQml/Protoblock/optional.hpp \
    $$PWD/../../ProRotoQml/Protoblock/platform.h \
    $$PWD/../../ProRotoQml/Protoblock/uint128.hpp \
    $$PWD/../../ProRotoQml/Protoblock/utility.hpp \
    $$PWD/../../ProRotoQml/Protoblock/Commissioner.h \
    $$PWD/NameData.h



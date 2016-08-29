include($$PWD/../../pri/protos.pri)
include($$PWD/../../pri/deps.pri)

QT += core network
QT -= gui

CONFIG += c++11

TARGET = fantasybit-2016
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app


DEFINES += USE_NUM_NONE
DEFINES += USE_FIELD_10X26
DEFINES += USE_FIELD_INV_BUILTIN
DEFINES += USE_SCALAR_8X32
DEFINES += USE_SCALAR_INV_BUILTIN
DEFINES += TRACE

DEFINES += TRADE_FEATURE


win32 {
    include($$PWD/../../pri/core.pri)
}


INCLUDEPATH  += $$PWD/../../ProRotoQml/Protoblock
INCLUDEPATH  += $$PWD/../../ProRotoQml/Protoblock/bitcoin-core-base58

SOURCES += \
    main.cpp \
    $$PWD/Data.cpp \
    $$PWD/DistributionAlgo.cpp \
#    $$PWD/FantasyName.cpp \
    $$PWD/Processor.cpp \
#    $$PWD/Source.cpp \
    $$PWD/RestFullCall.cpp \
#    $$PWD/LAPIWorker.cpp \
#    $$PWD/NodeWorker.cpp \
    $$PWD/PeerNode.cpp \
    $$PWD/blockrecorder.cpp \
#    $$PWD/platform.cpp \
#    $$PWD/appsettings.cpp \
#    $$PWD/core.cpp \
#    $$PWD/dataservice.cpp \
#    $$PWD/tablemodels.cpp \
#    $$PWD/RunGuard.cpp \
#    $$PWD/FantasyAgent.cpp \
#    $$PWD/timestate.cpp \
    $$PWD/ExchangeData.cpp \
#    $$PWD/trading.cpp \
    $$PWD/NameData.cpp \
    $$PWD/core.cpp

HEADERS += \
    $$PWD/PeerNode.h \
    pbutils.h \
    data.h \
    core.h \
    $$PWD/NameData.h \
    $$PWD/Processor.h \
    $$PWD/ExchangeData.h \
    $$PWD/RestFullCall.h

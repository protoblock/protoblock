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
#DEFINES += ONE_TIME_PROD
#DEFINES += ONE_TIME_PROD_SEASON_TRADING
#DEFINES += PRODFOOTBALL
#DEFINES += USE_LOCALHOST_SERVER
DEFINES += NO_WRITE_NAMES_2015_FIRST
DEFINES += PROD_SEASON_TRADING
DEFINES += SEASON_TRADING
DEFINES += MAX_NAMES_LB=1000
DEFINES += TRACE


win32 {
    include($$PWD/../../pri/core.pri)
}


INCLUDEPATH  += $$PWD/../../ProRotoQml/Protoblock
INCLUDEPATH  += $$PWD/../../ProRotoQml/Protoblock/bitcoin-core-base58

SOURCES   += \
    $$PWD/main.cpp \
    $$PWD/server.cpp \
    $$PWD/NameData.cpp \
    $$PWD/Data.cpp \
    $$PWD/txpool.cpp \
    $$PWD/ExchangeData.cpp \
    LiteServer.cpp \
    TxServer.cpp \
    combinations.cpp



HEADERS += \
    $$PWD/server.h \
    $$PWD/NameData.h \
    $$PWD/Data.h \
    $$PWD/txpool.h \
    $$PWD/ExchangeData.h \
    LiteServer.h \
    TxServer.h

## FIXME add in libs here for levelDB as it is the only thing ATM that is using it.
macx{
        ## FIXME compile levelDB
        LIBS += -L/usr/local/lib  -lleveldb

        ##FIXME compile levelDB
        INCLUDEPATH += /usr/local/Cellar/leveldb/1.18/include
        DEPENDPATH += /usr/local/Cellar/leveldb/1.18/include

        LIBS += -L$$OUT_PWD/../../ProRotoQml/jsonpb/ -ljsonpb

        INCLUDEPATH += $$PWD/../../ProRotoQml/jsonpb
        DEPENDPATH += $$PWD/../../ProRotoQml/jsonpb

        LIBS += -L$$OUT_PWD/../../ProRotoQml/protoblock-core/ -lprotoblock-core
        INCLUDEPATH += $$PWD/../../ProRotoQml/protoblock-core/
        DEPENDPATH += $$PWD/../../ProRotoQml/protoblock-core/
        INCLUDEPATH += $$PWD/../../ProRotoQml/protoblock-core/bitcoin-core-base58
        DEPENDPATH += $$PWD/../../ProRotoQml/protoblock-core/bitcoin-core-base58
}




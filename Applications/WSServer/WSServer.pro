#include($$PWD/../../pri/protos.pri)
include($$PWD/../../pri/deps.pri)
#include($$PWD/../../pri/macrosAndModels.pri)
QT += core websockets network qml
QT -= gui

CONFIG += c++11 warn_off
TARGET = WSServer
CONFIG += console
CONFIG -= app_bundle
TEMPLATE = app
OTHER_FILES += $$PWD/sslechoclient.html

#DEFINES += TESTING_ONLY
#DEFINES += USE_NUM_NONE
#DEFINES += USE_FIELD_10X26
#DEFINES += USE_FIELD_INV_BUILTIN
#DEFINES += USE_SCALAR_8X32
#DEFINES += USE_SCALAR_INV_BUILTIN
##DEFINES += ONE_TIME_PROD
#DEFINES += ONE_TIME_PROD_SEASON_TRADING
#DEFINES += PRODFOOTBALL
##DEFINES += USE_LOCALHOST_SERVER
#DEFINES += NO_WRITE_NAMES_2015_FIRST
#DEFINES += PROD_SEASON_TRADING
#DEFINES += SEASON_TRADING
DEFINES += MAX_NAMES_LB=1000
#DEFINES += TRACE

#DEFINES += WSSERVER_WRITE_TWEET
contains(DEFINES, WSSERVER_WRITE_TWEET) {
    INCLUDEPATH += $$DIRPREFIX/windows/3rdParty/nanomsg
    LIBS += -lnanomsg
}

#win32 {
#    include($$PWD/../../pri/core.pri)
#}


#INCLUDEPATH  += $$PWD/../../ProRotoQml/Protoblock
#INCLUDEPATH  += $$PWD/../../ProRotoQml/Protoblock/bitcoin-core-base58

SOURCES   += \
    $$PWD/main.cpp \
    $$PWD/server.cpp \
#    $$PWD/NameData.cpp \
#    $$PWD/Data.cpp \
#    $$PWD/txpool.cpp \
#    $$PWD/ExchangeData.cpp \
    LiteServer.cpp
#    TxServer.cpp \
#    combinations.cpp



HEADERS += \
    $$PWD/server.h \
#    $$PWD/NameData.h \
#    $$PWD/Data.h \
#    $$PWD/txpool.h \
#    $$PWD/ExchangeData.h \
    LiteServer.h \
    $$PWD/wsgateway.h
#    TxServer.h

## FIXME add in libs here for levelDB as it is the only thing ATM that is using it.
#macx{
#        ## FIXME compile levelDB
#        LIBS += -L/usr/local/lib  -lleveldb

#        ##FIXME compile levelDB
#        INCLUDEPATH += /usr/local/Cellar/leveldb/1.18/include
#        DEPENDPATH += /usr/local/Cellar/leveldb/1.18/include

#        LIBS += -L$$OUT_PWD/../../ProRotoQml/jsonpb/ -ljsonpb

#        INCLUDEPATH += $$PWD/../../ProRotoQml/jsonpb
#        DEPENDPATH += $$PWD/../../ProRotoQml/jsonpb

#        LIBS += -L$$OUT_PWD/../../ProRotoQml/protoblock-core/ -lprotoblock-core
#        INCLUDEPATH += $$PWD/../../ProRotoQml/protoblock-core/
#        DEPENDPATH += $$PWD/../../ProRotoQml/protoblock-core/
#        INCLUDEPATH += $$PWD/../../ProRotoQml/protoblock-core/bitcoin-core-base58
#        DEPENDPATH += $$PWD/../../ProRotoQml/protoblock-core/bitcoin-core-base58
#}

FANTASYBITLIB += fantasybit-2016D

!contains(DEFINES, PRODFOOTBALL){
    FANTASYBITLIB = STAGING-$${FANTASYBITLIB}
}

contains(DEFINES, DATAAGENTDEFS){
    FANTASYBITLIB = ERROR-$${FANTASYBITLIB}
}

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../fantasybit-2016/release/ -l$$FANTASYBITLIB
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../fantasybit-2016/debug/ -l$$FANTASYBITLIB
else:macx: LIBS += -L$$OUT_PWD/../fantasybit-2016/ -l$$FANTASYBITLIB

INCLUDEPATH += $$PWD/../fantasybit-2016
DEPENDPATH += $$PWD/../fantasybit-2016

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../fantasybit-2016/release/lib$${FANTASYBITLIB}.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../fantasybit-2016/debug/lib$${FANTASYBITLIB}.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../fantasybit-2016/release/$${FANTASYBITLIB}.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../fantasybit-2016/debug/$${FANTASYBITLIB}.lib
else:macx: PRE_TARGETDEPS += $$OUT_PWD/../fantasybit-2016/lib$${FANTASYBITLIB}.a

INCLUDEPATH  += $$PWD/../../ProRotoQml/Protoblock
INCLUDEPATH +=  $$PWD/../../ProRotoQml/protoblock-core
INCLUDEPATH +=  $$PWD/../../ProRotoQml/protoblock-core/bitcoin-core-base58
INCLUDEPATH +=  $$PWD/../../ProRotoQml/QmlModels
INCLUDEPATH +=  $$PWD/../../ProRotoQml/QmlSuperMacros
INCLUDEPATH +=  $$PWD/../../../prebuiltLibs/windows/3rdParty
INCLUDEPATH += $$PWD/../../../prebuiltLibs/windows/3rdParty/secp256k1

include ($$PWD/../../pri/genproto.pri)

#message(win32 $$INCLUDEPATH)

#win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../ProRotoQml/Protoblock/release/ -lProRotoQml.Protoblockd
#else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../ProRotoQml/Protoblock/debug/ -lProRotoQml.Protoblockd
#else:unix: LIBS += -L$$OUT_PWD/../../ProRotoQml/Protoblock/ -lProRotoQml.Protoblockd

INCLUDEPATH += $$PWD/../../ProRotoQml/Protoblock
DEPENDPATH += $$PWD/../../ProRotoQml/Protoblock

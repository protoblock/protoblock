include($$PWD/../../pri/protos.pri)
include($$PWD/../../pri/deps.pri)
include($$PWD/../../pri/macrosAndModels.pri)

#QT -= gui
QT += core network websockets
CONFIG += qt c++11 static

#CONFIG += c++11

TARGET = fantasybit-2016D
#CONFIG += qt console
CONFIG -= app_bundle

TEMPLATE = lib



DEFINES += TRACE

DEFINES += TRADE_FEATURE


win32 {
    include($$PWD/../../pri/core.pri)
}


#INCLUDEPATH  += $$PWD/../../ProRotoQml/Protoblock
INCLUDEPATH +=  $$PWD/../../ProRotoQml/protoblock-core
INCLUDEPATH +=  $$PWD/../../ProRotoQml/protoblock-core/bitcoin-core-base58
INCLUDEPATH +=  $$PWD/../../ProRotoQml/QmlModels
INCLUDEPATH +=  $$PWD/../../ProRotoQml/QmlSuperMacros

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
    $$PWD/core.cpp \
    $$PWD/LAPIWorker.cpp \
    $$PWD/NodeWorker.cpp \
    $$PWD/iresolvable.cpp \
    $$PWD/dataservice.cpp
#    fullgateway.cpp



HEADERS += \
    $$PWD/PeerNode.h \
    pbutils.h \
    data.h \
    core.h \
    $$PWD/NameData.h \
    $$PWD/Processor.h \
    $$PWD/ExchangeData.h \
    $$PWD/RestFullCall.h \
    $$PWD/LAPIWorker.h \
    $$PWD/NodeWorker.h \
    $$PWD/threadedqobject.h \
    $$PWD/iresolvable.h \
    $$PWD/dataservice.h
#    fullgateway.h



#INCLUDEPATH += $$PWD/../../ProRotoQml/protoblock

SOURCES += \
##    $$PWD/getuserinfo.cpp \
##    $$PWD/plugin.cpp \
    $$PWD/../../ProRotoQml/protoblock/socketclient.cpp \
    $$PWD/../../ProRotoQml/protoblock/mediator.cpp \
    $$PWD/../../ProRotoQml/protoblock/playerquoteslicemodel.cpp \
    $$PWD/../../ProRotoQml/protoblock/depthmarketmodel.cpp \
    $$PWD/../../ProRotoQml/protoblock/sortfilterproxymodel.cpp


HEADERS += \
#    $$PWD/getuserinfo.h \
#    $$PWD/plugin.h \
    $$PWD/../../ProRotoQml/protoblock/socketclient.h \
    $$PWD/../../ProRotoQml/protoblock/mediator.h \
    $$PWD/../../ProRotoQml/protoblock/playerquoteslicemodel.h \
    $$PWD/../../ProRotoQml/protoblock/depthmarketmodel.h \
    $$PWD/../../ProRotoQml/protoblock/fantasynamemodel.h \
    $$PWD/../../ProRotoQml/protoblock/openordersmodel.h \
    $$PWD/../../ProRotoQml/protoblock/weeklyschedulemodel.h \
    $$PWD/../../ProRotoQml/protoblock/playerprojmodel.h \
    $$PWD/../../ProRotoQml/protoblock/sortfilterproxymodel.h \
    $$PWD/../../ProRotoQml/protoblock/pbgateways.h


#win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../ProRotoQml/Protoblock/release/ -lProRotoQml.Protoblockd
#else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../ProRotoQml/Protoblock/debug/ -lProRotoQml.Protoblockd
#else:unix: LIBS += -L$$OUT_PWD/../../ProRotoQml/Protoblock/ -lProRotoQml.Protoblockd

#INCLUDEPATH += $$PWD/../../ProRotoQml/Protoblock
#DEPENDPATH += $$PWD/../../ProRotoQml/Protoblock

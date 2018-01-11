include($$PWD/../../pri/protos.pri)
include($$PWD/../../pri/deps.pri)
include($$PWD/../../pri/macrosAndModels.pri)

#QT -= gui
QT += core network websockets
CONFIG += qt c++11 static

CONFIG += resources_big

#CONFIG += c++11


    !contains(DEFINES, PRODFOOTBALL){
        TARGET = STAGING-fantasybit-2016D
    }

    contains(DEFINES, PRODFOOTBALL){
        TARGET = fantasybit-2016D
    }


#CONFIG += qt console
CONFIG -= app_bundle

TEMPLATE = lib



#DEFINES += TRACE

    include($$PWD/../../pri/core.pri)

win32 {
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
#    $$PWD/ExchangeProxy.cpp \
#    $$PWD/trading.cpp \
    $$PWD/NameData.cpp \
    $$PWD/core.cpp \
    $$PWD/LAPIWorker.cpp \
    $$PWD/NodeWorker.cpp \
    $$PWD/iresolvable.cpp \
    $$PWD/dataservice.cpp \
    $$PWD/fullgateway.cpp \
    $$PWD/importLoader.cpp




HEADERS += \
    $$PWD/PeerNode.h \
    pbutils.h \
    data.h \
    core.h \
    $$PWD/NameData.h \
    $$PWD/Processor.h \
    $$PWD/ExchangeData.h \
#    $$PWD/ExchangeProxy.h \
    $$PWD/RestFullCall.h \
    $$PWD/LAPIWorker.h \
    $$PWD/NodeWorker.h \
    $$PWD/threadedqobject.h \
    $$PWD/iresolvable.h \
    $$PWD/dataservice.h \
    fullgateway.h \
    $$PWD/importLoader.h



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
    $$PWD/../../ProRotoQml/protoblock/PlayerSymbolsModel.h \
    $$PWD/../../ProRotoQml/protoblock/depthmarketmodel.h \
    $$PWD/../../ProRotoQml/protoblock/fantasynamemodel.h \
    $$PWD/../../ProRotoQml/protoblock/openordersmodel.h \
    $$PWD/../../ProRotoQml/protoblock/weeklyschedulemodel.h \
    $$PWD/../../ProRotoQml/protoblock/playerprojmodel.h \
    $$PWD/../../ProRotoQml/protoblock/playerresultmodel.h \
    $$PWD/../../ProRotoQml/protoblock/sortfilterproxymodel.h \
    $$PWD/../../ProRotoQml/protoblock/pbgateways.h

#SOURCES += \
#    $$PWD/../../ProRotoQml/Theme/plugin.cpp \
#    $$PWD/../../ProRotoQml/Theme/colors.cpp \
#    $$PWD/../../ProRotoQml/Theme/formfactor.cpp \
#    $$PWD/../../ProRotoQml/Theme/palette.cpp \
#    $$PWD/../../ProRotoQml/Theme/units.cpp \
#    $$PWD/../../ProRotoQml/Theme/listener.cpp \
#    $$PWD/../../ProRotoQml/Theme/teamcolors.cpp \
#    $$PWD/../../ProRotoQml/Theme/protoscreen.cpp

#HEADERS += \
#    $$PWD/../../ProRotoQml/Theme/plugin.h \
#    $$PWD/../../ProRotoQml/Theme/formfactor.h \
#    $$PWD/../../ProRotoQml/Theme/colors.h \
#    $$PWD/../../ProRotoQml/Theme/palette.h \
#    $$PWD/../../ProRotoQml/Theme/units.h \
#    $$PWD/../../ProRotoQml/Theme/listener.h \
#    $$PWD/../../ProRotoQml/Theme/teamcolors.h \
#    $$PWD/../../ProRotoQml/Theme/protoscreen.h

#INCLUDEPATH += $$PWD/../../ProRotoQml/Theme
#DEPENDPATH += $$PWD/../../ProRotoQml/Theme

#win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../ProRotoQml/Protoblock/release/ -lProRotoQml.Protoblockd
#else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../ProRotoQml/Protoblock/debug/ -lProRotoQml.Protoblockd
#else:unix: LIBS += -L$$OUT_PWD/../../ProRotoQml/Protoblock/ -lProRotoQml.Protoblockd

INCLUDEPATH += $$PWD/../../ProRotoQml/Protoblock
DEPENDPATH += $$PWD/../../ProRotoQml/Protoblock

contains(DEFINES, SQLSTUFF) {
    SOURCES += $$PWD/SqlStuff.cpp
    HEADERS += $$PWD/SqlStuff.h
    #include (./o2/o2.pri)
}

contains (DEFINES, PLAYERLOADERFD) {
    SOURCES += $$PWD/../../../fantasybit-2015/tradingfootball/playerloader.cpp
    HEADERS += $$PWD/../../../fantasybit-2015/tradingfootball/playerloader.h
}

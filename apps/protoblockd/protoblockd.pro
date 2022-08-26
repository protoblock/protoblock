#PRODORSTAGEFOOTBALL=PRODFOOTBALL
PRODORSTAGEFOOTBALL=STAGEFOOTBALL

include($$PWD/deps.pri)
include($$PWD/../../src/src.pri)
include($$PWD/../../share/protos.pri)


QT += core network websockets qml
CONFIG += qt c++11 static
TEMPLATE = lib
TARGET = protoblockd

contains(DEFINES, LIGHT_CLIENT_ONLY){
    TARGET = $${TARGET}-lite
}

!contains(DEFINES, PRODFOOTBALL){
    TARGET = STAGING-$${TARGET}
}

contains(DEFINES, DATAAGENTDEFS){
    TARGET = ERROR-$${TARGET}
}

osx{
    QMAKE_CXXFLAGS += -std=c++11
#    QMAKE_MAC_SDK = macosx10.14
    message( protoblockd.pro osx)
    CONFIG+=app_bundle
}


contains(DEFINES,LIGHT_CLIENT_ONLY)  {
    HEADERS +=  $$PWD/../../src/lightgateway.h
} else {
    HEADERS +=  $$PWD/../../src/fullgateway.h
}

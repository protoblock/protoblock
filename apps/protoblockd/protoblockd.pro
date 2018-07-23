include($$PWD/deps.pri)
include($$PWD/../../src/src.pri)
include($$PWD/../defs.pri)
include($$PWD/../../share/protos.pri)


QT += core network websockets qml
CONFIG += qt c++11 static
TEMPLATE = lib
TARGET = protoblockd

!contains(DEFINES, PRODFOOTBALL){
    TARGET = STAGING--$${TARGET}
}

contains(DEFINES, DATAAGENTDEFS){
    TARGET = DATAAGENT-$${TARGET}
}


include($$PWD/deps.pri)

include($$PWD/../../src/src.pri)
include($$PWD/../../princlude/defs.pri)
include($$PWD/../../princlude/protos.pri)


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


include($$PWD/deps.pri)
include($$PWD/../../src/src.pri)
include($$PWD/../../share/protos.pri)


QT += core network websockets qml
CONFIG += qt c++11 static
TEMPLATE = lib
TARGET = fantasybit-server

!contains(DEFINES, PRODFOOTBALL){
    TARGET = STAGING-$${TARGET}
}

contains(DEFINES, DATAAGENTGUI){
    TARGET = DATAAGENT-$${TARGET}
}

contains(DEFINES, WSSERVER){
    TARGET = WSSERVER-$${TARGET}
}


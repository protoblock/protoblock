#include ($$PWD/p2p/p2p.pri)
include ($$PWD/../pri/deps.pri)
include($$PWD/../pri/protos.pri)

QT += core network gui qml quick
#QT -= gui

QT += qml quick core websockets network xmlpatterns qml-private quick-private


CONFIG += c++11
QT -= gui
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

INCLUDEPATH += $$PWD/
INCLUDEPATH += $$PWD/bitcoin-core-base58


SOURCES += $$PWD/main.cpp \
#    protobufsocketreader.cpp \
    nodeclient.cpp \
    connectionmanager.cpp \
    peerwireclient.cpp \
    peerstore.cpp \
    nodeserver.cpp \
    node.cpp \
    peerwire.cpp \
    $$PWD/bitcoin-core-base58/base58.cpp \
    $$PWD/utils/utils.cpp \
    Commissioner.cpp \
    FantasyName.cpp



TARGET = testcore

HEADERS += \
#console.h \
#    protobufsocketreader.h \
    socketcopyinginputstream.h \
#    testserver.h \
    threadedqobject.h \
    nodeclient.h \
    connectionmanager.h \
    peerstore.h \
    peerwireclient.h \
    nodeserver.h \
    node.h \
    peerwire.h \
    $$PWD/bitcoin-core-base58/base58.h \
    $$PWD/utils/utils.h \
    Commissioner.h \
    FantasyName.h


DISTFILES += \
    main.qml

RESOURCES += $$PWD/qml/qml.qrc

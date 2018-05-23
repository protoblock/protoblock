include ($$PWD/p2p/p2p.pri)
include ($$PWD/../pri/deps.pri)
include($$PWD/../pri/protos.pri)

QT += core network gui qml quick
#QT -= gui

QT += qml quick core websockets network xmlpatterns qml-private quick-private


CONFIG += c++11

#CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += $$PWD/main.cpp \
    console.cpp \
    protobufsocketreader.cpp \
    nodeclient.cpp \
    connectionmanager.cpp \
    peerwireclient.cpp \
    peerstore.cpp \
    nodeserver.cpp

TARGET = testcore

HEADERS += console.h \
    protobufsocketreader.h \
    socketcopyinginputstream.h \
    testserver.h \
    threadedqobject.h \
    nodeclient.h \
    connectionmanager.h \
    peerstore.h \
    peerwireclient.h \
    nodeserver.h

DISTFILES += \
    main.qml

RESOURCES += $$PWD/qml/qml.qrc \
    qml/qml.qrc

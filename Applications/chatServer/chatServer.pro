QT += core network qml quick websockets webchannel
QT -= gui

CONFIG += c++11

TARGET = chatServer
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    chatserver.cpp \
    websocketclientwrapper.cpp \
    websockettransport.cpp

HEADERS += \
    chatserver.h \
    websocketclientwrapper.h \
    websockettransport.h

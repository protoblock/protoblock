include ($$PWD/p2p/p2p.pri)
include ($$PWD/../pri/deps.pri)
include($$PWD/../pri/protos.pri)

QT += core
QT -= gui

CONFIG += c++11

CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += \
    $$PWD/main.cpp

TARGET = testcore

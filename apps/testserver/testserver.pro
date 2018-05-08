include ($$PWD/../../pri/deps.pri)

#QT += quick
#CONFIG += c++11 console
#QT += core
#QT -= gui
#TEMPLATE = app

#SOURCES += \
#    $$PWD/main.cpp



#TARGET = testserver




QT += core
QT -= gui

CONFIG += c++11

TARGET = testserver
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp

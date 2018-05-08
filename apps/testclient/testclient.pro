include ($$PWD/../../pri/deps.pri)

QT += core
QT -= gui

CONFIG += c++11

CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp

TARGET = testclient

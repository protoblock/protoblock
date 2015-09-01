#-------------------------------------------------
#
# Project created by QtCreator 2014-04-08T11:09:34
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

#comment this line to generate
DEFINES += DATAAGENTGUI
#DEFINES += DATAAGENTGUIJay2015PrePreSeasonTestDemo
#DEFINES += Jay2015PrePreSeasonTestDemo

contains(DEFINES, DATAAGENTGUI){
TARGET = cute-fantasy-agent
QT += sql
}
!contains(DEFINES, DATAAGENTGUI){
TARGET = tradingfootball
}

TEMPLATE = app

CONFIG += c++11

INCLUDEPATH += ./../include
INCLUDEPATH += ./../generated
INCLUDEPATH += ./../src

CONFIG(debug, debug|release) {
    LIBS += -lfc_debug
    DESTDIR = ./../debugbin
    MOC_DIR = ./debug
    OBJECTS_DIR = ./debug
    contains(DEFINES, DATAAGENTGUI){ DESTDIR = ./../debugbinagent }
}

CONFIG(release, debug|release) {
    LIBS += -L./../lib
    LIBS += -lfc_release
    DESTDIR = ./../bin
    MOC_DIR = ./release
    OBJECTS_DIR = ./release
}

UI_DIR = ./tmp/ui
RCC_DIR = ./generatedfiles

LIBS += -L./../lib
LIBS += -lLevelDB  \
        -llibprotobuf

RC_FILE = cute-fantasy.rc

include(./boost-includes.pri)
include (./cute-fantasy.pri)
include (./openssl-includes.pri)

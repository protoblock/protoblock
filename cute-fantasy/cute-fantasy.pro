#-------------------------------------------------
#
# Project created by QtCreator 2014-04-08T11:09:34
#
#-------------------------------------------------

QT       += core gui sql network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

#comment this line to generate
#DEFINES += DATAAGENTGUI
#DEFINES += DATAAGENTGUIJay2015PrePreSeasonTestDemo
#DEFINES += Jay2015PrePreSeasonTestDemo

contains(DEFINES, DATAAGENTGUI){
TARGET = cute-fantasy-agent
}
!contains(DEFINES, DATAAGENTGUI){
TARGET = cute-fantasy
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
        -llibprotobuf \
        -lnanomsg \
        -llibeay32 \
        -lssleay32


#set the boost directory path here
# 1- download the binaries from
#  http://garr.dl.sourceforge.net/project/boost/boost-binaries/1.55.0/boost_1_55_0-msvc-12.0-64.exe
# 2- Enter the destination directory in the installer wizard step. That directory should be the
#    BOOST_DIR path.
BOOST_DIR = C:\Users\User\Documents\work\paperfootball\boost
LIBS += -L$${BOOST_DIR}/lib64-msvc-12.0
#LIBS += -lboost_log-vc120-1_55
INCLUDEPATH += $${BOOST_DIR}

include (./cute-fantasy.pri)

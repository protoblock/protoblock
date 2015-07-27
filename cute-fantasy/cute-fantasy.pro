#-------------------------------------------------
#
# Project created by QtCreator 2014-04-08T11:09:34
#
#-------------------------------------------------

QT       += core gui sql network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets network


DEFINES += FBWIN

#comment this line to generate
#DEFINES += DATAAGENTGUI
DEFINES += Jay2015PrePreSeasonTestDemo

contains(DEFINES, DATAAGENTGUI){
TARGET = cute-fantasy-agent
}
!contains(DEFINES, DATAAGENTGUI){
TARGET = cute-fantasy
}
TEMPLATE = app

CONFIG += c++11


#set the boost directory path here
# 1- download the binaries from
#  http://garr.dl.sourceforge.net/project/boost/boost-binaries/1.55.0/boost_1_55_0-msvc-12.0-64.exe
# 2- Enter the destination directory in the installer wizard step. That directory should be the
#    BOOST_DIR path.
BOOST_DIR = ./../../paperfootball/boost

INCLUDEPATH += ./../include
INCLUDEPATH += ./../include/nanomsg
INCLUDEPATH += $${BOOST_DIR}
INCLUDEPATH += ./../src





CONFIG(debug, debug|release) {
    LIBS += -lfc_debug
    DESTDIR = ./../debugbin

contains(DEFINES, DATAAGENTGUI){
DESTDIR = ./../debugbinagent
}

    MOC_DIR = ./debug
    OBJECTS_DIR = ./debug
}
CONFIG(release, debug|release) {
    LIBS += -L./../lib
    LIBS += -lfc_release
    DESTDIR = ./../bin
    MOC_DIR = ./release
    OBJECTS_DIR = ./release
}

UI_DIR = ./tmp/UI
RCC_DIR = ./GeneratedFiles

LIBS += -L./../lib
LIBS += -L$${BOOST_DIR}/lib64-msvc-12.0
LIBS += -lLevelDB
LIBS += -llibprotobuf
LIBS += -lnanomsg
LIBS += -lboost_log-vc120-1_55
LIBS += -lboost_date_time-vc120-1_55
LIBS += -llibeay32 -lssleay32


include (./cute-fantasy.pri)


#-------------------------------------------------
#
# Project created by QtCreator 2014-04-08T11:09:34
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets


DEFINES += PRODFOOTBALL

#DEFINES += MIKECLAYIMPORT

#comment this line to generate
#DEFINES += DATAAGENTGUI
#DEFINES += DATAAGENTWRITENAMES
#DEFINES += DATAAGENTGUIJay2015PrePreSeasonTestDemo
#DEFINES += Jay2015PrePreSeasonTestDemo
#DEFINES += DATAAGENTWRITENAMES_FORCE
#DEFINES += TIMEAGENTGUI
#DEFINES += TIMEAGENTWRITEFILLS
#DEFINES += TIMEAGENTWRITEFILLS_FORCE

DEFINES += TRADE_FEATURE

DEFINES += BUILD_STABLE
#DEFINES += ALLOW_DEBUG
#DEFINES += PLAYER_ID
#DEFINES += TRACE
#DEFINES += AUTOMMMIKECLAY
#DEFINES += WRITE_BOOTSTRAP
#DEFINES += CHECKPOINTS

contains(DEFINES, TIMEAGENTWRITEFILLS){
    QT += sql
}

contains(DEFINES, AUTOMMMIKECLAY){
    QT += sql
}

contains(DEFINES, MIKECLAYIMPORT){
    QT += sql
}

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

CONFIG(debug, debug|release) {

    !contains(DEFINES, PRODFOOTBALL) {
        DESTDIR = ./../stage
    }

    contains(DEFINES, PRODFOOTBALL) {
        DESTDIR = ./../debugbin
    }

    MOC_DIR = ./debug
    OBJECTS_DIR = ./debug
    contains(DEFINES, DATAAGENTGUI){
        !contains(DEFINES, PRODFOOTBALL) {
            DESTDIR = ./../stage
        }

        contains(DEFINES, PRODFOOTBALL) {
            DESTDIR = ./../debugbinagent
        }

    }
}

CONFIG(release, debug|release) {
#    LIBS += -L./../lib
#    LIBS += -lfc_release
    !contains(DEFINES, PRODFOOTBALL) {
        DESTDIR = ./../stage
    }

    contains(DEFINES, PRODFOOTBALL) {
        DESTDIR = ./../bin
    }

    MOC_DIR = ./release
    OBJECTS_DIR = ./release
}

UI_DIR = ./tmp/ui
RCC_DIR = ./generatedfiles


unix {
    macx {
      LIBS+= -L./../libosx64
    } else {

    }
}

win32 {
    contains(QMAKE_TARGET.arch, x86_64) {
        # x86_64 build
        message("x86_64 build")
        LIBS+= -L./../libwin64
        RC_FILE = cute-fantasy.rc
    } else {
        # x86 build

    }
}


CONFIG(debug, debug|release) {
   LIBS+= -llibprotobufd \
          -lleveldbd \
          -llibeay32 \
          -lssleay32 \
          -lfcd

}
CONFIG(release, debug|release) {
   LIBS+= -llibprotobuf \
          -lleveldb \
          -llibeay32 \
          -lssleay32 \
          -lfc
}

include (./boost-includes.pri)
include (./cute-fantasy.pri)

FORMS +=






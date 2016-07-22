include($$PWD/../../pri/protos.pri)
include($$PWD/../../pri/deps.pri)
include($$PWD/../../pri/macrosAndModels.pri)
QT += core websockets network
QT -= gui

CONFIG += c++11 warn_off
TARGET = WSServer
CONFIG += console
CONFIG -= app_bundle
TEMPLATE = app
## NOT needed
#OTHER_FILES += $$PWD/sslechoclient.html


## Not sure why these are getting defined here.  Seems like it would be better in the lib
## protoblock-core only and to try and keep this as clean as can be

DEFINES += USE_NUM_NONE
DEFINES += USE_FIELD_10X26
DEFINES += USE_FIELD_INV_BUILTIN
DEFINES += USE_SCALAR_8X32
DEFINES += USE_SCALAR_INV_BUILTIN
DEFINES += ONE_TIME_PROD


SOURCES   += \
    $$PWD/main.cpp \
    $$PWD/server.cpp \
    $$PWD/NameData.cpp \
    src/ldbwriter.cpp \
    src/blockmeta.cpp \
    src/getuserinfo.cpp \
    src/ldbhashreadertool.cpp \
    src/leveldbhelper.cpp \
#    src/protoblockapi.cpp \
#    src/tempapi.cpp \
    src/utils.cpp \
    src/maps/ldbhashreadermap.cpp \
#    src/maps/playermap.cpp \
#    src/maps/teammap.cpp


HEADERS += \
    $$PWD/server.h \
    $$PWD/NameData.h \
    src/ldbhashreadertool.h \
    src/ldbwriter.h \
    src/blockmeta.h \
    src/getuserinfo.h \
    src/leveldbhelper.h \
#    src/protoblockapi.h \
#    src/tempapi.h \
    src/utils.h \
    src/maps/ldbhashreadermap.h \
#    src/maps/playermap.h \
#    src/maps/teammap.h





## FIXME add in libs here for levelDB as it is the only thing ATM that is using it.
macx{
        ## FIXME compile levelDB
        LIBS += -L/usr/local/lib  -lleveldb

        ##FIXME compile levelDB
        INCLUDEPATH += /usr/local/Cellar/leveldb/1.18/include
        DEPENDPATH += /usr/local/Cellar/leveldb/1.18/include

        LIBS += -L$$OUT_PWD/../../ProRotoQml/jsonpb/ -ljsonpb

        INCLUDEPATH += $$PWD/../../ProRotoQml/jsonpb
        DEPENDPATH += $$PWD/../../ProRotoQml/jsonpb

        LIBS += -L$$OUT_PWD/../../ProRotoQml/protoblock-core/ -lprotoblock-core
        INCLUDEPATH += $$PWD/../../ProRotoQml/protoblock-core/
        DEPENDPATH += $$PWD/../../ProRotoQml/protoblock-core/
        INCLUDEPATH += $$PWD/../../ProRotoQml/protoblock-core/bitcoin-core-base58
        DEPENDPATH += $$PWD/../../ProRotoQml/protoblock-core/bitcoin-core-base58
}



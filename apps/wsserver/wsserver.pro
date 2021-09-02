FANTASYBITSERVERPATH = $PWD/../../fantasybit-server

include ($${FANTASYBITSERVERPATH}/deps.pri)
include ($$PWD/../../share/protos.pri)
include ($$PWD/../../res/res.pri)

QT += core websockets network qml
QT -= gui
QT += core network sql

CONFIG += c++11
TARGET = WSServer
CONFIG += console
CONFIG -= app_bundle
TEMPLATE = app

DEFINES += MAX_NAMES_LB=1000

#DEFINES += WSSERVER_WRITE_TWEET
contains(DEFINES, WSSERVER_WRITE_TWEET) {
    INCLUDEPATH += $$DIRPREFIX/windows/3rdParty/nanomsg
    LIBS += -lnanomsg
}

!contains(DEFINES, PRODFOOTBALL){
    FANTASYBITSERVERLIB = STAGING-$${FANTASYBITSERVERLIB}
}

contains(DEFINES, WSSERVER){
    FANTASYBITSERVERLIB = WSSERVER-$${FANTASYBITSERVERLIB}
}

contains(DEFINES, DATAAGENTGUI){
    FANTASYBITSERVERLIB = ERROR-$${FANTASYBITSERVERLIB}
}

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/$${FANTASYBITSERVERPATH}/release -l$${FANTASYBITSERVERLIB}
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/$${FANTASYBITSERVERPATH}/debug/ -l$${FANTASYBITSERVERLIB}
else:macx: LIBS += -L$$OUT_PWD/$${FANTASYBITSERVERPATH}-l$${FANTASYBITSERVERLIB}

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/$${FANTASYBITSERVERPATH}//release/lib$${FANTASYBITSERVERLIB}.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/$${FANTASYBITSERVERPATH}//debug/lib$${FANTASYBITSERVERLIB}.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/$${FANTASYBITSERVERPATH}/release/$${FANTASYBITSERVERLIB}.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/$${FANTASYBITSERVERPATH}/debug/$${FANTASYBITSERVERLIB}.lib
else:macx: PRE_TARGETDEPS += $$OUT_PWD/../protoblockd/lib$${FANTASYBITSERVERLIB}.a

message (PRE_TARGETDEPS $$PRE_TARGETDEPS)

INCLUDEPATH += $$PWD/../../src
INCLUDEPATH += $$PWD/../../src/state-model
INCLUDEPATH += $$PWD/../../src/core
INCLUDEPATH += $$PWD/../../src/core/bitcoin-core-base58
INCLUDEPATH += $$PWD/../../src/QmlModels
INCLUDEPATH += $$PWD/../../src/QmlSuperMacros

#SOURCES   += \
#    $$PWD/main.cpp \
#    $$PWD/server.cpp \
##    $$PWD/NameData.cpp \
##    $$PWD/Data.cpp \
##    $$PWD/txpool.cpp \
##    $$PWD/ExchangeData.cpp \
#    LiteServer.cpp
##    TxServer.cpp \
##    combinations.cpp



#HEADERS += \
#    $$PWD/server.h \
##    $$PWD/NameData.h \
##    $$PWD/Data.h \
##    $$PWD/txpool.h \
##    $$PWD/ExchangeData.h \
#    LiteServer.h \
#    $$PWD/wsgateway.h
##    TxServer.h

SOURCES += \
    $$PWD/src/main.cpp \
    $$PWD/src/server.cpp \
    $$PWD/src/LiteServer.cpp

HEADERS += \
    $$PWD/src/server.h \
    $$PWD/src/LiteServer.h \
    $$PWD/src/wsgateway.h

#    $$PWD/src/sqlstuff.h \
#    $$PWD/src/dataagent.h \
#    $$PWD/src/fantasydataagent.h \
#    $$PWD/src/txpool.h




include ($$PWD/../../pri/deps.pri)
include ($$PWD/../../pri/macrosAndModels.pri)
include($$PWD/../../pri/protos.pri)

ios{
    CXX_MODULE=qml
    QMAKE_MOC_OPTIONS += -Muri=ProRotoQml.Backend
}

TEMPLATE = lib
TARGETPATH=ProRotoQml/Backend
TARGET = ProRotoQml.Backend
QT += qml quick core gui network
CONFIG += qt plugin c++11 warn_off
DEFINES += JAY_TRY
TARGET = $$qtLibraryTarget($$TARGET)
uri = ProRotoQml.Backend

# Input
SOURCES += \
    $$PWD/src/backend_plugin.cpp \
    ## Utils
    $$PWD/src/utils.cpp \
    $$PWD/src/tempapi.cpp \
    #$$PWD/src/ldbwriter.cpp \
#    $$PWD/src/ldbhashreadertool.cpp \
#    $$PWD/src/blockmeta.cpp \
    $$PWD/src/models/playermodel.cpp \
    $$PWD/src/models/projectionsmodel.cpp \
    $$PWD/src/models/scheduledgamesmodel.cpp \
    $$PWD/src/models/closedgamesmodel.cpp \
    $$PWD/src/models/statsmodel.cpp \
    $$PWD/src/models/leaderboardmodel.cpp \
#    $$PWD/src/models/closedgamesmodel.cpp \
    $$PWD/src/maps/playermap.cpp \
#    $$PWD/src/maps/ldbhashreadermap.cpp \
    $$PWD/src/maps/teammap.cpp \
    $$PWD/src/models/awardsmodels.cpp \
    $$PWD/src/models/pnlmodel.cpp \
    $$PWD/src/getuserinfo.cpp \
    $$PWD/src/models/testmodel.cpp \


HEADERS += \
    $$PWD/src/backend_plugin.h \
    $$PWD/src/utils.h \
    $$PWD/src/tempapi.h \
    #$$PWD/src/ldbwriter.h \
#    $$PWD/src/ldbhashreadertool.h \
#    $$PWD/src/blockmeta.h \
    $$PWD/src/models/playermodel.h \
    $$PWD/src/models/projectionsmodel.h \
    $$PWD/src/models/scheduledgamesmodel.h \
    $$PWD/src/models/closedgamesmodel.h \
    $$PWD/src/models/statsmodel.h \
#    $$PWD/src/models/closedgamesmodel.h \
    $$PWD/src/models/leaderboardmodel.h \
    $$PWD/src/maps/playermap.h \
    $$PWD/src/maps/teammap.h \
#    $$PWD/src/maps/ldbhashreadermap.h \
    $$PWD/src/models/awardsmodels.h \
    $$PWD/src/models/pnlmodel.h \
    $$PWD/src/getuserinfo.h \
    $$PWD/src/models/testmodel.h \
    $$PWD/src/models/pnltestmodel.h \
    $$PWD/src/models/playernewmodel.h

DISTFILES = qmldir

!equals(_PRO_FILE_PWD_, $$OUT_PWD) {
    copy_qmldir.target = $$OUT_PWD/qmldir
    copy_qmldir.depends = $$_PRO_FILE_PWD_/qmldir
    copy_qmldir.commands = $(COPY_FILE) \"$$replace(copy_qmldir.depends, /, $$QMAKE_DIR_SEP)\" \"$$replace(copy_qmldir.target, /, $$QMAKE_DIR_SEP)\"
    QMAKE_EXTRA_TARGETS += copy_qmldir
    PRE_TARGETDEPS += $$copy_qmldir.target
}

qmldir.files = qmldir

installPath = $$[QT_INSTALL_QML]/$$replace(uri, \\., /)
qmldir.path = $$installPath
target.path = $$installPath

INSTALLS += target qmldir



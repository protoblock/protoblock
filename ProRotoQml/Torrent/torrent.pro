ios{
    CXX_MODULE=qml
    QMAKE_MOC_OPTIONS += -Muri=ProRotoQml.Torrent
}


TEMPLATE = lib
TARGET = ProRotoQml.Torrent
TARGETPATH=ProRotoQml/Torrent

QT += qml quick network core
CONFIG += qt plugin c++11

TARGET = $$qtLibraryTarget($$TARGET)
uri = ProRotoQml.Torrent

SOURCES += \
#main.cpp \
#    addtorrentdialog.cpp \
    bencodeparser.cpp \
    connectionmanager.cpp \
#    mainwindow.cpp \
    metainfo.cpp \
    peerwireclient.cpp \
    ratecontroller.cpp \
    filemanager.cpp \
    torrentclient.cpp \
    torrentserver.cpp \
    trackerclient.cpp \
    torrentmodel.cpp \
    torrentmodelitem.cpp \
    torrentserializer.cpp \
    torrent_plugin.cpp



HEADERS += \
    bencodeparser.h \
    connectionmanager.h \
#    mainwindow.h \
    metainfo.h \
    peerwireclient.h \
    ratecontroller.h \
    filemanager.h \
    torrentclient.h \
    torrentserver.h \
    trackerclient.h \
    torrentmodel.h \
    torrentmodelitem.h \
    torrentserializer.h \
    torrent_plugin.h


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

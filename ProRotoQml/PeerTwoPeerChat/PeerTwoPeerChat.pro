ios{
    CXX_MODULE=qml
    QMAKE_MOC_OPTIONS += -Muri=ProRotoQml.PeerTwoPeer
}
TEMPLATE = lib
TARGETPATH=ProRotoQml/PeerTwoPeer
TARGET = ProRotoQml.PeerTwoPeer
QT += qml quick network core
CONFIG += qt plugin c++11

TARGET = $$qtLibraryTarget($$TARGET)
uri = ProRotoQml.PeerTwoPeer

# Input
SOURCES += \
    $$PWD/peertwopeerchat_plugin.cpp \
    $$PWD/peertwopeer.cpp \
    $$PWD/chatdialog.cpp \
    $$PWD/client.cpp \
    $$PWD/connection.cpp \
    $$PWD/peermanager.cpp \
    $$PWD/server.cpp

HEADERS += \
    $$PWD/peertwopeerchat_plugin.h \
    $$PWD/peertwopeer.h \
    $$PWD/chatdialog.h \
    $$PWD/client.h \
    $$PWD/connection.h \
    $$PWD/peermanager.h \
    $$PWD/server.h

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


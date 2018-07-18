osx {
QMAKE_MAC_SDK = macosx10.12
}
include ($$PWD/../../pri/macrosAndModels.pri)

ios{
    CXX_MODULE=qml
    QMAKE_MOC_OPTIONS += -Muri=ProRotoQml.Utils
}

android{
    QT += androidextras
}


TEMPLATE = lib
TARGETPATH=ProRotoQml/Utils
TARGET =  ProRotoQml.Utils
QT += qml quick core network websockets
CONFIG += qt plugin c++11

TARGET = $$qtLibraryTarget($$TARGET)
uri = ProRotoQml.Utils

# Input
SOURCES += \
    $$PWD/passwordstrength.cpp \
    $$PWD/plugininisetter.cpp \
#    qmlsesttings.cpp \
#    qqmlprocess.cpp \
    $$PWD/utils.cpp \
    $$PWD/downloader.cpp \
    $$PWD/plugin.cpp \
    $$PWD/device.cpp \
    $$PWD/units.cpp \
    $$PWD/qqmlwebsockets.cpp


HEADERS += \
    $$PWD/passwordstrength.h \
    $$PWD/plugininisetter.h \
#    qmlsesttings.h \
#    qqmlprocess.h \
    $$PWD/utils.h \
    $$PWD/downloader.h \
    $$PWD/plugin.h \
    $$PWD/units.h \
    $$PWD/device.h \
    $$PWD/qqmlwebsockets.h

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


ios{
    CXX_MODULE=qml
    QMAKE_MOC_OPTIONS += -Muri=ProRotoQml.Audio
}

TEMPLATE = lib
TARGETPATH=ProRotoQml/Audio
TARGET = ProRotoQml.Audio
QT += qml quick multimedia
CONFIG += qt plugin c++11

TARGET = $$qtLibraryTarget($$TARGET)
uri = ProRotoQml.Audio

# Input
SOURCES += \
    $$PWD/audio_plugin.cpp \
    $$PWD/src/audio.cpp \
    $$PWD/src/qaudiolevel.cpp

HEADERS += \
    $$PWD/audio_plugin.h \
    $$PWD/src/audio.h \
    $$PWD/src/qaudiolevel.h

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

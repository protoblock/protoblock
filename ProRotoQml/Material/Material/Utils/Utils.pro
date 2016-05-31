ios{
    CXX_MODULE=qml
    QMAKE_MOC_OPTIONS += -Muri=Material.Utils
}

TEMPLATE = lib
TARGETPATH=Material/Utils
TARGET = materialutils
uri = Material.Utils

QT += qml quick
unix:android{
    QT += androidextras
}
CONFIG += qt plugin c++11
TARGET = $$qtLibraryTarget($$TARGET)


# Input
SOURCES += \
    $$PWD/plugin.cpp \
    $$PWD/device.cpp \
    $$PWD/units.cpp

HEADERS += \
    $$PWD/plugin.h \
    $$PWD/device.h \
    $$PWD/units.h



DISTFILES = qmldir \



!equals(_PRO_FILE_PWD_, $$OUT_PWD) {
    copy_qmldir.target = $$OUT_PWD/qmldir
    copy_qmldir.depends = $$_PRO_FILE_PWD_/qmldir
    copy_qmldir.commands = $(COPY_FILE) \"$$replace(copy_qmldir.depends, /, $$QMAKE_DIR_SEP)\" \"$$replace(copy_qmldir.target, /, $$QMAKE_DIR_SEP)\"
    QMAKE_EXTRA_TARGETS += copy_qmldir
    PRE_TARGETDEPS += $$copy_qmldir.target
}

qmldir.files = $$DISTFILES
installPath = $$[QT_INSTALL_QML]/$$replace(uri, \\., /)
qmldir.path = $$installPath
target.path = $$installPath
INSTALLS += target qmldir

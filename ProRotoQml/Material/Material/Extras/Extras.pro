TEMPLATE = lib
TARGET = Material.Extras
QT += qml quick
CONFIG += qt plugin c++11

win32:{
    CONFIG+= static
}


TARGET = $$qtLibraryTarget($$TARGET)
uri = Material.Extras

DISTFILES = qmldir \
    circle.png \
    AutomaticGrid.qml \
    CircleImage.qml \
    CircleMask.qml \
    ColumnFlow.qml \
    Image.qml


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

HEADERS += \
    dummyios.h

SOURCES += \
    dummyios.cpp





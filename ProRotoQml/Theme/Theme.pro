ios{
    CXX_MODULE=qml
    QMAKE_MOC_OPTIONS += -Muri=ProRotoQml.Theme
}

TEMPLATE = lib
TARGETPATH=ProRotoQml/Theme
TARGET = ProRotoQml.Theme
QT += qml quick core gui websockets
CONFIG += qt plugin c++11
TARGET = $$qtLibraryTarget($$TARGET)
uri = ProRotoQml.Theme

# Input
SOURCES += \
    $$PWD/plugin.cpp \
    $$PWD/colors.cpp \
    $$PWD/formfactor.cpp \
    $$PWD/palette.cpp \
    $$PWD/units.cpp \
    $$PWD/listener.cpp \
    $$PWD/teamcolors.cpp \

HEADERS += \
    $$PWD/plugin.h \
    $$PWD/formfactor.h \
    $$PWD/colors.h \
    $$PWD/palette.h \
    $$PWD/units.h \
    $$PWD/listener.h \
    $$PWD/teamcolors.h \

DISTFILES = qmldir \
    README.md

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


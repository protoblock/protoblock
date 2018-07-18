osx {
QMAKE_MAC_SDK = macosx10.12
}

ios{
    CXX_MODULE=qml
    QMAKE_MOC_OPTIONS += -Muri=ProRotoQml.Models
}
!ios{
    TEMPLATE = lib
}
TARGETPATH=ProRotoQml/Models
TARGET = ProRotoQml.Models

TARGET =  ProRotoQml.Models
QT += qml quick core
CONFIG += qt plugin c++11

TARGET = $$qtLibraryTarget($$TARGET)
uri = ProRotoQml.Models

# Input
SOURCES += \
    models_plugin.cpp \
    sortfilterproxymodel.cpp \

HEADERS += \
    models_plugin.h \
    sortfilterproxymodel.h \


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



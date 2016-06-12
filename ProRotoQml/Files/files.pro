ios{
    CXX_MODULE=qml
    QMAKE_MOC_OPTIONS += -Muri=ProRotoQml.Files
}


TEMPLATE = lib
TARGETPATH=ProRotoQml/Files
TARGET =  ProRotoQml.Files
QT += qml quick core
CONFIG += qt plugin c++11

TARGET = $$qtLibraryTarget($$TARGET)
uri = ProRotoQml.Files

# Input
SOURCES += \
    files_plugin.cpp \
    qfile.cpp \
    qmldir.cpp \
    qmlfileinfo.cpp \
    qmlfilesystemwatcher.cpp \
    qmlmimetype.cpp

HEADERS += \
    files_plugin.h \
    qfile.h \
    qmldir.h \
    qmlfileinfo.h \
    qmlfilesystemwatcher.h \
    qmlmimetype.h

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

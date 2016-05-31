ios{
    CXX_MODULE=qml
    QMAKE_MOC_OPTIONS += -Muri=ProRotoQml.Utils
}

TEMPLATE = lib
TARGETPATH=ProRotoQml/Utils
TARGET =  ProRotoQml.Utils
QT += qml quick core
CONFIG += qt plugin c++11

TARGET = $$qtLibraryTarget($$TARGET)
uri = ProRotoQml.Utils

# Input
SOURCES += \
    passwordstrength.cpp \
    plugininisetter.cpp \
#    qmlsesttings.cpp \
#    qqmlprocess.cpp \
    utils.cpp \
    downloader.cpp \
    plugin.cpp

HEADERS += \
    passwordstrength.h \
    plugininisetter.h \
#    qmlsesttings.h \
#    qqmlprocess.h \
    utils.h \
    downloader.h \
    plugin.h

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


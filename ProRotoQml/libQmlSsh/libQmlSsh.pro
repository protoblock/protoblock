TEMPLATE = lib
TARGET =  QmlSsh
QT += qml quick core network
CONFIG += qt plugin c++11
TARGET = $$qtLibraryTarget($$TARGET)
uri = QmlSsh


# Input
SOURCES += \
    ssh_plugin.cpp \
#    generatekeys.cpp \
    securenodes.cpp \
#    sftpupload.cpp \
#    sshremoteprocess.cpp \
#    uploadkeys.cpp

HEADERS += \
    ssh_plugin.h \
#    generatekeys.h \
#    remotelinux_export.h \
    securenodes.h \
#    sftpupload.h \
#    sshremoteprocess.h \
#    uploadkeys.h


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


win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../libBotan/release/ -lBotan
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../libBotan/debug/ -lBotan
else:unix: LIBS += -L$$OUT_PWD/../libBotan/ -lBotan

INCLUDEPATH += $$PWD/../libBotan
DEPENDPATH += $$PWD/../libBotan

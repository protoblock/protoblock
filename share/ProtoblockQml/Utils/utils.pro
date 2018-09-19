ios{
    CXX_MODULE=qml
    QMAKE_MOC_OPTIONS += -Muri=ProtoblockQml.Utils
}

android{
    QT += androidextras
}


TEMPLATE = lib
TARGETPATH=ProtoblockQml/Utils
TARGET =  ProtoblockQml.Utils
QT += qml quick core network websockets
CONFIG += qt plugin c++11

TARGET = $$qtLibraryTarget($$TARGET)
uri = ProtoblockQml.Utils

# Input
SOURCES += \
    $$PWD/passwordstrength.cpp \
    $$PWD/plugininisetter.cpp \
    $$PWD/utils.cpp \
    $$PWD/downloader.cpp \
    $$PWD/plugin.cpp \
    $$PWD/device.cpp \
    $$PWD/units.cpp \
    $$PWD/qqmlwebsockets.cpp


HEADERS += \
    $$PWD/passwordstrength.h \
    $$PWD/plugininisetter.h \
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


win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../QmlModelsAndMacros/QmlMacros/release/ -lQmlMacros
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../QmlModelsAndMacros/QmlMacros/debug/ -lQmlMacros
else:unix: LIBS += -L$$OUT_PWD/../../QmlModelsAndMacros/QmlMacros/ -lQmlMacros
win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../QmlModelsAndMacros/QmlMacros/release/libQmlMacros.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../QmlModelsAndMacros/QmlMacros/debug/libQmlMacros.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../QmlModelsAndMacros/QmlMacros/release/QmlMacros.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../QmlModelsAndMacros/QmlMacros/debug/QmlMacros.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../../QmlModelsAndMacros/QmlMacros/libQmlMacros.a
INCLUDEPATH += $$PWD/../../QmlModelsAndMacros/QmlMacros
DEPENDPATH += $$PWD/../../QmlModelsAndMacros/QmlMacros

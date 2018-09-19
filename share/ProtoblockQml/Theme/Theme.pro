ios{
    CXX_MODULE=qml
    QMAKE_MOC_OPTIONS += -Muri=ProtoblockQml.Theme
}

TEMPLATE = lib
TARGETPATH=ProtoblockQml/Theme
TARGET = ProtoblockQml.Theme
QT += qml quick core gui websockets
CONFIG += qt plugin c++11
TARGET = $$qtLibraryTarget($$TARGET)
uri = ProtoblockQml.Theme

# Input
SOURCES += \
    $$PWD/plugin.cpp \
    $$PWD/colors.cpp \
    $$PWD/formfactor.cpp \
    $$PWD/palette.cpp \
    $$PWD/units.cpp \
    $$PWD/listener.cpp \
    $$PWD/teamcolors.cpp \
    $$PWD/protoscreen.cpp

HEADERS += \
    $$PWD/plugin.h \
    $$PWD/formfactor.h \
    $$PWD/colors.h \
    $$PWD/palette.h \
    $$PWD/units.h \
    $$PWD/listener.h \
    $$PWD/teamcolors.h \
    $$PWD/protoscreen.h

DISTFILES = \
    qmldir \
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

osx {
#QMAKE_MAC_SDK = macosx10.12
}
TEMPLATE = lib
TARGET = Material.Styles
QT += qml quick
CONFIG += qt plugin c++11

win32:{
    CONFIG+= static
}


## 5.7 >
#CONFIG += qtquickcompiler




TARGET = $$qtLibraryTarget($$TARGET)
uri = Material.Styles

DISTFILES = qmldir \
    ApplicationWindowStyle.qml \
    ButtonStyle.qml \
    CheckBoxStyle.qml \
    ProgressBarStyle.qml \
    RadioButtonStyle.qml \
    SliderStyle.qml \
    SwitchStyle.qml \
    TextFieldStyle.qml \
    ToolBarStyle.qml \
    ToolButtonStyle.qml \
    ComboBoxStyle.qml



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





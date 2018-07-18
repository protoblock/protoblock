CONFIG += c++11
QT += qml quick

android {
    QT += androidextras svg xml
}

#HEADERS += $$PWD/src/plugin.h \
#           $$PWD/src/core/device.h \
#           $$PWD/src/core/units.h

#SOURCES += $$PWD/src/plugin.cpp \
#           $$PWD/src/core/device.cpp \
#           $$PWD/src/core/units.cpp

RESOURCES += $$PWD/../Applications/Protoblock2016/qml-material/src/material.qrc \
             $$PWD/../Applications/Protoblock2016/qml-material/src/components/components.qrc \
             $$PWD/../Applications/Protoblock2016/qml-material/src/controls/controls.qrc \
             $$PWD/../Applications/Protoblock2016/qml-material/src/core/core.qrc \
             $$PWD/../Applications/Protoblock2016/qml-material/src/extras/extras.qrc \
             $$PWD/../Applications/Protoblock2016/qml-material/src/listitems/listitems.qrc \
             $$PWD/../Applications/Protoblock2016/qml-material/src/popups/popups.qrc \
             $$PWD/../Applications/Protoblock2016/qml-material/src/styles/styles.qrc \
             $$PWD/../Applications/Protoblock2016/qml-material/src/window/window.qrc \
             $$PWD/../Applications/Protoblock2016/qml-material/icons/core_icons.qrc

OTHER_FILES = $$PWD/README.md $$PWD/CHANGELOG.md

contains(OPTIONS, roboto) {
    RESOURCES += $$PWD/../Applications/Protoblock2016/qml-material/fonts/fonts.qrc
}

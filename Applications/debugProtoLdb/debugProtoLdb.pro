include($$PWD/../../pri/deps.pri)
TEMPLATE = app
QT += qml quick
CONFIG += c++11 warn_off

SOURCES += main.cpp

RESOURCES +=  \
          qml.qrc \
           Common/common.qrc
# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH = . ..

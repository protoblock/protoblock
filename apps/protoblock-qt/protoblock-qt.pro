include ($$PWD/../protoblockd/deps.pri)
include ($$PWD/../../share/protos.pri)
include ($$PWD/../../res/res.pri)

win32 { DEFINES += INTEL_KLUDGE }

TARGET = protoblock-qt
TEMPLATE = app
QT += qml quick core websockets network qml-private quick-private quickcontrols2
CONFIG += c++11

contains(DEFINES, SQLSTUFF){
    QT += sql
}

contains(DEFINES, CONSOLE) {
    CONFIG += Console
}

osx{
    CONFIG+=app_bundle
    QMAKE_MAC_SDK = macosx10.14
}

macx {
    # set fags
    QMAKE_CFLAGS += -gdwarf-2
    QMAKE_CXXFLAGS += -gdwarf-2
    ICON = icon.icns
    CONFIG += x86
}

win32:RC_ICONS += myappico.ico


# Additional import path used to resolve QML modules in Qt Creator's code model
#QML_IMPORT_PATH = . ..

PROTOBLOCKDLIB += protoblockd
!contains(DEFINES, PRODFOOTBALL){
    PROTOBLOCKDLIB = STAGING-$${PROTOBLOCKDLIB}
}

contains(DEFINES, DATAAGENTDEFS){
    PROTOBLOCKDLIB = ERROR-$${PROTOBLOCKDLIB}
}

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../protoblockd/release/ -l$$PROTOBLOCKDLIB
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../protoblockd/debug/ -l$$PROTOBLOCKDLIB
else:macx: LIBS += -L$$OUT_PWD/../protoblockd/ -l$$PROTOBLOCKDLIB

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../protoblockd/release/lib$${PROTOBLOCKDLIB}.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../protoblockd/debug/lib$${PROTOBLOCKDLIB}.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../protoblockd/release/$${PROTOBLOCKDLIB}.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../protoblockd/debug/$${PROTOBLOCKDLIB}.lib
else:macx: PRE_TARGETDEPS += $$OUT_PWD/../protoblockd/lib$${PROTOBLOCKDLIB}.a

message (PRE_TARGETDEPS $$PRE_TARGETDEPS)

INCLUDEPATH += $$PWD/../../src
INCLUDEPATH += $$PWD/../../src/state-model
INCLUDEPATH += $$PWD/../../src/core
INCLUDEPATH += $$PWD/../../src/core/bitcoin-core-base58
INCLUDEPATH += $$PWD/../../src/QmlModels
INCLUDEPATH += $$PWD/../../src/QmlSuperMacros


INCLUDEPATH += $$PWD/../../../prebuiltLibs/windows/3rdParty
INCLUDEPATH += $$PWD/../../../prebuiltLibs/windows/3rdParty/secp256k1

SOURCES += \
    $$PWD/src/main.cpp \
    $$PWD/src/runguard.cpp

RESOURCES += \
        $$PWD/../../src/gui/qml/qml-components/qml-components.qrc \
        $$PWD/../../src/gui/qml/qml-pages/qml-pages.qrc \
        $$PWD/../../src/gui/qml/qml-news/qml-news.qrc \
        $$PWD/../../src/gui/qml/qml-realChat/qml-realChat.qrc \
        $$PWD/qml-main/qml-main.qrc

CONFIG(debug,debug|release):message("Debug mode")
CONFIG(release,debug|release):message("Release mode")

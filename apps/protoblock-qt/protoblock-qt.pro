include(../libs.pri)

TARGET = protoblock-qt
TEMPLATE = app
QT += \
     qml \
    quick \
    core \
    websockets \
    network \
    xmlpatterns \
    qml-private \
    quick-private \
    quickcontrols2

CONFIG += c++11

SQL{
    QT += sql
}

macx {
    CONFIG+=app_bundle
    # set fags
    QMAKE_CFLAGS += -gdwarf-2
    QMAKE_CXXFLAGS += -gdwarf-2
    ICON = icon.icns
    CONFIG += x86
}

win32 {
    DEFINES += INTEL_KLUDGE
    win32:RC_ICONS += myappico.ico
}

SOURCES += \
    $$PWD/src/main.cpp \
    $$PWD/src/runguard.cpp

RESOURCES += \
    $$PWD/res/teams/teams.qrc \
    $$PWD/res/icons/icons.qrc \
    $$PWD/res/math/math.qrc \
    $$PWD/gui/qml/qml-components/qml-components.qrc \
    $$PWD/gui/qml/qml-pages/qml-pages.qrc \
    $$PWD/gui/qml/qml-news/qml-news.qrc \
    $$PWD/gui/qml/qml-realChat/qml-realChat.qrc \
    $$PWD/qml-main/qml-main.qrc




win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../protoblockd/release/ -lprotoblockd
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../protoblockd/debug/ -lprotoblockd
else:unix: LIBS += -L$$OUT_PWD/../protoblockd/ -lprotoblockd
INCLUDEPATH += $$PWD/../protoblockd
DEPENDPATH += $$PWD/../protoblockd
INCLUDEPATH += $$PWD/../protoblockd/src
DEPENDPATH += $$PWD/../protoblockd/src


win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../share/libprotoblock-core/release/ -lprotoblock-core
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../share/libprotoblock-core/debug/ -lprotoblock-core
else:unix: LIBS += -L$$OUT_PWD/../../share/libprotoblock-core/ -lprotoblock-core
win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../share/libprotoblock-core/release/libprotoblock-core.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../share/libprotoblock-core/debug/libprotoblock-core.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../share/libprotoblock-core/release/protoblock-core.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../share/libprotoblock-core/debug/protoblock-core.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../../share/libprotoblock-core/libprotoblock-core.a
INCLUDEPATH += $$PWD/../../share/libprotoblock-core
DEPENDPATH += $$PWD/../../share/libprotoblock-core

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../share/GenerateProto/release/ -lProtoblockProtocols
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../share/GenerateProto/debug/ -lProtoblockProtocols
else:unix: LIBS += -L$$OUT_PWD/../../share/GenerateProto/ -lProtoblockProtocols
INCLUDEPATH += $$GENERATEDDIR
DEPENDPATH += $$GENERATEDDIR

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../share/libsecp256k1/release/ -lsecp256k1
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../share/libsecp256k1/debug/ -lsecp256k1
else:unix: LIBS += -L$$OUT_PWD/../../share/libsecp256k1/ -lsecp256k1
win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../share/libsecp256k1/release/libsecp256k1.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../share/libsecp256k1/debug/libsecp256k1.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../share/libsecp256k1/release/secp256k1.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../share/libsecp256k1/debug/secp256k1.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../../share/libsecp256k1/libsecp256k1.a
INCLUDEPATH += $$PWD/../../share/libsecp256k1
DEPENDPATH += $$PWD/../../share/libsecp256k1

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../share/libstatemodel/release/ -lstatemodel
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../share/libstatemodel/debug/ -lstatemodel
else:unix: LIBS += -L$$OUT_PWD/../../share/libstatemodel/ -lstatemodel
win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../share/libstatemodel/release/libstatemodel.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../share/libstatemodel/debug/libstatemodel.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../share/libstatemodel/release/statemodel.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../share/libstatemodel/debug/statemodel.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../../share/libstatemodel/libstatemodel.a
INCLUDEPATH += $$PWD/../../share/libstatemodel
DEPENDPATH += $$PWD/../../share/libstatemodel

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../share/QmlModelsAndMacros/QmlMacros/release/ -lQmlMacros
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../share/QmlModelsAndMacros/QmlMacros/debug/ -lQmlMacros
else:unix: LIBS += -L$$OUT_PWD/../../share/QmlModelsAndMacros/QmlMacros/ -lQmlMacros
win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../share/QmlModelsAndMacros/QmlMacros/release/libQmlMacros.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../share/QmlModelsAndMacros/QmlMacros/debug/libQmlMacros.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../share/QmlModelsAndMacros/QmlMacros/release/QmlMacros.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../share/QmlModelsAndMacros/QmlMacros/debug/QmlMacros.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../../share/QmlModelsAndMacros/QmlMacros/libQmlMacros.a
INCLUDEPATH += $$PWD/../../share/QmlModelsAndMacros/QmlMacros
DEPENDPATH += $$PWD/../../share/QmlModelsAndMacros/QmlMacros

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../share/QmlModelsAndMacros/QmlModels/release/ -lQmlModels
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../share/QmlModelsAndMacros/QmlModels/debug/ -lQmlModels
else:unix: LIBS += -L$$OUT_PWD/../../share/QmlModelsAndMacros/QmlModels/ -lQmlModels
win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../share/QmlModelsAndMacros/QmlModels/release/libQmlModels.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../share/QmlModelsAndMacros/QmlModels/debug/libQmlModels.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../share/QmlModelsAndMacros/QmlModels/release/QmlModels.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../share/QmlModelsAndMacros/QmlModels/debug/QmlModels.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../../share/QmlModelsAndMacros/QmlModels/libQmlModels.a
INCLUDEPATH += $$PWD/../../share/QmlModelsAndMacros/QmlModels
DEPENDPATH += $$PWD/../../share/QmlModelsAndMacros/QmlModels


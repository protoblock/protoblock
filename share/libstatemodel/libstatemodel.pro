TARGET = statemodel
TEMPLATE = lib
QT += core gui qml
CONFIG += \
    c++11 \
    static \
    warn_off

SOURCES += \
    $$PWD/socketclient.cpp \
    $$PWD/mediator.cpp \
    $$PWD/playerquoteslicemodel.cpp \
    $$PWD/depthmarketmodel.cpp \
    $$PWD/sortfilterproxymodel.cpp

HEADERS += \
    $$PWD/socketclient.h \
    $$PWD/mediator.h \
    $$PWD/playerquoteslicemodel.h \
    $$PWD/PlayerSymbolsModel.h \
    $$PWD/depthmarketmodel.h \
    $$PWD/fantasynamemodel.h \
    $$PWD/openordersmodel.h \
    $$PWD/weeklyschedulemodel.h \
    $$PWD/playerprojmodel.h \
    $$PWD/playerresultmodel.h \
    $$PWD/sortfilterproxymodel.h \
    $$PWD/pbgateways.h

headers.files += $$HEADERS
headers.CONFIG += no_check_exist

win32|win64{
    CONFIG += static
    headers.path = $$INSTALL_PREFIX/$$TARGET/usr/include
    target.path = $$INSTALL_PREFIX/$$TARGET/usr/lib
}
ios{
    headers.path = $$INSTALL_PREFIX/usr/local/include/$$TARGET
    target.path = $$INSTALL_PREFIX/usr/local/lib
}
macx{
    headers.path = $$INSTALL_PREFIX/usr/local/include/$$TARGET
    target.path = $$INSTALL_PREFIX/usr/local/lib
}
linux{
    headers.path = $$INSTALL_PREFIX/usr/local/include/$$TARGET
    target.path = $$INSTALL_PREFIX/usr/local/lib
}

INSTALLS += \
    target \
    headers



############################
## LIBS
############################


## Protocols
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../GenerateProto/release/ -lProtoblockProtocols
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../GenerateProto/debug/ -lProtoblockProtocols
else:unix: LIBS += -L$$OUT_PWD/../GenerateProto/ -lProtoblockProtocols
INCLUDEPATH += $$PWD/../GenerateProto
DEPENDPATH += $$PWD/../GenerateProto
INCLUDEPATH += $$GENERATEDDIR
DEPENDPATH += $$GENERATEDDIR

## Macros
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../QmlModelsAndMacros/QmlMacros/release/ -lQmlMacros
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../QmlModelsAndMacros/QmlMacros/debug/ -lQmlMacros
else:unix: LIBS += -L$$OUT_PWD/../QmlModelsAndMacros/QmlMacros/ -lQmlMacros
win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../QmlModelsAndMacros/QmlMacros/release/libQmlMacros.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../QmlModelsAndMacros/QmlMacros/debug/libQmlMacros.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../QmlModelsAndMacros/QmlMacros/release/QmlMacros.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../QmlModelsAndMacros/QmlMacros/debug/QmlMacros.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../QmlModelsAndMacros/QmlMacros/libQmlMacros.a
INCLUDEPATH += $$PWD/../QmlModelsAndMacros/QmlMacros
DEPENDPATH += $$PWD/../QmlModelsAndMacros/QmlMacros

## Models
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../QmlModelsAndMacros/QmlModels/release/ -lQmlModels
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../QmlModelsAndMacros/QmlModels/debug/ -lQmlModels
else:unix: LIBS += -L$$OUT_PWD/../QmlModelsAndMacros/QmlModels/ -lQmlModels
win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../QmlModelsAndMacros/QmlModels/release/libQmlModels.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../QmlModelsAndMacros/QmlModels/debug/libQmlModels.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../QmlModelsAndMacros/QmlModels/release/QmlModels.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../QmlModelsAndMacros/QmlModels/debug/QmlModels.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../QmlModelsAndMacros/QmlModels/libQmlModels.a
INCLUDEPATH += $$PWD/../QmlModelsAndMacros/QmlModels
DEPENDPATH += $$PWD/../QmlModelsAndMacros/QmlModels


## protoblock core
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../libprotoblock-core/release/ -lprotoblock-core
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../libprotoblock-core/debug/ -lprotoblock-core
else:unix: LIBS += -L$$OUT_PWD/../libprotoblock-core/ -lprotoblock-core
win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../libprotoblock-core/release/libprotoblock-core.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../libprotoblock-core/debug/libprotoblock-core.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../libprotoblock-core/release/protoblock-core.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../libprotoblock-core/debug/protoblock-core.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../libprotoblock-core/libprotoblock-core.a
INCLUDEPATH += $$PWD/../libprotoblock-core
DEPENDPATH += $$PWD/../libprotoblock-core

## secp256k1
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../libsecp256k1/release/ -lsecp256k1
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../libsecp256k1/debug/ -lsecp256k1
else:unix: LIBS += -L$$OUT_PWD/../libsecp256k1/ -lsecp256k1
win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../libsecp256k1/release/libsecp256k1.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../libsecp256k1/debug/libsecp256k1.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../libsecp256k1/release/secp256k1.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../libsecp256k1/debug/secp256k1.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../libsecp256k1/libsecp256k1.a
INCLUDEPATH += $$PWD/../libsecp256k1
DEPENDPATH += $$PWD/../libsecp256k1

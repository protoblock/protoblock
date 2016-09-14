include ($$PWD/../../pri/deps.pri)
include ($$PWD/../../pri/artwork.pri)
include ($$PWD/../../pri/qml-components.pri)
include ($$PWD/../../pri/qml-pages.pri)
#include ($$PWD/../../pri/qml-pinky.pri)
include ($$PWD/../../pri/qml-news.pri)
include ($$PWD/../../pri/qml-utils.pri)
#include ($$PWD/../../pri/qml-models.pri)
#include ($$PWD/../../pri/qml-torrent.pri)
include ($$PWD/../../pri/qml-realChat.pri)


#qtHaveModule(webengine) {
#    QT += webengine
#    DEFINES += QT_WEBVIEW_WEBENGINE_BACKEND
#}


TEMPLATE = app
#QT += xmlpatterns xml qml quick core websockets sql network qml-private quick-private testlib
QT += qml quick core websockets network xmlpatterns qml-private quick-private sql
#sql webchannel
CONFIG += c++11

osx{
    CONFIG+=app_bundle
}

ios{
OTHER_FILES += \
#    Infonfo.plist
}

## for the store.
macx {
    # Minimum OS X version for submission is 10.stdlib++ needs 10.7
#    QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.7


# Copy the custom Info.plist to the app bundle
    OTHER_FILES += Info.plist
    plist.path = "$$DESTDIR/$$join(TARGET,,,.app)/Contents"
    plist.files = $PWD/Info.plist
    INSTALLS += plist
    # set fags
    QMAKE_CFLAGS += -gdwarf-2
    QMAKE_CXXFLAGS += -gdwarf-2
    ICON = icon.icns
    CONFIG += x86

}

SOURCES += \
    $$PWD/src/main.cpp \
    $$PWD/src/runguard.cpp

RC_FILE = myap.rc

RESOURCES += $$PWD/qml/qml.qrc
# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH = . ..

DISTFILES += \
    android/AndroidManifest.xml \
    android/gradle/wrapper/gradle-wrapper.jar \
    android/gradlew \
    android/res/values/libs.xml \
    android/build.gradle \
    android/gradle/wrapper/gradle-wrapper.properties \
    android/gradlew.bat


ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android

contains(ANDROID_TARGET_ARCH,armeabi-v7a) {
    ANDROID_EXTRA_LIBS = \
             $$DIRPREFIX/android/extrenal-android/lib/libprotobuf.so
#             $$DIRPREFIX/android/extrenal-android/lib/libcrypto.so \
#             $$DIRPREFIX/android/extrenal-android/lib//libssl.so
}


ios{
    LIBS += \
                /Users/satoshi/Qt/5.6/ios/lib/libIrcCore.a \
                /Users/satoshi/Qt/5.6/ios/lib/libIrcModel.a \
                /Users/satoshi/Qt/5.6/ios/lib/libIrcUtil.a
}


win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../fantasybit-2016/release/ -lfantasybit-2016D
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../fantasybit-2016/debug/ -lfantasybit-2016D
else:macx: LIBS += -L$$OUT_PWD/../fantasybit-2016/ -lfantasybit-2016D

INCLUDEPATH += $$PWD/../fantasybit-2016
DEPENDPATH += $$PWD/../fantasybit-2016

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../fantasybit-2016/release/libfantasybit-2016D.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../fantasybit-2016/debug/libfantasybit-2016D.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../fantasybit-2016/release/fantasybit-2016D.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../fantasybit-2016/debug/fantasybit-2016D.lib
else:macx: PRE_TARGETDEPS += $$OUT_PWD/../fantasybit-2016/libfantasybit-2016D.a

INCLUDEPATH  += $$PWD/../../ProRotoQml/Protoblock
INCLUDEPATH +=  $$PWD/../../ProRotoQml/protoblock-core
INCLUDEPATH +=  $$PWD/../../ProRotoQml/protoblock-core/bitcoin-core-base58
INCLUDEPATH +=  $$PWD/../../ProRotoQml/QmlModels
INCLUDEPATH +=  $$PWD/../../ProRotoQml/QmlSuperMacros
INCLUDEPATH +=  $$PWD/../../../prebuiltLibs/windows/3rdParty
INCLUDEPATH += $$PWD/../../../prebuiltLibs/windows/3rdParty/secp256k1

include ($$PWD/../../pri/genproto.pri)

#message(win32 $$INCLUDEPATH)

#win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../ProRotoQml/Protoblock/release/ -lProRotoQml.Protoblockd
#else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../ProRotoQml/Protoblock/debug/ -lProRotoQml.Protoblockd
#else:unix: LIBS += -L$$OUT_PWD/../../ProRotoQml/Protoblock/ -lProRotoQml.Protoblockd

INCLUDEPATH += $$PWD/../../ProRotoQml/Protoblock
DEPENDPATH += $$PWD/../../ProRotoQml/Protoblock


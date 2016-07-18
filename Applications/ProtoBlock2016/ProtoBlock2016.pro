include ($$PWD/../../pri/deps.pri)
include ($$PWD/../../pri/artwork.pri)
include ($$PWD/../../pri/qml-components.pri)
include ($$PWD/../../pri/qml-pages.pri)
#include ($$PWD/../../pri/qml-pinky.pri)
include ($$PWD/../../pri/qml-news.pri)
include ($$PWD/../../pri/qml-utils.pri)
include ($$PWD/../../pri/qml-models.pri)
#include ($$PWD/../../pri/qml-torrent.pri)
include ($$PWD/../../pri/qml-realChat.pri)


qtHaveModule(webengine) {
    QT += webengine
    DEFINES += QT_WEBVIEW_WEBENGINE_BACKEND
}


TEMPLATE = app
QT += xmlpatterns xml qml quick core websockets network sql qml-private quick-private testlib
#sql webchannel
CONFIG += c++11 app_bundle
#osx{
#CONFIG+=app_bundle
#}

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
             $$DIRPREFIX/android/extrenal-android/lib/libprotobuf.so \
             $$DIRPREFIX/android/extrenal-android/lib/libcrypto.so \
             $$DIRPREFIX/android/extrenal-android/lib//libssl.so
}


ios{
    LIBS += \
                /Users/satoshi/Qt/5.6/ios/lib/libIrcCore.a \
                /Users/satoshi/Qt/5.6/ios/lib/libIrcModel.a \
                /Users/satoshi/Qt/5.6/ios/lib/libIrcUtil.a
}

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
    QT += xmlpatterns xml qml quick core websockets network sql widgets
#sql webchannel
CONFIG += c++11 app_bundle
#osx{
#CONFIG+=app_bundle
#}

SOURCES += \
    $$PWD/src/main.cpp \

RC_FILE = myap.rc

ICON = myappico.ico

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
                /Users/satoshi/Qt/5.6/android_armv7/lib/libprotobuf.so \
                /Users/satoshi/Qt/5.6/android_armv7/lib/libcrypto.so \
                /Users/satoshi/Qt/5.6/android_armv7/lib/libssl.so
#                /Users/satoshi/Qt/5.6/android_armv7/lib/lib.so

#       /Users/satoshi/Desktop/fc/prebuilt/android/extrenal-android/lib/libprotobuf.so
#       /Users/satoshi/Desktop/fc/prebuilt/android/extrenal-android/lib/libcrypto.so \
#       /Users/satoshi/Desktop/fc/prebuilt/android/extrenal-android/lib/libssl.so
}


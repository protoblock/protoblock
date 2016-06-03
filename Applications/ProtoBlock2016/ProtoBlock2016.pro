include ($$PWD/../../pri/deps.pri)
include ($$PWD/../../pri/artwork.pri)
include ($$PWD/../../pri/qml-pages.pri)
include ($$PWD/../../pri/qml-utils.pri)


qtHaveModule(webengine) {
    QT += webengine
    DEFINES += QT_WEBVIEW_WEBENGINE_BACKEND
}


TEMPLATE = app
    QT += qml quick sql core widgets sql websockets webchannel network

CONFIG += c++11

SOURCES += \
    $$PWD/src/main.cpp \


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
    android/gradlew.bat \
    qml-pages/Feeds/WebView.qml \
    qml-pages/BoxText.qml

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android

contains(ANDROID_TARGET_ARCH,armeabi-v7a) {
    ANDROID_EXTRA_LIBS = \
        $$PWD/../../../../prebuilt/android/extrenal-android/lib/libprotobuf.so
}

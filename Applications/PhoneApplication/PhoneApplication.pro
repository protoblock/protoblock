include ($$PWD/../../pri/deps.pri)
include ($$PWD/../../pri/protos.pri)
include ($$PWD/../../pri/artwork.pri)
include ($$PWD/../../pri/macrosAndModels.pri)
#include ($$PWD/../../pri/database.pri)
TEMPLATE = app

QT += qml quick qml-private websockets webchannel webengine sql
CONFIG += c++11 warn_off app_bundle
unix:android {
    QT_AUTO_SCREEN_SCALE_FACTOR=1
    QT+= androidextras quick-private sql
    DISTFILES += \
        android/AndroidManifest.xml \
        android/gradle/wrapper/gradle-wrapper.jar \
        android/gradlew \
        android/res/values/libs.xml \
        android/build.gradle \
        android/gradle/wrapper/gradle-wrapper.properties \
        android/gradlew.bat

    ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android
}

SOURCES += $$PWD/src/main.cpp \
    $$PWD/src/qqmlwebsockets.cpp
#    $$PWD/src//mediator.cpp \
#    $$PWD/src/socketclient.cpp

HEADERS += \
    $$PWD/src/qqmlwebsockets.h
#    $$PWD/src/mediator.h \
#    $$PWD/src/socketclient.h


databaseFile.files += $$PWD/../../assets/database/tfprod.db
databaseFile.path += $$OUT_PWD/PhoneApplication.app/Contents/MacOS/
DEPLOYMENT += databaseFile

RESOURCES += qml/qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH = . ..

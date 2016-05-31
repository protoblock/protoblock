include ($$PWD/../../pri/deps.pri)
include ($$PWD/../../pri/artwork.pri)
include ($$PWD/../../pri/macrosAndModels.pri)
include ($$PWD/../../pri/qml-pages.pri)
include ($$PWD/../../pri/qml-utils.pri)


TEMPLATE = app
QT += qml quick sql core widgets sql websockets webchannel network webengine
CONFIG += c++11

SOURCES += \
    $$PWD/src/main.cpp \
    $$PWD/src/socketclient.cpp \
    $$PWD/src/qqmlwebsockets.cpp

HEADERS += \
    $$PWD/src/socketclient.h \
    $$PWD/src/qqmlwebsockets.h

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



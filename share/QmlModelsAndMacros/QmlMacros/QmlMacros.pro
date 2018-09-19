TEMPLATE = lib
TARGET = QmlMacros
QT += \
    core \
    qml \
    quick

CONFIG += \
    c++11 \
    static


MOC_DIR = $$OUT_PWD/../$$TARGET-moc
OBJECTS_DIR = $$OUT_PWD/../$$TARGET-moc

HEADERS += \
    $$PWD/QQmlVarPropertyHelpers.h \
    $$PWD/QQmlPtrPropertyHelpers.h \
    $$PWD/QQmlEnumClassHelper.h \
    $$PWD/QQmlConstRefPropertyHelpers.h \
    $$PWD/QQmlNonConstPropertyHelper.h \
    $$PWD/QQmlListPropertyHelper.h \
    $$PWD/QQmlAutoPropertyHelpers.h \
    $$PWD/QQmlHelpersCommon.h \
    $$PWD/qquickpolygon.h \
    $$PWD/jsonpath.h

SOURCES += $$PWD/qquickpolygon.cpp

headers.files = $$HEADERS
headers.CONFIG += no_check_exist

!win32{
    target.path = $$INSTALL_PREFIX/lib
    headers.path = $$INSTALL_PREFIX/include/$$TARGET
}

win32{
    target.path = $$[QT_INSTALL_LIBS]/
    headers.path = $$[QT_INSTALL_HEADERS]/$$TARGET
}
android{
    CONFIG -= android_install
}

INSTALLS += \
    target \
    headers


TEMPLATE = lib
TARGET = QmlModels
CONFIG += \
    c++11

CONFIG += \
    static


MOC_DIR = $$OUT_PWD/../$$TARGET-moc
OBJECTS_DIR = $$OUT_PWD/../$$TARGET-moc

HEADERS += \
    $$PWD/QQmlObjectListModel.h \
    $$PWD/QQmlVariantListModel.h \
    $$PWD/qqmlgadgetlistmodel.h \

SOURCES += \
    $$PWD/QQmlObjectListModel.cpp \
    $$PWD/QQmlVariantListModel.cpp \
    $$PWD/qqmlgadgetlistmodel.cpp \

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



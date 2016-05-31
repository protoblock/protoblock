QT += core qml

INCLUDEPATH += $$PWD

HEADERS += \
    $$PWD/QQmlObjectListModel.h \
    $$PWD/QQmlVariantListModel.h

SOURCES += \
    $$PWD/QQmlObjectListModel.cpp \
    $$PWD/QQmlVariantListModel.cpp

    qmldir.files = qmldir
    installPath = $$[QT_INSTALL_QML]/$$replace(uri, \\., /)
    qmldir.path = $$installPath
    target.path = $$installPath
    INSTALLS += target qmldir

message(QmlSuperMacros $$PWD)

INCLUDEPATH += $$PWD

HEADERS += \
    $$PWD/QQmlVarPropertyHelpers.h \
    $$PWD/QQmlPtrPropertyHelpers.h \
    $$PWD/QQmlEnumClassHelper.h \
    $$PWD/QQmlConstRefPropertyHelpers.h \
    $$PWD/QQmlListPropertyHelper.h \
    $$PWD/QQmlAutoPropertyHelpers.h \
    $$PWD/QQmlHelpersCommon.h

SOURCES += \
    $$PWD/QQmlHelpers.cpp


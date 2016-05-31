ios{
    CXX_MODULE=qml
    QMAKE_MOC_OPTIONS += -Muri=ProRotoQml.Sql
}

TEMPLATE = lib
TARGETPATH=ProRotoQml/Sql
TARGET = ProRotoQml.Sql
QT += qml quick sql network
CONFIG += qt plugin c++11

TARGET = $$qtLibraryTarget($$TARGET)
uri = ProRotoQml.Sql

# Input
SOURCES += \
    $$PWD/plugin.cpp \
    $$PWD/qmlsqldatabase.cpp \
    $$PWD/qqmlsqlquery.cpp \
    $$PWD/qmlsqlquerymodel.cpp \
    $$PWD/qmlsqlcreatedatabase.cpp
#    qmlsqltablemode.cpp \
#    qmlsqltablehelper.cpp

HEADERS += \
    $$PWD/plugin.h \
    $$PWD/qmlsqldatabase.h \
    $$PWD/qqmlsqlquery.h \
    $$PWD/qmlsqlquerymodel.h \
    $$PWD/qmlsqlcreatedatabase.h
#    qmlsqltablemode.h \
#    qmlsqltablehelper.h

DISTFILES = qmldir

!equals(_PRO_FILE_PWD_, $$OUT_PWD) {
    copy_qmldir.target = $$OUT_PWD/qmldir
    copy_qmldir.depends = $$_PRO_FILE_PWD_/qmldir
    copy_qmldir.commands = $(COPY_FILE) \"$$replace(copy_qmldir.depends, /, $$QMAKE_DIR_SEP)\" \"$$replace(copy_qmldir.target, /, $$QMAKE_DIR_SEP)\"
    QMAKE_EXTRA_TARGETS += copy_qmldir
    PRE_TARGETDEPS += $$copy_qmldir.target
}

qmldir.files = qmldir
installPath = $$[QT_INSTALL_QML]/$$replace(uri, \\., /)
qmldir.path = $$installPath
target.path = $$installPath
INSTALLS += target qmldir

#-------------------------------------------------
#
# Project created by QtCreator 2015-08-21T17:35:47
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = mnemonic-test
TEMPLATE = app


SOURCES += main.cpp\
        testmnemonicdialog.cpp \
    mnemonic.cpp

HEADERS  += testmnemonicdialog.h \
    mnemonic.h

FORMS    += testmnemonicdialog.ui


INCLUDEPATH+=$$PWD/../../openssl/include


LIBS += -L./../../lib


LIBS += -llibeay32 \
        -lssleay32

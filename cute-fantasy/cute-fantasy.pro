#-------------------------------------------------
#
# Project created by QtCreator 2014-04-08T11:09:34
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = cute-fantasy
TEMPLATE = app

CONFIG += c++11

SOURCES += main.cpp sfgui.cpp client.cpp

HEADERS  += sfgui.h client.h nn.hpp

FORMS    += sfgui.ui

LIBS += -L"$$_PRO_FILE_PWD_/../lib/" -lnanomsg -lfb -llibprotobuf
INCLUDEPATH += "$$_PRO_FILE_PWD_/../include/"
INCLUDEPATH += "$$_PRO_FILE_PWD_/../src/"

TEMPLATE = lib
TARGET = QSsh
QT += network widgets gui
CONFIG += c++11
DEFINES += QSSH_LIBRARY
FORMS = $$PWD/sshkeycreationdialog.ui


INCLUDEPATH += "../libBotan/"

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../libBotan/release/ -lBotan
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../libBotan/debug/ -lBotan
else:unix: LIBS += -L$$OUT_PWD/../libBotan/ -lBotan

INCLUDEPATH += $$PWD/../libBotan
DEPENDPATH += $$PWD/../libBotan

HEADERS += \
    sftpchannel_p.h \
    sftpchannel.h \
    sftpdefs.h \
    sftpfilesystemmodel.h \
    sftpincomingpacket_p.h \
    sftpoperation_p.h \
    sftpoutgoingpacket_p.h \
    sftppacket_p.h \
    ssh_global.h \
    sshbotanconversions_p.h \
    sshcapabilities_p.h \
    sshchannel_p.h \
    sshchannelmanager_p.h \
    sshconnection_p.h \
    sshconnection.h \
    sshconnectionmanager.h \
    sshcryptofacility_p.h \
    sshdirecttcpiptunnel_p.h \
    sshdirecttcpiptunnel.h \
    ssherrors.h \
    sshexception_p.h \
    sshhostkeydatabase.h \
    sshincomingpacket_p.h \
    sshinit_p.h \
    sshkeycreationdialog.h \
    sshkeyexchange_p.h \
    sshkeygenerator.h \
    sshkeypasswordretriever_p.h \
    sshlogging_p.h \
    sshoutgoingpacket_p.h \
    sshpacket_p.h \
    sshpacketparser_p.h \
    sshpseudoterminal.h \
    sshremoteprocess_p.h \
    sshremoteprocess.h \
    sshremoteprocessrunner.h \
    sshsendfacility_p.h

SOURCES += \
    sftpchannel.cpp \
    sftpdefs.cpp \
    sftpfilesystemmodel.cpp \
    sftpincomingpacket.cpp \
    sftpoperation.cpp \
    sftpoutgoingpacket.cpp \
    sftppacket.cpp \
    sshcapabilities.cpp \
    sshchannel.cpp \
    sshchannelmanager.cpp \
    sshconnection.cpp \
    sshconnectionmanager.cpp \
    sshcryptofacility.cpp \
    sshdirecttcpiptunnel.cpp \
    sshhostkeydatabase.cpp \
    sshincomingpacket.cpp \
    sshinit.cpp \
    sshkeycreationdialog.cpp \
    sshkeyexchange.cpp \
    sshkeygenerator.cpp \
    sshkeypasswordretriever.cpp \
    sshlogging.cpp \
    sshoutgoingpacket.cpp \
    sshpacket.cpp \
    sshpacketparser.cpp \
    sshremoteprocess.cpp \
    sshremoteprocessrunner.cpp \
    sshsendfacility.cpp

installPath = $$[QT_INSTALL_QML]/$$replace(uri, \\., /)
target.path = $$installPath
INSTALLS += target

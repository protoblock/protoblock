include ($$PWD/../../pri/macrosAndModels.pri)

QT += network widgets

HEADERS += \
           bencodeparser.h \
           connectionmanager.h \
           mainwindow.h \
           metainfo.h \
           peerwireclient.h \
           ratecontroller.h \
           filemanager.h \
           torrentclient.h \
           torrentserver.h \
           trackerclient.h \
           addtorrent.h \
    torrentratecontroller.h \
    torrentqml.h \
    torrentmodel.h

SOURCES += \
           bencodeparser.cpp \
           connectionmanager.cpp \
           mainwindow.cpp \
           metainfo.cpp \
           peerwireclient.cpp \
           ratecontroller.cpp \
           filemanager.cpp \
           torrentclient.cpp \
           torrentserver.cpp \
           trackerclient.cpp \
    addtorrent.cpp \
    torrentratecontroller.cpp \
    torrentqml.cpp \
    torrentmodel.cpp

# Forms and resources
FORMS += forms/addtorrentform.ui
RESOURCES += icons.qrc

# install
target.path = $$[QT_INSTALL_EXAMPLES]/network/torrent
INSTALLS += target

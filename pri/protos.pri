include (genproto.pri)
INCLUDEPATH += $$GENERATEDDIR
HEADERS += \
    $$GENERATEDDIR/P2PData.pb.h

SOURCES += \
    $$GENERATEDDIR/P2PData.pb.cc


OTHER_FILES += $$PWD/../core/proto/P2Pdata.proto

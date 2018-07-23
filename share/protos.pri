include (genproto.pri)
INCLUDEPATH += $$GENERATEDDIR
HEADERS += \
    $$GENERATEDDIR/*.pb.h

SOURCES += \
    $$GENERATEDDIR/*.pb.cc


#OTHER_FILES += $$PWD/../core/proto/*.proto

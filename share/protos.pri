include (genproto.pri)
INCLUDEPATH += $$GENERATEDDIR
#HEADERS += \
#    $$GENERATEDDIR/*.pb.h

#SOURCES += \
#    $$GENERATEDDIR/*.pb.cc

SOURCES += $$files($$GENERATEDDIR/*.pb.cc, true)
HEADERS += $$files($$GENERATEDDIR/*.pb.h, true)
#OTHER_FILES += $$PWD/../core/proto/*.proto

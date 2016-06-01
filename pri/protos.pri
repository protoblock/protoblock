include (genproto.pri)
INCLUDEPATH += $$GENERATEDDIR
HEADERS += \
    $$GENERATEDDIR/ApiData.pb.h \
    $$GENERATEDDIR/ExData.pb.h \
    $$GENERATEDDIR/NameData.pb.h \
    $$GENERATEDDIR/ProtoData.pb.h \
    $$GENERATEDDIR/StateData.pb.h \
    $$GENERATEDDIR/StaticData.pb.h \
    $$GENERATEDDIR/StatusData.pb.h

SOURCES += \
    $$GENERATEDDIR/ApiData.pb.cc \
    $$GENERATEDDIR/ExData.pb.cc \
    $$GENERATEDDIR/NameData.pb.cc \
    $$GENERATEDDIR/ProtoData.pb.cc \
    $$GENERATEDDIR/StateData.pb.cc \
    $$GENERATEDDIR/StaticData.pb.cc \
    $$GENERATEDDIR/StatusData.pb.cc

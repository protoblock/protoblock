include ($$PWD/../../pri/protos.pri)
include ($$PWD/../../pri/deps.pri)
QT -= gui
TARGET = jsonpb
TEMPLATE = lib
CONFIG += c++11 warn_off

DEFINES += JSONPB_LIBRARY
SOURCES += pbjson.cpp
HEADERS += rapidjson/error/en.h \
    $$PWD/rapidjson/error/error.h \
    $$PWD/rapidjson/internal/biginteger.h \
    $$PWD/rapidjson/internal/diyfp.h \
    $$PWD/rapidjson/internal/dtoa.h \
    $$PWD/rapidjson/internal/ieee754.h \
    $$PWD/rapidjson/internal/itoa.h \
    $$PWD/rapidjson/internal/meta.h \
    $$PWD/rapidjson/internal/pow10.h \
    $$PWD/rapidjson/internal/stack.h \
    $$PWD/rapidjson/internal/strfunc.h \
    $$PWD/rapidjson/internal/strtod.h \
    $$PWD/rapidjson/msinttypes/inttypes.h \
    $$PWD/rapidjson/msinttypes/stdint.h \
    $$PWD/rapidjson/allocators.h \
    $$PWD/rapidjson/document.h \
    $$PWD/rapidjson/encodedstream.h \
    $$PWD/rapidjson/encodings.h \
    $$PWD/rapidjson/filereadstream.h \
    $$PWD/rapidjson/filewritestream.h \
    $$PWD/rapidjson/memorybuffer.h \
    $$PWD/rapidjson/memorystream.h \
    $$PWD/rapidjson/prettywriter.h \
    $$PWD/rapidjson/rapidjson.h \
    $$PWD/rapidjson/reader.h \
    $$PWD/rapidjson/stringbuffer.h \
    $$PWD/rapidjson/writer.h \
    $$PWD/bin2ascii.h \
    $$PWD/pbjson.hpp


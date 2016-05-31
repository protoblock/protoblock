include($$PWD/../pri/genproto.pri)

TEMPLATE = aux

CONFIG += c++11 \
                    ordered \
                    warn_off

## SOURCES
PROTOS += \
    $$PWD/ApiData.proto \
    $$PWD/ExData.proto \
    $$PWD/NameData.proto \
    $$PWD/StatusData.proto \
    $$PWD/StaticData.proto \
    $$PWD/ProtoData.proto \
    $$PWD/StateData.proto

protobuf_decl.name = protobuf headers
protobuf_decl.input = PROTOS
protobuf_decl.output = $$PWD/../generated/${QMAKE_FILE_BASE}.pb.h
osx{
protobuf_decl.commands = /Users/satoshi/Desktop/fc/ios/extrenal/prototbuf/platform/x86_64/bin/protoc --cpp_out=$$PWD/../generated/ --proto_path=${QMAKE_FILE_IN_PATH} ${QMAKE_FILE_NAME}
}

android{
protobuf_decl.commands = /usr/local/protobuf_2_6_1/bin/protoc --cpp_out=$$PWD/../generated/ --proto_path=${QMAKE_FILE_IN_PATH} ${QMAKE_FILE_NAME}
}


win32{
protobuf_decl.commands = protoc --cpp_out=$$PWD/../generated/ --proto_path=${QMAKE_FILE_IN_PATH} ${QMAKE_FILE_NAME}
}
protobuf_decl.variable_out = HEADERS
QMAKE_EXTRA_COMPILERS += protobuf_decl

protobuf_impl.name = protobuf sources
protobuf_impl.input = PROTOS
protobuf_impl.output = $$PWD/../generated/${QMAKE_FILE_BASE}.pb.cc
protobuf_impl.depends = $$PWD/../generated/${QMAKE_FILE_BASE}.pb.h
protobuf_impl.commands = $$escape_expand(\n)
protobuf_impl.variable_out = SOURCES
QMAKE_EXTRA_COMPILERS += protobuf_impl


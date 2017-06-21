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
    $$PWD/StaticData.proto \
    $$PWD/StatusData.proto \
    $$PWD/ProtoData.proto \
    $$PWD/StateData.proto


DEFINES += BUILD_PROTO

protobuf_decl.name = protobuf headers
protobuf_decl.input = PROTOS
protobuf_decl.output = $$GENERATEDDIR/${QMAKE_FILE_BASE}.pb.h

contains(DEFINES, BUILD_PROTO) {
osx{
protobuf_decl.commands = /Users/$$(USER)/Desktop/fc/prebuilt/osx/bin/protoc --cpp_out=$$GENERATEDDIR/ --proto_path=${QMAKE_FILE_IN_PATH} ${QMAKE_FILE_NAME}
}


ios{
protobuf_decl.commands = /Users/$$(USER)/Desktop/fc/ios/extrenal/prototbuf/platform/x86_64/bin/protoc --cpp_out=$$PWD/../$$GENERATEDDIR/ --proto_path=${QMAKE_FILE_IN_PATH} ${QMAKE_FILE_NAME}
}

android{

##FIXME check the HOST to see if it is win32 if not warn that it can not build
## IF YOU NEED THIS INSTALL VIA CELLAR BREW
message (Android coming in )

protobuf_decl.commands =/usr/local/Cellar/protobuf/2.5.0/bin/protoc --cpp_out=$$PWD/../$$GENERATEDDIR/ --proto_path=${QMAKE_FILE_IN_PATH} ${QMAKE_FILE_NAME}

}


win32{
    protobuf_decl.commands = protoc --cpp_out=$$PWD/../$$GENERATEDDIR/ --proto_path=${QMAKE_FILE_IN_PATH} ${QMAKE_FILE_NAME}
}

protobuf_decl.variable_out = HEADERS
QMAKE_EXTRA_COMPILERS += protobuf_decl

protobuf_impl.name = protobuf sources
protobuf_impl.input = PROTOS
protobuf_impl.output = $$GENERATEDDIR/${QMAKE_FILE_BASE}.pb.cc
protobuf_impl.depends = $$GENERATEDDIR/${QMAKE_FILE_BASE}.pb.h
protobuf_impl.commands = $$escape_expand(\n)
protobuf_impl.variable_out = SOURCES
QMAKE_EXTRA_COMPILERS += protobuf_impl
}

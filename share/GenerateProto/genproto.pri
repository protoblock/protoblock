QMAKE_DEBUG{
    !isEmpty(PROTOBUF_INSTALL_PREFIX) : message( Setting Google Protobuf are to $$PROTOBUF_INSTALL_PREFIX )
    isEmpty(PROTOBUF_INSTALL_PREFIX) : message( Google Protobuf is not set, falling back to sysem )
}

osx{
    LIBS += -L$$PROTOBUF_INSTALL_PREFIX/lib -lprotobuf
    INCLUDEPATH += $$PROTOBUF_INSTALL_PREFIX/include
    DEPENDPATH += $$PROTOBUF_INSTALL_PREFIX/include
}

ios{
    LIBS += -L$$PROTOBUF_INSTALL_PREFIX/lib -lprotobuf
    INCLUDEPATH += $$PROTOBUF_INSTALL_PREFIX/include
    DEPENDPATH += $$PROTOBUF_INSTALL_PREFIX/include
}

android{
    LIBS += -L$$PROTOBUF_INSTALL_PREFIX/lib -lprotobuf
    INCLUDEPATH += $$PROTOBUF_INSTALL_PREFIX/include
    DEPENDPATH += $$PROTOBUF_INSTALL_PREFIX/include
}

win32{
    win32-g++: LIBS += -L$$PROTOBUF_INSTALL_PREFIX/lib -lprotobuf
    win32-g++: INCLUDEPATH += $$PROTOBUF_INSTALL_PREFIX/include
    win32-g++: DEPENDPATH += $$PROTOBUF_INSTALL_PREFIX/include

    !win32-g++: LIBS += -L$$PROTOBUF_INSTALL_PREFIX/lib -lprotobuf
    !win32-g++: INCLUDEPATH += $$PROTOBUF_INSTALL_PREFIX/include
    !win32-g++: DEPENDPATH += $$PROTOBUF_INSTALL_PREFIX/include
}

linux:!android{
    isEmpty(PROTOBUF_INSTALL_PREFIX){
        ## try pkg-config
        packagesExist(protobuf){
            PKGCONFIG += protobuf
            QMAKE_DEBUG : message( $$TARGET pkgconfig protobuf )
        }else{
            error( $$TARGET pkgconfig protobuf But not found !  )
        }
    }

    !isEmpty(PROTOBUF_INSTALL_PREFIX){
        LIBS += -L$$PROTOBUF_INSTALL_PREFIX/lib -lprotobuf
        INCLUDEPATH += $$PROTOBUF_INSTALL_PREFIX/include
        DEPENDPATH += $$PROTOBUF_INSTALL_PREFIX/include
        QMAKE_DEBUG : message( $$TARGET: Local Build protobuf )
    }

    LIBS += \
        -pthread \
        -lpthread
}

#################
## BUILD the lib
#################
PROTOS += \
    $$PWD/src/P2PData.proto \
    $$PWD/src/ExData.proto \
    $$PWD/src/NameData.proto \
    $$PWD/src/StaticData.proto \
    $$PWD/src/StatusData.proto \
    $$PWD/src/ProtoData.proto \
    $$PWD/src/StateData.proto \
    $$PWD/src/ApiData.proto


protobuf_decl.name = protobuf headers
protobuf_decl.input = PROTOS
protobuf_decl.output = $$GENERATEDDIR/${QMAKE_FILE_BASE}.pb.h
protobuf_decl.commands = $$PROTOBUF_INSTALL_PREFIX/bin/protoc --cpp_out=$$GENERATEDDIR/ --proto_path=${QMAKE_FILE_IN_PATH} ${QMAKE_FILE_NAME}
protobuf_decl.variable_out = HEADERS
QMAKE_EXTRA_COMPILERS += protobuf_decl

protobuf_impl.name = protobuf sources
protobuf_impl.input = PROTOS
protobuf_impl.output = $$GENERATEDDIR/${QMAKE_FILE_BASE}.pb.cc
protobuf_impl.depends = $$GENERATEDDIR/${QMAKE_FILE_BASE}.pb.h
protobuf_impl.commands = $$escape_expand(\n)
protobuf_impl.variable_out = SOURCES
QMAKE_EXTRA_COMPILERS += protobuf_impl

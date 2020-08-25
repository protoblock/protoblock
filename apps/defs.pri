##############
## Globals
##############

DIRPREFIX = $$PWD/../libs-newpb/
##############
##  WINDOWS
##############
win32 {
    message(win32 Build)
    INCLUDEPATH += $$DIRPREFIX/include
    LIBS+= -L$$DIRPREFIX

    #protobuf
    CONFIG(debug, debug|release) {
        LIBS += -llibprotobufd
        LIBS += -lleveldbd
    }
    else {
        LIBS += -llibprotobuf
        LIBS += -lleveldb
    }
    #openssl
    #openssl
    INCLUDEPATH += $$OPENSSL_PREFIX/include
    LIBS += -L$$OPENSSL_PREFIX/lib
    LIBS += -llibssl -llibcrypto
}
macx {
    message(macx Build)
    INCLUDEPATH += $$DIRPREFIX/include
    LIBS+= -L$$DIRPREFIX

    #protobuf
    LIBS += -lprotobuf
    LIBS += -lleveldb
    LIBS += -lssl
    LIBS += -lcrypto
}


##############
## Globals
##############

LDDIRPREFIX = $$PWD/../libs-ldb17/
PBDIRPREFIX = $$PWD/../libs-pb3/

##############
##  WINDOWS
##############
win32 {
    message(win32 Build)
    INCLUDEPATH += $$LDDIRPREFIX/include
    INCLUDEPATH += $$PBDIRPREFIX/include
    LIBS+= -L$$LDDIRPREFIX
    LIBS+= -L$$PBDIRPREFIX

    #protobuf
    CONFIG(debug, debug|release) {
        LIBS += -llibprotobufd
        LIBS += -lleveldb
    }
    else {
        LIBS += -llibprotobuf
        LIBS += -lleveldb
    }
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


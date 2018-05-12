##############
## Globals
##############

DIRPREFIX = $$PWD/../libs/
##############
##  WINDOWS
##############
win32 {
    message(win32 Build)
    INCLUDEPATH += $$DIRPREFIX
    INCLUDEPATH += $$DIRPREFIX/nng/
    LIBS+= -L$$DIRPREFIX

    #nng
    LIBS += -lnng


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
    LIBS += -llibeay32 \
            -lssleay32
}
macx {
    message(macx Build)
    INCLUDEPATH += $$DIRPREFIX
    LIBS+= -L$$DIRPREFIX

    #nng
    LIBS += -lnng_static


    #protobuf
    LIBS += -lprotobuf
    LIBS += -lleveldb
    LIBS += -lssl
    LIBS += -lcrypto
}


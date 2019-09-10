##############
## Globals
##############

DIRPREFIX = $$PWD/../libs/
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
    LIBS += -llibcrypto \
            -llibssl

#    LIBS += -llibeay32 \
#            -lssleay32
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


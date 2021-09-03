##############
## Globals
##############

DIRPREFIX = $$PWD/../libs/
LDBDIRPREFIX = $$PWD/../libldb/
#OPENSSL_PREFIX=C:\Qt\Tools\OpenSSL\Win_x64
#OPENSSL_PREFIX = $$PWD/../libssl/
OPENSSL_PREFIX=C:\work\protoblock\libsslnew

#Note for QT 5.12.4 and on - there must be both a link to openssl lib and the shared DLLs in the executable path.
##############
##  WINDOWS
##############
win32 {
    message(win32 Build)
    INCLUDEPATH += $$DIRPREFIX/include
    LIBS+= -L$$DIRPREFIX

    INCLUDEPATH += $$LDBDIRPREFIX/include
    LIBS+= -L$$LDBDIRPREFIX

    #protobuf
    CONFIG(debug, debug|release) {
        LIBS += -llibprotobufd
        LIBS += -llibleveldbd
    }
    else {
        LIBS += -llibprotobuf
        LIBS += -llibleveldb
    }
    #openssl
    INCLUDEPATH += $$OPENSSL_PREFIX/include
#    LIBS += -L$$OPENSSL_PREFIX
#    LIBS += -llibeay32 -lssleay32

    LIBS += -L$$OPENSSL_PREFIX
    #    LIBS += -llibssl -llibcrypto
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


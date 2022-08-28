##############
## Globals
##############

DIRPREFIX = $$PWD/../libs/

#Note for QT 5.12.4 and on - there must be both a link to openssl lib and the shared DLLs in the executable path.
##############
##  WINDOWS
##############
win32 {
    message(win32 Build)
    DIRPREFIX = $$DIRPREFIX/win/
    INCLUDEPATH += $$DIRPREFIX/include
    LIBS+= -L$$DIRPREFIX

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
    LIBS += -llibssl -llibcrypto

}
macx {
    message(macx Build)
    DIRPREFIX = $$DIRPREFIX/osx/
    INCLUDEPATH += $$DIRPREFIX/include
    LIBS+= -L$$DIRPREFIX
    message(macx $$LIBS)

    #protobuf
    LIBS += -lprotobuf
    LIBS += -lleveldb
    LIBS += -lssl
    LIBS += -lcrypto
}

ios {

    DIRPREFIX = /Users/$$(USER)/Desktop/fc/ios/extrenal/
    message(iosmacx Build)
    INCLUDEPATH += $$DIRPREFIX/include
    LIBS+= -L$$DIRPREFIX/lib

    #protobuf
    LIBS += -lprotobuf
    LIBS += -lleveldb
    LIBS += -lssl
    LIBS += -lcrypto
}

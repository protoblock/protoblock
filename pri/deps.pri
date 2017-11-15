include ($$PWD/defs.pri)

##############
## Globals
##############

contains (QMAKE_HOST.os, Darwin){
    message("Host is OSX")
    DIRPREFIX = /Users/$$(USER)/Desktop/fc/prebuilt
}else{
    message("Host is Windows")
    DIRPREFIX = D:\work\prebuiltLibs
}



##############
##  WINDOWS
##############

win32 {
    message(win32 Build)
    INCLUDEPATH +=   $$DIRPREFIX/windows/3rdParty
    INCLUDEPATH += $$DIRPREFIX/windows/3rdParty/secp256k1


#    contains(DEFINES, WSSERVER_WRITE_TWEET) {
#        INCLUDEPATH += $$DIRPREFIX/windows/3rdParty/nanomsg
#        LIBS += -lnanomsg
#    }

   ## FIXME
    LIBS+= -L$$DIRPREFIX/windows/libwin64
    #LIBS+= -L$$PWD/../libwin64
    CONFIG(debug, debug|release) {
       LIBS+= -llibprotobufd  \
              -lleveldbd \
              -llibeay32 \
              -lssleay32
#              -lsecp256k1
    }
    CONFIG(release, debug|release) {
       LIBS+= -llibprotobuf \
              -lleveldb \
              -llibeay32 \
              -lssleay32
#              -lsecp256k1
    }

#    BOOST_DIR = C:\local\boost_1_55_0
#    LIBS += -L$${BOOST_DIR}/lib64-msvc-12.0
#    LIBS += -lboost_log-vc120-1_55
 #   INCLUDEPATH += $${BOOST_DIR}

}
osx {
    QMAKE_MAC_SDK = macosx10.13
}
macx {
#    include(/Users/satoshi/work/trading.football/externals/leveldb.pri)
#    LIBS+=/Users/satoshi/work/fantasybit-2015/libosx64/libleveldb.a
}

unix:macx {
#    LIBS+=/usr/local/lib/libleveldb.a
#    LIBS+=/Users/satoshi/work/leveldb/libleveldb.a
#    INCLUDEPATH += /Users/satoshi/Desktop/fc/osx/protoblock/3rdParty
    LIBS += /Users/satoshi/work/fantasybit-2015/externals/leveldb/libleveldb.a
    INCLUDEPATH += /Users/satoshi/work/fantasybit-2015/externals/leveldb/include/
}

##############Ï
##     OSX
##############

macx{
    message(OSX BUILD)
    INCLUDEPATH += /Users/$$(USER)/Desktop/fc/prebuilt/osx/include
    DEPENDPATH += /Users/$$(USER)/Desktop/fc/prebuilt/osx/include

#    LIBS += /Users/$$(USER)/Desktop/fc/prebuilt/osx/lib/libprotobuf.a
#    PRE_TARGETDEPS += /Users/$$(USER)/Desktop/fc/prebuilt/osx/lib/libprotobuf.a
    LIBS += /Users/satoshi/Desktop/fc/osx/protobuf/protobuf-2.5.0/src/.libs/libprotobuf.a
    PRE_TARGETDEPS += /Users/satoshi/Desktop/fc/osx/protobuf/protobuf-2.5.0/src/.libs/libprotobuf.a

#    LIBS += /Users/$$(USER)/Desktop/fc/prebuilt/osx/lib/libsecp256k1.a
#    PRE_TARGETDEPS += /Users/$$(USER)/Desktop/fc/prebuilt/osx/lib/libsecp256k1.a

    LIBS += /Users/$$(USER)/Desktop/fc/prebuilt/osx/lib/libssl.a
    PRE_TARGETDEPS += /Users/$$(USER)/Desktop/fc/prebuilt/osx/lib/libssl.a

    LIBS+=/Users/$$(USER)/Desktop/fc/prebuilt/osx/lib/libcrypto.a
    PRE_TARGETDEPS+=/Users/$$(USER)/Desktop/fc/prebuilt/osx/lib/libcrypto.a

}

##############
##     IOS
##############

ios {
    message (IOS BUILD)
    ##PATHS
    INCLUDEPATH += /Users/$$(USER)/Desktop/fc/ios/extrenal/include
    DEPENDPATH += /Users/$$(USER)/Desktop/fc/ios/extrenal/include

    ##OPENSSL
    LIBS +=/Users/$$(USER)/Desktop/fc/ios/extrenal/lib/libcrypto.a
    PRE_TARGETDEPS +=/Users/$$(USER)/Desktop/fc/ios/extrenal/lib/libcrypto.a
    LIBS +=/Users/$$(USER)/Desktop/fc/ios/extrenal/lib/libssl.a
    PRE_TARGETDEPS +=/Users/$$(USER)/Desktop/fc/ios/extrenal/lib/libssl.a

    # SECP251K1
    LIBS +=/Users/$$(USER)/Desktop/fc/ios/extrenal/lib/libsecp256k1.a
    PRE_TARGETDEPS +=/Users/$$(USER)/Desktop/fc/ios/extrenal/lib/libsecp256k1.a

    ## PROTOBUFF
    LIBS +=/Users/$$(USER)/Desktop/fc/ios/extrenal/lib/libprotobuf.a
    PRE_TARGETDEPS +=/Users/$$(USER)/Desktop/fc/ios/extrenal/lib/libprotobuf.a

}



##############
##  ANDROID
##############
android {
        message(Android Build)
        ##PATHS
        INCLUDEPATH += $$DIRPREFIX/android/extrenal-android/include
        DEPENDPATH += $$DIRPREFIX/android/extrenal-android/include

        ##OPENSSL
        LIBS += $$DIRPREFIX/android/extrenal-android/lib/libcrypto.a
        PRE_TARGETDEPS += $$DIRPREFIX/android/extrenal-android/lib/libcrypto.a
        LIBS += $$DIRPREFIX/android/extrenal-android/lib/libssl.a
        PRE_TARGETDEPS += $$DIRPREFIX/android/extrenal-android/lib/libssl.a

        ##  SECP251K1
        LIBS +=$$DIRPREFIX/android/extrenal-android/lib/libsecp256k1.a
        PRE_TARGETDEPS += $$DIRPREFIX/android/extrenal-android/lib/libsecp256k1.a

        # PROTOBUFF
        LIBS += $$DIRPREFIX/android/extrenal-android/lib/libprotobuf.so
        ## BOTAN
#          LIBS += -$$DIRPREFIX/android/extrenal-android/lib -lBotan
}



##############
##   LINUX
##############
linux!android{
        message(Linux Build)
        CONFIG += link_pkgconfig
        PKGCONFIG += openssl \
                                     protobuf \

        ##  SECP251K1
        LIBS +=$$DIRPREFIX/linux/lib/libsecp256k1.a
        PRE_TARGETDEPS += $$DIRPREFIX/linux/lib/libsecp256k1.a
}


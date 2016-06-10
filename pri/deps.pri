##############
##  WINDOWS
##############

win32 {
    INCLUDEPATH +=   $$PWD/../3rdParty
    INCLUDEPATH += $$PWD/../3rdParty/secp256k1

   ## FIXME
   LIBS+= -L$$PWD/../libwin64

    CONFIG(debug, debug|release) {
#       LIBS += -L$$PWD/../ProRotoQml/jsonpb/debug/ -ljsonpb
       LIBS+= -llibprotobufd  \
              -lleveldbd \
              -llibeay32 \
              -lssleay32 \
    }
    CONFIG(release, debug|release) {
#       LIBS += -L$$PWD/../ProRotoQml/jsonpb/release/ -ljsonpb
       LIBS+= -llibprotobuf \
              -lleveldb \
              -llibeay32 \
              -lssleay32 \
              -lsecp256k1
    }

    BOOST_DIR = C:\local\boost_1_55_0
    LIBS += -L$${BOOST_DIR}/lib64-msvc-12.0
    LIBS += -lboost_log-vc120-1_55
    INCLUDEPATH += $${BOOST_DIR}

}

##############
##     OSX
##############

osx{
message(OSX BUILD)
    INCLUDEPATH += /Users/$$(USER)/Desktop/fc/osx/extrenal/include
    DEPENDPATH += Users/satoshi/Desktop/fc/osx/extrenal/include

    ##FIXME compile levelDB
    INCLUDEPATH += /usr/local/Cellar/leveldb/1.18/include
    DEPENDPATH += /usr/local/Cellar/leveldb/1.18/include

    LIBS += /Users/$$(USER)/Desktop/fc/osx/extrenal/lib/libprotobuf.a
    PRE_TARGETDEPS += /Users/$$(USER)/Desktop/fc/osx/extrenal/lib/libprotobuf.a


    LIBS += /Users/$$(USER)/Desktop/fc/osx/extrenal/lib/libsecp256k1.a
    PRE_TARGETDEPS += /Users/$$(USER)/Desktop/fc/osx/extrenal/lib/libsecp256k1.a

    LIBS += /Users/$$(USER)/Desktop/fc/osx/extrenal/lib/libssl.a
    PRE_TARGETDEPS += /Users/$$(USER)/Desktop/fc/osx/extrenal/lib/libssl.a

    LIBS+=/Users/$$(USER)/Desktop/fc/osx/extrenal/lib/libcrypto.a
    PRE_TARGETDEPS+=/Users/$$(USER)/Desktop/fc/osx/extrenal/lib/libcrypto.a

    ##FIXME compile levelDB
#    LIBS += -L/usr/local/lib  -lleveldb



}



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

    ## BOTAN
    LIBS +=/Users/$$(USER)/Desktop/fc/ios/extrenal/lib/libbotan.a
    PRE_TARGETDEPS +=/Users/$$(USER)/Desktop/fc/ios/extrenal/lib/libbotan.a
}






android {
    ##PATHS
    INCLUDEPATH +=/Users/$$(USER)/Desktop/fc/prebuilt/android/extrenal-android/include
    DEPENDPATH += /Users/$$(USER)/Desktop/fc/prebuilt/android/extrenal-android/lib/include

    ##OPENSSL
    LIBS +=/Users/$$(USER)/Desktop/fc/prebuilt/android/extrenal-android/lib/libcrypto.so
#    PRE_TARGETDEPS +=/Users/$$(USER)/Desktop/fc/prebuilt/android/extrenal-android/lib/libcrypto.a

    LIBS +=/Users/$$(USER)/Desktop/fc/prebuilt/android/extrenal-android/lib/libssl.so
#    PRE_TARGETDEPS +=/Users/$$(USER)/Desktop/fc/prebuilt/android/extrenal-android/lib/libssl.a

    ##  SECP251K1
    LIBS +=/Users/$$(USER)/Desktop/fc/prebuilt/android/extrenal-android/lib/libsecp256k1.a
    PRE_TARGETDEPS +=/Users/$$(USER)/Desktop/fc/prebuilt/android/extrenal-android/lib/libsecp256k1.a

    # PROTOBUFF
      LIBS += -L/Users/$$(USER)/Desktop/fc/prebuilt/android/extrenal-android/lib -lprotobuf

    ## BOTAN
      LIBS += -L/Users/$$(USER)/Desktop/fc/prebuilt/android/extrenal-android/lib -lBotan
}

#LATERFOOL
#unix:!macx:{
#     Debian UbuntuMint ect
#    LIBS += /usr/lib/x86_64-linux-gnu/libmysqlclient_r.so
#    INCLUDEPATH += /usr/include
#}


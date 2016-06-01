osx {
    GENERATEDDIR = $$PWD/../generated
    DEFINES += GOOGLE_NAMESPACE=google_public
    INCLUDEPATH += /Users/$$(USER)/Desktop/fc/ios/extrenal/include
    DEPENDPATH += /Users/$$(USER)/Desktop/fc/ios/extrenal/include
}

ios {
    GENERATEDDIR = $$PWD/../generated
    DEFINES += GOOGLE_NAMESPACE=google_public
    INCLUDEPATH += /Users/$$(USER)/Desktop/fc/ios/extrenal/include
    DEPENDPATH += /Users/$$(USER)/Desktop/fc/ios/extrenal/include
}

android{
#    LIBS += -L/Users/$$(USER)/Qt/5.6/android_armv7/lib -lprotobuf-cpp-full-ndk
#    INCLUDEPATH += /Users/$$(USER)/Qt/5.6/android_armv7/include
INCLUDEPATH += /usr/local/protobuf_2_6_1/include
GENERATEDDIR = $$PWD/../generated
    DEFINES += GOOGLE_NAMESPACE=google
}

win32{
    GENERATEDDIR = $$PWD/../generated
    DEFINES += GOOGLE_NAMESPACE=google
    INCLUDEPATH +=  $$PWD/../3rdParty/
}

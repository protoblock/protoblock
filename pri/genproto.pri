osx :{
    GENERATEDDIR = $$PWD/../generated
    DEFINES += GOOGLE_NAMESPACE=google_public
    INCLUDEPATH += /Users/satoshi/Desktop/fc/ios/extrenal/include
    DEPENDPATH += /Users/satoshi/Desktop/fc/ios/extrenal/include
}

ios {
    DEFINES += GOOGLE_NAMESPACE=google_public
    GENERATEDDIR = $$PWD/../generated
    INCLUDEPATH += /Users/satoshi/Desktop/fc/ios/extrenal/include
    DEPENDPATH += /Users/satoshi/Desktop/fc/ios/extrenal/include
}

android{
    DEFINES += GOOGLE_NAMESPACE=google
    GENERATEDDIR = $$PWD/../generated
    INCLUDEPATH += /Users/satoshi/Desktop/fc/prebuilt/android/extrenal-android/include
    DEPENDPATH += /Users/satoshi/Desktop/fc/prebuilt/android/extrenal-android/include
}

win32:{
    GENERATEDDIR = $$PWD/../generated
    DEFINES += GOOGLE_NAMESPACE=google
    INCLUDEPATH +=  $$PWD/../3rdParty/
}

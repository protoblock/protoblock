osx{
    DEFINES += GOOGLE_NAMESPACE=google_public
    GENERATEDDIR = $$PWD/../generated
#    INCLUDEPATH += /Users/$$(USER)/Desktop/fc/prebuilt/osx/include
    INCLUDEPATH += /Users/satoshi/Desktop/fc/prebuilt/osx/include
    DEPENDPATH+= /Users/satoshi/Desktop/fc/prebuilt/osx/include
}

ios{
    DEFINES += GOOGLE_NAMESPACE=google_public
    GENERATEDDIR = $$PWD/../generated
    INCLUDEPATH += /Users/$$(USER)/Desktop/fc/prebuilt/ios/include
    DEPENDPATH += /Users/$$(USER)/Desktop/fc/prebuilt/ios/include
}

android{
    DEFINES += GOOGLE_NAMESPACE=google
    GENERATEDDIR = $$PWD/../generated
    INCLUDEPATH += /Users/$$(USER)/Desktop/fc/prebuilt/android/extrenal-android/include
    DEPENDPATH += /Users/$$(USER)/Desktop/fc/prebuilt/android/extrenal-android/include
}

win32{
    GENERATEDDIR = $$PWD/../generated
    DEFINES += GOOGLE_NAMESPACE=google
    INCLUDEPATH +=  $$PWD/../3rdParty/
}

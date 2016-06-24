osx {
    GENERATEDDIR = $$PWD/../generated
    DEFINES += GOOGLE_NAMESPACE=google
    INCLUDEPATH += /Users/$$(USER)/Desktop/fc/prebuilt/osx/include
    DEPENDPATH += /Users/$$(USER)/Desktop/fc/prebuilt/osx/include
}

ios {
    DEFINES += GOOGLE_NAMESPACE=google_public
    GENERATEDDIR = $$PWD/../generated_public
    INCLUDEPATH += /Users/$$(USER)/Desktop/fc/ios/extrenal/include
    DEPENDPATH += /Users/$$(USER)/Desktop/fc/ios/extrenal/include
}

android{
    message("genpro android !x86 build")
    DEFINES += GOOGLE_NAMESPACE=google
    GENERATEDDIR = $$PWD/../generated
    DIRPREFIX = D:\work\prebuiltLibs
#        DIRPREFIX = /Users/$$(USER)/Desktop/fc/prebuilt

    INCLUDEPATH += $$DIRPREFIX/android/extrenal-android/include
    DEPENDPATH += $$DIRPREFIX/android/extrenal-android/include
}


win32:{
    GENERATEDDIR = $$PWD/../generated
    DEFINES += GOOGLE_NAMESPACE=google
    INCLUDEPATH +=  $$PWD/../3rdParty/
}

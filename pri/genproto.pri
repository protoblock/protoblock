macx {
    GENERATEDDIR = $$PWD/../core/proto/generated
    DEFINES += GOOGLE_NAMESPACE=google
    message(macx $$GENERATEDDIR)
}

ios{
    DEFINES += GOOGLE_NAMESPACE=google_public
    GENERATEDDIR = $$PWD/../generated-public
    INCLUDEPATH += /Users/$$(USER)/Desktop/fc/prebuilt/ios/extrenal/include
    DEPENDPATH += /Users/$$(USER)/Desktop/fc/prebuilt/ios/extrenal/include

}

android{
    DEFINES += GOOGLE_NAMESPACE=google
    GENERATEDDIR = $$PWD/../generated
#    INCLUDEPATH += /Users/$$(USER)/Desktop/fc/prebuilt/android/extrenal-android/include
#    DEPENDPATH += /Users/$$(USER)/Desktop/fc/prebuilt/android/extrenal-android/include
}

win32 {
    GENERATEDDIR = $$PWD/../core/proto/generated
    DEFINES += GOOGLE_NAMESPACE=google
    message(win32osx $$GENERATEDDIR)
}


INCLUDEPATH += $$GENERATEDDIR
message(gen $$GENERATEDDIR)

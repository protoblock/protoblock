macx {
    GENERATEDDIR = $$PWD/../share/generated
    DEFINES += GOOGLE_NAMESPACE=google
    message(macx $$GENERATEDDIR)
}

ios{
    DEFINES += GOOGLE_NAMESPACE=google_public
    GENERATEDDIR = $$PWD/../generated-public
}

android{
    DEFINES += GOOGLE_NAMESPACE=google
    GENERATEDDIR = $$PWD/../generated
}

win32 {
    GENERATEDDIR = $$PWD/../share/generated
    DEFINES += GOOGLE_NAMESPACE=google
    message(win32 $$GENERATEDDIR)
}


INCLUDEPATH += $$GENERATEDDIR
message(gen $$GENERATEDDIR)

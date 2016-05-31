osx {
    GENERATEDDIR = $$PWD/generated
    DEFINES += GOOGLE_NAMESPACE=google_public
}

ios {
    GENERATEDDIR = $$PWD/generated
    DEFINES += GOOGLE_NAMESPACE=google_public
}

android{
    GENERATEDDIR = $$PWD/generated
    DEFINES += GOOGLE_NAMESPACE=google
}

win32{
    GENERATEDDIR = $$PWD/generated
    DEFINES += GOOGLE_NAMESPACE=google
}

INCLUDEPATH += $$GENERATEDDIR

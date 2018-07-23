macx {
    GENERATEDDIR = $$PWD/generated
    DEFINES += GOOGLE_NAMESPACE=google
    message(macx $$GENERATEDDIR)
}

win32 {
    GENERATEDDIR = $$PWD/generated
    DEFINES += GOOGLE_NAMESPACE=google
    message(win32 $$GENERATEDDIR)
}

#ios{
#    DEFINES += GOOGLE_NAMESPACE=google_public
#    GENERATEDDIR = $$PWD/generated-public
#}

#android{
#    DEFINES += GOOGLE_NAMESPACE=google
#    GENERATEDDIR = $$PWD/generated
#}

INCLUDEPATH += $$GENERATEDDIR
message(gen $$GENERATEDDIR)


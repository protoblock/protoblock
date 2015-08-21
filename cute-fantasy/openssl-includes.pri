INCLUDEPATH+=$$PWD/../openssl/include

#CONFIG(debug, debug|release) {
#    LIBS += -L$$PWD/openssl/lib/debug
#}
#else {
#    LIBS += -L$$PWD/openssl/lib/release
#}

LIBS += -llibeay32 \
        -lssleay32

CONFIG += ordered
TEMPLATE = subdirs

message(p $$CONFIG )

SUBDIRS += \
    $$PWD/apps \
    $$PWD/share

OTHER_FILES += \
        $$PWD/README.md \
        $$PWD/LICENSE

# additional arguments to make
# "OPENSSL_PREFIX=C:/Qt/Tools/OpenSSL/Win_x64"

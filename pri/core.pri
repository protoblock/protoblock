message(core $$PWD)

INCLUDEPATH += $$PWD/../ProRotoQml/protoblock-core
INCLUDEPATH += $$PWD/../ProRotoQml/protoblock-core/bitcoin-core-base58

INCLUDEPATH += $$PWD/../ProRotoQml/QmlModels
INCLUDEPATH += $$PWD/../ProRotoQml/QmlModels/QmlSuperMacros

SOURCES += \
#    protoblockcore.cpp \
    $$PWD/../ProRotoQml/protoblock-core/FantasyAgent.cpp \
    $$PWD/../ProRotoQml/protoblock-core/appsettings.cpp \
    $$PWD/../ProRotoQml/protoblock-core/city.cpp \
    $$PWD/../ProRotoQml/protoblock-core/Commissioner.cpp \
    $$PWD/../ProRotoQml/protoblock-core/crc.cpp \
    $$PWD/../ProRotoQml/protoblock-core/FantasyName.cpp \
    $$PWD/../ProRotoQml/protoblock-core/mnemonic.cpp \
    $$PWD/../ProRotoQml/protoblock-core/platform.cpp \
    $$PWD/../ProRotoQml/protoblock-core/bitcoin-core-base58/base58.cpp \
    $$PWD/../ProRotoQml/protoblock-core/utils/utils.cpp \
    $$PWD/../ProRotoQml/protoblock-core/ldbwriter.cpp

HEADERS += \
#protoblockcore.h\
#        protoblock-core_global.h \
    $$PWD/../ProRotoQml/protoblock-core/appsettings.h \
    $$PWD/../ProRotoQml/protoblock-core/city.hpp \
    $$PWD/../ProRotoQml/protoblock-core/Commissioner.h \
    $$PWD/../ProRotoQml/protoblock-core/DataPersist.h \
    $$PWD/../ProRotoQml/protoblock-core/FantasyAgent.h \
    $$PWD/../ProRotoQml/protoblock-core/FantasyName.h \
    $$PWD/../ProRotoQml/protoblock-core/fbutils.h \
    $$PWD/../ProRotoQml/protoblock-core/genericsingleton.h \
    $$PWD/../ProRotoQml/protoblock-core/globals.h \
    $$PWD/../ProRotoQml/protoblock-core/mnemonic.h \
    $$PWD/../ProRotoQml/protoblock-core/optional.hpp \
    $$PWD/../ProRotoQml/protoblock-core/platform.h \
    $$PWD/../ProRotoQml/protoblock-core/uint128.hpp \
    $$PWD/../ProRotoQml/protoblock-core/utility.hpp \
    $$PWD/../ProRotoQml/protoblock-core/bitcoin-core-base58/allocators.h \
    $$PWD/../ProRotoQml/protoblock-core/bitcoin-core-base58/base58.h \
    $$PWD/../ProRotoQml/protoblock-core/bitcoin-core-base58/hash.h \
    $$PWD/../ProRotoQml/protoblock-core/utils/utils.h \
    $$PWD/../ProRotoQml/protoblock-core/ldbwriter.h


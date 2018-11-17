message(core $$PWD)

INCLUDEPATH += $$PWD
INCLUDEPATH += $$PWD/bitcoin-core-base58

SOURCES += \
    $$PWD/FantasyAgent.cpp \
    $$PWD/appsettings.cpp \
    $$PWD/city.cpp \
    $$PWD/Commissioner.cpp \
    $$PWD/crc.cpp \
    $$PWD/FantasyName.cpp \
    $$PWD/mnemonic.cpp \
    $$PWD/platform.cpp \
    $$PWD/bitcoin-core-base58/base58.cpp \
    $$PWD/utils/utils.cpp \
    $$PWD/ldbwriter.cpp \
    $$PWD/bitcoinutils.cpp \
    $$PWD/bitcoinapi.cpp

HEADERS += \
    $$PWD/appsettings.h \
    $$PWD/city.hpp \
    $$PWD/Commissioner.h \
    $$PWD/DataPersist.h \
    $$PWD/FantasyAgent.h \
    $$PWD/FantasyName.h \
    $$PWD/fbutils.h \
    $$PWD/genericsingleton.h \
    $$PWD/globals.h \
    $$PWD/mnemonic.h \
    $$PWD/optional.hpp \
    $$PWD/platform.h \
    $$PWD/uint128.hpp \
    $$PWD/utility.hpp \
    $$PWD/bitcoin-core-base58/allocators.h \
    $$PWD/bitcoin-core-base58/base58.h \
    $$PWD/bitcoin-core-base58/hash.h \
    $$PWD/utils/utils.h \
    $$PWD/ldbwriter.h \
    $$PWD/bitcoinutils.h \
    $$PWD/bitcoinapi.h


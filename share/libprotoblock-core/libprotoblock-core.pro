TARGET = protoblock-core
TEMPLATE = lib
QT += \
    core \
    gui \
    network
CONFIG += \
    c++11 \
    static

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
    $$PWD/RestFullCall.cpp \
    $$PWD/ExchangeData.cpp \
    $$PWD/PeerNode.cpp \
    $$PWD/Processor.cpp \
    $$PWD/blockrecorder.cpp \
    $$PWD/Data.cpp \
    $$PWD/DistributionAlgo.cpp \
    $$PWD/NameData.cpp \
    $$PWD/dataservice.cpp  \
    $$PWD/core.cpp \
    $$PWD/LAPIWorker.cpp \
    $$PWD/NodeWorker.cpp \
    $$PWD/iresolvable.cpp

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
    $$PWD/RestFullCall.h \
    $$PWD/ExchangeData.h \
    $$PWD/PeerNode.h \
    $$PWD/Processor.h \
    $$PWD/blockrecorder.h \
    $$PWD/Data.h \
    $$PWD/DistributionAlgo.h \
    $$PWD/pbutils.h \
    $$PWD/NameData.h \
    $$PWD/dataservice.h \
    $$PWD/core.h \
    $$PWD/threadedqobject.h \
    $$PWD/LAPIWorker.h \
    $$PWD/NodeWorker.h \
    $$PWD/iresolvable.h \
    $$PWD/pbgateways.h



headers.files += $$HEADERS
headers.CONFIG += no_check_exist

win32|win64{
    CONFIG += static
    headers.path = $$INSTALL_PREFIX/$$TARGET/usr/include
    target.path = $$INSTALL_PREFIX/$$TARGET/usr/lib
}
ios{
    headers.path = $$INSTALL_PREFIX/usr/local/include/$$TARGET
    target.path = $$INSTALL_PREFIX/usr/local/lib
}
macx{
    headers.path = $$INSTALL_PREFIX/usr/local/include/$$TARGET
    target.path = $$INSTALL_PREFIX/usr/local/lib
}
linux{
    headers.path = $$INSTALL_PREFIX/usr/local/include/$$TARGET
    target.path = $$INSTALL_PREFIX/usr/local/lib
}

INSTALLS += \
    target \
    headers



################
## LIBS
###############
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../GenerateProto/release/ -lProtoblockProtocols
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../GenerateProto/debug/ -lProtoblockProtocols
else:unix: LIBS += -L$$OUT_PWD/../GenerateProto/ -lProtoblockProtocols
INCLUDEPATH += $$PWD/../GenerateProto
DEPENDPATH += $$PWD/../GenerateProto
INCLUDEPATH += $$GENERATEDDIR
DEPENDPATH += $$GENERATEDDIR


win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../libsecp256k1/release/ -lsecp256k1
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../libsecp256k1/debug/ -lsecp256k1
else:unix: LIBS += -L$$OUT_PWD/../libsecp256k1/ -lsecp256k1
win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../libsecp256k1/release/libsecp256k1.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../libsecp256k1/debug/libsecp256k1.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../libsecp256k1/release/secp256k1.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../libsecp256k1/debug/secp256k1.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../libsecp256k1/libsecp256k1.a
INCLUDEPATH += $$PWD/../libsecp256k1
DEPENDPATH += $$PWD/../libsecp256k1
INCLUDEPATH += $$PWD/../libsecp256k1/include
DEPENDPATH += $$PWD/../libsecp256k1/include

message(src $$PWD)

include($$PWD/QmlModels/QmlModels.pri)
include($$PWD/QmlSuperMacros/QmlSuperMacros.pri)
include($$PWD/core/core.pri)
include($$PWD/state-model/state-model.pri)

INCLUDEPATH += $$PWD

SOURCES += \
    $$PWD/main.cpp \
    $$PWD/Data.cpp \
    $$PWD/DistributionAlgo.cpp \
    $$PWD/Processor.cpp \
    $$PWD/RestFullCall.cpp \
    $$PWD/PeerNode.cpp \
    $$PWD/blockrecorder.cpp \
    $$PWD/ExchangeData.cpp \
    $$PWD/NameData.cpp \
    $$PWD/core.cpp \
    $$PWD/LAPIWorker.cpp \
    $$PWD/NodeWorker.cpp \
    $$PWD/iresolvable.cpp \
    $$PWD/dataservice.cpp \
    $$PWD/fullgateway.cpp \
    $$PWD/importLoader.cpp \
    $$PWD/SwapStateData.cpp \
    $$PWD/bitcoinwalletworker.cpp

HEADERS += \
    $$PWD/PeerNode.h \
    $$PWD/pbutils.h \
    $$PWD/data.h \
    $$PWD/core.h \
    $$PWD/NameData.h \
    $$PWD/Processor.h \
    $$PWD/ExchangeData.h \
    $$PWD/RestFullCall.h \
    $$PWD/LAPIWorker.h \
    $$PWD/NodeWorker.h \
    $$PWD/threadedqobject.h \
    $$PWD/iresolvable.h \
    $$PWD/dataservice.h \
    $$PWD/fullgateway.h \
    $$PWD/importLoader.h \
    $$PWD/SwapStateData.h \
    $$PWD/bitcoinwalletworker.h

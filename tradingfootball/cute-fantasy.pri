SOURCES += \
    $$PWD/Commissioner.cpp \
    $$PWD/Data.cpp \
    $$PWD/DistributionAlgo.cpp \
    $$PWD/FantasyName.cpp \
    $$PWD/Processor.cpp \
    $$PWD/Source.cpp \
    $$PWD/main.cpp \        
    $$PWD/decorable.cpp \
    $$PWD/descriptable.cpp \
    $$PWD/singleapplication.cpp \    
    $$PWD/spinboxdelegate.cpp \        
    $$PWD/RestFullCall.cpp \
    $$PWD/LAPIWorker.cpp \
    $$PWD/NodeWorker.cpp \
    $$PWD/PeerNode.cpp \
    $$PWD/blockrecorder.cpp \
    $$PWD/platform.cpp \
    $$PWD/appsettings.cpp \
    $$PWD/playerdataviewmodel.cpp \
    $$PWD/core.cpp \
    $$PWD/mainwindow.cpp \
    $$PWD/playerprojectionwidget.cpp \
    $$PWD/currentweekwidget.cpp \
    $$PWD/previousweekwidget.cpp \
    $$PWD/nextweekwidget.cpp \
    $$PWD/iresolvable.cpp \    
    $$PWD/dataservice.cpp \
    $$PWD/tablemodels.cpp \
    $$PWD/waitmodaldialog.cpp \
    $$PWD/animationandmessagewidget.cpp \
    $$PWD/mnemonic.cpp \ 
    $$PWD/RunGuard.cpp \
    $$PWD/FantasyAgent.cpp \
    $$PWD/fnametool.cpp \
    $$PWD/timestate.cpp \
    $$PWD/ExchangeData.cpp \
    $$PWD/trading.cpp \
    $$PWD/julylightchanges.cpp \
    $$PWD/NameData.cpp





HEADERS += \
    $$PWD/Commissioner.h \
    $$PWD/Data.h \
    $$PWD/DataPersist.h \
    $$PWD/DistributionAlgo.h \
    $$PWD/FantasyAgent.h \
    $$PWD/FantasyName.h \
    $$PWD/fbutils.h \
    $$PWD/Processor.h \
    $$PWD/Source.h \         
    $$PWD/tlistmodel.h \
    $$PWD/tlistmodel.h \
    $$PWD/decorable.h \
    $$PWD/descriptable.h \
    $$PWD/globals.h \
    $$PWD/singleapplication.h \
    $$PWD/spinboxdelegate.h \
    $$PWD/tkeyedlist.h \
    $$PWD/RestFullCall.h \
    $$PWD/LAPIWorker.h \
    $$PWD/NodeWorker.h \
    $$PWD/PeerNode.h \
    $$PWD/blockrecorder.h \
    $$PWD/threadedqobject.h \
    $$PWD/platform.h \
    $$PWD/genericsingleton.h \
    $$PWD/appsettings.h \
    $$PWD/playerdataviewmodel.h \
    $$PWD/viewmodels.h \
    $$PWD/core.h \
    $$PWD/mainwindow.h \
    $$PWD/playerprojectionwidget.h \
    $$PWD/currentweekwidget.h \
    $$PWD/previousweekwidget.h \
    $$PWD/nextweekwidget.h \
    $$PWD/iresolvable.h \
    $$PWD/projectionstrategybase.h \
    $$PWD/dataservice.h \
    $$PWD/tablemodels.h \
    $$PWD/propertynames.h \
    $$PWD/datacache.h \
    $$PWD/waitmodaldialog.h \
    $$PWD/animationandmessagewidget.h \
    $$PWD/mnemonic.h \ 
    $$PWD/RunGuard.h \
    $$PWD/fnametool.h \
    $$PWD/timestate.h \
    $$PWD/ExchangeData.h \
    $$PWD/trading.h \
    $$PWD/julylightchanges.h \
    $$PWD/NameData.h



FORMS += \       
    $$PWD/mainwindow.ui \
    $$PWD/playerprojectionwidget.ui \
    $$PWD/currentweekwidget.ui \
    $$PWD/previousweekwidget.ui \
    $$PWD/nextweekwidget.ui \
    $$PWD/waitmodaldialog.ui \
    $$PWD/animationandmessagewidget.ui \
    $$PWD/fnametool.ui \
    $$PWD/trading.ui



RESOURCES += \
    $$PWD/resources/resources.qrc


contains(DEFINES, DATAAGENTGUI){
    SOURCES += $$PWD/testingwindow.cpp   
    SOURCES += $$PWD/playerloader.cpp
    SOURCES += $$PWD/dataagent.cpp
    HEADERS += $$PWD/dataagent.h

    HEADERS += $$PWD/playerloader.h
    FORMS += $$PWD/testingwindow.ui
    HEADERS += $$PWD/testingwindow.h
}

contains(DEFINES, MIKECLAYIMPORT){
    SOURCES += $$PWD/playerloader.cpp
    HEADERS += $$PWD/playerloader.h
}

contains(DEFINES, BLOCK_EXPLORER) {
    #DISTFILES += ./../../block-explorer/proto/StateData.proto
}

contains(DEFINES, TESTING_PB) {
    PB = ../../protoblock\ProRotoQml\Protoblock-core\

    SOURCES += \
        $$PB/bitcoin-core-base58/base58.cpp \
        $$PB/utils/utils.cpp \
        $$PB/crc.cpp


    HEADERS += \
        $$PB/bitcoin-core-base58/allocators.h \
        $$PB/bitcoin-core-base58/base58.h \
        $$PB/bitcoin-core-base58/hash.h \
        $$PB/utils/utils.h \
        $$PB/optional.hpp \
        $$PB/uint128.hpp

}



    SOURCES += \
    $$PWD/../generated/ProtoData.pb.cc \
    $$PWD/../generated/StaticData.pb.cc \
    $$PWD/../generated/StatusData.pb.cc \
    $$PWD/../generated/ApiData.pb.cc \
    $$PWD/../generated/NameData.pb.cc \
    $$PWD/../generated/ExData.pb.cc \
    $$PWD/../generated/StateData.pb.cc

#    DISTFILES += \
#        $$PWD/../proto/ProtoData.proto \
#        $$PWD/../proto/StaticData.proto \
#        $$PWD/../proto/StatusData.proto \
#        $$PWD/../proto/NameData.proto \
#        $$PWD/../proto/ApiData.proto \
#        $$PWD/../proto/ExData.proto


    HEADERS += $$PWD/../generated/ApiData.pb.h \
    $$PWD/../generated/ProtoData.pb.h \
    $$PWD/../generated/StaticData.pb.h \
    $$PWD/../generated/StatusData.pb.h \
    $$PWD/../generated/NameData.pb.h \
    $$PWD/../generated/ExData.pb.h \
    $$PWD/../generated/StateData.pb.h

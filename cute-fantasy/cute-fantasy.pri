SOURCES += \
    $$PWD/../src/Commissioner.cpp \
    $$PWD/../src/Data.cpp \
    $$PWD/../src/DistributionAlgo.cpp \
    $$PWD/../src/FanatsyAgent.cpp \
    $$PWD/../src/FantasyName.cpp \
    $$PWD/../src/Processor.cpp \
    $$PWD/../src/Source.cpp \
    $$PWD/main.cpp \        
    $$PWD/decorable.cpp \
    $$PWD/descriptable.cpp \
    $$PWD/singleapplication.cpp \    
    $$PWD/spinboxdelegate.cpp \        
    $$PWD/RestFullCall.cpp \
    $$PWD/LAPIWorker.cpp \
    $$PWD/NodeWorker.cpp \
    $$PWD/../src/PeerNode.cpp \
    $$PWD/../src/blockrecorder.cpp \
    $$PWD/platform.cpp \
    $$PWD/appsettings.cpp \
    $$PWD/playerdataviewmodel.cpp \
    $$PWD/core.cpp \
    $$PWD/../generated/ProtoData.pb.cc \
    $$PWD/../generated/StaticData.pb.cc \
    $$PWD/../generated/StatusData.pb.cc \        
    $$PWD/../generated/ApiData.pb.cc \
    $$PWD/../src/NameData.cpp \
    $$PWD/../generated/NameData.pb.cc \
    $$PWD/mainwindow.cpp \
    $$PWD/playerprojectionwidget.cpp \
    $$PWD/currentweekwidget.cpp \
    $$PWD/previousweekwidget.cpp \
    $$PWD/nextweekwidget.cpp \
    $$PWD/iresolvable.cpp \    
    $$PWD/dataservice.cpp \
    $$PWD/tablemodels.cpp \
    $$PWD/dataagent.cpp \
    $$PWD/waitmodaldialog.cpp \
    $$PWD/animationandmessagewidget.cpp \
    $$PWD/../src/mnemonic.cpp

DISTFILES += \
    $$PWD/../proto/ProtoData.proto \
    $$PWD/../proto/StaticData.proto \
    $$PWD/../proto/StatusData.proto \
    $$PWD/../proto/NameData.proto \
    $$PWD/../proto/ApiData.proto

HEADERS += \
    $$PWD/../src/Commissioner.h \
    $$PWD/../src/Data.h \
    $$PWD/../src/DataPersist.h \
    $$PWD/../src/DistributionAlgo.h \
    $$PWD/../src/FantasyAgent.h \
    $$PWD/../src/FantasyName.h \
    $$PWD/../src/fbutils.h \
    $$PWD/../src/Processor.h \
    $$PWD/../src/Source.h \         
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
    $$PWD/../src/PeerNode.h \
    $$PWD/../src/blockrecorder.h \
    $$PWD/threadedqobject.h \
    $$PWD/platform.h \
    $$PWD/genericsingleton.h \
    $$PWD/appsettings.h \
    $$PWD/playerdataviewmodel.h \
    $$PWD/viewmodels.h \
    $$PWD/core.h \
    $$PWD/../generated/ApiData.pb.h \
    $$PWD/../generated/ProtoData.pb.h \
    $$PWD/../generated/StaticData.pb.h \
    $$PWD/../generated/StatusData.pb.h \        
    $$PWD/../src/NameData.h \
    $$PWD/../generated/NameData.pb.h \
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
    $$PWD/dataagent.h \
    $$PWD/waitmodaldialog.h \
    $$PWD/animationandmessagewidget.h \
    $$PWD/../src/mnemonic.h

FORMS += \       
    $$PWD/mainwindow.ui \
    $$PWD/playerprojectionwidget.ui \
    $$PWD/currentweekwidget.ui \
    $$PWD/previousweekwidget.ui \
    $$PWD/nextweekwidget.ui \
    $$PWD/waitmodaldialog.ui \
    $$PWD/animationandmessagewidget.ui

RESOURCES += \
    $$PWD/resources/resources.qrc


contains(DEFINES, DATAAGENTGUI){
    SOURCES += $$PWD/testingwindow.cpp   
    HEADERS += $$PWD/playerloader.h
    FORMS += $$PWD/testingwindow.ui
    HEADERS += $$PWD/testingwindow.h
}


message(state-model $$PWD)

INCLUDEPATH += $$PWD

SOURCES += \
    $$PWD/socketclient.cpp \
    $$PWD/mediator.cpp \
    $$PWD/playerquoteslicemodel.cpp \
    $$PWD/depthmarketmodel.cpp \
    $$PWD/sortfilterproxymodel.cpp

HEADERS += \
    $$PWD/socketclient.h \
    $$PWD/mediator.h \
    $$PWD/playerquoteslicemodel.h \
    $$PWD/PlayerSymbolsModel.h \
    $$PWD/depthmarketmodel.h \
    $$PWD/fantasynamemodel.h \
    $$PWD/openordersmodel.h \
    $$PWD/weeklyschedulemodel.h \
    $$PWD/playerprojmodel.h \
    $$PWD/playerresultmodel.h \
    $$PWD/sortfilterproxymodel.h \
    $$PWD/pbgateways.h

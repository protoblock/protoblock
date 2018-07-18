#include "torrent_plugin.h"
#include <qqml.h>

void ProRotoQmlTorrentPlugin::registerTypes(const char *uri)
{
    // @uri ProRotoQml.Torrent
    qmlRegisterType<TorrentModel>( uri, 1, 0, "TorrentModel" );
    qmlRegisterType<TorrentClient>(uri, 1, 0, "TorrentClient" );
    qmlRegisterSingletonType<RateController>(uri, 1, 0, "RateController", rateControllerProvider);
}

//void ProRotoQmlTorrentPlugin::initializeEngine(QQmlEngine *engine, const char *uri)
//{
//    Q_UNUSED (uri)
//    TorrentModel torrentModel;
//    TorrentSerializer   serializer( &torrentModel );
//                        serializer.load();
//                        serializer.startAutoSave();
//        engine->rootContext()->setContextProperty("torrentModel", &torrentModel);
//}


QObject *rateControllerProvider(QQmlEngine *engine, QJSEngine *scriptEngine)
{
    Q_UNUSED (engine)
    Q_UNUSED( scriptEngine )
    return new RateController();
}


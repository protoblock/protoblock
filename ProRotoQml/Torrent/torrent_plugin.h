#ifndef TORRENT_PLUGIN_H
#define TORRENT_PLUGIN_H

#include <QQmlExtensionPlugin>
#include <QQmlEngine>
#include <QQmlContext>
#include "torrentmodel.h"
#include "ratecontroller.h"
#include "torrentserializer.h"

static QObject *rateControllerProvider(QQmlEngine *engine, QJSEngine *scriptEngine);

class ProRotoQmlTorrentPlugin : public QQmlExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QQmlExtensionInterface")

public:
    void registerTypes(const char *uri);
//    void  initializeEngine(QQmlEngine *engine, const char *uri);

};

#endif // TORRENT_PLUGIN_H

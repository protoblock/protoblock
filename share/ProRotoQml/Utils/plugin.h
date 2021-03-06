#ifndef ProRotoQmlUtilsPlugin_H
#define ProRotoQmlUtilsPlugin_H
#include <QJSEngine>
#include <QQmlExtensionPlugin>
static QObject *deviceSingleton(QQmlEngine *engine, QJSEngine *scriptEngine);

class ProRotoQmlUtilsPlugin : public QQmlExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QQmlExtensionInterface")

public:
    void registerTypes(const char *uri);
};


//#include "utils_plugin.moc"
#endif // UTILS_PLUGIN_H

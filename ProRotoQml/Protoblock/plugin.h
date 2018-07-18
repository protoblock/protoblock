#ifndef PROTOBLOCK_PLUGIN_H
#define PROTOBLOCK_PLUGIN_H

#include <QQmlExtensionPlugin>
#include <QJSEngine>
#include <QQmlEngine>
#include <QQmlContext>

static QObject *middleMan(QQmlEngine *engine, QJSEngine *scriptEngine);

class ProRotoQmlProtoblockPlugin : public QQmlExtensionPlugin
{

    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QQmlExtensionInterface")

public:
    void registerTypes(const char *uri);
private:
//    static
};

#endif // PROTOBLOCK_PLUGIN_H

#ifndef BACKEND_PLUGIN_H
#define BACKEND_PLUGIN_H

// qt
#include <QQmlExtensionPlugin>
#include <QQmlEngine>
#include <QQmlContext>
#include <QDebug>



static QObject *playerMap(QQmlEngine *engine, QJSEngine *scriptEngine);
static QObject *teamMap(QQmlEngine *engine, QJSEngine *scriptEngine);
static QObject *pLdbHashReaderTool(QQmlEngine *engine, QJSEngine *scriptEngine);
static QObject *pLdbHashReaderMap(QQmlEngine *engine, QJSEngine *scriptEngine);
static QObject *playerNewModel(QQmlEngine *engine, QJSEngine *scriptEngine);
static QObject *pTempApi(QQmlEngine *engine, QJSEngine *scriptEngine);

class ProRotoQmlBackendPlugin : public QQmlExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QQmlExtensionInterface")

public:
    void registerTypes(const char *uri);
    void initializeEngine(QQmlEngine *engine, const char *uri);

};

#endif // BACKEND_PLUGIN_H

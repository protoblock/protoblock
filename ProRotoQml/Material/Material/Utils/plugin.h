#ifndef MATERIALUTILSPLUGIN_H
#define MATERIALUTILSPLUGIN_H

#include <QQmlExtensionPlugin>
#include <QtQml>
#include <QQmlEngine>
#include <QJSEngine>

static QObject *deviceSingleton(QQmlEngine *engine, QJSEngine *scriptEngine);


class MaterialUtilsPlugin : public QQmlExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QQmlExtensionInterface")
public:
    void registerTypes(const char *uri);
};

#endif // MATERIALUTILSPLUGIN_H

#ifndef THEMEPLUGIN_H
#define THEMEPLUGIN_H

#include <QQmlExtensionPlugin>
#include <QStandardPaths>
#include <QQmlContext>
#include <QQmlEngine>
#include <QString>
#include <QGuiApplication>

#include "colors.h"

static QObject *colors(QQmlEngine *engine, QJSEngine *scriptEngine);
static QObject *formFactor(QQmlEngine *engine, QJSEngine *scriptEngine);
static QObject *themes(QQmlEngine *engine, QJSEngine *scriptEngine);
static QObject *teamColors(QQmlEngine *engine, QJSEngine *scriptEngine);

class ProRotoQmlThemePlugin : public QQmlExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QQmlExtensionInterface")

public:
    void registerTypes(const char *uri);
    void  initializeEngine(QQmlEngine *engine, const char *uri);

};

#endif // PLUGIN_H

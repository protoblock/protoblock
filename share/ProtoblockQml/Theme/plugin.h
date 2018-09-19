#ifndef PROTOBLOCKQMLTHEMEPLUGIN_H
#define PROTOBLOCKQMLTHEMEPLUGIN_H

#include <QQmlExtensionPlugin>
#include <QStandardPaths>
#include <QQmlContext>
#include <QQmlEngine>
#include <QString>
#include <QGuiApplication>

#include "colors.h"
#include "formfactor.h"
#include "palette.h"
#include "units.h"
#include "listener.h"
#include "teamcolors.h"
#include "protoscreen.h"

class ProtoblockQmlThemePlugin : public QQmlExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QQmlExtensionInterface")

public:
    void registerTypes(const char *uri);
    void  initializeEngine(QQmlEngine *engine, const char *uri);

};

#endif // PROTOBLOCKQMLTHEMEPLUGIN_H

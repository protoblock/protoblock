#include <QJSEngine>
#include <QQmlExtensionPlugin>
#include "utils.h"
#include "plugininisetter.h"
//#include "qqmlprocess.h"
#include "passwordstrength.h"
#include "device.h"
#include "units.h"
#include "qqmlwebsockets.h"

#ifndef PROTOBLOCKQMLUTILSPLUGIN_H
#define PROTOBLOCKQMLUTILSPLUGIN_H

class ProRotoQmlUtilsPlugin : public QQmlExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QQmlExtensionInterface")

public:
    void registerTypes( const char *uri );
};
#endif // PROTOBLOCKQMLUTILSPLUGIN_H

#include "plugin.h"
#include "device.h"
#include "units.h"


void MaterialUtilsPlugin::registerTypes(const char *uri)
{
    // @uri Material.Utils
    qmlRegisterSingletonType<Device>(uri, 1, 0, "Device", deviceSingleton );
    qmlRegisterUncreatableType<Units>(uri, 1, 0, "Units", QStringLiteral("Units can only be used via the attached property."));
}


QObject *deviceSingleton(QQmlEngine *engine, QJSEngine *scriptEngine)
{
    Q_UNUSED(engine)
    Q_UNUSED(scriptEngine)
    Device *device = new Device();
    return device;
}

//#include "plugin.moc"

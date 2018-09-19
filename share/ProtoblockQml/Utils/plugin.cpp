#include "plugin.h"

static QObject *deviceSingleton(QQmlEngine *engine, QJSEngine *scriptEngine)
{
    Q_UNUSED( engine )
    Q_UNUSED( scriptEngine )
    Device *device = new Device;
    return device;
}

void ProRotoQmlUtilsPlugin::registerTypes(const char *uri)
{
    // @uri Protoblock.Utils
    qmlRegisterType<PluginIniSetter> (uri,1,0,"PluginIniSetter");
    qmlRegisterType<Utils>(uri, 1, 0, "Utils");
    qmlRegisterType<QQmlWebSocket>(uri,1,0,"ProtoblockSocket");
    qmlRegisterType<PasswordStrength>(uri,1,0,"PasswordStrength");
    qmlRegisterSingletonType<Device>(uri, 1, 0, "Device", &deviceSingleton );
    qmlRegisterUncreatableType<Units>(uri, 1, 0, "Units", QStringLiteral("Units can only be used via the attached property."));
}

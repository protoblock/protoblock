#include "plugin.h"
#include "utils.h"
#include "plugininisetter.h"
//#include "qqmlprocess.h"
#include "passwordstrength.h"
#include "device.h"
#include "units.h"
#include "qqmlwebsockets.h"
#include <qqml.h>

void ProRotoQmlUtilsPlugin::registerTypes(const char *uri)
{
    // @uri Protoblock.Utils

    // We need to fix this to run system commands
//    qmlRegisterType<QQmlProcess>(uri,1,0,"QmlProcess");
    qmlRegisterType<PluginIniSetter> (uri,1,0,"PluginIniSetter");
    qmlRegisterType<Utils>(uri, 1, 0, "Utils");
    qmlRegisterType<QQmlWebSocket>(uri,1,0,"ProtoblockSocket");
    qmlRegisterType<PasswordStrength>(uri,1,0,"PasswordStrength");
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


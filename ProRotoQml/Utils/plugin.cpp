#include "plugin.h"
#include "utils.h"
#include "plugininisetter.h"
//#include "qqmlprocess.h"
#include "passwordstrength.h"

#include <qqml.h>

void ProRotoQmlUtilsPlugin::registerTypes(const char *uri)
{
    // @uri Yacm.Utils
//    qmlRegisterType<QQmlProcess>(uri,1,0,"QmlProcess");
    qmlRegisterType<PluginIniSetter> (uri,1,0,"PluginIniSetter");
    qmlRegisterType<Utils>(uri, 1, 0, "Utils");
    qmlRegisterType<PasswordStrength>(uri,1,0,"PasswordStrength");
}
//#include "plugin.moc"


#include "plugin.h"
#include <qqml.h>
#include <QCoreApplication>
#include "mediator.h"
#include "getuserinfo.h"
void ProRotoQmlProtoblockPlugin::registerTypes(const char *uri)
{
    // @uri ProRotoQml.Protoblock

    qmlRegisterType<GetUserInfo>(uri,1,0,"UserInfo");
    qmlRegisterSingletonType<Mediator>(uri,1,0,"MiddleMan",middleMan);

}
QObject *middleMan(QQmlEngine *engine, QJSEngine *scriptEngine)
{
    Q_UNUSED(engine)
    Q_UNUSED(scriptEngine)
    return Mediator::instance();
}


#include "plugin.h"
#include <qqml.h>
#include <QCoreApplication>
#include "mediator.h"
//#include "getuserinfo.h"
using namespace pb;
void ProRotoQmlProtoblockPlugin::registerTypes(const char *uri)
{
    // @uri ProRotoQml.Protoblock

//    qmlRegisterType<GetUserInfo>(uri,1,0,"UserInfo");
    qmlRegisterSingletonType<Mediator>(uri,1,0,"MiddleMan",middleMan);
//    qmlRegisterType<PlayerQuoteSliceModelItem>(uri,1,0,"mPlayerQuoteSliceModelItem");
//    qmlRegisterType<PlayerQuoteSliceModel>(uri,1,0,"ptrPlayerQuoteSliceModel");
    qmlRegisterUncreatableType<PlayerQuoteSliceModel>(uri, 1, 0, "PlayerQuoteSliceModel","");

}
QObject *middleMan(QQmlEngine *engine, QJSEngine *scriptEngine)
{
    Q_UNUSED(engine)
    Q_UNUSED(scriptEngine)
    return pb::Mediator::instance();
}


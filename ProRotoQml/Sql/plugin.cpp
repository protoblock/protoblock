#include "plugin.h"
#include "qmlsqldatabase.h"
#include "qqmlsqlquery.h"
#include "qmlsqlquerymodel.h"
#include "qmlsqlcreatedatabase.h"
//#include "qmlsqltablemode.h"
#include <qqml.h>

void ProRotoQmlSqlPlugin::registerTypes(const char *uri)
{
    // @uri ProRotoQml.Sql
    qmlRegisterType<QmlSqlDatabase>(uri, 1, 0, "QmlSqlDatabase");
    qmlRegisterType<QQmlSqlQuery>(uri,1,0,"QmlSqlQuery");
    qmlRegisterType<QmlSqlQueryModel>(uri,1,0,"QmlSqlQueryModel");
//    qmlRegisterType<QmlSqlTableModel>(uri,1,0,"QmlSqlTableModel");
    qmlRegisterType<QmlSqlCreateDatabase>(uri,1,0,"QmlSqlCreateDatabase");
}


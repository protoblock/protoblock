#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QScreen>
#include <QFont>
#include <QStandardPaths>
#include <QQmlContext>
#include <QtQml>
#include <QQmlEngine>
#include <qqml.h>
#include <QObject>
#include <QDebug>
#include <QtGlobal>
#include "dataservice.h"

#include "core.h"
#include "mediator.h"
#include "fullgateway.h"
#include "importLoader.h"
//#include "RunGuard.h"
#ifdef DATAAGENTWRITENAMES
#include "../../../fantasybit-2015/tradingfootball/playerloader.h"
#endif


#ifdef QT_WEBVIEW_WEBENGINE_BACKEND
#include <QtWebEngine>
#endif // QT_WEBVIEW_WEBENGINE_BACKEND
#include <QSysInfo>

//fix me check the arguments here and
// add help page
int main(int argc, char *argv[])
{
//    RunGuard guard( "protoblockprodapp" );
//        if ( !guard.tryToRun() )
//            return 0;

    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;
//    QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
//    QSysInfo sysInfo;
//    QString sInfo = sysInfo.productType ();
#ifdef QT_WEBVIEW_WEBENGINE_BACKEND
    QtWebEngine::initialize();
#endif // QT_WEBVIEW_WEBENGINE_BACKEND

//    if (sInfo == "windows"){
//        engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
//    }
//    else if (sInfo == "osx" )
//    {
//        engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
//    }
//    else if (sInfo == "android")
//    {
//        QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
//        QFont mFont;
//        mFont.setPixelSize (48);
//        qApp->setFont (mFont);
//        engine.load(QUrl(QStringLiteral("qrc:/phone-main.qml")));
//    }
//    // Window phone
//    else if (sInfo  == "winrt")
//    {
//        QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
//        QFont mFont;
//        mFont.setPixelSize (48);
//        qApp->setFont (mFont);
//        engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
//    }
//    else if (sInfo == "ios" )`
//    {
//        QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
//        QFont mFont;
//        mFont.setPixelSize (48);
//        qApp->setFont (mFont);
//        engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
//    }


//    qputenv("QT_QUICK_CONTROLS_STYLE", "Base");

    Core::instance()->bootstrap();


    AllStatsLoader2014 asl;
    auto res = asl.loadAllDataWeek1to16for2014();
    return 0;

    //MainLAPIWorker *mw = ;
    //mw->dataService = DataService::instance();
    pb::FullGateway *fg = new pb::FullGateway(Core::resolveByName<MainLAPIWorker>("coreapi"),
                                     DataService::instance());

    pb::Mediator::instance()->setContext(fg);

    engine.rootContext()->setContextProperty("MiddleMan", pb::Mediator::instance());
    pb::ImportLoader il;
    engine.rootContext()->setContextProperty("ImportLoader", &il);

    //qmlRegisterSingletonType<pb::Mediator>(uri,1,0,"MiddleMan",middleMan);

//    SqlStuff sql(true,"testingmain");
//    qDebug() << " last seq " << sql.lastSeq() << " last seq";
//    return 0;

    engine.dumpObjectInfo();
    engine.load(QUrl(QStringLiteral("qrc:/MaterialMain.qml")));
    return app.exec();
}

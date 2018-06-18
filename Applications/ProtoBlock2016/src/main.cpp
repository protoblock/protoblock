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
#include "DataPersist.h"
#include <QQmlContext>
#include <QQuickStyle>


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


    QQuickStyle::setStyle("Material");

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




//    GameStatsLoaderFD gamestatsloader;
//    auto out = gamestatsloader.loadPastYearGameStatsFromFantasyData(1);
//    auto &gamer = out["201410101"];
//    qDebug() << " gr fgr" << gamer.DebugString().data();
//    return 0;

    /*
    AllStatsLoader2014 asl;
    std::vector<Transaction> txs = asl.loadAllDataWeek1to16for2014();
    return 0;
    /*
    {
        Writer<void> writetx("D:\\data\\Transition2014.out",ios::app);
        if ( !writetx.good() )
            qDebug() << " bad Writer";
        else for ( auto &tx : txs)
            writetx(tx);
    }

    {
    Reader<Transaction> readertx("D:\\data\\Transition2014.out");
    Transaction tx;
    while ( readertx.ReadNext(tx) ) {
        qDebug() << tx.DebugString().data();
    }
    }
    return 0;

    /**/

    //MainLAPIWorker *mw = ;
    //mw->dataService = DataService::instance();

    Core::instance()->bootstrap();
    pb::FullGateway *fg = new pb::FullGateway(Core::resolveByName<MainLAPIWorker>("coreapi"),
                                     DataService::instance());
    pb::Mediator::instance()->setContext(fg);



    engine.rootContext()->setContextProperty("MiddleMan", pb::Mediator::instance());
    pb::ImportLoader il;
    engine.rootContext()->setContextProperty("ImportLoader", &il);

    MainLAPIWorker *ml = Core::resolveByName<MainLAPIWorker>("coreapi");
//    QObject::connect(&engine, SIGNAL(quit()),
    //QObject::connect(&app, SIGNAL(aboutToQuit()),
    app.setQuitOnLastWindowClosed(false);
    QObject::connect(&app, SIGNAL(lastWindowClosed()),
                     ml, SLOT(Quit()));

    //qmlRegisterSingletonType<pb::Mediator>(uri,1,0,"MiddleMan",middleMan);

//    SqlStuff sql(true,"testingmain");
//    qDebug() << " last seq " << sql.lastSeq() << " last seq";
//    return 0;

    qDebug() << engine.importPathList();
    engine.dumpObjectInfo();
    engine.load(QUrl(QStringLiteral("qrc:/MaterialMain.qml")));

    /*
    AllStatsLoader2014 asl;
    std::vector<Transaction> txs = asl.loadAllDataWeek1to16for2014();

    {
        Writer<void> writetx("D:\\data\\Transition2014.out",ios::app);
        if ( !writetx.good() )
            qDebug() << " bad Writer";
        else for ( auto &tx : txs)
            writetx(tx);
    }

    {
    Reader<Transaction> readertx("D:\\data\\Transition2014.out");
    Transaction tx;
    while ( readertx.ReadNext(tx) ) {
        qDebug() << " read one";
        qDebug() << tx.DebugString().data();
    }
    }
    /**/
    qDebug() << "xxxx" << QCoreApplication::libraryPaths();
    return app.exec();
}

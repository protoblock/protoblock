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
#include "RunGuard.h"
#include "DataPersist.h"
#include <QQmlContext>
#include <QQuickStyle>


#ifdef QT_WEBVIEW_WEBENGINE_BACKEND
#include <QtWebEngine>
#endif // QT_WEBVIEW_WEBENGINE_BACKEND
#include <QSysInfo>


#ifndef NO_DEBUG_FILE_OUT
void messageHandler(QtMsgType type,
                    const QMessageLogContext &context,
                    const QString &message)
{

#ifndef ALLOW_DEBUG
    return;
#else
    static QString logFileName = AppSettings::instance()->getSetting(AppSettings::LogFilePath).toString();
    static std::ofstream  logFile(logFileName.toStdString(),std::ofstream::app);
    if (logFile)
        logFile << qPrintable(qFormatLogMessage(type,context, message))<< std::endl;
#endif

}
#endif

//fix me check the arguments here and
// add help page
int main(int argc, char *argv[])
{
    RunGuard guard( "protoblockprodapp" );
        if ( !guard.tryToRun() )
            return 0;

    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

#ifndef NO_DEBUG_FILE_OUT
    qSetMessagePattern(AppSettings::instance()->getSetting(AppSettings::LogMessagePattern).toString());
    qInstallMessageHandler(messageHandler);
#endif


    QQuickStyle::setStyle("Material");

    QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QSysInfo sysInfo;
    QString sInfo = sysInfo.productType ();

    qputenv("QT_QUICK_CONTROLS_STYLE", "Base");

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


    qDebug() << " exist " << QFileInfo(":/materialMain.qml").exists();

    qDebug() << " exist2 " << QFileInfo("qrc:/materialMain.qml").exists();

    engine.load(QUrl(QStringLiteral("qrc:/materialMain.qml")));

//    engine.addImportPath(QStringLiteral("qrc:/"));
    qDebug() << engine.importPathList();
    engine.dumpObjectInfo();
//    engine.load(QUrl(QStringLiteral(":/materialMain.qml")));
//    engine.load(QUrl(":/materialMain.qml"));


//    QDirIterator it(":", QDirIterator::Subdirectories);
//    while (it.hasNext()) {
//        qDebug() << it.next();
//    }
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

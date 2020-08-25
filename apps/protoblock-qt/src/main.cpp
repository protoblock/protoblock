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

int main(int argc, char *argv[])
{
#ifdef INTEL_KLUDGE
    QGuiApplication::setAttribute(Qt::AA_UseSoftwareOpenGL);
#endif

    RunGuard guard( "protoblockqtprodapp" );
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
    qputenv("QT_QUICK_CONTROLS_STYLE", "Base");

    Core::instance()->bootstrap();
    pb::FullGateway *fg = new pb::FullGateway(Core::resolveByName<MainLAPIWorker>("coreapi"),
                                     DataService::instance());
    pb::Mediator::instance()->setContext(fg);

    engine.rootContext()->setContextProperty("MiddleMan", pb::Mediator::instance());
    pb::ImportLoader il;
    engine.rootContext()->setContextProperty("ImportLoader", &il);

    MainLAPIWorker *ml = Core::resolveByName<MainLAPIWorker>("coreapi");
    app.setQuitOnLastWindowClosed(false);
    QObject::connect(&app, SIGNAL(lastWindowClosed()),
                     pb::Mediator::instance(), SLOT(guiReady()));

    QObject::connect(&app, SIGNAL(lastWindowClosed()),
                     ml, SLOT(Quit()));


    engine.load(QUrl(QStringLiteral("qrc:/materialMain.qml")));

    return app.exec();
}

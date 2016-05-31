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
#include "qqmlwebsockets.h"
#include <QtWebEngine/qtwebengineglobal.h>

//#include "mediator.h"



int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

#ifdef Q_OS_ANDROID
    QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QFont mFont;
    mFont.setPixelSize (48);
    qApp->setFont (mFont);
#endif

    QQmlApplicationEngine engine;

    QtWebEngine::initialize();

    QString appDir = qApp->applicationDirPath ();
    QQmlContext *context = new QQmlContext(engine.rootContext());

    context->setContextProperty ("AppDir", appDir);

    qmlRegisterType<QQmlWebSocket>("Socket",1,0,"ProtoblockSocket");

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}

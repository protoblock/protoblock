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

#ifdef QT_WEBVIEW_WEBENGINE_BACKEND
#include <QtWebEngine>
#endif // QT_WEBVIEW_WEBENGINE_BACKEND

#include <QSysInfo>



//#include "mediator.h"






int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QSysInfo sysInfo;
    QString sInfo = sysInfo.productType ();


#ifdef QT_WEBVIEW_WEBENGINE_BACKEND
    QtWebEngine::initialize();
#endif // QT_WEBVIEW_WEBENGINE_BACKEND


    QQmlApplicationEngine engine;
    QString appDir = qApp->applicationDirPath ();


    QQmlContext *context = new QQmlContext(engine.rootContext());

    context->setContextProperty ("AppDir", appDir);
    qmlRegisterType<QQmlWebSocket>("ProtoblockSocket",1,0,"ProtoblockSocket");




    if (sInfo == "windows"){
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    }
    else if (sInfo == "osx" )
    {
     engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    }
    else if (sInfo == "android")
    {
        QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
        QFont mFont;
        mFont.setPixelSize (48);
        qApp->setFont (mFont);
        engine.load(QUrl(QStringLiteral("qrc:/phone-main.qml")));
    }
    // Window phone
    else if (sInfo  == "winrt")
    {
        QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
        QFont mFont;
        mFont.setPixelSize (48);
        qApp->setFont (mFont);
        engine.load(QUrl(QStringLiteral("qrc:/phone-main.qml")));
    }
    else if (sInfo == "ios" )
    {
        QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
        QFont mFont;
        mFont.setPixelSize (48);
        qApp->setFont (mFont);
        engine.load(QUrl(QStringLiteral("qrc:/phone-main.qml")));
    }







    return app.exec();
}

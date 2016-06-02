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

#ifdef QT_WEBVIEW_WEBENGINE_BACKEND
#include <QtWebEngine>
#endif // QT_WEBVIEW_WEBENGINE_BACKEND
#include <QSysInfo>

//fix me check the arguments here and
// add help page
int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;
//    QSysInfo sysInfo;
//    QString sInfo = sysInfo.productType ();
//#ifdef QT_WEBVIEW_WEBENGINE_BACKEND
//    QtWebEngine::initialize();
//#endif // QT_WEBVIEW_WEBENGINE_BACKEND

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
//    else if (sInfo == "ios" )
//    {
//        QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
//        QFont mFont;
//        mFont.setPixelSize (48);
//        qApp->setFont (mFont);
//        engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
//    }
        engine.load(QUrl(QStringLiteral("qrc:/FirstApp.qml")));
    return app.exec();
}

#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include <QStandardPaths>
#include <QQmlContext>
#include <QQmlEngine>
#include <QString>

#include <qqml.h>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;


    QString homeDir = QStandardPaths::standardLocations(QStandardPaths::HomeLocation).first();
    engine.rootContext ()->setContextProperty ("homePath", homeDir);

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}

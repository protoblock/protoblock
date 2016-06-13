#include <QtCore/QCoreApplication>
#include <QtCore/QCommandLineParser>
#include <QtCore/QCommandLineOption>
#include "server.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QCommandLineParser parser;
    parser.setApplicationDescription("ProRoto WebSocket server");
    parser.addHelpOption();

    QCommandLineOption dbgOption(QStringList() << "d" << "debug",
            QCoreApplication::translate("main", "Debug output [default: off]."));
    parser.addOption(dbgOption);
    QCommandLineOption portOption(QStringList() << "p" << "port",
            QCoreApplication::translate("main", "Port for echoserver [default: PB_WS_TX_PORT]."),
            QCoreApplication::translate("main", "port"), QLatin1Literal("PB_WS_TX_PORT"));
    parser.addOption(portOption);
    parser.process(a);

    bool debug = parser.isSet(dbgOption);
    int port = parser.value(portOption).toInt();

    Server *server = new Server(PB_WS_TX_PORT, debug);
    QObject::connect(server, &Server::closed, &a, &QCoreApplication::quit);

    Server *nameServer = new Server(PB_WS_LITE_AGENT_PORT,debug);
    QObject::connect(nameServer, &Server::closed, &a, &QCoreApplication::quit);

    return a.exec();
}

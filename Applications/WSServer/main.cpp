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

    TxServer *txserver = new TxServer(PB_WS_TX_PORT, debug);
    QObject::connect(txserver, &TxServer::closed, &a, &QCoreApplication::quit);

    LiteServer *nameServer = new LiteServer(PB_WS_LITE_AGENT_PORT,debug);
    QObject::connect(nameServer, &LiteServer::closed, &a, &QCoreApplication::quit);

    return a.exec();
}

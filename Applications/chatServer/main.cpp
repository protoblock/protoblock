#include "qwebchannel.h"

#include <QCoreApplication>
#include <QUrl>
#include <QDebug>

#include <QtWebSockets/QWebSocketServer>

#include "websocketclientwrapper.h"
#include "websockettransport.h"
#include "chatserver.h"
#include "fbutils.h"

int main(int argc, char** argv)
{
    QCoreApplication app(argc, argv);

    QWebSocketServer server(QStringLiteral("Protoblock Chat Server"),
                            QWebSocketServer::NonSecureMode);
    if (!server.listen(QHostAddress::Any, fantasybit::PB_WS_CHAT_PORT)) {
        qFatal("Failed to open web socket server.");
        return 1;
    }

    // wrap WebSocket clients in QWebChannelAbstractTransport objects
    WebSocketClientWrapper clientWrapper(&server);

    // setup the channel
    QWebChannel channel;
    QObject::connect(&clientWrapper, &WebSocketClientWrapper::clientConnected,
                     &channel, &QWebChannel::connectTo);

    // setup the dialog and publish it to the QWebChannel
    ChatServer* chatserver = new ChatServer(&app);
    channel.registerObject("chatserver", chatserver);

    return app.exec();
}

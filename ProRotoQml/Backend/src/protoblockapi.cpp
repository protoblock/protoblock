#include <QtNetwork>

#include <stdlib.h>

#include "protoblockapi.h"

ProtoBlobkApi::ProtoBlobkApi(QObject *parent) :
    QObject(parent),
    tcpServer(Q_NULLPTR),
    networkSession(0)
{

    //    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
//    statusLabel->setTextInteractionFlags(Qt::TextBrowserInteraction);

    QNetworkConfigurationManager manager;
    if (manager.capabilities() & QNetworkConfigurationManager::NetworkSessionRequired) {
        // Get saved network configuration
        QSettings settings(QSettings::UserScope, QLatin1String("QtProject"));
        settings.beginGroup(QLatin1String("QtNetwork"));
        const QString id = settings.value(QLatin1String("DefaultNetworkConfiguration")).toString();
        settings.endGroup();

        // If the saved network configuration is not currently discovered use the system default
        QNetworkConfiguration config = manager.configurationFromIdentifier(id);
        if ((config.state() & QNetworkConfiguration::Discovered) !=
            QNetworkConfiguration::Discovered) {
            config = manager.defaultConfiguration();
        }

        networkSession = new QNetworkSession(config, this);
        connect(networkSession, &QNetworkSession::opened, this, &ProtoBlobkApi::sessionOpened);

        qDebug() << "Opening network session.";
        networkSession->open();
    } else {
        sessionOpened();
    }

        fortunes << tr("You've been leading a dog's life. Stay off the furniture.")
                 << tr("You've got to think about tomorrow.")
                 << tr("You will be surprised by a loud noise.")
                 << tr("You will feel hungry again in another hour.")
                 << tr("You might have mail.")
                 << tr("You cannot kill time without injuring eternity.")
                 << tr("Computers are not intelligent. They only think they are.");

//        QPushButton *quitButton = new QPushButton(tr("Quit"));
//        quitButton->setAutoDefault(false);
//        connect(quitButton, &QAbstractButton::clicked, this, &QWidget::close);
        connect(tcpServer, &QTcpServer::newConnection, this, &ProtoBlobkApi::sendFortune);

//        QHBoxLayout *buttonLayout = new QHBoxLayout;
//        buttonLayout->addStretch(1);
//        buttonLayout->addWidget(quitButton);
//        buttonLayout->addStretch(1);

//        QVBoxLayout *mainLayout = Q_NULLPTR;
//        if (QGuiApplication::styleHints()->showIsFullScreen() || QGuiApplication::styleHints()->showIsMaximized()) {
//            QVBoxLayout *outerVerticalLayout = new QVBoxLayout(this);
//            outerVerticalLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::Ignored, QSizePolicy::MinimumExpanding));
//            QHBoxLayout *outerHorizontalLayout = new QHBoxLayout;
//            outerHorizontalLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::MinimumExpanding, QSizePolicy::Ignored));
//            QGroupBox *groupBox = new QGroupBox(QGuiApplication::applicationDisplayName());
//            mainLayout = new QVBoxLayout(groupBox);
//            outerHorizontalLayout->addWidget(groupBox);
//            outerHorizontalLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::MinimumExpanding, QSizePolicy::Ignored));
//            outerVerticalLayout->addLayout(outerHorizontalLayout);
//            outerVerticalLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::Ignored, QSizePolicy::MinimumExpanding));
//        } else {
//            mainLayout = new QVBoxLayout(this);
//        }

//        mainLayout->addWidget(statusLabel);
//        mainLayout->addLayout(buttonLayout);

//?        setWindowTitle(QGuiApplication::applicationDisplayName());
}

void ProtoBlobkApi::sessionOpened()
{
    // Save the used configuration
    if (networkSession) {
        QNetworkConfiguration config = networkSession->configuration();
        QString id;
        if (config.type() == QNetworkConfiguration::UserChoice)
            id = networkSession->sessionProperty(QLatin1String("UserChoiceConfiguration")).toString();
        else
            id = config.identifier();

        QSettings settings(QSettings::UserScope, QLatin1String("QtProject"));
        settings.beginGroup(QLatin1String("QtNetwork"));
        settings.setValue(QLatin1String("DefaultNetworkConfiguration"), id);
        settings.endGroup();
    }


    tcpServer = new QTcpServer(this);
    if (!tcpServer->listen()) {
       qDebug() <<  "ProtoBlockApi  Unable to start the ProtoBlockApi:." << tcpServer->errorString() ;
//        close();
        return;
    }
//! [0]
    QString ipAddress;
    QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
    // use the first non-localhost IPv4 address
    for (int i = 0; i < ipAddressesList.size(); ++i) {
        if (ipAddressesList.at(i) != QHostAddress::LocalHost &&
            ipAddressesList.at(i).toIPv4Address()) {
            ipAddress = ipAddressesList.at(i).toString();
            break;
        }
    }
    // if we did not find one, use IPv4 localhost
    if (ipAddress.isEmpty())
        ipAddress = QHostAddress(QHostAddress::LocalHost).toString();
    qDebug() << "The ProtoBlockApi is running on\n\nIP: %1\nport: %2\n\n"
                          << " "
                         << ipAddress
                         << " "
                         <<  tcpServer->serverPort();

}


void ProtoBlobkApi::sendFortune()
{

    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_0);

    out << (quint16)0;
    out << fortunes.at(qrand() % fortunes.size());
    out.device()->seek(0);
    out << (quint16)(block.size() - sizeof(quint16));


    QTcpSocket *clientConnection = tcpServer->nextPendingConnection();
    connect(clientConnection, &QAbstractSocket::disconnected,
            clientConnection, &QObject::deleteLater);


    clientConnection->write(block);
    clientConnection->disconnectFromHost();

}


#ifndef MEDIATOR_H
#define MEDIATOR_H

#include "socketclient.h"
#include <QObject>
#include "QQmlPtrPropertyHelpers.h"
#include "QQmlConstRefPropertyHelpers.h"
#include "QQmlAutoPropertyHelpers.h"
#include "QQmlEnumClassHelper.h"
#include "QMap"
#include "StateData.pb.h"
#include "FantasyAgent.h"

//QML_ENUM_CLASS (nameStatus, none=1, notavil, requested, confirmed )

class Mediator : public QObject
{
    Q_OBJECT

    //protoected :)
    QML_READONLY_CSTREF_PROPERTY (QString, namefrompk)
    QML_CONSTANT_CSTREF_PROPERTY (QVariantMap, nameStatuses)

    QML_CONSTANT_CSTREF_PROPERTY (QString, secert3File)

    Q_PROPERTY(QString playersName READ playersName  NOTIFY playersNameChanged)
    Q_PROPERTY(QString  playersStatus READ playersStatus  NOTIFY playersStatusChanged)

    Q_PROPERTY(MyNameStatus myNameStatus READ myNameStatus NOTIFY myNameStatusChanged)
    Q_ENUMS (MyNameStatus)

    QML_CONSTANT_CSTREF_PROPERTY (QString, chatServerAddr)

public:

    //FIXME make destroctor as this wikl leak
    static Mediator *instance();

    QString playersStatus()const;
    void setPlayersStatus(const QString &playersStatus);

    QString playersName()const;
    void setPlayersName(const QString &playersName);


    enum MyNameStatus{
        None=1,
        NotAvil=2,
        Requested=3,
        TransactionSent=4,
        Confirmed=5
    };
    MyNameStatus myNameStatus()const;
    void setMyNameStatus(const MyNameStatus &myNameStatus);

    qint64 sendBinaryMessage(const QByteArray &data);
    qint64 sendTextMessage(const QString &message);
    QHostAddress  peerAddress() const;
    QAbstractSocket::SocketState  state()const;
    QWebSocketProtocol::Version  version()const;
#ifdef Q_OS_WIN32
    Q_INVOKABLE QString lastKnowLoc();
#endif
    Q_INVOKABLE QString nameStatusGet(const QString&);
    Q_INVOKABLE void pk2fname(const QString&);
    Q_INVOKABLE void checkname(const QString&);
    Q_INVOKABLE QString importMnemonic(const QString &importStr);
    Q_INVOKABLE void signPlayer(const QString &name);
    Q_INVOKABLE void init();


signals:
    void usingFantasyName(QString);
    void nameStatusChanged (QString, QString);
    void nameCheckGet( const QString & name, const QString & status );
    //    void myNameChang (const QString & name, QString status );
    void error(QString);
    void myNameStatusChanged();
    void errorStringChanged();
    void playersNameChanged();
    void playersStatusChanged();
protected slots:
    void handdleUsingName(const QString &name);
    void handdleNameStatus(const QString &name,const QString &status );
    void handleSocketError(QAbstractSocket::SocketError error);
    void handdleNameStatuses();
    void handleError(const QString err);
    void handleClosed();
    void onConnected();
    void handleAboutToClose();
    void onTextMessageReceived( QString message);
    void onBinaryMessageRecived(const QByteArray &message);

private:
    QWebSocket m_webSocket, m_txsocket;
    fantasybit::WsReq lastPk2name;
    fantasybit::FantasyAgent m_fantasy_agent;
    static Mediator *myInstance;
    explicit Mediator(QObject *parent = 0);
    MyNameStatus m_myNameStatus;
    QString m_errorString;
    QString m_playersName;
    QString m_playersStatus;
};

#endif // MEDIATOR_H

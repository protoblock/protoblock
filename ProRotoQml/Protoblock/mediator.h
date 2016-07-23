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
#include "QQmlListPropertyHelper.h"
#include <QTimer>
#include "fbutils.h"

//QML_ENUM_CLASS (nameStatus, none=1, notavil, requested, confirmed )

class Mediator : public QObject
{
    Q_OBJECT

    //protoected :)
    QML_READONLY_CSTREF_PROPERTY (QString, namefrompk)
//    QML_CONSTANT_CSTREF_PROPERTY (QVariantMap, nameStatuses)

  //  QML_CONSTANT_CSTREF_PROPERTY (QString, secert3File)
    QML_READONLY_CSTREF_PROPERTY (QString, encyptPath)
    QML_READONLY_CSTREF_PROPERTY (bool, engineStatus)

    Q_PROPERTY(QString errorString READ errorString NOTIFY errorStringChanged)
    Q_PROPERTY(QString webSocketErrorString  READ webSocketErrorString NOTIFY webSocketErrorStringChanged)

    Q_PROPERTY(SocketState socketState READ socketState NOTIFY socketStateChanged)
    Q_ENUMS (SocketState)

//    Q_PROPERTY(QString playersName READ playersName  NOTIFY playersNameChanged)
//    Q_PROPERTY(QString  playersStatus READ playersStatus  NOTIFY playersStatusChanged)

//    Q_PROPERTY(MyNameStatus myNameStatus READ myNameStatus NOTIFY myNameStatusChanged)
//    Q_ENUMS (MyNameStatus)

    QML_CONSTANT_CSTREF_PROPERTY (QString, chatServerAddr)

//    QML_LIST_PROPERTY(Mediator,goodFname,QString)


//    QML_READONLY_CSTREF_PROPERTY (QStringList, allNames2)
    //    Q_PROPERTY(QQmlListProperty<QString> goodFnames READ goodFnames NOTIFY goodFnamesChanged)

public:
    /*!
     * \brief webSocketErrorString
     * For SOCKET errors only
     */
    QString webSocketErrorString()const;

    /*!
     * \brief errorString
     * Font NON-Socket releated errors
     */
    QString errorString()const;

    /*!
     * socket state used alot to check what is going on with the sockets from QML
     */
    enum SocketState{
        Unconnected,
        Lookup,
        Connecting,
        Connected,
        Bound,
        Closing,
        Listening,
        Default
    };
    SocketState socketState()const{
        return m_socketState;
    }






    QStringList m_goodList;
    QStringList m_allNamesList;
    QStringList m_allROWList;

    Q_INVOKABLE void startDepth(const QString&);


    Q_INVOKABLE void stopDepth(const QString&);



//     QList<QString *> m_goodFnames;
//     QQmlListProperty<QString> goodFnames() {
//         return QQmlListProperty<QString>(this, m_goodFnames);
//     }

//     QString *goodFname(int index) const
//     {
//         return m_goodFnames.at(index);
//     }

//    int goodFnameCount() const{
//        return m_goodFnames.count();
//    }

    //FIXME make destroctor as this wikl leak
    static Mediator *instance();

    QString playersStatus()const;
    void setPlayersStatus(const QString &playersStatus);

    QString playersName();
    void setPlayersName(const QString &playersName);


    Q_INVOKABLE void newOrder(const QString &id, int qty, int price);

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
//    std::string lastYearPath();


    Q_INVOKABLE void allNamesGet();
//    Q_INVOKABLE QString nameStatusGet(const QString&);
    Q_INVOKABLE void pk2fname(const QString&);
    Q_INVOKABLE void checkname(const QString&);
    Q_INVOKABLE QString importMnemonic(const QString &importStr);
    Q_INVOKABLE void signPlayer(const QString &name);
    Q_INVOKABLE void useName(const QString &name);
    Q_INVOKABLE QString init();
    Q_INVOKABLE QString getSecret();
    Q_INVOKABLE QStringList goodList() { return m_goodList; }
    Q_INVOKABLE QStringList allNamesList() { return m_allNamesList; }
    Q_INVOKABLE bool isTestNet() { return fantasybit::IS_TEST_NET; }

    Q_INVOKABLE QStringList allRowMarketList() { return m_allNamesList; }

    qint64 sendBinaryMessage(const GOOGLE_NAMESPACE::protobuf::Message &data);


signals:
    void importSuccess(const QString name, bool passfail);

    void usingFantasyName(const QString &name, bool isdefault = false);
//    void nameStatusChanged (QString, QString);
    void nameCheckGet( const QString & name, const QString & status );
    //    void myNameChang (const QString & name, QString status );
//    void myNameStatusChanged();


    void error(QString);
    void socketError(QString);
    void socketStateChanged();
    void webSocketErrorStringChanged();
    void errorStringChanged();

    void goodFnamesChanged();
    void playersNameChanged();
    void playersStatusChanged();
    void leaderBoardchanged();
    // for QML only
    bool engineUpdate(bool);

protected slots:
//    void handdleUsingName(const QString &name);
//    void handdleNameStatus(const QString &name,const QString &status );
//    void handdleNameStatuses();




    void handleError(const QString err);
    void handleWebSocketError(const QString err);

    void handleClosed();
    void onConnected();
    void handleAboutToClose();
    void onTextMessageReceived( QString message);
    void onBinaryMessageRecived(const QByteArray &message);
    void getSignedPlayerStatus();

    // slot to update QML ONLY propertys
    void handleEngineUpdate(const bool &sta);

private slots:
    void handleSocketError(QAbstractSocket::SocketError err);
//    void handleSocketState(QAbstractSocket::SocketState sta);
    void handleSocketState(QAbstractSocket::SocketState sta);

private:
    QWebSocket m_webSocket, m_txsocket;
    std::string lastPk2name;
    fantasybit::FantasyAgent m_fantasy_agent;
    static Mediator *myInstance;
    explicit Mediator(QObject *parent = 0);
    MyNameStatus m_myNameStatus;
    QString m_errorString;
    QString m_webSocketErrorString;
    SocketState m_socketState;
    QAbstractSocket::SocketState m_internalSocketState;

    QString m_playersName;
    QString m_playersStatus;
    std::string m_lastSignedplayer;

    std::unordered_map<std::string, std::string> m_myPubkeyFname;

    void doPk2fname(const std::string &pkstr);

    QTimer signPlayerStatus;
};

#endif // MEDIATOR_H

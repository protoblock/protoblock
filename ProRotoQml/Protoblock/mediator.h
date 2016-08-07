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
#include "playerquoteslicemodel.h"
#include "depthmarketmodel.h"
#include "fantasynamemodel.h"
#include "openordersmodel.h"

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

    QML_READONLY_CSTREF_PROPERTY (QString, currentPidContext)


    Q_PROPERTY(QString errorString READ errorString NOTIFY errorStringChanged)
    Q_PROPERTY(QString webSocketErrorString  READ webSocketErrorString NOTIFY webSocketErrorStringChanged)

    Q_PROPERTY(SocketState socketState READ socketState NOTIFY socketStateChanged)
    Q_ENUMS (SocketState)

//    Q_PROPERTY(QString playersName READ playersName  NOTIFY playersNameChanged)
//    Q_PROPERTY(QString  playersStatus READ playersStatus  NOTIFY playersStatusChanged)

//    Q_PROPERTY(MyNameStatus myNameStatus READ myNameStatus NOTIFY myNameStatusChanged)
//    Q_ENUMS (MyNameStatus)

    QML_CONSTANT_CSTREF_PROPERTY (QString, chatServerAddr)

    QML_READONLY_PTR_PROPERTY(PlayerQuoteSliceModel, pPlayerQuoteSliceModel)
    QML_READONLY_PTR_PROPERTY(DepthMarketModel, pDepthMarketModel)
    QML_READONLY_PTR_PROPERTY(FantasyNameBalModel, pFantasyNameBalModel)
    QML_READONLY_PTR_PROPERTY(FantasyNameBalModel, pGoodNameBalModel)
    QML_READONLY_PTR_PROPERTY(OpenOrdersModel, pGlobalOpenOrdersModel)
    QML_WRITABLE_PTR_PROPERTY(PlayerQuoteSliceModelItem, pPlayerQuoteSliceModelItem)

//    QML_WRITABLE_PTR_PROPERTY(TradingPositionsModel, pTradingPositionsModel)

    QML_READONLY_PTR_PROPERTY(TradingPositionsModel, pTradingPositionsModel)

    std::unordered_map<std::string,TradingPositionsModel *> modelMap;

//    QML_READONLY_PTR_PROPERTY(PlayerQuoteSliceModelItem, pPlayerQuoteSliceModel)



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
     * Font NON -Socket releated errors
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

    Q_INVOKABLE void startDepth(const QString& symbol) {
        depthBackup--;
        if ( depthBackup < 0 ) {
            depthBackup = 0;
            depthInterval = 1000;
        }
        else
            depthInterval = 1000 * (depthBackup / 5);

        depthCount = 0;
        depthInterval = 1000 * (depthBackup / 5);
        if ( depthInterval < 1000 )
           depthInterval = 1000;

        changeDepthContext(symbol);
        getDepthRep();
        qDebug() << " depthInterval " << depthInterval;

        polldepth.start(depthInterval);
//        getOrderReq(FantasyName::name_hash(m_fantasy_agent.currentClient()));
        getOrderPos();
    }

    Q_INVOKABLE void stopDepth(const QString& symbol) {
        polldepth.stop();
        depthBackup -= 5;
        if ( depthBackup < 0 ) depthBackup = 0;
        depthCount = 0;
    }

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


//    Q_INVOKABLE void newOrder(const QString &id, int qty, int price);

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


    Q_INVOKABLE void changeDepthContext(const QString& context) {
        if ( mGetDepthReq.GetExtension(GetDepthReq::req).pid().data() != context )
            mGetDepthReq.MutableExtension(GetDepthReq::req)->set_pid(context.toStdString());

        m_currentPidContext = context;
    }

    Q_INVOKABLE void doCancel(qint32 id);
    Q_INVOKABLE void doTrade(QString symbol, bool isbuy, const qint32 price, qint32 size);
    Q_INVOKABLE void allNamesGet();
    Q_INVOKABLE void rowMarketGet();
    Q_INVOKABLE void getOrderPos(const QString&);
    Q_INVOKABLE void getOrderPos();
    Q_INVOKABLE void setOrderModel(const QString& symbol) {
        auto model = m_pTradingPositionsModel->getByUid(symbol);
        if ( model == nullptr ) {
            qDebug() << " bad data for m_pTradingPositionsModel " << symbol;
        }
        else
            m_pGlobalOpenOrdersModel = model->get_pOpenOrdersModel();
    }

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

    Q_INVOKABLE QString getPlayerNamePos(const QString &uid) {
        auto model = m_pPlayerQuoteSliceModel->getByUid(uid);
        if ( model == nullptr ) {
            qDebug() << " bad data for getPlayerNamePos " << uid;
            return "";
        }
        else
            return model->get_fullname() + " (" + model->get_position() +")" ;
    }

    Q_INVOKABLE QString getTeamid(const QString &uid) {
        auto model = m_pPlayerQuoteSliceModel->getByUid(uid);
        if ( model == nullptr ) {
            qDebug() << " bad data for getTeamid " << uid;
            return "";
        }
        else
            return model->get_teamid();
    }

    qint64 sendBinaryMessage(const GOOGLE_NAMESPACE::protobuf::Message &data);


    void subscribeOrderPos(const QString &name);
    void getOrderReq(const QString &name,const QString symbol="");
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
    void portfolioChanged();
    // for QML only
    bool engineUpdate(bool);

protected slots:
//    void handdleUsingName(const QString &name);
//    void handdleNameStatus(const QString &name,const QString &status );
//    void handdleNameStatuses();


    void getDepthRep();

    void handleError(const QString err);
    void handleWebSocketError(const QString err);

    void handleClosed();
    void onConnected();
    void handleAboutToClose();
    void onTextMessageReceived( QString message);
    void onBinaryMessageRecived(const QByteArray &message);
    void getSignedPlayerStatus();
    void doTestTrade();

    // slot to update QML ONLY propertys
    void handleEngineUpdate(const bool &sta);

private slots:
    void handleSocketError(QAbstractSocket::SocketError err);
//    void handleSocketState(QAbstractSocket::SocketState sta);
    void handleSocketState(QAbstractSocket::SocketState sta);
    void OnpPlayerQuoteSliceModelItemChanged (PlayerQuoteSliceModelItem * name); \
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

    std::unordered_map<std::string, uint64_t> m_myPubkeyHash;

    std::unordered_map<uint64_t, std::string> m_myHashFname;

    void doPk2fname(const std::string &pkstr);

    QTimer signPlayerStatus;

    QTimer polldepth;

    QTimer tradeTesting;
    PlayerQuoteSliceModel mPlayerQuoteSliceModel;
    DepthMarketModel mDepthMarketModel;
    FantasyNameBalModel mFantasyNameBalModel, mGoodNameBalModel;
    OpenOrdersModel mOpenOrdersModel;
    WsReq mGetDepthReq;
    QString testid;
    bool isbid;
//    void getOrderReq(uint64_t cname);
    int depthCount;
    int depthBackup;
    int depthInterval;
    TradingPositionsModel mTradingPositionsModel;
};

#endif // MEDIATOR_H




/*

int netqty = p.second.first.netqty;
double avg = 0;
double pnl = 0;
if ( netqty ==0 ) {
    pnl = p.second.first.netprice * 100;
}
else  {
    ViewModel * item = mPlayerListModel.itemByKey(p.first.data());
    int bid = item->propertyValue<PropertyNames::BID>().toInt();
    int ask = item->propertyValue<PropertyNames::ASK>().toInt();
    int price = (netqty > 0) ? bid :  ask;

    if ( bid == 0 && ask == 0 )
        pnl = 0;
    else
        pnl = 100 * ((price * netqty) + p.second.first.netprice);

}

mPlayerListModel.updateItemProperty<PropertyNames::MYPOS>(p.first.data(),netqty);
mPlayerListModel.updateItemProperty<PropertyNames::MYAVG>(p.first.data(),avg);
mPlayerListModel.updateItemProperty<PropertyNames::MYPNL>(p.first.data(),pnl);

if ( p.first == myPlayerid) {
    ui->posQty->setValue(netqty);
    ui->posAvgPrice->setValue(avg);
    ui->posOpenPnl->setValue(pnl);
}

totpnl += pnl;

}

ui->fantasybitPnl->setValue(ui->fantasybitPnl->value()+totpnl);

}*/

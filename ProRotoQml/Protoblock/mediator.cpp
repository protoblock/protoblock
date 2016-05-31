#include "mediator.h"
#include "StateData.pb.h"
#include <QSettings>
#include "Commissioner.h"


using namespace fantasybit;
Mediator::Mediator(QObject *parent) : QObject(parent) {

    m_webSocket.open(QUrl(QStringLiteral("ws://192.168.42.80:4001")));
    m_txsocket.open(QUrl(QStringLiteral("ws://192.168.42.80:4000")));

//    init ();
    connect (this,SIGNAL (error(QString)),this,SLOT (handleError(QString)));
    connect(&m_webSocket, SIGNAL(connected()), this, SLOT(onConnected()));
    connect (&m_webSocket,SIGNAL(aboutToClose()),this,SLOT(handleAboutToClose()));
    connect (&m_webSocket, SIGNAL(disconnected()), this, SLOT(handleClosed()));

        connect (&m_webSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(handleSocketError(QAbstractSocket::SocketError)));

    connect(this,SIGNAL (nameStatusChanged(QString,QString))
            ,this, SLOT (handdleNameStatus(QString,QString)));
    connect(this,SIGNAL(usingFantasyName(QString)),
            this,SLOT(handdleUsingName(QString)));


}


QString Mediator::nameStatusGet(const QString &name) {
    QVariantMap::const_iterator iter = m_nameStatuses.find(name);
    if ( !iter->isValid() )
        return "none";
    else
        qDebug() << "Suggested Name:  "  << iter.value ().toString ();
        return iter.value().toString();


}

void Mediator::pk2fname(const QString &pk) {
    //WsReq req;// = lastNameCheck;
    lastPk2name.set_ctype(PK2FNAME);
    lastPk2name.set_data(pk.toStdString());
    auto txstr = lastPk2name.SerializeAsString();
    QByteArray qb(txstr.data(),(size_t)txstr.size());

    m_webSocket.sendBinaryMessage(qb);
}

void Mediator::checkname(const QString &name) {
    WsReq req;
    req.set_ctype(CHECKNAME);
    req.set_data(name.toStdString());
    auto txstr = req.SerializeAsString();
    QByteArray qb(txstr.data(),(size_t)txstr.size());
    m_webSocket.sendBinaryMessage(qb);
}



void Mediator::handleError(const QString err) {
    if ( m_errorString == err)
        return;
    m_errorString = err;
    emit errorStringChanged();

}

Mediator *Mediator::instance() {
    if (myInstance == NULL) {
        myInstance = new Mediator();
    }
    return myInstance;
}

/*!
 * \brief Mediator::playersStatus
 * \return
    returns the current fantasy players status that is in focus
*/
QString Mediator::playersStatus() const
{
    return m_playersStatus;
}

/*!
 * \brief Mediator::setPlayersStatus
 * \param playersStatus
    set the curreent fantasy players status
*/
void Mediator::setPlayersStatus(const QString &playersStatus)
{
    if(m_playersStatus == playersStatus)
        return;
    m_playersStatus = playersStatus;
    emit playersStatusChanged();
}

/*!
 * \brief Mediator::playersName
 * \return
    return the current fantasys players name that is in focus
*/
QString Mediator::playersName() const
{
    return m_playersName;
}

/*!
 * \brief Mediator::setPlayersName
 * \param playersName
    Set the Current players name that is in focus
*/
void Mediator::setPlayersName(const QString &playersName)
{
    if( m_playersName == playersName)
        return;
    m_playersName = playersName;
    emit playersNameChanged();
}

Mediator::MyNameStatus Mediator::myNameStatus() const
{
    return m_myNameStatus;
}

void Mediator::setMyNameStatus(const Mediator::MyNameStatus &myNameStatus)
{
    if (m_myNameStatus == myNameStatus){
        return;
    }else {
        switch (myNameStatus) {
        case None :
            m_myNameStatus = None;
            break;
        case NotAvil:
            m_myNameStatus = NotAvil;
            break;
        case Requested:
            m_myNameStatus = Requested;
            break;
        case TransactionSent:
            m_myNameStatus = TransactionSent;
            break;
        case Confirmed:
            m_myNameStatus = Confirmed;
            break;
        }
        emit myNameStatusChanged();
    }
}

qint64 Mediator::sendBinaryMessage(const QByteArray &data) {
    return m_webSocket.sendBinaryMessage(data);
}

void Mediator::handleClosed() {
    qDebug() << "Close Reason " << m_webSocket.closeReason ();
    return;
}

void Mediator::onConnected() {
    QHostAddress hInfo = m_webSocket.peerAddress ();
    qDebug() << "connected to " <<  hInfo.toString () << " on Port " << m_webSocket.peerPort ();
    connect(&m_webSocket, SIGNAL(binaryMessageReceived(QByteArray)),
            this, SLOT ( onBinaryMessageRecived(QByteArray) ));
}

void Mediator::handleAboutToClose()
{
    // Read the error forr the connection and then //
    // start timmer to reconnect to server//
}


void Mediator::onTextMessageReceived(QString message) {
        qDebug() << "Message received:" << message;
}


void Mediator::onBinaryMessageRecived(const QByteArray &message) {
    fantasybit::WSReply rep;
    rep.ParseFromString(message.toStdString());

    qDebug() << "Mediator::onBinaryMessageRecived " << rep.DebugString().data();
    switch ( rep.ctype()) {
        case PK2FNAME:
        {
            auto name= rep.data();
            if ( name == "" ) {
                error(QString("import failed. please input valid secret"));
                qDebug() << "Mediator::onBinaryMessageRecived import failed. please input valid secret";

                return;
            }


            m_fantasy_agent.finishImportMnemonic(rep.req().data(), name);

            //FIXME lets make this as a real map to pass to a string
            m_nameStatuses[name.data()] = QString("confirmed");
            nameStatusChanged( name.data() , "confirmed" );

            if ( !m_fantasy_agent.HaveClient() ||
                 lastPk2name.data() == name)
                if ( !m_fantasy_agent.UseName(name) )
                    qDebug() << "error using name " << name.data () ;
//                    error(QString("error using name").append(name.data()));

            usingFantasyName(m_fantasy_agent.currentClient().data());
            break;
        }
        case CHECKNAME:
            qDebug() << " emit " << rep.req().data().data() << rep.data().data();
            nameCheckGet(rep.req().data().data(),rep.data().data());
//            update_checkname(rep.data().data());
    }
//    emit gotPk2fname(name);
}

/*!
 * \brief GetUserInfo::importMnemonic
 * \param importStr
 * \return
 * This will return the mn for import
 */
QString Mediator::importMnemonic(const QString &importStr) {
    auto pk = m_fantasy_agent.startImportMnemonic(importStr.toStdString());
    if ( pk == "" )
        return "none";

    pk2fname(pk.data());
    return "pending";
}

void Mediator::signPlayer(const QString &name)  {
    m_fantasy_agent.signPlayer(name.toStdString());
    NameTrans nt{};
    nt.set_public_key(m_fantasy_agent.pubKeyStr());
    nt.set_fantasy_name(name.toStdString());


    Transaction trans{};
    trans.set_version(Commissioner::TRANS_VERSION);
    trans.set_type(TransType::NAME);
    trans.MutableExtension(NameTrans::name_trans)->CopyFrom(nt);
    SignedTransaction sn = m_fantasy_agent.makeSigned(trans);
    auto txstr = sn.SerializeAsString();

    QByteArray qb(txstr.data(),(size_t)txstr.size());

    m_txsocket.sendBinaryMessage(qb);

    m_nameStatuses[name] = QString("requested");
    nameStatusChanged(name,"requested");
    usingFantasyName(m_fantasy_agent.currentClient().data());
}


/*!
 * \brief Mediator::init
    Try to find the best fantasy name that there is to use
    ? There might be a reace on on this ?
 */
void Mediator::init() {
    std::string dname = m_fantasy_agent.defaultName();
    if ( dname == "") {
        qDebug() << " Mediator::init() no name";
        return;
    }

    qDebug() << " Mediator::init() " << dname.data();
    QString defaultName = QString::fromStdString (m_fantasy_agent.currentClient().data());
    m_nameStatuses[defaultName] = QString("requested");
    nameStatusChanged( defaultName , "requested" );

    usingFantasyName( defaultName ) ;
}

void Mediator::handdleUsingName(const QString &name)
{
#ifdef TRACE
    qDebug() << " handdleUsingName " << name;
#endif
    setPlayersName (name);
}

void Mediator::handdleNameStatus(const QString &name, const QString &status)
{
    setPlayersName (name);
    setPlayersStatus (status);
}

void Mediator::handleSocketError(QAbstractSocket::SocketError error)
{
    qDebug() << error;
}

void Mediator::handdleNameStatuses()
{
    setMyNameStatus (m_myNameStatus);
}

#ifdef Q_OS_WIN32
QString Mediator::lastKnowLoc() {
    QSettings settings("HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\{79913283-A35B-4274-927C-1B52D286D939}_is1", QSettings::NativeFormat);
    return settings.value( "InstallLocation" ).toString();
}
#endif

Mediator *Mediator::myInstance;

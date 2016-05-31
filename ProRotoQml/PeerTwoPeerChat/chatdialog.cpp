#include "chatdialog.h"

ChatDialog::ChatDialog(QObject *parent)
    : QObject(parent)
{
    init();
    setWelcomeMessage();
    connect(this , SIGNAL( error(QString) ) , this , SLOT(setErrorString(QString) ) ) ;

    connect(this, SIGNAL(enterPressed()), this, SLOT(returnPressed()));

    connect(&client, SIGNAL(newMessage(QString,QString)),
            this, SLOT(appendMessage(QString,QString)));

    connect(&client, SIGNAL(newParticipant(QString)),
            this, SLOT(newParticipant(QString)));

    connect(&client, SIGNAL(participantLeft(QString)),
            this, SLOT(participantLeft(QString)));


    if ( m_myNickName != client.nickName()){
        m_myNickName = client.nickName ();
        qDebug() << "My Full Name " << client.nickName ();
        myNickNameChanged();
    }
    newParticipant(m_myNickName);
}

QStringList ChatDialog::peopleInRoom() const{
    return m_pInRoom;
}

void ChatDialog::setPeopleInRoom(const QList<QString> &mPeople){
    if (mPeople == m_peopleInRoom)
        return;

    foreach (QString inPeople, mPeople) {
        m_pInRoom << inPeople;
    }
    emit peopleInRoomChanged();
}

QString ChatDialog::welcomeMessage() const{
    return m_welcomeMessage;
}

void ChatDialog::setWelcomeMessage(){
    m_welcomeMessage = "Wecome to Protoblock's Chat room.  Please be nice to others and theat is the rules" ;
}

QString ChatDialog::errorString() const{
    return m_errorString;
}

void ChatDialog::init()
{
    QNetworkConfigurationManager manager;
    if (manager.capabilities() & QNetworkConfigurationManager::NetworkSessionRequired) {
        // Get saved network configuration
        QSettings settings(QSettings::UserScope, QLatin1String("Protoblock"));
        settings.beginGroup(QLatin1String("ProtoblockNetwork"));
        const QString id = settings.value(QLatin1String("DefaultNetworkConfiguration")).toString();
        settings.endGroup();

        // If the saved network configuration is not currently discovered use the system default
        QNetworkConfiguration config = manager.configurationFromIdentifier(id);
        if ((config.state() & QNetworkConfiguration::Discovered) !=
            QNetworkConfiguration::Discovered) {
            config = manager.defaultConfiguration();
        }

        QNetworkSession *networkSession = new QNetworkSession(config);
        networkSession->open();
        networkSession->waitForOpened();

        if (networkSession->isOpen()) {
            // Save the used configuration
            QNetworkConfiguration config = networkSession->configuration();
            QString id;
            if (config.type() == QNetworkConfiguration::UserChoice) {
                id = networkSession->sessionProperty(
                        QLatin1String("UserChoiceConfiguration")).toString();
            } else {
                id = config.identifier();
            }

            QSettings settings(QSettings::UserScope, QLatin1String("Protoblock"));
            settings.beginGroup(QLatin1String("ProtoblockNetwork"));
            settings.setValue(QLatin1String("DefaultNetworkConfiguration"), id);
            settings.endGroup();
        }
    }
}

void ChatDialog::sendMessage(const QString &n, const QString &msg)
{
    QString m = n + " " + msg;
    client.sendMessage (m);
}

QString ChatDialog::myNickName() const{
    return m_myNickName;
}



/*!
 * \brief ChatDialog::appendMessage
 * \param from
 * \param message
 *
 * add the new message to the chat room and
 * FIXME make it so that it also goes to a log file.
 */

QString ChatDialog::appendMessage(const QString &from, const QString &message)
{
    if (from.isEmpty() || message.isEmpty())
        error("message can not be empty");
    QString msg = '<' + from + "> " +  message;
    return msg;
    qDebug() << "message coming back from a peer " <<  msg;
}

/*!
 * \brief ChatDialog::returnPressed
Someone is trying to send a message to the chatroom
 */
void ChatDialog::returnPressed()
{
    if (m_text.isEmpty())
        error ("Message can not be empty");

    client.sendMessage(m_text);
    appendMessage(m_myNickName, m_text);
}

/*!
 * \brief ChatDialog::newParticipant
 * \param nick
    Someone has joined the chat room
*/

void ChatDialog::newParticipant(const QString &nick)
{
    if (nick.isEmpty()){
        error("You must have at least one charecter in your nick");
        return;
    }
    m_peopleInRoom.push_back ( nick) ;

    qDebug() << "A new User has Joined " << nick;
    m_newNick.append(tr("* %1 has joined").arg(nick));
}

/*!
 * \brief ChatDialog::participantLeft
 * \param nick
    Alert everone that someone has left the chat room
*/
void ChatDialog::participantLeft(const QString &nick)
{
    if (nick.isEmpty()){
        error("wait this should never ever happen you can not joibn if you do not have a nick");
        return;
    }
    int removeAt = m_pInRoom.indexOf (nick);
    if (removeAt == 0 )
        return;
    m_pInRoom.removeAt (removeAt);
    m_newNick.append(tr("* %1 has left").arg(nick));
    emit someLeft();

}



void ChatDialog::setErrorString(const QString &errorString)
{
    if(m_errorString == errorString)
        return;
    m_errorString= errorString;
    emit errorStringChanged() ;
}

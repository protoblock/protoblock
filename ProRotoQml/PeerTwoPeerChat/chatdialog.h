#ifndef CHATDIALOG_H
#define CHATDIALOG_H

#include <QObject>
#include <QString>
#include <QList>
#include <QDebug>
#include <QTimer>
#include <QtCore/QSettings>
#include <QtNetwork/QNetworkConfigurationManager>
#include <QtNetwork/QNetworkSession>

#include "client.h"


#include "../QmlSuperMacros/QQmlVarPropertyHelpers.h"
#include "../QmlSupermacros/QQmlConstRefPropertyHelpers.h"
#include "../QmlSuperMacros/QQmlAutoPropertyHelpers.h"
#include "../QmlSuperMacros/QQmlEnumClassHelper.h"

class ChatDialog : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QStringList peopleInRoom READ peopleInRoom NOTIFY peopleInRoomChanged)

    Q_PROPERTY(QString welcomeMessage READ welcomeMessage NOTIFY welcomeMessageChanged)

    Q_PROPERTY(QString  errorString READ errorString NOTIFY errorStringChanged)

    Q_PROPERTY(QString myNickName READ myNickName NOTIFY myNickNameChanged)
public:
    ChatDialog(QObject *parent = 0);
    QStringList peopleInRoom()const;
    void setPeopleInRoom(const QList<QString> &mPeople);
    QString welcomeMessage()const;
    void setWelcomeMessage();
    QString errorString()const;
    void init() ;
    Q_INVOKABLE void sendMessage(const QString &n,const QString &msg );
    QString myNickName()const;
signals:
    void peopleInRoomChanged();
    void errorStringChanged();
    void error(QString);
    void welcomeMessageChanged();
    void enterPressed();
    void myNickNameChanged();

    void someLeft();
public slots:

    QString appendMessage(const QString &from, const QString &message);

private slots:
    void returnPressed();
    void newParticipant(const QString &nick);
    void participantLeft(const QString &nick);
//    void showInformation();
    void setErrorString(const QString &errorString);
private:
    Client client;
    QString m_myNickName;
//    QTextTableFormat tableFormat;
    QString m_text;
    QString m_newNick;
    QList<QString> m_peopleInRoom;
    QStringList m_pInRoom;
    QString m_welcomeMessage;
    QString m_errorString;

};

#endif

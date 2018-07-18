#ifndef SSHKEYDEPLOYER_H
#define SSHKEYDEPLOYER_H

#include "remotelinux_export.h"

#include <QObject>
#include <QFile>

namespace QSsh { class SshConnectionParameters; }

namespace RemoteLinux {
namespace Internal { class SshKeyDeployerPrivate; }

class  SshKeyDeployer : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(SshKeyDeployer)

    Q_PROPERTY(QString keyFilePath READ keyFilePath WRITE setKeyPath NOTIFY keyFilePathChanged)

    Q_PROPERTY(QString username READ username WRITE setUsername NOTIFY usernameChanged)
    Q_PROPERTY(QString hostName READ hostName WRITE setHostName NOTIFY hostNameChanged)
    Q_PROPERTY(QString passwd READ passwd WRITE setPasswd NOTIFY passwdChanged)
    Q_PROPERTY(int sshPort READ sshPort WRITE setSshPort NOTIFY sshPortChanged)
    Q_PROPERTY(int timeout READ timeout WRITE setTimeout NOTIFY timeoutChanged)
    Q_PROPERTY(QString privateKey READ privateKey WRITE setPrivateKey)

    Q_PROPERTY(OsType osType READ osType WRITE setOsType NOTIFY osTypeChanged)

    Q_PROPERTY(AuthenticationType authenticationType READ authenticationType WRITE setAuthenticationType NOTIFY authenticationTypeChanged)
    Q_ENUMS(AuthenticationType)



public:
    explicit SshKeyDeployer(QObject *parent = 0);


    enum OsType{
        Linux,
        Windows,
        OSX,
        //Who knows maybe in the future or something
        Android
    };
    OsType osType()const;
    void setOsType(const OsType osType);


    enum AuthenticationType {
        AuthenticationByPassword,
        AuthenticationByKey,
        AuthenticationTypeKeyboardInteractive,
        // Some servers disable "password", others disable "keyboard-interactive".
        AuthenticationTypeTryAllPasswordBasedMethods
    };

    AuthenticationType authenticationType();
    void setAuthenticationType(const AuthenticationType &authenticationType);

    QString username();
    void setUsername(const QString &username);

    QString hostName();
    void setHostName(const QString &hostName);

    QString passwd();
    void setPasswd(const QString &passwd);

    int sshPort();
    void setSshPort(int &sshPort);

    int timeout();
    void setTimeout(int &timeout);

    //NOT THE UPLOAD KEY
    QString privateKey();
    void setPrivateKey(const QString &privateKey);

    Q_INVOKABLE void deployPublicKey();
    void stopDeployment();
    QString keyFilePath()const;
    void setKeyPath(const QString &keyFilePath);

signals:
    void error(const QString &errorMsg);
    void finishedSuccessfully();
    void keyFilePathChanged();

    void usernameChanged();
    void hostNameChanged();
    void passwdChanged();
    void sshPortChanged();
    void commandChanged();
    void timeoutChanged();
    void authenticationTypeChanged();
    void privateKeyChanged();
    void osTypeChanged();

private slots:
    void handleConnectionFailure();
    void handleKeyUploadFinished(int exitStatus);

    void destory();

private:
    void cleanup();

    QByteArray m_byte;
    QString m_keyFilePath;

    //parmas
    int m_timeout;
    QString m_username;
    QString m_hostName;
    QString m_passwd;
    int m_sshPort;


    OsType m_osType;

    AuthenticationType m_authenticationType;
    //FOR LOGIN NOT THE GENERATED KEY
    QString m_privateKey;


    Internal::SshKeyDeployerPrivate * const d;
};

} // namespace RemoteLinux

#endif // SSHKEYDEPLOYER_H

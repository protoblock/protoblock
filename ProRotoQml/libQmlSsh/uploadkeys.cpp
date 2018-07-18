#include "uploadkeys.h"
#include <sshremoteprocessrunner.h>

using namespace QSsh;
namespace RemoteLinux {
namespace Internal {

class SshKeyDeployerPrivate
{
public:
    SshRemoteProcessRunner deployProcess;
};

} // namespace Internal

SshKeyDeployer::SshKeyDeployer(QObject *parent)
    : QObject(parent),
    m_timeout(10),
    m_sshPort(22),
    m_osType(Linux),
    //because we are uploading a key we assume that the default should be set to passwd
    m_authenticationType(AuthenticationByPassword),
  d(new Internal::SshKeyDeployerPrivate)

{
}

SshKeyDeployer::OsType SshKeyDeployer::osType() const
{
    return m_osType;
}

void SshKeyDeployer::setOsType(const SshKeyDeployer::OsType osType)
{
    if(m_osType == osType){
        return;
    }
    else
    {
        switch (osType) {
        case Linux:
            m_osType =Linux ;
            break;
        case Windows:
            m_osType = Windows;
            break;
        case OSX:
            m_osType = OSX;
            break;
        case Android:
            m_osType = Android;
            break;
        }
    }
}

SshKeyDeployer::AuthenticationType SshKeyDeployer::authenticationType()
{
    return m_authenticationType;
}

void SshKeyDeployer::setAuthenticationType(const SshKeyDeployer::AuthenticationType &authenticationType)
{
    if(m_authenticationType == authenticationType)
    {
        return;
    }
    else
    {
        if (authenticationType == AuthenticationByPassword)
        {
            m_authenticationType = AuthenticationByPassword;
        }
        else if (m_authenticationType ==  AuthenticationByKey)
        {
            m_authenticationType = AuthenticationByKey;
        }
        else if (authenticationType == AuthenticationTypeKeyboardInteractive)
        {
            m_authenticationType = AuthenticationTypeKeyboardInteractive;
        }
        else if (m_authenticationType == AuthenticationTypeTryAllPasswordBasedMethods)
        {
            m_authenticationType = AuthenticationTypeTryAllPasswordBasedMethods;
        }
        emit authenticationTypeChanged();
    }
}

QString SshKeyDeployer::username()
{
    return m_username;
}

void SshKeyDeployer::setUsername(const QString &username)
{
    if(m_username == username)
        return;
    m_username = username;
    emit usernameChanged();
}

QString SshKeyDeployer::hostName()
{
    return m_hostName;
}

void SshKeyDeployer::setHostName(const QString &hostName)
{
    if(m_hostName == hostName)
        return;
    m_hostName = hostName ;
    emit hostNameChanged();
}

QString SshKeyDeployer::passwd()
{
    return m_passwd;
}

void SshKeyDeployer::setPasswd(const QString &passwd)
{
    if(m_passwd == passwd)
        return;
    m_passwd = passwd;
    emit  passwdChanged();
}

int SshKeyDeployer::sshPort()
{
    return m_sshPort;
}

void SshKeyDeployer::setSshPort(int &sshPort)
{
    if(m_sshPort == sshPort)
        return;
    m_sshPort = sshPort;
    emit  sshPortChanged();
}

int SshKeyDeployer::timeout()
{
    return m_timeout;
}

void SshKeyDeployer::setTimeout(int &timeout)
{
    if(m_timeout == timeout)
        return;
    m_timeout = timeout;
    emit timeoutChanged();
}

QString SshKeyDeployer::privateKey()
{
    return m_privateKey;
}

void SshKeyDeployer::setPrivateKey(const QString &privateKey)
{
    if(m_privateKey == privateKey)
        return;
    m_privateKey = privateKey;
    emit privateKeyChanged() ;
}



void SshKeyDeployer::destory()
{
    cleanup();
    delete d;
}

void SshKeyDeployer::deployPublicKey()
{
    cleanup();


    // FIXME make sure that the bare minimum is set
      QSsh::SshConnectionParameters sshParams;
         //    params.proxyType = QSsh::SshConnectionParameters::DefaultProxy;
         if (m_authenticationType == AuthenticationByPassword)
         {
             sshParams.authenticationType = QSsh::SshConnectionParameters::AuthenticationTypePassword;
             sshParams.password = m_passwd;
         }
         else if (m_authenticationType == AuthenticationByKey)
         {
             sshParams.authenticationType = QSsh::SshConnectionParameters::AuthenticationTypePublicKey;
             sshParams.privateKeyFile = m_privateKey;
         }
         sshParams.port = m_sshPort;
         sshParams.host = m_hostName;
         sshParams.userName = m_username;
         sshParams.timeout = m_timeout;


    QFile m_file(m_keyFilePath);
    if(!m_file.open(QIODevice::ReadOnly|QIODevice::Text))
    {
         emit error(tr("Public key error: %1").arg(m_file.errorString()));
         return;
    }


    m_byte = m_file.readAll();

    //FIXME make windows cmd also
    connect(&d->deployProcess, &SshRemoteProcessRunner::connectionError,
            this, &SshKeyDeployer::handleConnectionFailure);
    connect(&d->deployProcess, &SshRemoteProcessRunner::processClosed,
            this, &SshKeyDeployer::handleKeyUploadFinished);

   // maybe I should get the home dir then upload and add home dir here
    QByteArray command;
    switch (m_osType) {
    case Linux:
       command = "test -d .ssh "
             "|| mkdir .ssh && chmod 0700 .ssh && echo '"
             + m_byte+ "' >> .ssh/authorized_keys && chmod 0600 .ssh/authorized_keys";
        break;
    case Windows:
        qDebug() << "Windows sucks";

        break;
    case OSX:
        command = "test -d .ssh "
             "|| mkdir .ssh && chmod 0700 .ssh && echo '"
             + m_byte+ "' >> .ssh/authorized_keys && chmod 0600 .ssh/authorized_keys";
        break;
    case Android:
        command = "test -d .ssh "
             "|| mkdir .ssh && chmod 0700 .ssh && echo '"
             + m_byte+ "' >> .ssh/authorized_keys && chmod 0600 .ssh/authorized_keys";
        break;
    }

    d->deployProcess.run(command, sshParams);

    m_file.close();
}

void SshKeyDeployer::handleConnectionFailure()
{
    cleanup();
    emit error(tr("Connection failed: %1").arg(d->deployProcess.lastConnectionErrorString()));
}

void SshKeyDeployer::handleKeyUploadFinished(int exitStatus)
{
    Q_ASSERT(exitStatus == SshRemoteProcess::FailedToStart
        || exitStatus == SshRemoteProcess::CrashExit
        || exitStatus == SshRemoteProcess::NormalExit);

    const int exitCode = d->deployProcess.processExitCode();
    const QString errorMsg = d->deployProcess.processErrorString();
    cleanup();
    if (exitStatus == SshRemoteProcess::NormalExit && exitCode == 0)
        emit finishedSuccessfully();
    else
        emit error(tr("Key deployment failed: %1.").arg(errorMsg));
}

void SshKeyDeployer::stopDeployment()
{
    cleanup();
}

QString SshKeyDeployer::keyFilePath() const
{
    return m_keyFilePath;
}

void SshKeyDeployer::setKeyPath(const QString &keyFilePath)
{
    if(m_keyFilePath == keyFilePath)
        return;
    m_keyFilePath = keyFilePath;
    emit keyFilePathChanged();
}

void SshKeyDeployer::cleanup()
{
    disconnect(&d->deployProcess, 0, this, 0);
}
} // namespace RemoteLinux

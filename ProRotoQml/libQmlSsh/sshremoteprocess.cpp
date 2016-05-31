#include "sshremoteprocess.h"

QmlSshRemoteProcess::QmlSshRemoteProcess(QObject *parent) :
    QObject(parent),
    m_needsTerminal(false),
    m_timeout(20),
    m_sshPort(22),
    m_authenticationType(AuthenticationByPassword),
    m_proxyType("none")
{
}

QmlSshRemoteProcess::State QmlSshRemoteProcess::status()
{
    return m_status;
}

/*!
  \qmlproperty bool QmlSshRemoteProcess::needsTerminal
    bool property that is used to launch things with a terminal emulator,
*/
bool QmlSshRemoteProcess::needsTerminal()
{
    return m_needsTerminal;
}

void QmlSshRemoteProcess::setNeedsTerminal(bool &needsTerminal)
{
    m_needsTerminal = needsTerminal;
}
/*!
 \qmlproperty int QmlSshRemoteProcess::timeout
 sets the timeout for connection this is in seconds.
*/
int QmlSshRemoteProcess::timeout()
{
    return m_timeout;
}

void QmlSshRemoteProcess::setTimeout(int &timeout)
{
    m_timeout = timeout;
}

/*!
  \qmlproperty strinn QmlSshRemoteProcess::username
  This is the Username of the remote machine that you would like to upload to.
 */

QString QmlSshRemoteProcess::username()
{
    return m_username;
}

void QmlSshRemoteProcess::setUsername(const QString &username)
{
    m_username = username;
}

/*!
 \qmlproperty string QmlSshRemoteProcess::hostName
 This is the hostname of the remote machine that you would like to upload your files too.Used with the ssh connection to upload the file.
 */
QString QmlSshRemoteProcess::hostName()
{
    return m_hostName;
}

void QmlSshRemoteProcess::setHostName(const QString &hostName)
{
    m_hostName = hostName;

}
/*!
 \qmlproperty string QmlSshRemoteProcess::passwd
 The Password for the remote machine that you would like to upload the file to.
 */
QString QmlSshRemoteProcess::passwd()
{
    return m_passwd;
}

void QmlSshRemoteProcess::setPasswd(const QString &passwd)
{
    m_passwd = passwd;
}
/*!
  \qmlproperty int QmlSshRemoteProcess::sshPort
 This is used to set the port that ssh is listening on,  By default this is set to port 22
 */
int QmlSshRemoteProcess::sshPort()
{
    return m_sshPort;
}

void QmlSshRemoteProcess::setSshPort(int &sshPort)
{
    m_sshPort = sshPort;
}
/*!
 \qmlproperty string QmlSshRemoteProcess::command
  This is the command that you want to run after you are connected via ssh
 */
QString QmlSshRemoteProcess::command()
{
    return m_command;
}

void QmlSshRemoteProcess::setCommand(const QString &command)
{
    m_command = command;
}
/*!
  \qmlsignal QmlSshRemoteProcess::runCommand()
   sets up the connection and runs the command .
*/
void QmlSshRemoteProcess::runCommand()
{
    QSsh::SshConnectionParameters params;

    if (m_proxyType == "none")
    {
//        params.proxyType = QSsh::SshConnectionParameters::NoProxy;
        params.host = m_hostName;
        params.userName = m_username;
        params.password = m_passwd;
        params.timeout = m_timeout;
        if (m_authenticationType == AuthenticationByPassword)
        {
            params.authenticationType = QSsh::SshConnectionParameters::AuthenticationTypePassword;
        }
        else if (m_authenticationType == AuthenticationByKey)
        {
            params.authenticationType = QSsh::SshConnectionParameters::AuthenticationTypePublicKey;
            params.privateKeyFile = m_privateKey;
        }
        params.port = m_sshPort;
    }
    else if (m_proxyType == "default ")
    {
//    params.proxyType = QSsh::SshConnectionParameters::DefaultProxy;
    params.host = m_hostName;
    params.userName = m_username;
    params.password = m_passwd;
    params.timeout = m_timeout;
    if (m_authenticationType == AuthenticationByPassword)
    {
        params.authenticationType = QSsh::SshConnectionParameters::AuthenticationTypePassword;
    }
    else if (m_authenticationType == AuthenticationByKey)
    {
        params.authenticationType = QSsh::SshConnectionParameters::AuthenticationTypePublicKey;
        params.privateKeyFile = m_privateKey;
    }
    params.port = m_sshPort;

    }

    // TODO free this pointer!
    m_connection = new QSsh::SshConnection(params, this);

    connect(m_connection, SIGNAL(connected()), SLOT(onConnected()));
    connect(m_connection, SIGNAL(error(QSsh::SshError)), SLOT(onConnectionError(QSsh::SshError)));

    m_connection->connectToHost();
}

QString QmlSshRemoteProcess::getStderr()
{
    return m_stderr;
}

QString QmlSshRemoteProcess::getStdout()
{
    return m_stdout;
}

/*!
  \qmlproperty string QmlSshRemoteProcess::authenticationType
  \b{under heavey development}

    This is used to change the authenticationType from password (default) or key these are the only 2 options.
    if you change from password to keys you must set the publicKey,

    see also publicKey

 */
QmlSshRemoteProcess::AuthenticationType QmlSshRemoteProcess::authenticationType()
{
    return m_authenticationType;
}

void QmlSshRemoteProcess::setAuthenticationType(const QmlSshRemoteProcess::AuthenticationType &authenticationType)
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
QString QmlSshRemoteProcess::proxyType()
{
    return m_proxyType;
}

/*!
  \qmlproperty string QmlSshRemoteProcess::privateKey
  This is used to set the private key that one would like to use if there going to set the authenticationType to \b{key}
  by default authenticationType is set to \b{password} so this does not need to be set.  But if you are to use keys to login
  then one must set the private key that there going to useconds_t

  \b{see also} authenticationType

 */
QString QmlSshRemoteProcess::privateKey()
{
    return m_privateKey;

}

void QmlSshRemoteProcess::setPrivateKey(const QString &privateKey)
{
    m_privateKey = privateKey;
}

/*!
 \qmlproperty readonly QmlSshRemoteProcess::exit
 returns true if the ssh connection is over, else returns false

 see also kill() close()
 */
bool QmlSshRemoteProcess::exit()
{
    return m_exit;
}



void QmlSshRemoteProcess::onConnected()
{


    emit cleanlyConnected();
//    qDebug() << "QmlSshRemoteProcess Connected: to " << m_hostName;
    emit running(true);
    emit closed(false);
//    qDebug() << "QmlSshRemoteProcess Running Commands...   \t " << m_command;

    QByteArray ba =  m_command.toLatin1();
      const QByteArray comman(ba);
      remoteProc = m_connection->createRemoteProcess(comman);

      if(remoteProc){
          connect(remoteProc.data(), SIGNAL(started()), SLOT(onChannelStarted()) );
          connect(remoteProc.data(), SIGNAL(readyReadStandardOutput()), SLOT(readyReadStandardOutput()));
          connect(remoteProc.data(), SIGNAL(readyReadStandardError()), SLOT(readyReadStandardOutput()));


          connect(remoteProc.data(), SIGNAL(closed(int)), SLOT(handleProcessFinished(int)));
          remoteProc->start();
      }
}

void QmlSshRemoteProcess::onConnectionError(QSsh::SshError)
{
    if (m_errorString == m_connection->errorString()){
        emit error(m_errorString);
        return;
    }
    else
    {
        m_errorString =  m_connection->errorString();
        emit errorStringChanged();
        emit error(m_errorString);
        emit running(false);
        //    qDebug() << "QmlSshRemoteProcess Connection error" << m_connection->errorString();
    }
}
void QmlSshRemoteProcess::onChannelError(const QString &err)
{
    qDebug() << err;
}

void QmlSshRemoteProcess::onChannelStarted(){
//    qDebug() << "QmlSshRemoteProcess Channel Started";
    emit running(true);
}


// get the standered output
void QmlSshRemoteProcess::readyReadStandardOutput()
{
    QString mStdout = QString::fromLatin1(remoteProc->readAllStandardOutput());
    QString mStdall = QString::fromLatin1(remoteProc->readAll());
    QString mSterr = QString::fromLatin1(remoteProc->readAllStandardError());



    if (!mSterr.isEmpty())
    {

        if(m_stderr == mSterr)
        {
            return;
        }
        else
        {
//        qDebug() <<  "QmlSshRemoteProcess Remote Command output stderr: " << sterr;
        m_stderr = mSterr.trimmed();
        emit  getStderrChanged();

        }
    }
    if (!mStdout.isEmpty())
    {
        if(m_stdout == mStdout)
        {
            return;
        }
        else
        {
//            qDebug() << "QmlSshRemoteProcess Remote Command output stdout : " << stdout;
            m_stdout = mStdout.trimmed();
            emit getStdoutChanged();
        }
    }
    if (!mStdall.isEmpty())
    {

//        qDebug() <<  "QmlSshRemoteProcess Remote Command output stdAll : " << stdall.trimmed();
    }

}

// FIXME add signal slot here
void QmlSshRemoteProcess::processFinished(int code)
{
    qDebug() << "Process has finshed Here is the exit code " << code ;
}

void QmlSshRemoteProcess::setStatus()
{
    if ( QSsh::SshConnection::Unconnected ){
        m_status = Unconnected;
    }
    if (QSsh::SshConnection::Connecting){
      m_status = Connecting;
    }
    if (QSsh::SshConnection::Connected){
        m_status = Connected;
    }
}

/*!
  \qmlsignal QmlSshRemoteProcess::close()
  This closes the process that is running but not the ssh connection

see also kill() exit
 */
void QmlSshRemoteProcess::close()
{
    remoteProc->close();
    emit running(false);
    emit closed(remoteProc->exitCode());
    m_exit = true;
}

/*!
 \qmlsignal QmlSshRemoteProcess::kill()
 This kills the process that is running then stops the ssh connection.

see also close() exit
*/
void QmlSshRemoteProcess::kill()
{
    remoteProc->close() ;
    m_connection->disconnectFromHost();
    emit running(false);
    emit closed(remoteProc->exitCode());
    m_exit = true ;
}


void QmlSshRemoteProcess::handleProcessFinished(int exitStatus)
{
    if (m_exitScore == exitStatus)
        return;
    m_exitScore = exitStatus;
    emit exitStatusChanged();
    setProcessExitStatus(exitStatus);
}
void QmlSshRemoteProcess::setProcessExitStatus(int exitStatus)
{

    switch(exitStatus)
    {
    case 0:
        m_exitStatus = FailedToStart;
        break;
    case 1:
        m_exitStatus = CrashExit;
        break;
    case 2:
        m_exitStatus = NormalExit;
        m_connection->closeAllChannels ();
        m_connection->disconnectFromHost ();
        break;

    }
    emit done();

}

/*!
 * \qmlproperty QmlSshRemoteProcess::getProcessExitStatus
 Read only property that is used to get the last exit status from the Qprocess that is
 running in the Ssh connection.  This can be useful when combined with something like
 onExitStatusChanged

 Example:

 \code
    QmlSshRemoteProcess{
        id: QmlSshRemoteProcessCopyFile
        // Set the ssh property's
        username: "UserName"
        hostName: "192.168.1.31"
        passwd: "Password"
        sshPort: 22
        timeout: 20
        command:"cp -r ~/Downloads ~/Downloads_bak"
        onExitStatusChanged: {
            if (getExitStatus === QmlSshRemoteProcess.NormalExit){
                console.log("[QML] QmlSshRemoteProcessCopyFile normal exit")
            }
            else if (getExitStatus === QmlSshRemoteProcess.CrashExit){
                console.log("[QML] QmlSshRemoteProcessCopyFile crash exit")
            }
            else if (getExitStatus === QmlSshRemoteProcess.FailedToStart){
                console.log("[QML] QmlSshRemoteProcessCopyFile failed to start")
            }
            else {
                console.log("[QML] QmlSshRemoteProcessCopyFile Impossible to understand")
            }
        }
    }


    see also:  onError() errorString
 \endcode
 */
QmlSshRemoteProcess::ExitStatus QmlSshRemoteProcess::getProcessExitStatus()
{
    return m_exitStatus;
}

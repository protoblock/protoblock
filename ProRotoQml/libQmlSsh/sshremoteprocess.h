/**
 ** This file is part of the yacm project.
 ** Copyright 2016 Joseph Mills <josephjamesmills@gmail.com>.
 **
 ** This program is free software: you can redistribute it and/or modify
 ** it under the terms of the GNU Lesser General Public License as
 ** published by the Free Software Foundation, either version 3 of the
 ** License, or (at your option) any later version.
 **
 ** This program is distributed in the hope that it will be useful,
 ** but WITHOUT ANY WARRANTY; without even the implied warranty of
 ** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 ** GNU Lesser General Public License for more details.
 **
 ** You should have received a copy of the GNU Lesser General Public License
 ** along with this program.  If not, see <http://www.gnu.org/licenses/>.
 **/

#ifndef QMLSSHREMOTEPROCESS_H
#define QMLSSHREMOTEPROCESS_H

#include <QObject>
#include <QByteArray>
#include <QProcess>
#include <QObject>
#include <QSharedPointer>
#include <QIODevice>

#include "../libQSsh/sshremoteprocess.h"
#include <sshremoteprocess_p.h>

#include <sshconnection.h>
#include <sshconnection_p.h>

class QmlSshRemoteProcess : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString username READ username WRITE setUsername NOTIFY usernameChanged)
    Q_PROPERTY(QString hostName READ hostName WRITE setHostName NOTIFY hostNameChanged)
    Q_PROPERTY(QString passwd READ passwd WRITE setPasswd NOTIFY passwdChanged)
    Q_PROPERTY(int sshPort READ sshPort WRITE setSshPort NOTIFY sshPortChanged)
    Q_PROPERTY(bool needsTerminal READ needsTerminal WRITE setNeedsTerminal NOTIFY needsTerminalChanged)
    Q_PROPERTY(QString command READ command WRITE setCommand NOTIFY commandChanged)
    Q_PROPERTY(int timeout READ timeout WRITE setTimeout NOTIFY timeoutChanged)
    Q_PROPERTY(QString getStderr READ getStderr NOTIFY getStderrChanged)
    Q_PROPERTY(QString getStdout READ getStdout NOTIFY getStdoutChanged)
    Q_PROPERTY(QString  proxyType READ proxyType)
    Q_PROPERTY(QString privateKey READ privateKey WRITE setPrivateKey)
    Q_PROPERTY(QString errorString READ errorString)

    // FIXME make so we can run one after another with QQuery

    Q_PROPERTY(bool exit READ exit)
    //Q_PROPERTY(State state READ state NOTIFY stateChanged)

    Q_PROPERTY(enum ExitStatus getProcessExitStatus READ getProcessExitStatus)
    Q_PROPERTY(State status READ status  NOTIFY statusChanged)
    Q_PROPERTY(AuthenticationType authenticationType READ authenticationType WRITE setAuthenticationType NOTIFY authenticationTypeChanged)

    Q_ENUMS(AuthenticationType)
    Q_ENUMS(ExitStatus)
    Q_ENUMS(State)


public:
    explicit QmlSshRemoteProcess(QObject *parent = 0);

    enum State{
        Connected,
        Unconnected,
        Connecting,
        UserAuthServiceRequested,
        UserAuthRequested
    };

    State status();


    enum ProxyType {
        DefaultProxy,
        NoProxy
    };
    enum AuthenticationType {
        AuthenticationByPassword,
        AuthenticationByKey,
        AuthenticationTypeKeyboardInteractive,
        // Some servers disable "password", others disable "keyboard-interactive".
        AuthenticationTypeTryAllPasswordBasedMethods
    };

    enum ExitStatus {
        FailedToStart,
        CrashExit,
        NormalExit
    };
    enum Signal {
        AbrtSignal,
        AlrmSignal,
        FpeSignal,
        HupSignal,
        IllSignal,
        IntSignal,
        KillSignal,
        PipeSignal,
        QuitSignal,
        SegvSignal,
        TermSignal,
        Usr1Signal,
        Usr2Signal,
        NoSignal
    };


    Q_INVOKABLE void close();
    Q_INVOKABLE void kill();
    Q_INVOKABLE void runCommand();

    // qml property set up
    QString username();
    void setUsername(const QString &username);

    QString hostName();
    void setHostName(const QString &hostName);

    QString passwd();
    void setPasswd(const QString &passwd);

    int sshPort();
    void setSshPort(int &sshPort);

    QString command();
    void setCommand(const QString &command);

    bool needsTerminal();
    void setNeedsTerminal(bool &needsTerminal);

    int timeout();
    void setTimeout(int &timeout);

    QString getStderr();
    QString getStdout();

    AuthenticationType authenticationType();
    void setAuthenticationType(const AuthenticationType &authenticationType);

    QString proxyType();

    QString privateKey();
    void setPrivateKey(const QString &privateKey);
    QString errorString(){return m_errorString; }
    bool exit();

    void setProcessExitStatus(int exitStatus);
    ExitStatus getProcessExitStatus();



signals:
    void finished(QString,int);
    void done();
    void started();
    void readyReadStandardError();
    void closed(int);
    void exitStatusChanged();
    void signalChanged();
    void needsTerminalChanged();
    void usernameChanged();
    void hostNameChanged();
    void passwdChanged();
    void sshPortChanged();
    void commandChanged();
    void timeoutChanged();
    void authenticationTypeChanged();
    void running(bool);
    void cleanlyConnected();
    void error(QString);
    void statusChanged();
    void errorStringChanged();
    void getStdoutChanged();
    void getStderrChanged();

private slots:
    void onConnected();
    void onConnectionError(QSsh::SshError);
    void onChannelError(const QString &err);
    void onChannelStarted();
    void handleProcessFinished(int exitStatus);
    void readyReadStandardOutput();
    void processFinished(int code);
    void setStatus();

private:
    bool m_needsTerminal;
    int m_timeout;
    QString m_username;
    QString m_hostName;
    QString m_passwd;
    int m_sshPort;
    AuthenticationType m_authenticationType;
    QString m_command;
    QString m_stderr;
    QString m_stdout;
    QString m_proxyType;
    QString m_privateKey;
    QString m_errorString;
    bool m_exit;
    int m_exitScore;
    ExitStatus m_exitStatus;
    State m_status;
//    QSsh::SshConnection::


    QSsh::SshRemoteProcess::Ptr remoteProc;
    QSsh::SshConnection *m_connection;
};
//}// namespace QSsh
#endif // QMLSSHREMOTEPROCESS_H

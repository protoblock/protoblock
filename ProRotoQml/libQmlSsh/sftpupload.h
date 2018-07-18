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


#include <sftpchannel.h>
#include <sshconnection.h>
#include <QFileInfo>
#include <QDebug>
#include <QObject>
#include <QAbstractListModel>


class SftpItem
{
public:
    SftpItem(
              const QString &name,
              const QString &type,
              const QString &size,
              const QString &permissions
              );

    QString name() const;
    QString type() const;
    QString size()const;
    QString permissions()const;

private:
    QString m_name;
    QString m_type;
    QString m_size;
    QString m_permissions;

};


class SftpRemoteFileSystemModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int count READ count  NOTIFY countChanged)
public:
    explicit SftpRemoteFileSystemModel(QObject *parent = 0);
    enum QmlNode {
        Name = Qt::UserRole + 1,
        Type,
        Size,
        Permissions
    };
    Q_INVOKABLE void clearModel();


    void fillModel();
    int count();

    void addToModel(const SftpItem &pluginInfo);
    int rowCount(const QModelIndex & parent = QModelIndex()) const;
    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;

protected:
    QHash<int, QByteArray> roleNames() const;

signals:
    void modelRest();
    void countChanged();
    void dataIn();
protected slots:
    void setCount();

private:
    int m_count;
    QList<SftpItem> m_sftpItemList;
};




class Sftp : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString localFile READ localFile WRITE setLocalFile NOTIFY localFileChanged)
    Q_PROPERTY(QString destination READ destination WRITE setDestination NOTIFY destinationChanged)
    Q_PROPERTY(QString username READ username WRITE setUsername NOTIFY usernameChanged)
    Q_PROPERTY(QString hostName READ hostName WRITE setHostName NOTIFY hostNameChanged)
    Q_PROPERTY(QString passwd READ passwd WRITE setPasswd NOTIFY passwdChanged)
    Q_PROPERTY(QString keyFile READ keyFile WRITE setKeyFile NOTIFY keyFileChanged)

    Q_PROPERTY(int sshPort READ sshPort WRITE setSshPort NOTIFY sshPortChanged)
    Q_PROPERTY(int timeout READ timeout WRITE setTimeout)
    Q_PROPERTY(QString errorString READ errorString NOTIFY errorStringChanged)
    Q_PROPERTY(QString localDir READ localDir WRITE setLocalDir NOTIFY localDirChanged)
    Q_PROPERTY(QString remoteDir READ remoteDir WRITE setRemoteDir NOTIFY remoteDirChanged)
    Q_PROPERTY(QString linkName READ linkName WRITE setLinkName NOTIFY linkNameChanged)
    Q_PROPERTY(QString remoteFileName READ remoteFileName WRITE setRemoteFileName NOTIFY remoteFileNameChanged)
    Q_PROPERTY(QStringList remoteFileNames READ remoteFileNames NOTIFY remoteFileNamesChanged)
    Q_PROPERTY(QStringList remoteFilesSize READ remoteFilesSize)
    Q_PROPERTY(QStringList remoteFilesType READ remoteFilesType)
    Q_PROPERTY(QStringList remoteFilePermissions READ remoteFilePermissions )

    Q_PROPERTY(SshError sshError READ sshError NOTIFY sshErrorChanged)
    Q_PROPERTY(AuthenticationType authenticationType READ authenticationType WRITE setAuthenticationType NOTIFY authenticationTypeChanged)
    Q_PROPERTY(SftpOverwriteMode overwriteMode READ overwriteMode WRITE setOverwriteMode NOTIFY overwriteModeChanged)
    Q_PROPERTY(JobId jobId READ jobId WRITE setJobId NOTIFY jobIdChanged)

    Q_PROPERTY(bool showHidden READ showHidden WRITE setShowHidden NOTIFY showHiddenChanged)

    Q_ENUMS(SshError)
    Q_ENUMS(AuthenticationType)
    Q_ENUMS(SftpOverwriteMode)
    Q_ENUMS(JobId)

public:
    explicit Sftp(QObject *parent = 0);
//    ~Sftp();

    //remoteFIle Info
    QStringList remoteFilesSize()const;
    QStringList remoteFilesType()const;
    QStringList remoteFilePermissions()const;
    Q_INVOKABLE void exec();
//    Q_INVOKABLE SftpFileModel fileModel();

    //FIXME add these enums
    // Add state and call it status
    //enum Status {
    //    Inactive,
    //    SessionRequested,
    //    SessionEstablished,
    //    CloseRequested,
    //    Closed
    //};


    enum JobId {
        DownloadFile,
        UploadDir,
        CreateDirectory,
        CreateFile,
        CreateLink,
        ListDirectory,
        RemoveFile,
        RemoveDirectory,
        StatFile,
        RenameFileOrDirectory,
        UploadFile
    };
    JobId jobId()const;
    void setJobId(const JobId &jobId);


    enum SftpFileType {
        FileTypeRegular,
        FileTypeDirectory,
        FileTypeOther,
        FileTypeUnknown
    };

    enum SftpOverwriteMode {
        SftpOverwriteExisting,
        SftpAppendToExisting,
        SftpSkipExisting
    };
    SftpOverwriteMode overwriteMode()const;
    void setOverwriteMode(const SftpOverwriteMode &overwriteMode);

    enum AuthenticationType {
        AuthenticationByPassword,
        AuthenticationByKey,
        AuthenticationTypeKeyboardInteractive,
        // Some servers disable "password", others disable "keyboard-interactive".
        AuthenticationTypeTryAllPasswordBasedMethods
    };
    AuthenticationType authenticationType()const;
    void setAuthenticationType(const AuthenticationType &authenticationType);


    enum SshError {
        SshNoError,
        SshSocketError,
        SshTimeoutError,
        SshProtocolError,
        SshHostKeyError,
        SshKeyFileError,
        SshAuthenticationError,
        SshClosedByServerError,
        SshInternalError
    };
    SshError sshError()const;


    QStringList remoteFileNames()const;
//    void remoteFileNames(const QStringList &fileList);

    QString localDir()const;
    void setLocalDir(const QString &localDir);

    QString remoteDir()const;
    void setRemoteDir(const QString &remoteDir);

    QString linkName()const;
    void setLinkName(const QString &linkName);

    QString keyFile()const;
    void setKeyFile(const QString &keyFile);

    QString localFile();
    void setLocalFile(const QString &localFIle);

    QString remoteFileName()const;
    void setRemoteFileName(const QString &remoteFileName);

    QString destination();
    void setDestination(const QString &destination);

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

    QString setPermissions(const QFile::Permissions fi);

     bool showHidden()const;
     void setShowHidden(const bool &showHidden);

    QString errorString()const;

signals:
  void finshed();
  void connected();

  void localFileChanged();
  void destinationChanged();
  void usernameChanged();
  void hostNameChanged();
  void passwdChanged();
  void sshPortChanged();
  void done();
  void error(QString);
  void timeoutChanged();
  void errorStringChanged();
  void sshErrorChanged();
  void authenticationTypeChanged();
  void overwriteModeChanged();
  void keyFileChanged();
  void jobIdChanged();
  void linkNameChanged();
  void remoteDirChanged();
  void localDirChanged();
  void remoteFileNameChanged();
  void remoteFileNamesChanged();

  void showHiddenChanged();
private slots:
    void onConnected();
    void connectionError(QSsh::SshError err);
    void onChannelInitialized();
    void onChannelError(const QString err);
    void onOpfinished(QSsh::SftpJobId job, const QString & error = QString());
    void handelError(const QString err);
    void dataAvailable(const QString data);
    void reciveFileInfo(QSsh::SftpJobId id,QList<QSsh::SftpFileInfo> fileInfoList);
private:
    QString m_localFilename;
    QString m_remoteFilename;
    QString m_localDir;
    QString m_remoteDir;
    QString m_linkName;

    QString m_keyFile;
    QString m_localfile;
    QString m_destination;
    QString m_username;
    QString m_hostName;
    QString m_passwd;
    int m_sshPort;
    int m_timeout;

    QSsh::SftpChannel::Ptr m_channel;
    //    could be a object
    QSsh::SshConnection *m_connection;

    JobId m_jobId;
    SshError m_sshError;
    AuthenticationType m_authenticationType;
    QSsh::SshConnectionParameters::AuthenticationType mm_authenticationType;

    SftpOverwriteMode m_overwriteMode;
    QSsh::SftpOverwriteMode mm_overwriteMode;
    void parseDestination(const QString &dest);
    QString m_errorString;

    // remote file management
    QStringList m_remoteFileNames;
    QStringList m_remotePermissions;
    QStringList m_permissionsValid;
    QStringList m_remoteFileSize;
    QStringList m_remoteFileSizeValid;
    QStringList m_remoteType;
    bool m_showHidden;
//    SftpFileModel m_fileInfoModel;
};


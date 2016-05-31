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




//FIXME make these things
// QQuery connection to upload one file after another
// watch the speed of up/down load

#include "sftpfilemodel.h"
#include "sftpupload.h"


//////////////////////////////////
///////////// ITEMS
SftpItem::SftpItem(
        const QString &name ,
        const QString &type,
        const QString &size,
        const QString &permissions) :
      m_name(name),
      m_type(type),
      m_size(size),
      m_permissions(permissions)
{
}

QString SftpItem::name() const
{
    return  m_name;
}
QString SftpItem::type() const
{
    return  m_type;
}

QString SftpItem::size() const
{
    return  m_size;
}

QString SftpItem::permissions() const
{
    return  m_permissions;
}


//////////////////
//MODEL
/////////////////////



// Model Side
SftpRemoteFileSystemModel::SftpRemoteFileSystemModel(QObject *parent)
    : QAbstractListModel(parent),
      m_count(0)
{
//    fillModel();
    connect(this,SIGNAL(dataIn()),this,SLOT(setCount()));
}

void SftpRemoteFileSystemModel::fillModel()
{

}

void SftpRemoteFileSystemModel::clearModel()
{
        beginResetModel();
        m_sftpItemList.clear();
        endResetModel();
        emit modelRest();
}

int SftpRemoteFileSystemModel::count()
{
    return m_count;
}

void SftpRemoteFileSystemModel::setCount(){
    if( m_sftpItemList.count()  == m_count )
        return;

    m_count = m_sftpItemList.count();
    emit countChanged();
}


///  add to the model
void SftpRemoteFileSystemModel::addToModel(const SftpItem &sftpItemInfo)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    m_sftpItemList << sftpItemInfo;
    endInsertRows();
}

int SftpRemoteFileSystemModel::rowCount(const QModelIndex & parent) const
{
    Q_UNUSED(parent);
    return m_sftpItemList.count();
}

QVariant SftpRemoteFileSystemModel::data(const QModelIndex & index, int role) const
{
    if (index.row() < 0 || index.row() >= m_sftpItemList.count())
        return QVariant();
    const SftpItem &sftpItem = m_sftpItemList[index.row()];
    if (role == Name)
        return sftpItem.name();
    else if (role == Type)
        return sftpItem.type();
    else if(role == Size)
        return sftpItem.size();
    else if (role == Permissions)
        return sftpItem.permissions();
    return QVariant();
}

QHash<int, QByteArray> SftpRemoteFileSystemModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[Name] = "name";
    roles[Type] = "type";
    roles[Size] = "size";
    roles[Permissions] = "permissions";
    return roles;
}



















////////////////////////////
// IMPAMENTATION
//////////////////////////////
Sftp::Sftp(QObject *parent) :
    QObject(parent),
    m_sshPort(22),
    m_timeout(10),
    m_jobId(UploadFile),
    m_authenticationType(AuthenticationByPassword),
    mm_authenticationType(QSsh::SshConnectionParameters::AuthenticationTypePassword),
    m_overwriteMode(SftpOverwriteExisting),
    mm_overwriteMode(QSsh::SftpOverwriteExisting),
    m_showHidden(false)

{
    connect(this,SIGNAL(error(QString)),this,SLOT(handelError(QString)));
}

//Sftp::~Sftp()
//{
//    delete m_connection;
//}


/*!
 \qmlproperty string Sftp::localFile
 This is the local file on your local computer that you would like to upload to a remote server
 */
QString Sftp::localFile()
{
    return m_localfile;
}

void Sftp::setLocalFile(const QString &localFIle)
{
    if(m_localfile == localFIle)
        return;
    m_localfile = localFIle;
    emit localFileChanged();
}

QString Sftp::remoteFileName() const
{
    return m_remoteFilename;
}

void Sftp::setRemoteFileName(const QString &remoteFileName)
{
    if(m_remoteFilename == remoteFileName)
        return;
    m_remoteFilename = remoteFileName;
    emit remoteFileNameChanged();
}
/*!
 \qmlproperty string Sftp::destination
 This is the full path that you want to upload to.

 example:

say I have a server with the dir /home/someUser/uploads IF I wanted to upload to that folder then I would set it to that,

\code
destination: "/home/someUser/uploaders"
\endcode

 */

QString Sftp::destination()
{
    return m_destination;
}

void Sftp::setDestination(const QString &destination)
{
    if(m_destination == destination)
        return;
    m_destination = destination;
    emit destinationChanged();
}

/*!
  \qmlproperty strinn Sftp::username
  This is the Username of the remote machine that you would like to upload to.
 */

QString Sftp::username()
{
    return m_username;
}

void Sftp::setUsername(const QString &username)
{
    if(m_username == username)
        return;
    m_username = username;
    emit usernameChanged();
}

/*!
 \qmlproperty string Sftp::hostName
 This is the hostname of the remote machine that you would like to upload your files too.Used with the ssh connection to upload the file.
 */
QString Sftp::hostName()
{
    return m_hostName;
}

void Sftp::setHostName(const QString &hostName)
{
    if(m_hostName == hostName)
        return;
    m_hostName = hostName;
    emit hostNameChanged();
}
/*!
 \qmlproperty string Sftp::passwd
 The Password for the remote machine that you would like to upload the file to.
 */
QString Sftp::passwd()
{
    return m_passwd;
}

void Sftp::setPasswd(const QString &passwd)
{
    if(m_passwd == passwd)
        return;
    m_passwd = passwd;
    emit passwdChanged();
}
/*!
  \qmlproperty int Sftp::sshPort
 This is used to set the port that ssh is listening on,  By default this is set to port 22
 */
int Sftp::sshPort()
{
    return m_sshPort;
}

void Sftp::setSshPort(int &sshPort)
{
    if(m_sshPort == sshPort)
        return;
    m_sshPort = sshPort;
    emit sshPortChanged();
}

int Sftp::timeout()
{
    return m_timeout;
}

void Sftp::setTimeout(int &timeout)
{
    if(m_timeout == timeout)
        return;
    m_timeout = timeout;
    emit timeoutChanged();
}



QString Sftp::setPermissions(const QFile::Permissions fi)
{
//    const QFile::Permissions permissions = fi;
    QString result = QLatin1String("----------");

    //FIXME make so that I can see if system link or
    //    if (fi.isSymLink())
//        result[0] = QLatin1Char('l');
//    else if (fi.isDir())
//        result[0] = QLatin1Char('d');

    if (fi & QFileDevice::ReadUser)
        result[1] = QLatin1Char('r');
    if (fi & QFileDevice::WriteUser)
        result[2] = QLatin1Char('w');
    if (fi & QFileDevice::ExeUser)
        result[3] = QLatin1Char('x');
    if (fi & QFileDevice::ReadGroup)
        result[4] = QLatin1Char('r');
    if (fi & QFileDevice::WriteGroup)
        result[5] = QLatin1Char('w');
    if (fi & QFileDevice::ExeGroup)
        result[6] = QLatin1Char('x');
    if (fi & QFileDevice::ReadOther)
        result[7] = QLatin1Char('r');
    if (fi & QFileDevice::WriteOther)
        result[8] = QLatin1Char('w');
    if (fi & QFileDevice::ExeOther)
        result[9] = QLatin1Char('x');
    return result;
//    qDebug() << result;

}

bool Sftp::showHidden() const
{
    return m_showHidden;
}

void Sftp::setShowHidden(const bool &showHidden)
{
    if(m_showHidden == showHidden)
        return;
    m_showHidden = showHidden;
    emit showHiddenChanged();
}

QString Sftp::errorString() const
{
    return m_errorString;
}

void Sftp::exec()
{
    QFileInfo info(m_localfile);
    m_localFilename = info.canonicalFilePath();
    m_remoteFilename = m_destination + "/" + info.fileName();
    QSsh::SshConnectionParameters params;
    params.host = m_hostName;
    params.userName = m_username;
    params.authenticationType = mm_authenticationType;

    //FIXME add the other types
    if(m_authenticationType == AuthenticationByKey){
        params.privateKeyFile = m_keyFile;
    }
    else if (m_authenticationType == AuthenticationByPassword)
    {
        params.password = m_passwd;
    }

    params.timeout = m_timeout;
    params.port = m_sshPort;

    m_connection = new QSsh::SshConnection(params, this);
    connect(m_connection,SIGNAL(disconnected()),this,SIGNAL(finshed()));
    connect(m_connection,SIGNAL(connected()),this,SIGNAL(connected()));
    connect(m_connection,SIGNAL(dataAvailable(QString)),this,SLOT(dataAvailable(QString)));
    connect(m_connection,SIGNAL(error(QSsh::SshError)),this,SLOT(connectionError(QSsh::SshError)));
    connect(m_connection, SIGNAL(connected()), SLOT(onConnected()));
    connect(m_connection,SIGNAL(error(QSsh::SshError)),this,SLOT(connectionError(QSsh::SshError)));

    m_connection->connectToHost();
}

//SftpFileModel Sftp::fileModel()
//{
//    return m_fileInfoModel;
//}

Sftp::JobId Sftp::jobId() const
{
    return m_jobId;
}

void Sftp::setJobId(const Sftp::JobId &jobId)
{
    if(m_jobId == jobId)
    {
        return;
    }
    else
    {
        switch (jobId) {
        case DownloadFile:
            m_jobId = DownloadFile;
            break;
        case UploadDir:
            m_jobId = UploadDir ;
            break;
        case CreateDirectory:
            m_jobId = CreateDirectory;
            break;
        case CreateFile:
            m_jobId = CreateFile;
            break;
        case CreateLink:
            m_jobId = CreateLink;
            break;
        case ListDirectory:
            m_jobId = ListDirectory;
            break;
        case RemoveFile:
            m_jobId = RemoveFile;
            break;
        case RemoveDirectory:
            m_jobId = RemoveDirectory;
            break;
        case StatFile:
            m_jobId = StatFile;
            break;
        case RenameFileOrDirectory:
            m_jobId = RenameFileOrDirectory ;
            break;
        case UploadFile:
            m_jobId = UploadFile;
            break;
        }
        emit jobIdChanged();
    }
}

Sftp::SftpOverwriteMode Sftp::overwriteMode() const
{
    return m_overwriteMode;
}

void Sftp::setOverwriteMode(const Sftp::SftpOverwriteMode &overwriteMode)
{
    if(m_overwriteMode == overwriteMode)
    {
        return;
    }
    else
    {
        switch (overwriteMode) {
        case SftpOverwriteExisting:
            m_overwriteMode = SftpOverwriteExisting ;
            mm_overwriteMode = QSsh::SftpOverwriteExisting ;
            break;
        case SftpAppendToExisting:
            m_overwriteMode = SftpAppendToExisting;
            mm_overwriteMode = QSsh::SftpAppendToExisting ;
            break;
        case SftpSkipExisting:
            m_overwriteMode = SftpSkipExisting;
            mm_overwriteMode = QSsh::SftpSkipExisting ;
            break;
        }

        emit overwriteModeChanged();
    }
}

Sftp::AuthenticationType Sftp::authenticationType() const
{
    return m_authenticationType;
}

void Sftp::setAuthenticationType(const Sftp::AuthenticationType &authenticationType)
{
    int aT = authenticationType;
    if(m_authenticationType == authenticationType)
    {
        return;
    }
    else
    {
        qDebug() << "checking auth type " ;
        switch (aT)
        {
        case 0 :
            m_authenticationType = AuthenticationByPassword;
            mm_authenticationType = QSsh::SshConnectionParameters::AuthenticationTypePassword;
            break;
        case 1:
            m_authenticationType = AuthenticationByKey;
            mm_authenticationType = QSsh::SshConnectionParameters::AuthenticationTypePublicKey ;
            break;
        case 2:
            m_authenticationType = AuthenticationTypeKeyboardInteractive;
            mm_authenticationType = QSsh::SshConnectionParameters::AuthenticationTypeKeyboardInteractive ;
            break;
        case 3:
            m_authenticationType = AuthenticationTypeTryAllPasswordBasedMethods ;
            mm_authenticationType = QSsh::SshConnectionParameters::AuthenticationTypeTryAllPasswordBasedMethods ;
            break;
        }
        emit authenticationTypeChanged();
    }
}

Sftp::SshError Sftp::sshError() const
{
    return m_sshError;
}

QStringList Sftp::remoteFileNames() const
{
    return m_remoteFileNames;
}

QStringList Sftp::remoteFilesSize() const
{
    return m_remoteFileSize;
}

QStringList Sftp::remoteFilesType() const
{
    return m_remoteType;
}

QStringList Sftp::remoteFilePermissions() const
{
     return m_remotePermissions;
}


QString Sftp::localDir() const
{
    return m_localDir;
}

void Sftp::setLocalDir(const QString &localDir)
{
    if(m_localDir == localDir)
        return;
    m_localDir = localDir;
    emit localDirChanged();
}

QString Sftp::remoteDir() const
{
    return m_remoteDir;
}

void Sftp::setRemoteDir(const QString &remoteDir)
{
    if(m_remoteDir == remoteDir)
        return;
    m_remoteDir = remoteDir;
    emit remoteDirChanged();
}

QString Sftp::linkName() const
{
    return m_linkName;
}

void Sftp::setLinkName(const QString &linkName)
{
    if(m_linkName == linkName)
        return;
    m_linkName = linkName;
    emit linkNameChanged();
}

QString Sftp::keyFile() const
{
    return m_keyFile;
}

void Sftp::setKeyFile(const QString &keyFile)
{
    if(m_keyFile == keyFile)
        return;
    m_keyFile = keyFile;
    emit keyFileChanged();
}

void Sftp::onConnected()
{
    m_channel = m_connection->createSftpChannel();

    if (m_channel) {
        connect(m_channel.data(), SIGNAL(initialized()), SLOT(onChannelInitialized()));
        connect(m_channel.data(), SIGNAL(finished(QSsh::SftpJobId, QString)),
                SLOT(onOpfinished(QSsh::SftpJobId, QString)));

        connect(m_channel.data(),SIGNAL(fileInfoAvailable(QSsh::SftpJobId,QList<QSsh::SftpFileInfo>))
                ,this,SLOT(reciveFileInfo(QSsh::SftpJobId,QList<QSsh::SftpFileInfo>)));

        m_channel->initialize();

    } else {
//        qDebug() << "SftpUpload Error null channel";
    }
}

void Sftp::connectionError(QSsh::SshError err)
{
    int sErr = err;
    if(m_sshError == sErr)
    {
        return;
    }
    else
    {

        switch (sErr) {
        case 0:
            m_sshError = SshNoError;
            break;
        case 1:
            m_sshError = SshSocketError;
            error("Socket Error");
            break;
        case 2:
            m_sshError = SshTimeoutError;
            error("Could not connect to server it timed out");
            break;
        case 3:
            m_sshError = SshProtocolError;
            error("Protocol error");
            break;
        case 4 :
            m_sshError = SshHostKeyError;
            error("Host Key was not accepted or does not exsist");
            break;
        case 5:
            m_sshError = SshKeyFileError;
            error("File Key Error");
            break;
        case 6:
            m_sshError = SshAuthenticationError;
            error("There was a error in Authentication");
            break;
        case 7:
            m_sshError = SshClosedByServerError;
            error("Remote host Closed the session");
            break;
        case 8:
            m_sshError = SshInternalError;
            error("Internal Error");
            break;
        }
        emit sshErrorChanged();
    }

}

void Sftp::onChannelInitialized()
{
    qDebug() << "SftpUpload Channel Initialized Setting up the job" ;
    QSsh::SftpJobId jId;
    switch (m_jobId) {
    case DownloadFile:
        jId = m_channel->downloadFile(
                    m_remoteFilename,
                    m_localfile,
                    mm_overwriteMode
                    );
                break;
    case UploadDir:
        jId = m_channel->uploadDir(
                    m_localDir,
                    m_remoteDir
                    );
        break;
    case CreateDirectory:
        jId = m_channel->createDirectory(
                    m_remoteDir
                    );
        break;
    case CreateFile:
        jId = m_channel->createFile(
                    m_remoteFilename,
                    mm_overwriteMode
                    );
        break;
    case CreateLink:
        jId = m_channel->createLink(
                    m_remoteFilename,
                    m_linkName
                    );
        break;
    case ListDirectory:
        jId = m_channel->listDirectory(
                    m_remoteDir
                    );
        break;
    case RemoveFile:
        jId = m_channel->removeFile(
                    m_remoteFilename
                    );
        break;
    case RemoveDirectory:
        jId = m_channel->removeDirectory(
                    m_remoteDir
                    );
        break;
    case StatFile:
        jId = m_channel->statFile(
                    m_remoteFilename
                    );
        break;
    case RenameFileOrDirectory:
        break;
    case UploadFile:
        jId = m_channel->uploadFile(
                    m_localFilename,
                    m_remoteFilename,
                    mm_overwriteMode
                    );
        break;
    }
    if (jId != QSsh::SftpInvalidJob)
    {
        QString er = QString("SftpUpload Starting job #%1").arg(jId);
        qInfo() << er;
    }
    else
    {
        QString er = QString("SftpUpload Invalid Job #%1").arg(jId);
        error(er);
    }
}
//This is not needed anymore
void Sftp::onChannelError(const QString err)
{
    qDebug() << "SftpUpload Channel Error: " << err;
}

void Sftp::onOpfinished(QSsh::SftpJobId job, const QString &err)
{
    if (!err.isEmpty())
    {
        QString strErr = QString("SftpUpload Finished job (#%1) But there was errors::  %2").arg(job).arg(err) ;
        emit error(strErr);
        m_connection->deleteLater();
    }
    else
    {
        qDebug() << "SftpUpload Success Finished job # " << job;
        // This will not be needed as finished is on disconnected
        emit done();
        m_connection->deleteLater();
    }
}

void Sftp::handelError(const QString err)
{
    if(m_errorString == err)
        return;
    m_errorString = err;
    emit errorStringChanged();
}

void Sftp::dataAvailable(const QString data)
{
    qDebug() << "Here is the Data that is coming in " << data;
}

void Sftp::reciveFileInfo(QSsh::SftpJobId id, QList<QSsh::SftpFileInfo> fileInfoList)
{
    qDebug() << "recive file ID " << id;
    m_remoteFileNames.clear();
    m_remotePermissions.clear();
    m_permissionsValid.clear();
    m_remoteFileSize.clear();
    m_remoteFileSizeValid.clear();
    m_remoteType.clear();

    QString ty;
    ty.clear();
    //model that si not working
    SftpRemoteFileSystemModel model;
    model.clearModel();

    //show hidden files but no dot and dotdot
    if (m_showHidden == true){
    for (int i = 0 ; i < fileInfoList.count(); i++){
        //        remove the dir . and ...
        if (fileInfoList.at(i).name != QLatin1String(".") &&   fileInfoList.at(i).name != QLatin1String("..") ){
                m_remoteFileNames  << fileInfoList.at(i).name;
                // file or dir
                if (fileInfoList.at(i).type == QSsh::FileTypeDirectory)
                {
                    ty = "Dir";
                    m_remoteType << "Dir";
                }
                else if (fileInfoList.at(i).type == QSsh::FileTypeRegular)
                {
                    ty  = "File";
                    m_remoteType << "File";
                }
                else if (fileInfoList.at(i).type == QSsh::FileTypeUnknown || fileInfoList.at(i).type == QSsh::FileTypeOther)
                {
                    ty = "Unknown";
                    m_remoteType << "Unknown";
                }
                // size of dir or file
                model.addToModel(
                            SftpItem(
                                fileInfoList.at(i).name,
                                ty,
                                QString("%1").arg(fileInfoList.at(i).size),
                                setPermissions(fileInfoList.at(i).permissions)
                                )
                            );
                model.dataIn();
                ty.clear();
                m_remoteFileSize << QString("%1").arg(fileInfoList.at(i).size);
                m_remotePermissions <<   setPermissions(fileInfoList.at(i).permissions);
            }
            }
        }


    // show hidden
    else if (m_showHidden == false ){
        for (int i = 0 ; i < fileInfoList.count(); i++){

            QString hF = fileInfoList.at(i).name;

            if (!hF.startsWith(".")){
                    m_remoteFileNames  << fileInfoList.at(i).name;
                    // file or dir
                    if (fileInfoList.at(i).type == QSsh::FileTypeDirectory)
                    {
                        ty = "Dir";
                        m_remoteType << "Dir";
                    }
                    else if (fileInfoList.at(i).type == QSsh::FileTypeRegular)
                    {
                        ty  = "File";
                        m_remoteType << "File";
                    }
                    else if (fileInfoList.at(i).type == QSsh::FileTypeUnknown || fileInfoList.at(i).type == QSsh::FileTypeOther)
                    {
                        ty = "Unknown";
                        m_remoteType << "Unknown";
                    }

                    // size of dir or file

                    model.addToModel(
                                SftpItem(
                                    fileInfoList.at(i).name,
                                    ty,
                                    QString("%1").arg(fileInfoList.at(i).size),
                                    setPermissions(fileInfoList.at(i).permissions)
                                    )
                                );
                    model.dataIn();
                    ty.clear();
                    m_remoteFileSize << QString("%1").arg(fileInfoList.at(i).size);
                    m_remotePermissions <<   setPermissions(fileInfoList.at(i).permissions);
                }
                }

    }
    remoteFileNamesChanged();
    }






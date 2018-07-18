
#include "qmlfileinfo.h"

QmlFileInfo::QmlFileInfo(QObject *parent) :
    QObject(parent),
    m_file("")
{
    connect(this,SIGNAL(newFile(QString)),this,SLOT(setProperties(QString)));

}

QString QmlFileInfo::file() const
{
    return m_file;
}

void QmlFileInfo::setFile(const QString &file)
{
    if(m_file == file)
        return;
    m_file = file;
    emit fileChanged();
    newFile(m_file);
}

QmlFileInfo::Type QmlFileInfo::type() const
{
    return m_type;
}

QString QmlFileInfo::absoluteFilePath() const
{
    return m_absoluteFilePath  ;
}

QString QmlFileInfo::absolutePath() const
{
    return m_absolutePath ;
}

QString QmlFileInfo::baseName() const
{
    return m_baseName  ;
}

QString QmlFileInfo::bundleName() const
{
    return m_bundleName ;
}

bool QmlFileInfo::caching() const
{
    return m_caching  ;
}

QString QmlFileInfo::canonicalFilePath() const
{
    return m_canonicalFilePath  ;
}

QString QmlFileInfo::canonicalPath() const
{
    return m_canonicalPath     ;
}

QString QmlFileInfo::completeBaseName() const
{
    return m_completeBaseName  ;
}

QString QmlFileInfo::completeSuffix() const
{
    return m_completeSuffix  ;
}

QDateTime QmlFileInfo::created() const
{
    return m_created  ;
}

QDir QmlFileInfo::dir() const
{
    return m_dir  ;
}

bool QmlFileInfo::exists() const
{
    return m_exists  ;
}

QString QmlFileInfo::fileName() const
{
    return m_fileName  ;
}

QString QmlFileInfo::filePath() const
{
    return m_filePath  ;
}

QString QmlFileInfo::group() const
{
    return m_group  ;
}

uint QmlFileInfo::groupId() const
{
    return m_groupId  ;
}

bool QmlFileInfo::isAbsolute() const
{
    return m_isAbsolute  ;
}

bool QmlFileInfo::isBundle() const
{
    return m_isBundle  ;
}

bool QmlFileInfo::isDir() const
{
    return m_isDir  ;
}

bool QmlFileInfo::isExecutable() const
{
    return m_isExecutable  ;
}

bool QmlFileInfo::isFile() const
{
    return m_isFile ;
}

bool QmlFileInfo::isHidden() const
{
    return m_isHidden  ;
}

bool QmlFileInfo::isNativePath() const
{
    return m_isNativePath  ;
}

bool QmlFileInfo::isReadable() const
{
    return m_isReadable  ;
}

bool QmlFileInfo::isRelative() const
{
    return m_isRelative  ;
}

bool QmlFileInfo::isRoot() const
{
    return m_isRoot  ;
}

bool QmlFileInfo::isSymLink() const
{
    return m_isSymLink  ;
}

bool QmlFileInfo::isWritable() const
{
    return m_isWritable  ;
}

QDateTime QmlFileInfo::lastModified() const
{
    return m_lastModified  ;
}

QDateTime QmlFileInfo::lastRead() const
{
    return m_lastRead  ;
}

QString QmlFileInfo::owner() const
{
    return m_owner  ;
}

uint QmlFileInfo::ownerId() const
{
    return m_ownerId  ;
}

QString QmlFileInfo::path() const
{
    return m_path  ;
}

QString QmlFileInfo::permissions() const
{
    return m_permissions;
}

//TODO add more checkers to this for signals
void QmlFileInfo::setCache(bool &enable,QString file)
{
    QFileInfo f(file);
    f.setCaching(enable);
}

qint64 QmlFileInfo::size() const
{
    return m_size  ;
}

QString QmlFileInfo::prettySize() const
{
    return m_prettySize;
}

QString QmlFileInfo::suffix() const
{
    return m_suffix  ;
}


void QmlFileInfo::setAbsoluteFilePath(const QString &absoluteFilePath)
{
    if( m_absoluteFilePath ==absoluteFilePath )
        return;
    m_absoluteFilePath = absoluteFilePath;
    emit absoluteFilePathChanged();
}

void QmlFileInfo::setAbsolutePath(const QString &absolutePath)
{
    if( m_absolutePath == absolutePath)
        return;
    m_absolutePath = absolutePath;
    emit absolutePathChanged();
}

void QmlFileInfo::setBaseName(const QString &baseName)
{
    if( m_baseName == baseName)
        return;
    m_baseName = baseName;
    emit baseNameChanged();
}

void QmlFileInfo::setBundleName(const QString &bundleName)
{
    if( m_bundleName == bundleName)
        return;
    m_bundleName = bundleName;
    emit bundleNameChanged();
}

void QmlFileInfo::setCaching(const bool &caching)
{
    if( m_caching == caching)
        return;
    m_caching = caching;
    emit cachingChanged();
}

void QmlFileInfo::setCanonicalFilePath(const QString &canonicalFilePath)
{
    if( m_canonicalFilePath == canonicalFilePath)
        return;
    m_canonicalFilePath = canonicalFilePath;
    emit canonicalFilePathChanged();
}

void QmlFileInfo::setCanonicalPath(const QString &canonicalPath)
{
    if( m_canonicalPath == canonicalPath)
        return;
    m_canonicalPath = canonicalPath;
    emit canonicalPathChanged();
}

void QmlFileInfo::setCompleteBaseName(const QString &completeBaseName)
{
    if( m_completeBaseName ==completeBaseName )
        return;
    m_completeBaseName = completeBaseName;
    emit completeBaseNameChanged();
}

void QmlFileInfo::setCompleteSuffix(const QString &completeSuffix)
{
    if( m_completeSuffix == completeSuffix)
        return;
    m_completeSuffix = completeSuffix;
    emit completeSuffixChanged();
}

void QmlFileInfo::setCreated(const QDateTime &created)
{
    if( m_created == created)
        return;
    m_created = created;
    emit createdChanged();
}

void QmlFileInfo::setDir(const QDir &dir)
{
    if( m_dir == dir)
        return;
    m_dir = dir;
    emit dirChanged();
}

void QmlFileInfo::setExists(const bool &exists)
{
    if( m_exists == exists)
        return;
    m_exists = exists;
    emit existsChanged();
}

void QmlFileInfo::setFileName(const QString &fileName)
{
    if( m_fileName == fileName )
        return;
    m_fileName = fileName;
    emit fileNameChanged();
}

void QmlFileInfo::setFilePath(const QString &filePath)
{
    if( m_filePath == filePath)
        return;
    m_filePath = filePath;
    emit filePathChanged();
}

void QmlFileInfo::setGroup(const QString &group)
{
    if( m_group == group)
        return;
    m_group = group;
    emit groupChanged();
}

void QmlFileInfo::setGroupId(const uint &groupId)
{
    if( m_groupId ==groupId )
        return;
    m_groupId = groupId;
    emit groupIdChanged();
}

void QmlFileInfo::setIsAbsolute(const bool &isAbsolute)
{
    if( m_isAbsolute == isAbsolute)
        return;
    m_isAbsolute = isAbsolute;
    emit isAbsoluteChanged();
}

void QmlFileInfo::setIsBundle(const bool &isBundle)
{
    if( m_isBundle == isBundle)
        return;
    m_isBundle = isBundle;
    emit isBundleChanged();
}

void QmlFileInfo::setIsDir(const bool &isDir)
{
    if( m_isDir == isDir)
        return;
    m_isDir = isDir;
    emit isDirChanged();
}

void QmlFileInfo::setIsExecutable(const bool &isExecutable)
{
    if( m_isExecutable == isExecutable)
        return;
    m_isExecutable = isExecutable;
    emit isExecutableChanged();
}

void QmlFileInfo::setIsFile(const bool &isFile)
{
    if( m_isFile == isFile)
        return;
    m_isFile = isFile;
    emit isFileChanged();
}

void QmlFileInfo::setIsHidden(const bool &isHidden)
{
    if( m_isHidden == isHidden)
        return;
    m_isHidden = isHidden;
    emit isHiddenChanged();
}

void QmlFileInfo::setIsNativePath(const bool &isNativePath)
{
    if( m_isNativePath == isNativePath)
        return;
    m_isNativePath = isNativePath;
    emit isNativePathChanged();
}

void QmlFileInfo::setIsReadable(const bool &isReadable)
{
    if( m_isReadable == isReadable)
        return;
    m_isReadable = isReadable;
    emit isReadableChanged();
}

void QmlFileInfo::setIsRelative(const bool &isRelative)
{
    if( m_isRelative == isRelative)
        return;
    m_isRelative = isRelative;
    emit isRelativeChanged();
}

void QmlFileInfo::setIsRoot(const bool &isRoot)
{
    if( m_isRoot == isRoot)
        return;
    m_isRoot = isRoot;
    emit isRootChanged();
}

void QmlFileInfo::setIsSymLink(const bool &isSymLink)
{
    if( m_isSymLink == isSymLink)
        return;
    m_isSymLink = isSymLink;
    emit isSymLinkChanged();
}

void QmlFileInfo::setIsWritable(const bool &isWritable)
{
    if( m_isWritable == isWritable)
        return;
    m_isWritable = isWritable;
    emit isWritableChanged();
}

void QmlFileInfo::setLastModified(const QDateTime &lastModified)
{
    if( m_lastModified == lastModified)
        return;
    m_lastModified = lastModified;
    emit lastModifiedChanged();
}

void QmlFileInfo::setLastRead(const QDateTime &lastRead)
{
    if( m_lastRead == lastRead)
        return;
    m_lastRead = lastRead;
    emit lastReadChanged();
}



void QmlFileInfo::setOwner(const QString &owner)
{
    if( m_owner == owner )
        return;
    m_owner = owner;
    emit ownerChanged();
}

void QmlFileInfo::setOwnerId(const uint &ownerId)
{
    if( m_ownerId == ownerId)
        return;
    m_ownerId = ownerId;
    emit ownerIdChanged();
}

void QmlFileInfo::setPath(const QString &path)
{
    if( m_path == path)
        return;
    m_path = path;
    emit pathChanged();
}

void QmlFileInfo::setPermissions(const QString &permissions)
{
    if(m_permissions == permissions)
        return;
    m_permissions = permissions;
    emit permissionsChanged();
}

//void QmlFileInfo::setPermissions(const QFileDevice::Permissions &permissions)
//{
//    if( m_permissions == permissions)
//        return;
//    m_permissions = permissions ;
//    emit permissionsChanged();
//}

void QmlFileInfo::setSize(const qint64 &size)
{
    if( m_size == size)
        return;
    m_size = size;
    emit sizeChanged();
}

void QmlFileInfo::setPrettySize(const int &prettySize)
{




    QString after;
    after = formatSize(prettySize);
//    QString yy = QString("%1").arg(after);


    if( m_prettySize == after )
        return;
    m_prettySize = after;
    emit prettySizeChanged();
}

void QmlFileInfo::setSuffix(const QString &suffix)
{
    if( m_suffix == suffix)
        return;
    m_suffix = suffix;
    emit suffixChanged();
}

QString QmlFileInfo::formatSize(const int bytes)
{
    float num = bytes;
    QStringList list;
    list << "KB" << "MB" << "GB" << "TB";

    QStringListIterator i(list);
    QString unit("bytes");

    while(num >= 1024.0 && i.hasNext())
    {
        unit = i.next();
        num /= 1024.0;
    }
    return QString().setNum(num,'f',2)+" "+unit;
}



//FIXME AFTER TONIGHT
// FEB 13th

void QmlFileInfo::setProperties(const QString fi)
{
    QFileInfo d(fi);

    //permisiions
    prettyPermissions(d);

    // everything else
//    setAbsoluteDir( d.absoluteDir()   );
    setAbsoluteFilePath( d.absoluteFilePath() );
    setAbsolutePath( d.absolutePath() );
    setBaseName( d.baseName() );
    setBundleName( d.bundleName() );
    setCaching( d.caching() );
    setCanonicalFilePath( d.canonicalFilePath() );
    setCanonicalPath( d.canonicalPath() );
    setCompleteBaseName( d.completeBaseName() );
    setCompleteSuffix( d.completeSuffix() );
    setCreated( d.created() );
    setDir( d.dir() );
    setExists( d.exists() );
    setFileName( d.fileName() );
    setFilePath( d.filePath() );
    setGroup( d.group() );
    setGroupId( d.groupId() );
    setIsAbsolute( d.isAbsolute() );
    setIsBundle( d.isBundle() );
    setIsDir( d.isDir() );
    setIsExecutable( d.isExecutable() );
    setIsFile( d.isFile() );
    setIsHidden( d.isFile() );
    setIsNativePath( d.isNativePath() );
    setIsReadable( d.isReadable() );
    setIsRelative( d.isRelative() );
    setIsRoot( d.isRoot() );
    setIsSymLink( d.isSymLink() );
    setIsWritable( d.isWritable() );
    setLastModified( d.lastModified() );
    setLastRead( d.lastRead() );
    setOwner( d.owner() );
    setOwnerId( d.ownerId() );
    setPath( d.path() );
    setSize( d.size() );
    setPrettySize( d.size() );
    setSuffix( d.suffix() );
}




void QmlFileInfo::prettyPermissions(const QFileInfo fi )
{
    const QFile::Permissions permissions = fi.permissions();
    QString result = QLatin1String("----------");

    //FIXME make so that I can see if system link or
        if (fi.isSymLink())
        result[0] = QLatin1Char('l');
    else if (fi.isDir())
        result[0] = QLatin1Char('d');
    if (permissions & QFileDevice::ReadUser)
        result[1] = QLatin1Char('r');
    if (permissions & QFileDevice::WriteUser)
        result[2] = QLatin1Char('w');
    if (permissions & QFileDevice::ExeUser)
        result[3] = QLatin1Char('x');
    if (permissions & QFileDevice::ReadGroup)
        result[4] = QLatin1Char('r');
    if (permissions & QFileDevice::WriteGroup)
        result[5] = QLatin1Char('w');
    if (permissions & QFileDevice::ExeGroup)
        result[6] = QLatin1Char('x');
    if (permissions & QFileDevice::ReadOther)
        result[7] = QLatin1Char('r');
    if (permissions & QFileDevice::WriteOther)
        result[8] = QLatin1Char('w');
    if (permissions & QFileDevice::ExeOther)
        result[9] = QLatin1Char('x');
    setPermissions(result);
}


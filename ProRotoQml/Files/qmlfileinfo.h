#ifndef QMLFILEINFO_H
#define QMLFILEINFO_H

#include <QObject>
#include <QFileInfo>
#include <QString>
#include <QStringList>
#include <QDebug>
#include <QDir>
#include <QDateTime>
#include <QFileDevice>
//  for pretty sizes
#include "../Utils/utils.h"


class QmlFileInfo : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString file READ file WRITE setFile NOTIFY fileChanged)

    Q_PROPERTY(QString absoluteFilePath READ absoluteFilePath NOTIFY absoluteFilePathChanged)
    Q_PROPERTY(QString absolutePath READ absolutePath NOTIFY absolutePathChanged)
    Q_PROPERTY(QString baseName READ baseName  NOTIFY baseNameChanged)
    Q_PROPERTY(QString bundleName READ bundleName  NOTIFY bundleNameChanged)
    Q_PROPERTY(bool caching READ caching  NOTIFY cachingChanged)
    Q_PROPERTY(QString canonicalFilePath READ canonicalFilePath NOTIFY canonicalFilePathChanged)
    Q_PROPERTY(QString canonicalPath READ canonicalPath NOTIFY canonicalPathChanged)
    Q_PROPERTY(QString completeBaseName READ completeBaseName  NOTIFY completeBaseNameChanged)
    Q_PROPERTY(QString completeSuffix READ completeSuffix NOTIFY completeSuffixChanged)
    Q_PROPERTY(QDateTime created READ created  NOTIFY createdChanged)
    Q_PROPERTY(bool exists READ exists  NOTIFY existsChanged)
    Q_PROPERTY(QString fileName READ fileName NOTIFY fileNameChanged)
    Q_PROPERTY(QString filePath READ filePath NOTIFY filePathChanged)
    Q_PROPERTY(QString group READ group  NOTIFY groupChanged)
    Q_PROPERTY(uint groupId READ groupId NOTIFY groupIdChanged)
    Q_PROPERTY(bool isAbsolute READ isAbsolute NOTIFY isAbsoluteChanged)
    Q_PROPERTY(bool isBundle READ isBundle  NOTIFY isBundleChanged)
    Q_PROPERTY(bool isDir READ isDir NOTIFY isDirChanged)
    Q_PROPERTY(bool isExecutable READ isExecutable  NOTIFY isExecutableChanged)
    Q_PROPERTY(bool isFile READ isFile  NOTIFY isFileChanged)
    Q_PROPERTY(bool isHidden READ isHidden  NOTIFY isHiddenChanged)
    Q_PROPERTY(bool isNativePath READ isNativePath  NOTIFY isNativePathChanged)
    Q_PROPERTY(bool isReadable READ isReadable  NOTIFY isReadableChanged)
    Q_PROPERTY(bool isRelative READ isRelative  NOTIFY isRelativeChanged)
    Q_PROPERTY(bool isRoot READ isRoot  NOTIFY isRootChanged)
    Q_PROPERTY(bool isSymLink READ isSymLink  NOTIFY isSymLinkChanged)
    Q_PROPERTY(bool isWritable READ isWritable NOTIFY isWritableChanged)
    Q_PROPERTY(QDateTime lastModified READ lastModified NOTIFY lastModifiedChanged)
    Q_PROPERTY(QDateTime lastRead READ lastRead NOTIFY lastReadChanged)
    //    Q_PROPERTY(bool name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QString owner READ owner NOTIFY ownerChanged)
    Q_PROPERTY(uint ownerId READ ownerId NOTIFY ownerIdChanged)
    Q_PROPERTY(QString path READ path  NOTIFY pathChanged)
    Q_PROPERTY(QString permissions READ permissions  NOTIFY permissionsChanged)
    Q_PROPERTY(qint64 size READ size  NOTIFY sizeChanged)
    Q_PROPERTY(QString prettySize READ prettySize NOTIFY prettySizeChanged)
    Q_PROPERTY(QString suffix READ suffix NOTIFY suffixChanged)


public:
    explicit QmlFileInfo(QObject *parent = 0);

    QString file()const;
    void setFile(const QString &file);

    enum Type{
        File,
        Dir,
        SystemLink
    };
    Type type()const;
    void setType(const Type &type);

    //Getters
    QString  absoluteFilePath() const;
    QString  absolutePath() const;
    QString  baseName() const;
    QString  bundleName() const;
    bool  caching() const;
    QString  canonicalFilePath() const;
    QString  canonicalPath() const;
    QString  completeBaseName() const;
    QString  completeSuffix() const;
    QDateTime  created() const;
    QDir  dir() const;
    bool  exists() const;
    QString  fileName() const;
    QString  filePath() const;
    QString  group() const;
    uint  groupId() const;
    bool  isAbsolute() const;
    bool  isBundle() const;
    bool  isDir() const;
    bool  isExecutable() const;
    bool  isFile() const;
    bool  isHidden() const;
    bool  isNativePath() const;
    bool  isReadable() const;
    bool  isRelative() const;
    bool  isRoot() const;
    bool  isSymLink() const;
    bool  isWritable() const;
    QDateTime  lastModified() const;
    QDateTime  lastRead() const;
    bool  makeAbsolute();
    QString  owner() const;
    uint  ownerId() const;
    QString  path() const;
    QString  permissions() const;
    void  refresh();
    qint64 size() const;
    QString prettySize()const;
    QString  suffix() const;

    //Setters

    void   setAbsoluteFilePath(const QString &absoluteFilePath );
    void   setAbsolutePath(const QString &absolutePath );
    void   setBaseName(const QString &baseName );
    void   setBundleName(const QString &bundleName );
    void   setCaching(const bool &caching );
    void   setCanonicalFilePath(const QString &canonicalFilePath );
    void   setCanonicalPath(const QString &canonicalPath );
    void   setCompleteBaseName(const QString &completeBaseName );
    void   setCompleteSuffix(const QString &completeSuffix );
    void   setCreated(const QDateTime &created );
    void   setDir(const QDir &dir );
    void   setExists(const bool &exists );
    void   setFileName(const QString &fileName );
    void   setFilePath(const QString &filePath );
    void  setGroup(const QString &group );
    void   setGroupId(const  uint &groupId );
    void   setIsAbsolute(const bool &isAbsolute );
    void   setIsBundle(const bool &isBundle );
    void   setIsDir(const bool &isDir );
    void   setIsExecutable(const bool &isExecutable );
    void   setIsFile(const bool &isFile );
    void   setIsHidden(const bool &isHidden );
    void   setIsNativePath(const bool &isNativePath );
    void   setIsReadable(const bool &isReadable );
    void   setIsRelative(const bool &isRelative );
    void   setIsRoot(const bool &isRoot );
    void   setIsSymLink(const bool &isSymLink );
    void   setIsWritable(const bool &isWritable );
    void   setLastModified(const  QDateTime &lastModified );
    void   setLastRead(const  QDateTime &lastRead );
    void   setOwner(const  QString &owner );
    void   setOwnerId(const  uint &ownerId );
    void   setPath(const  QString &path );
    void  setPermissions(const QString  &permissions );
    void  setSize(const qint64 &size );
    void  setPrettySize(const  int &prettySize );
    void   setSuffix(const QString &suffix );

    // internal
    Q_INVOKABLE  void setCache(bool &enable,QString file);
    QString formatSize(const int bytes);
    void prettyPermissions(const QFileInfo fi);

signals:
    void fileChanged();

    void absoluteDirChanged();
    void absoluteFilePathChanged();
    void absolutePathChanged();
    void baseNameChanged();
    void bundleNameChanged();
    void cachingChanged();
    void canonicalFilePathChanged();
    void canonicalPathChanged();
    void completeBaseNameChanged();
    void completeSuffixChanged();
    void createdChanged();
    void dirChanged();
    void existsChanged();
    void fileNameChanged();
    void filePathChanged();
    void groupChanged();
    void groupIdChanged();
    void isAbsoluteChanged();
    void isBundleChanged();
    void isDirChanged();
    void isExecutableChanged();
    void isFileChanged();
    void isHiddenChanged();
    void isNativePathChanged();
    void isReadableChanged();
    void isRelativeChanged();
    void isRootChanged();
    void isSymLinkChanged();
    void isWritableChanged();
    void lastModifiedChanged();
    void lastReadChanged();
    void makeAbsoluteChanged();
    void ownerChanged();
    void ownerIdChanged();
    void pathChanged();
    void permissionsChanged();
    void sizeChanged();
    void prettySizeChanged();
    void suffixChanged();

    void newFile(const QString fi);

protected slots:
    void setProperties(const QString fi);

private:
    QString m_file;
    QDir  m_absoluteDir;
    QString  m_absoluteFilePath;
    QString  m_absolutePath;
    QString  m_baseName;
    QString  m_bundleName;
    bool  m_caching;
    QString  m_canonicalFilePath;
    QString  m_canonicalPath;
    QString  m_completeBaseName;
    QString  m_completeSuffix;
    QDateTime  m_created;
    QDir  m_dir;
    bool  m_exists;
    QString  m_fileName;
    QString  m_filePath;
    QString  m_group;
    uint  m_groupId;
    bool  m_isAbsolute;
    bool  m_isBundle;
    bool  m_isDir;
    bool  m_isExecutable;
    bool  m_isFile;
    bool  m_isHidden;
    bool  m_isNativePath;
    bool  m_isReadable;
    bool  m_isRelative;
    bool  m_isRoot;
    bool  m_isSymLink;
    bool  m_isWritable;
    QDateTime  m_lastModified;
    QDateTime  m_lastRead;
    bool  m_makeAbsolute();
    QString  m_owner;
    uint  m_ownerId;
    QString  m_path;
    QString  m_permissions;
    void  m_refresh();
    qint64 m_size;
    QString m_prettySize ;
    QString  m_suffix;

    Type m_type;
};

#endif // QMLFILEINFO_H

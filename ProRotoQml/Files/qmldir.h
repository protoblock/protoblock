#ifndef QMLDIR_H
#define QMLDIR_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QDir>
#include <QDirIterator>
#include <QDebug>

class QmlDir : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString dir READ dir WRITE setDir NOTIFY dirChanged)
    Q_PROPERTY(QString errorString READ errorString NOTIFY errorStringChanged)
    Q_PROPERTY(QString currentPath READ currentPath NOTIFY currentPathChanged)
    //BUG not in QDir for some reason or another.
//    Q_PROPERTY(QString owner READ owner  NOTIFY ownerChanged)
    Q_PROPERTY(int count READ count NOTIFY countChanged)

    Q_PROPERTY(QStringList files READ files NOTIFY filesChanged)

    Q_PROPERTY(bool scanRecursively READ scanRecursively WRITE setScanRecursively NOTIFY scanRecursivelyChanged)


    Q_PROPERTY(Flags flags READ flags WRITE setFlags NOTIFY flagsChanged)
    Q_ENUMS(Flags)

    Q_PROPERTY(IteratorFlag iteratorFlag READ iteratorFlag WRITE setIteratorFlag NOTIFY iteratorFlagChanged)
    Q_ENUMS(IteratorFlag)

public:
    explicit QmlDir(QObject *parent = 0);


    enum Flags{
        Dirs,
        AllDirs,
        Files,
        Drives,
        NoSymLinks,
        NoDotAndDotDot,
        NoDot,
        NoDotDot,
        AllEntries,
        Readable,
        Writable,
        Executable,
        Modified,
        Hidden,
        System,
        CaseSensitive
    };

    Flags flags()const;
    void setFlags(const Flags &flags);




    enum IteratorFlag {
        NoIteratorFlags,
        Subdirectories,
        FollowSymlinks
    };


    IteratorFlag iteratorFlag()const;
    void setIteratorFlag(const IteratorFlag &iteratorFlag);

    // getters and setters

    bool scanRecursively()const;
    void setScanRecursively(const bool &scanRecursively);

    QStringList files()const;
    Q_INVOKABLE void setFiles(const QString thePath);

    int count()const;
    void setCount(const int c);

    QString dir()const;
    void setDir(const QString &dir);


    QString errorString()const;

    QString currentPath()const;
    void setCurrentPath(const QString currentPath);


    // BUG function for owner is not working in QDir :/
    //    QString owner()const;
    //    void setOwner(const QString path);

    // methods
    Q_INVOKABLE bool rename(const QString oldDir,const QString newDir);
    Q_INVOKABLE bool createPath();
    Q_INVOKABLE bool createDir();
    Q_INVOKABLE bool dirExists();


    Q_INVOKABLE bool cdUp();


//    Q_INVOKABLE void scanDir();
    Q_INVOKABLE bool removeRecursively();
    Q_INVOKABLE bool remove();
    Q_INVOKABLE bool isReadable();

    Q_INVOKABLE void copyFile(const QString oldFile ,const QString newFile);

signals:
    void dirChanged();
    void setQDir(QString);
    void error(QString);
    void errorStringChanged();
    void currentPathChanged();
//    void ownerChanged();
    void scanRecursivelyChanged();
    void iteratorFlagChanged();
    void filesRecursivelyChanged();
    void filesChanged();
    void newDirNameChanged();
    void countChanged();
    void flagsChanged();

protected slots:
    void handdelSetQDir(const QString incoming);
    void hanndelError(const QString err);

private:
    QDir d;
//    QDirIterator dIt;

    IteratorFlag m_itFlag;
    QDirIterator::IteratorFlag mm_itFlag;

    Flags m_flags;
    QDir::Filter mm_flags;

    QString m_dir;
    QString m_newDir;
    QString m_errorString;

    bool m_dirExists;
    QString m_currentPath;
//    QString m_owner;

    int m_countInDir;
    int m_counteRecursively;

    QStringList m_filesInDir;
    QStringList m_fileseRecursively;

    bool m_scanRecursively;
};

#endif // QMLDIR_H

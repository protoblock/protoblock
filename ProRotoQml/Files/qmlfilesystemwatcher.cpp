#include "qmlfilesystemwatcher.h"

QmlFileSystemWatcher::QmlFileSystemWatcher(QObject *parent) : QObject(parent)
{

            connect(&d,SIGNAL(directoryChanged(QString)),
                    this,SLOT(handdelDirChanged(QString)));

            connect(&d,SIGNAL(fileChanged(QString)),
                    this, SLOT(handdelFileChanged(QString)));

}

//QmlFileSystemWatcher::~QmlFileSystemWatcher()
//{
//    delete d;
//}

QStringList QmlFileSystemWatcher::directories() const
{
    return d.directories();
}

QStringList QmlFileSystemWatcher::files() const
{
    return d.files();
}

bool QmlFileSystemWatcher::addPath(const QString &path)
{
       if ( d.addPath(path) )
       {
            qDebug() << "added a "<< path <<" path to the filesystem watcher";
            return true;
       }
       else
       {
           return false;
       }
}

void QmlFileSystemWatcher::addPaths(const QStringList &paths)
{
    d.addPaths(paths);
}

bool QmlFileSystemWatcher::removePath(const QString &path)
{
    if ( d.removePath(path) ){
        return true;
    }
    else
    {
        return false;
    }
}

void QmlFileSystemWatcher::removePaths(const QStringList &paths)
{
    d.removePaths(paths);
}

void QmlFileSystemWatcher::handdelDirChanged(const QString &path)
{
    qDebug() << "Dir  " << path << " Changed";
}

void QmlFileSystemWatcher::handdelFileChanged(const QString &path)
{
    qDebug() << "File  " << path << " Changed" ;
}

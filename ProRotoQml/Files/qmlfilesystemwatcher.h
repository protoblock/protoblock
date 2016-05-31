#ifndef QMLFILESYSTEMWATCHER_H
#define QMLFILESYSTEMWATCHER_H

#include <QObject>
#include <QStringList>
#include <QFileSystemWatcher>
#include <QDebug>
class QmlFileSystemWatcher : public QObject
{
    Q_OBJECT
public:
    explicit QmlFileSystemWatcher(QObject *parent = 0);
//    ~QmlFileSystemWatcher();

    Q_INVOKABLE QStringList directories() const ;
    Q_INVOKABLE QStringList files() const;

    //invoakbles
    Q_INVOKABLE bool addPath(const QString &path);
    Q_INVOKABLE void addPaths(const QStringList &paths);

    Q_INVOKABLE bool removePath(const QString &path);
    Q_INVOKABLE void removePaths(const QStringList &paths);

protected slots:
    void handdelDirChanged(const QString &path);
    void handdelFileChanged(const QString &path);

private:
    QFileSystemWatcher d;
};

#endif // QMLFILESYSTEMWATCHER_H

#ifndef QMLSQLCREATEDATABASE_H
#define QMLSQLCREATEDATABASE_H

#include <QObject>
#include <QString>
#include <QFile>
#include <QDir>
#include <QStandardPaths>
#include <QCryptographicHash>
#include <QTime>

#include <QDebug>
#include <QSqlDatabase>

class QmlSqlCreateDatabase : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString filePath READ filePath WRITE setFilePath NOTIFY filePathChanged)
    Q_PROPERTY(QString fileName READ fileName WRITE setFileName NOTIFY fileNameChanged)
    Q_PROPERTY(bool useMd5 READ useMd5 WRITE setUseMd5 NOTIFY useMd5Changed)
    Q_PROPERTY(QString errorString READ errorString NOTIFY errorStringChanged)
    Q_PROPERTY(QString databaseName READ databaseName WRITE setDatabaseName NOTIFY databaseNameChanged)
    Q_PROPERTY(QString lastCreatedDatabaseFile READ lastCreatedDatabaseFile  NOTIFY lastCreatedDatabaseFileChanged)

public:
    explicit QmlSqlCreateDatabase(QObject *parent = 0);

    QString filePath() const;
    void setFilePath(const QString &filePath);

    QString fileName()const ;
    void setFileName(const QString &fileName);

    bool useMd5()const;
    void setUseMd5(const bool &useMd5);

    QString databaseName()const;
    void setDatabaseName(const QString &databaseName);

    QString errorString();

    QString lastCreatedDatabaseFile()const;
    void setLastCreatedDatabaseFile(const QString lastCreatedDatabaseFile);


    Q_INVOKABLE void exec();
    QString generateMd5Sum(const QString &databaseName);
    QString getRandomString();


signals:
    void filePathChanged();
    void fileNameChanged();
    void useMd5Changed();
    void errorStringChanged();
    void databaseNameChanged();
    void lastCreatedDatabaseFileChanged();
    void error(QString);

    void created();

public slots:
    void handelError(const QString er);

private:
    QString m_filePath;
    QString m_fileName;
    bool m_useMd5;
    QString m_databaseName;
    QString m_errorString;
    QString m_lastCreatedDatabaseFile;
};

#endif // QMLSQLCREATEDATABASE_H

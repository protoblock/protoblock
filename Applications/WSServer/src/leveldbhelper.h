#ifndef LEVELDBHELPER_H
#define LEVELDBHELPER_H

#include <QObject>
#include <QDebug>


#include <leveldb/iterator.h>
#include <leveldb/db.h>
#include <leveldb/options.h>

#include <iostream>

#include "utils.h"
#include "generated/ProtoData.pb.h"


class LevelDBHelper : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString connectionName READ connectionName WRITE setConnectionName NOTIFY connectionNameChanged)
    Q_PROPERTY(QString databaseFile READ databaseFile WRITE setDatabaseFile NOTIFY databaseFileChanged)
    Q_PROPERTY(bool open READ open WRITE setOpen NOTIFY openChanged)
    Q_PROPERTY(QString lastError READ lastError NOTIFY lastErrorChanged)



public:
    explicit LevelDBHelper(QObject *parent = 0);

    QString connectionName()const;
    void setConnectionName(const QString &connectionName);

    QString databaseFile()const;
    void setDatabaseFile(const QString &databaseFile);

    bool open()const;
    void setOpen(const bool &open);

    QString lastError()const;
    void setLastError(const QString &lastError);

    Q_INVOKABLE void addDatabase();

signals:
    void connectionNameChanged();
    void databaseFileChanged();
    void openChanged();
    void lastErrorChanged();

    void idel();
    void connecting();
    void connected();


    void error(QString);
protected slots:
    void handelError(const QString err);

private :
        QString m_connectionName;
        QString m_databaseName;
        bool m_open;

        QString m_lastError;



};

#endif // LEVELDBHELPER_H

#ifndef QQMLSQLQUERY_H
#define QQMLSQLQUERY_H

#include <QObject>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlDatabase>
#include <QSqlError>
#include <QStringList>
#include <QDebug>
#include <QString>


class QQmlSqlQuery : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString queryString READ queryString WRITE setQueryString NOTIFY queryStringChanged)
    Q_PROPERTY(QString lastQuery READ lastQuery WRITE setLastQuery NOTIFY lastQueryChanged)
    Q_PROPERTY(QString lastQueryOutPut READ lastQueryOutPut WRITE setLastQueryOutPut NOTIFY lastQueryOutPutChanged)
    Q_PROPERTY(QString connectionName READ connectionName WRITE setConnectionName NOTIFY connectionNameChanged)
    Q_PROPERTY(QString  errorString READ errorString WRITE setErrorString NOTIFY errorStringChanged)
    Q_PROPERTY(int rowsAffected READ rowsAffected WRITE setRowsAffected NOTIFY rowsAffectedChanged)

public:
    explicit QQmlSqlQuery(QObject *parent = 0);

    int rowsAffected()const;
    void setRowsAffected(const int &rowsAffected);

    QString queryString() const;
    void setQueryString(const QString &queryString);

    QString lastQuery()const;
    void setLastQuery(const QString &lastQuery);

    QString lastQueryOutPut()const;
    void setLastQueryOutPut(const QString &lastQueryOutPut);

    QString connectionName()const;
    void setConnectionName(const QString &connectionName);

    QString errorString()const;
    void setErrorString(const QString &errorString);

    Q_INVOKABLE void exec();
    Q_INVOKABLE void execWithQuery(const QString connectionName,
                                   const QString &query);
signals:
    void rowsAffectedChanged();
    void queryStringChanged();
    void lastQueryChanged();
    void lastQueryOutPutChanged();
    void connectionNameChanged();
    void errorStringChanged();

    void error(QString);
    void done();
public slots:
    void handelError(const QString &err);

private:
    int m_rowsAffected;
    QString m_queryString;
    QString   m_lastQuery;
    QString m_lastQueryOutPut;
    QString  m_connectionName;
    QString  m_errorString;

};

#endif // QQMLSQLQUERY_H

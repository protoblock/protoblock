#ifndef SQLQUERYMODEL_H
#define SQLQUERYMODEL_H
#include <QSqlQueryModel>
#include <QSqlDatabase>
#include <QSqlRecord>
#include <QSqlField>

#include <QDebug>
#include <QVariant>
#include <QStringList>


class QmlSqlQueryModel : public QSqlQueryModel
{
    Q_OBJECT
    Q_PROPERTY(QString connectionName READ connectionName WRITE setConnectionName NOTIFY connectionNameChanged)
    Q_PROPERTY(QString queryString READ queryString WRITE setQueryString NOTIFY queryStringChanged)
    Q_PROPERTY(QStringList rolesList READ rolesList  NOTIFY rolesListChanged)


public:

    explicit QmlSqlQueryModel(QObject *parent = 0);

    QString connectionName()const;
    void setConnectionName(const QString &connectionName);

    QString queryString()const;
    void setQueryString(const QString &queryString);

    QStringList rolesList() const;

     Q_INVOKABLE void exec();
     QVariant  data(const QModelIndex &index, int role)const;
     QHash<int, QByteArray>roleNames() const;
signals:
     void compleated();
    void connectionNameChanged();
    void queryStringChanged();
    void rolesListChanged();

private:
        QString m_connectionName;
        QString m_queryString;
        QStringList m_roleList;
};
#endif // QSQLQUERYMODEL_H

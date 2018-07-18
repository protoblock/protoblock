#include "qmlsqlquerymodel.h"

QmlSqlQueryModel::QmlSqlQueryModel(QObject *parent) :
    QSqlQueryModel(parent)
{
}

QString QmlSqlQueryModel::connectionName() const
{
    return m_connectionName;
}

void QmlSqlQueryModel::setConnectionName(const QString &connectionName)
{
    if ( m_connectionName == connectionName )
        return;
    m_connectionName = connectionName ;
    emit connectionNameChanged();
}

QString QmlSqlQueryModel::queryString() const
{
    return  m_queryString;
}

void QmlSqlQueryModel::setQueryString(const QString &queryString)
{
    if ( m_queryString == queryString )
        return;
    m_queryString = queryString ;
    emit queryStringChanged();
}

QStringList QmlSqlQueryModel::rolesList() const
{
    return m_roleList;
}

void QmlSqlQueryModel::exec()
{
    QSqlDatabase db = QSqlDatabase::database(m_connectionName);
    QSqlQueryModel::setQuery(m_queryString, db);
    roleNames();
}

QHash<int, QByteArray>QmlSqlQueryModel::roleNames() const
{
    QHash<int,QByteArray> hash;
    for( int i = 0; i < record().count(); i++) {
        hash.insert(Qt::UserRole + i + 1, QByteArray(record().fieldName(i).toLatin1()));
    }
    return hash;

}
// set up the model
QVariant QmlSqlQueryModel::data(const QModelIndex &index, int role)const
{
    QVariant value = QSqlQueryModel::data(index, role);
    if(role < Qt::UserRole)
    {
        value = QSqlQueryModel::data(index, role);
    }
    else
    {
        int columnIdx = role - Qt::UserRole - 1;
        QModelIndex modelIndex = this->index(index.row(), columnIdx);
        value = QSqlQueryModel::data(modelIndex, Qt::DisplayRole);
    }
    return value;
}

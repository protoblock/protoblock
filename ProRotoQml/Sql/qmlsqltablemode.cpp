#include "qmlsqltablemode.h"

QmlSqlTableModel::QmlSqlTableModel(QObject *parent) :
    QSqlTableModel(parent),
    m_sortType(Ascending),
    m_editStrategy(ManualSubmit),
    mm_editStrategy(QSqlTableModel::OnManualSubmit)
{

}

QmlSqlTableModel::EditStrategy QmlSqlTableModel::editStrategy() const
{
    return m_editStrategy;
}

void QmlSqlTableModel::setEditStrategy(const QmlSqlTableModel::EditStrategy &editStrategy)
{
    if(m_editStrategy == editStrategy){
        return;
    }else{
        switch (editStrategy) {
        case FieldChange:
            m_editStrategy = FieldChange;
            mm_editStrategy = QSqlRelationalTableModel::OnFieldChange ;
            break;
        case RowChange:
            m_editStrategy = RowChange;
            mm_editStrategy =  QSqlRelationalTableModel::OnRowChange ;
            break;
        case ManualSubmit:
            m_editStrategy = ManualSubmit;
            mm_editStrategy = QSqlRelationalTableModel::OnManualSubmit ;
            break;
        }
        emit editStrategyChanged();
    }
}

QmlSqlTableModel::SortType QmlSqlTableModel::sortType() const
{
    return m_sortType;
}

void QmlSqlTableModel::setSortType(const QmlSqlTableModel::SortType sortType)
{
    if(m_sortType == sortType){
        return;
    }else{
        switch (sortType) {
        case Ascending:
            m_sortType  = Ascending;
            break;
        case Descending:
            m_sortType = Descending;
            break;
        }
        emit sortTypeChanged();
    }
}

QString QmlSqlTableModel::connectionName() const
{
    return m_connectionName;
}

void QmlSqlTableModel::setConnectionName(const QString &connectionName)
{
    if ( m_connectionName == connectionName )
        return;
    m_connectionName = connectionName ;
    emit connectionNameChanged();
}

QString QmlSqlTableModel::table() const
{
    return m_tableName;
}

void QmlSqlTableModel::setT(const QString &table)
{
    if ( m_tableName == table )
        return;
    m_tableName = table ;
    emit tableNameChanged();
}

void QmlSqlTableModel::init()
{
    m_roleList.clear();
    QSqlDatabase db = QSqlDatabase::database(m_connectionName);
     d = new QSqlTableModel(this,db);

    qDebug() << "COnnection point "<< d->database();

    d->setTable(m_tableName);
//    setEditStrategy(mm_editStrategy);
    d->select();

    for( int i = 0; i < d->record().count(); i++) {
        m_roleList <<  d->record().fieldName(i).toLatin1();
    }
    qDebug () << " An Error ?  " << d->lastError().text();
    QSqlTableModel::roleNames();
}

QVariant QmlSqlTableModel::data(const QModelIndex &index, int role) const
{ 
    QVariant value = d->data(index, role);
    if(role < Qt::UserRole)
    {
        value = d->data(index, role);
    }
    else
    {
        int columnIdx = role - Qt::UserRole - 1;
        QModelIndex modelIndex = d->index(index.row(), columnIdx);
        value = d->data(modelIndex, Qt::EditRole);
    }
    return value;
}



QHash<int, QByteArray>QmlSqlTableModel::roleNames() const
{
    qDebug() << "Inserting roles now " << record().fieldName(1).toLatin1();
    QHash<int,QByteArray> hash;
    for( int i = 0; i < record().count(); i++) {
        hash.insert(Qt::UserRole + i + 1, QByteArray(record().fieldName(i).toLatin1()));
    }
    return hash;
}

QStringList QmlSqlTableModel::rolesList() const
{
    return  m_roleList;
}

int QmlSqlTableModel::count() const
{
     return columnCount() + rowCount();
}


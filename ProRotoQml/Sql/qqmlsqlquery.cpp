#include "qqmlsqlquery.h"

QQmlSqlQuery::QQmlSqlQuery(QObject *parent) : QObject(parent)
{
    connect(this,SIGNAL(error(QString)),this,SLOT(handelError(QString)));
}

int QQmlSqlQuery::rowsAffected() const
{
    return m_rowsAffected;
}

void QQmlSqlQuery::setRowsAffected(const int &rowsAffected)
{
    if ( m_rowsAffected == rowsAffected )
        return;
    m_rowsAffected = rowsAffected ;
    emit rowsAffectedChanged();
}

QString QQmlSqlQuery::queryString() const
{
    return m_queryString;
}

void QQmlSqlQuery::setQueryString(const QString &queryString)
{
    if ( m_queryString == queryString )
        return;
    m_queryString = queryString ;
    emit queryStringChanged();
}

QString QQmlSqlQuery::lastQuery() const
{
    return m_lastQuery;
}

void QQmlSqlQuery::setLastQuery(const QString &lastQuery)
{
    if ( m_lastQuery == lastQuery )
        return;
    m_lastQuery = lastQuery ;
    emit lastQueryChanged();
}

QString QQmlSqlQuery::lastQueryOutPut() const
{
    return  m_lastQueryOutPut;
}

void QQmlSqlQuery::setLastQueryOutPut(const QString &lastQueryOutPut)
{
    if ( m_lastQueryOutPut == lastQueryOutPut )
        return;
    m_lastQueryOutPut = lastQueryOutPut ;
    emit lastQueryOutPutChanged();
}

QString QQmlSqlQuery::connectionName() const
{
    return m_connectionName;
}

void QQmlSqlQuery::setConnectionName(const QString &connectionName)
{
    if ( m_connectionName == connectionName )
        return;
    m_connectionName =  connectionName;
    emit connectionNameChanged();
}

QString QQmlSqlQuery::errorString() const
{
    return  m_errorString;
}

void QQmlSqlQuery::setErrorString(const QString &errorString)
{
    if ( m_errorString == errorString )
        return;
    m_errorString = errorString ;
    emit errorStringChanged();
}

void QQmlSqlQuery::exec()
{
    QSqlDatabase db = QSqlDatabase::database(m_connectionName);
    QSqlQuery db_query(db);

    db_query.prepare(m_queryString);

    if (!db_query.exec())
    {
        QString er = QString("could not run query of %1 Reson:  %2").arg(m_queryString).arg( db_query.lastError().text() );
        error(er);
    }
    else
    {
        if (db_query.lastError().type() != QSqlError::NoError)
        {
            error(db_query.lastError().text());
        }
       else if (db_query.isSelect())
        {
            m_lastQueryOutPut.clear();
            QSqlRecord rec = db_query.record();
            const int rowCount = rec.count() - 1 ;

            while (db_query.next()){
                for (int i = 0;i<= rowCount ; i++)
                {
                    qDebug() << db_query.value(i).toString();
                    m_lastQueryOutPut.append(db_query.value(i).toString() + " "  );
                }
            }
            setLastQueryOutPut(m_lastQueryOutPut);
            emit done();
        }
        //INSERT UPDATE ect
        else
        {
            setRowsAffected(db_query.numRowsAffected());
            emit done();
        }
    }
}

void QQmlSqlQuery::execWithQuery(const QString connectionName, const QString &query)
{

    QSqlDatabase db = QSqlDatabase::database(connectionName);
    QSqlQuery db_query(db);
    db_query.prepare(query);

    if (!db_query.exec())
    {
        QString er = QString("could not run query of %1 Reson:  %2").arg(m_queryString).arg( db_query.lastError().text() );
        error(er);
    }
    else
    {
        if (db_query.lastError().type() != QSqlError::NoError)
        {
            error( db_query.lastError().text() );
        }
        else if (db_query.isSelect()){
            m_lastQueryOutPut.clear();
            QSqlRecord rec = db_query.record();
            const int rowCount = rec.count() - 1 ;
            while (db_query.next()){
                for (int i = 0;i<= rowCount ; i++)
                {
                    m_lastQueryOutPut.append(db_query.value(i).toString() + " "  );
                }
            }
            setLastQueryOutPut(m_lastQueryOutPut);
            emit done();
        }
        else
        {
            setRowsAffected(db_query.numRowsAffected());
            emit done();
        }
    }
}
void QQmlSqlQuery::handelError(const QString &err)
{

    setErrorString(err);
}

#include "qmlsqldatabase.h"

QmlSqlDatabase::QmlSqlDatabase(QObject *parent)
    : QObject(parent)

{
        setDatabaseDriverList();
//        This is to keep up with the track of how
        QObject::connect( this,SIGNAL(error(QString)), this,SLOT(handelError(QString)));
        QObject::connect( this,SIGNAL(connectionOpened(QSqlDatabase,QString)), this,SLOT(handelOpened(QSqlDatabase,QString)) );
        QObject::connect( this,SIGNAL(closeRequested(QString)), this,SLOT(handelCloseRequested(QString)) );
}

QmlSqlDatabase::DataBaseDriver QmlSqlDatabase::databaseDriver() const
{
    return m_databaseDriver;
}

void QmlSqlDatabase::setDatabaseDriver(const QmlSqlDatabase::DataBaseDriver &databaseDriver)
{
    if(m_databaseDriver == databaseDriver){
        return;
    }
    else
    {
        switch (databaseDriver) {
        case PostGre:
            m_databaseDriver = PostGre;
            m_databaseDriverString =  "QPSQL";
            break;
        case MySql:
            m_databaseDriver = MySql;
            m_databaseDriverString =  "QMYSQL";
            break;
        case OCI:
            m_databaseDriver = OCI;
            m_databaseDriverString =  "QOCI";
            break;
        case ODBC:
            m_databaseDriver = ODBC;
            m_databaseDriverString =  "QODBC";
            break;
        case DB2:
            m_databaseDriver = DB2;
            m_databaseDriverString =  "QDB2";
            break;
        case TDS:
            m_databaseDriver = TDS;
            m_databaseDriverString =  "QTDS";
            break;
        case SQLight:
            m_databaseDriver =SQLight ;
            m_databaseDriverString =  "QSQLITE";

            break;
        case SQLight2:
            m_databaseDriver = SQLight2;
            m_databaseDriverString =  "QSQLITE2";
            break;
        case IBase:
            m_databaseDriver = IBase;
            m_databaseDriverString =  "QIBASE";
            break;
        }
        emit databaseDriverChanged();
    }
}

QStringList QmlSqlDatabase::databaseDriverList() const
{
    return m_databaseDriverList;
}

void QmlSqlDatabase::setDatabaseDriverList()
{
    m_databaseDriverList.clear();
    QList<QString> li;
    li <<  "PostGre"<< "MySql"<< "OCI"<< "ODBC"<< "DB2"<< "TDS"<< "SQLight"<< "SQLight2"<< "IBase";
    m_databaseDriverList << li;
}

QString QmlSqlDatabase::errorString() const
{
    return m_errorString;
}

QString QmlSqlDatabase::source() const
{
    return m_source;
}
void QmlSqlDatabase::setSource(const QString &source)
{
    if ( m_source == source )
        return;
    m_source = source ;
    emit sourceChanged();
}


QString QmlSqlDatabase::databaseName() const
{
    return m_dbName;
}
void QmlSqlDatabase::setdatabaseName(const QString &databaseName)
{
    if ( m_dbName == databaseName )
        return;
    m_dbName = databaseName ;
    emit databaseNameChanged() ;
}

QString QmlSqlDatabase::user() const
{
    return m_user;
}
void QmlSqlDatabase::setUser(const QString &user)
{
    if ( m_user == user )
        return;
    m_user = user ;
    emit userChanged();
}


QString QmlSqlDatabase::password() const
{
    return m_password;
}
void QmlSqlDatabase::setPassword(const QString &password)
{
    if ( m_password == password )
        return;
    m_password = password ;
    emit passwordChanged();
}



int QmlSqlDatabase::port() const
{
    return m_port;
}
void QmlSqlDatabase::setPort(int port)
{
    if ( m_port == port )
        return;
    m_port = port ;
    emit portChanged();
}

//bool QmlSqlDatabase::connectAuto()const
//{
//    return m_ConnectAuto;
//}

//void QmlSqlDatabase::setConnectionAuto(bool &connectAuto)
//{
//    if ( m_connectAuto == connectAuto )
//        return;
//    m_connectAuto = connectAuto ;
//    emit connectAutoChanged();
//}


QString QmlSqlDatabase::connectionName() const
{
    return m_connectionName;
}

void QmlSqlDatabase::setConnectionName(const QString &connectionName)
{
    if ( m_connectionName == connectionName )
        return;
    m_connectionName = connectionName ;
    emit connectionNameChanged();
}


void QmlSqlDatabase::addDataBase()
{
    // check the file to make sure that it is readable and writiable
//    QFileInfo f (m_dbName);
//    qDebug() <<"Here is the file info ?  " << f.absoluteFilePath ();
//    if (m_databaseDriver == SQLight){
//        QFile dfile(f.absoluteFilePath ());
//        if (dfile.exists())
//        {
//            dfile.copy(f.absoluteFilePath ());
//            QFile::setPermissions( f.absoluteFilePath ().append (), QFile::WriteOwner | QFile::ReadOwner);
//        }else{
//            error ("Can not Find Sqlight File to change Permissions on");
//        }
//    }
    qDebug() << " Check database " << m_databaseDriverString;
    QSqlDatabase db = QSqlDatabase::addDatabase(m_databaseDriverString,m_connectionName);
    //FIXME if open create new name
    db.close();
    db.setHostName(m_source);
    db.setDatabaseName(m_dbName);
    db.setUserName(m_user);
    db.setPassword(m_password);
    db.setPort(m_port);
    if (!db.open())
    {
//        QString er =  QString("%1").arg( db.lastError().text() ) ;
        QString j = QString("%1").arg(db.lastError().text());
        qDebug() << "   LAST ERROR STRING "<<j;
        error(m_connectionName);
        closeRequested(m_connectionName);
    }
    else
    {


        connectionOpened(db ,m_connectionName);

    }
}

//void QmlSqlDatabase::removeDatabase(const QString &connectionName)
//{
//    qDebug() << "release  database aka clese ?
//    //db.removeDatabase(connectionName);
//}

QStringList QmlSqlDatabase::connectionNames()
{
    return db.connectionNames();
}



bool QmlSqlDatabase::running()
{
     return m_running;
}

void QmlSqlDatabase::handelError(const QString err)
{
    if (m_errorString == err)
        return;
    m_errorString = err;
    emit errorStringChanged();
}

void QmlSqlDatabase::handelOpened(QSqlDatabase database, const QString connectionName)
{
    qDebug() << connectionName  << " Is Open " << database.connectionName();
    // remove the dataase from here.  But a better way to play with connection names
    emit opened();
}

void QmlSqlDatabase::handelCloseRequested(const QString connectionName)
{
    qDebug() << "connection name has been requested to close " << connectionName ;
}



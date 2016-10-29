#include "leveldbhelper.h"

LevelDBHelper::LevelDBHelper(QObject *parent) : QObject(parent)
{
    connect(this,SIGNAL(error(QString)),this,SLOT(handelError(QString)));
}

QString LevelDBHelper::connectionName() const
{
    return m_connectionName;
}

void LevelDBHelper::setConnectionName(const QString &connectionName)
{
    if ( m_connectionName == connectionName )
        return;
    m_connectionName = connectionName ;
    emit connectionNameChanged();
}

QString LevelDBHelper::databaseFile() const
{

    return m_databaseName;
}

void LevelDBHelper::setDatabaseFile(const QString &databaseFile)
{
    if ( m_databaseName == databaseFile )
        return;
    m_databaseName = databaseFile ;
    emit databaseFileChanged();
}

bool LevelDBHelper::open() const
{
    return m_open;
}

void LevelDBHelper::setOpen(const bool &open)
{
    if ( m_open == open )
        return;
    m_open = open ;
    emit openChanged();
}

QString LevelDBHelper::lastError() const
{
    return m_lastError;
}

void LevelDBHelper::setLastError(const QString &lastError)
{
    if ( m_lastError == lastError )
        return;
    m_lastError = lastError ;
    emit lastErrorChanged();
}

void LevelDBHelper::addDatabase()
{

    fantasybit::Int32Comparator *cmp = new fantasybit::Int32Comparator();
    leveldb::Options optionsInt;
    optionsInt.create_if_missing = true;
    optionsInt.error_if_exists = false;
    optionsInt.comparator = cmp;

    leveldb::Status status;
    leveldb::DB *db;
    status = leveldb::DB::Open(optionsInt, "/home/joseph/Work/SatoshiFantasy/Git/ProRoto2016/Applications/tradingfootball-qml/meta", &db);
    emit connecting();
    if (!status.ok()) {
        error ("!!! error opening db ");
        exit(1);
    }
    else
    {
        emit connected();
    }

//    QStringList hj;

    leveldb::Iterator* it = db->NewIterator(leveldb::ReadOptions());
    for (  it->SeekToFirst();  it->Valid();  it->Next() )
    {
        if( it->Valid() )
        {
            std::cout << it->key().ToString().data()  << endl ; // << ": "  << it->value().ToString().data();
        }
        else
        {
            std::cout << it->key().data()  << endl ;
            error("Not a vaild something");
        }
    }

 // Check for any errors found during the scan
    if ( it->status().ok() == false )
    {
        qDebug() << "Error in ITerator for db ";
    }
    else
    {
        qDebug() << "Woot woot";
//        db->Get(leveldb::Options,"")
    }

    delete it;
    delete db;

}


void LevelDBHelper::handelError(const QString err)
{
    setLastError(err);
}

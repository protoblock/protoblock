#include "qmlsqlcreatedatabase.h"

QmlSqlCreateDatabase::QmlSqlCreateDatabase(QObject *parent) :
    QObject(parent),
    m_databaseName("NULL")
{
    connect(this,SIGNAL(error(QString)), this,SLOT(handelError(QString)));
}

QString QmlSqlCreateDatabase::filePath() const
{
    return m_filePath;
}

void QmlSqlCreateDatabase::setFilePath(const QString &filePath)
{
    if ( m_filePath == filePath )
        return;
    m_filePath = filePath ;
    emit filePathChanged();
}

QString QmlSqlCreateDatabase::fileName() const
{
    return m_fileName;
}

void QmlSqlCreateDatabase::setFileName(const QString &fileName)
{
    if ( m_fileName == fileName )
        return;
    m_fileName = fileName ;
    emit fileNameChanged();
}

bool QmlSqlCreateDatabase::useMd5() const
{
    return m_useMd5;
}

void QmlSqlCreateDatabase::setUseMd5(const bool &useMd5)
{
    if ( m_useMd5 == useMd5 )
        return;
    m_useMd5 = useMd5 ;
    emit useMd5Changed();
}

QString QmlSqlCreateDatabase::databaseName() const
{
    return m_databaseName;
}

void QmlSqlCreateDatabase::setDatabaseName(const QString &databaseName)
{
    if ( m_databaseName == databaseName )
        return;
    m_databaseName = databaseName ;
    emit databaseNameChanged();
}

QString QmlSqlCreateDatabase::errorString()
{
    return m_errorString;
}

QString QmlSqlCreateDatabase::lastCreatedDatabaseFile() const
{
    return m_lastCreatedDatabaseFile;
}

void QmlSqlCreateDatabase::setLastCreatedDatabaseFile(const QString lastCreatedDatabaseFile)
{
    if ( m_lastCreatedDatabaseFile == lastCreatedDatabaseFile )
        return;
    m_lastCreatedDatabaseFile = lastCreatedDatabaseFile ;
    emit lastCreatedDatabaseFileChanged();
}

void QmlSqlCreateDatabase::exec()
{
    QString dataDir = QStandardPaths::standardLocations(QStandardPaths::DataLocation).first() ;
    qDebug() << "Making the database ";
    QString finalName;
    //just in case
    finalName.clear();

    if(m_fileName.length() < 1 ){
        error("You forgot to set your databases Name");
        return;
    }

    // Deal with the path length
    if ( m_filePath.length() < 1 )
    {
        if ( m_useMd5 == true ){
            QString fName =  generateMd5Sum(m_databaseName);
            finalName = QString("%1/%2%3").arg(dataDir).arg(fName).arg(".sqlight");
            QFile file(finalName);
            if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
            {
                QString err = QString("Could not open the file %1 for writing to ").arg(finalName);
                error(err);
            }
            //ok we got the file open now lets make sure that we make into a db
            else
            {
                file.close();
                QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
                db.setDatabaseName(m_databaseName);

                if(db.open()){
                    qDebug()<< "Sweet we created the database " << finalName;
                    setLastCreatedDatabaseFile(finalName);
                    created();
                    db.close();
                }
                else
                {
                    error("Error in opening the init database");
                }
            }
        }
        else
            // NO MD5Sum no Path
        {
            finalName = QString("%1/%2%3").arg(dataDir).arg(m_fileName).arg(".sqlight");
            QFile file(finalName);
            if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
            {
                QString err = QString("Could not open the file %1 for writing to ").arg(finalName);
                error(err);
            }
            //ok we got the file open now lets make sure that we make into a db
            else
            {
                file.close();
                QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
                db.setDatabaseName(m_databaseName);
                if(db.open()){
                    qDebug()<< "Sweet we created the database " << finalName;
                    setLastCreatedDatabaseFile(finalName);
                    created();
                    db.close();
                }
                else
                {
                    error("Error in opening the init database");
                }
            }
        }
    }// END NO PATH NAME SET
    /////////////////////////////////////////



    // Ok so the file Path is set now deal with that


    else {
        if ( m_useMd5 == true ){
            QString fName =  generateMd5Sum(m_databaseName);
            finalName = QString("%1/%2%3").arg(m_filePath).arg(fName).arg(".sqlight");
            QFile file(finalName);
            if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
            {
                QString err = QString("Could not open the file %1 for writing to ").arg(finalName);
                error(err);
            }
            //ok we got the file open now lets make sure that we make into a db
            else
            {
                file.close();
                QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
                db.setDatabaseName(m_databaseName);

                if(db.open()){
                    qDebug()<< "Sweet we created the database " << finalName;
                    setLastCreatedDatabaseFile(finalName);
                    created();
                    db.close();
                }
                else
                {
                    error("Error in opening the init database");
                }
            }
        }
        else
            //not using md5Sum
        {
            finalName = QString("%1/%2%3").arg(m_filePath).arg(m_databaseName).arg(".sqlight");
            QFile file(finalName);
            if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
            {
                QString err = QString("Could not open the file %1 for writing to ").arg(finalName);
                error(err);
            }
            //ok we got the file open now lets make sure that we make into a db
            else
            {
                file.close();
                QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
                db.setDatabaseName(m_databaseName);

                if(db.open()){
                    qDebug()<< "Sweet we created the database " << finalName;
                    setLastCreatedDatabaseFile(finalName);
                    created();
                    db.close();
                }
                else
                {
                    error("Error in opening the init database");
                }
            }
        }
    }
}





QString QmlSqlCreateDatabase::generateMd5Sum(const QString &databaseName)
{
    QCryptographicHash md5(QCryptographicHash::Md5);
    // make the md5sum
    md5.addData(databaseName.toUtf8());
    // now that we have the name and the user did not enter in a path we can create a db
    QString fName(QLatin1String(md5.result().toHex()));
    return fName;
}



QString QmlSqlCreateDatabase::getRandomString()
{
    QString possibleCharacters = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
       int randomStringLength = 12;

       qsrand(QTime::currentTime().msec());

       QString rString;
       for(int i=0; i<randomStringLength; ++i)
       {

           int index = qrand() % possibleCharacters.length();
           QChar nextChar = possibleCharacters.at(index);
           rString.append(nextChar);
       }
       return rString;
}

void QmlSqlCreateDatabase::handelError(const QString er)
{
    qDebug() << er ;
    if ( m_errorString == er )
        return;
    m_errorString = er ;
    emit errorStringChanged();
}

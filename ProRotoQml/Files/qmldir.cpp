#include "qmldir.h"

QmlDir::QmlDir(QObject *parent)
    :QObject(parent),
    m_flags(AllDirs),
    mm_flags(QDir::AllDirs),
    m_scanRecursively(false)
{
    connect(this,SIGNAL(setQDir(QString)),this,SLOT(handdelSetQDir(QString)));
    connect(this,SIGNAL(error(QString)),this,SLOT(hanndelError(QString)));

}

QmlDir::Flags QmlDir::flags() const
{
    return m_flags;
}

void QmlDir::setFlags(const QmlDir::Flags &flags)
{
    if(m_flags == flags ){
        return;
    }
    else
    {
        switch (flags) {
        case Dirs:
            m_flags = Dirs;
            mm_flags = QDir::Dirs ;
            break;
        case AllDirs:
            m_flags = AllDirs;
            mm_flags = QDir::AllDirs ;
            break;
        case Files:
            m_flags = Files;
            mm_flags = QDir::Files ;
            break;
        case Drives:
            m_flags = Drives;
            mm_flags = QDir::Drives ;
            break;
        case NoSymLinks:
            m_flags = NoSymLinks;
            mm_flags = QDir::NoSymLinks ;
            break;
        case NoDotAndDotDot:
            m_flags = NoDotAndDotDot;
            mm_flags = QDir::NoDotAndDotDot ;
            break;
        case NoDot:
            m_flags = NoDot;
            mm_flags = QDir::NoDot ;
            break;
        case NoDotDot:
            m_flags = NoDotDot;
            mm_flags = QDir::NoDotDot ;
            break;
        case AllEntries:
            m_flags = AllEntries;
            mm_flags = QDir::AllEntries ;
            break;
        case Readable:
            m_flags =Readable ;
            mm_flags = QDir::Readable ;
            break;
        case Writable:
            m_flags = Writable;
            mm_flags = QDir::Writable ;
            break;
        case Executable:
            m_flags = Executable;
            mm_flags = QDir::Executable ;
            break;
        case Modified:
            m_flags = Modified;
            mm_flags = QDir::Modified ;
            break;
        case Hidden:
            m_flags =Hidden ;
            mm_flags = QDir::Hidden ;
            break;
        case System:
            m_flags =System ;
            mm_flags = QDir::System ;
            break;
        case CaseSensitive:
            m_flags = CaseSensitive;
            mm_flags = QDir::CaseSensitive ;
            break;
        }
            emit flagsChanged();

    }
}

QmlDir::IteratorFlag QmlDir::iteratorFlag() const
{
    return m_itFlag;
}

void QmlDir::setIteratorFlag(const QmlDir::IteratorFlag &iteratorFlag)
{
    if(m_itFlag == iteratorFlag){
        return;
    }
    else
    {
        int aFlag = iteratorFlag;
        switch (aFlag) {
        case 0:
            m_itFlag = NoIteratorFlags;
            mm_itFlag = QDirIterator::NoIteratorFlags ;
            setScanRecursively(false);
            break;
        case 1:
            m_itFlag = Subdirectories;
            mm_itFlag = QDirIterator::Subdirectories;
             setScanRecursively(true);
            break;
        case 2:
            m_itFlag = FollowSymlinks;
            mm_itFlag = QDirIterator::FollowSymlinks;
            setScanRecursively(true);
            break;
        }
        emit iteratorFlagChanged();
    }
}

bool QmlDir::scanRecursively() const
{
    return m_scanRecursively;
}

void QmlDir::setScanRecursively(const bool &scanRecursively)
{
    if(m_scanRecursively == scanRecursively)
        return;
    m_scanRecursively = scanRecursively;
    emit scanRecursivelyChanged();
}

QStringList QmlDir::files()const
{
    if(m_scanRecursively == false){
        return m_filesInDir;
    }
    else
    {
        return m_fileseRecursively;
    }
}

void QmlDir::setFiles(const QString thePath)
{
    m_filesInDir.clear();
    m_fileseRecursively.clear();
    d.setPath(thePath);
    d.setFilter(mm_flags);
    if(!d.exists()){
        error("The Directory you are trying to scan does not exsist");
        return;
    }
    else
    {

        if(m_itFlag == NoIteratorFlags){
            QDirIterator it(thePath ,mm_flags,QDirIterator::NoIteratorFlags);
            while(it.hasNext())
            {
                m_filesInDir << it.next();
            }
            emit filesChanged();
        }
        else if (m_itFlag == Subdirectories)
        {
            QDirIterator it(thePath,mm_flags, QDirIterator::Subdirectories);
            while(it.hasNext())
            {
                m_fileseRecursively << it.next();
            }
            emit filesRecursivelyChanged();
        }
        else if (m_itFlag == Subdirectories)
        {
            QDirIterator it(thePath,mm_flags,QDirIterator::FollowSymlinks);
            while(it.hasNext())
            {
                m_fileseRecursively << it.next();
            }
            emit filesRecursivelyChanged();
        }
    }
}
//FIXME this needs a lot of work !
int QmlDir::count() const
{
    if(m_scanRecursively == true )
    {
        return m_fileseRecursively.count();
    }
    else
    {
        return m_filesInDir.count();
    }

}

QString QmlDir::dir() const
{
    return m_dir;
}

void QmlDir::setDir(const QString &dir)
{
    if(m_dir == dir){
        return;
    }
    else
    {
        m_dir = dir;
        emit dirChanged();
        setQDir(m_dir);
        setCurrentPath(m_dir);
    }
}

QString QmlDir::errorString() const
{
    return m_errorString;
}

QString QmlDir::currentPath() const
{
    return m_currentPath;
}

void QmlDir::setCurrentPath(const QString currentPath)
{
    if(m_currentPath == currentPath)
        return;
    m_currentPath = currentPath;
    emit currentPathChanged();
}
//FIXME add to QDir and push upstream if neeeded
//QString QmlDir::owner() const
//{
//    return m_owner;
//}

//void QmlDir::setOwner(const QString path)
//{
//    d.setPath(path);
//    if(!d.exists())
//    {
//        error("The dir that you are trying to see who owns it is not a vaild folder , in other words it does not exsist");
//        return;
//    }
//    else
//    {
//        QString dOwn = QString("%1").arg(d.owner());
//        if (m_owner == dOwn){
//            return;
//        }
//        else
//        {
//            m_owner = dOwn;
//            emit ownerChanged();
//        }
//    }
//}

bool QmlDir::rename(const QString oldDir, const QString newDir)
{
    d.setPath(oldDir);
    if (!d.exists()){
        error("The Old Dir is not found on this system");
        return false;
    }
    else if (!d.rename(oldDir,newDir))
    {
        error("Could not rename dir");
        return false;
    }else {
        return true;
    }
}

bool QmlDir::createPath()
{
    d.setPath(m_dir);
    if(d.exists()){
        error("The Dir path that you are trying to create already exists maybe you would like to remove or rename");
        return false;
    }
    else if(!d.mkpath(m_dir))
    {
        error("Something wicked happened when trying to create the new path");
        return false;
    }
    else
    {
        return true;
    }
}

bool QmlDir::createDir()
{
    d.setPath(m_dir);
    if(d.exists()){
        error("The Dir  that you are trying to create already exists maybe you would like to remove or rename");
        return false;
    }
    else if(!d.mkdir(m_dir))
    {
        error("Something wicked happened when trying to create the new dir maybe you meant setPath ? ");
        return false;
    }
    else
    {
        return true;
    }
}

bool QmlDir::dirExists()
{
    d.setPath(m_dir);
    if(d.exists()){
        return true;
    }
    else
    {
        return false;
    }
}

bool QmlDir::cdUp()
{
    if (d.cdUp()){
        return true ;
    }
    else
    {
        return false;
    }
}

bool QmlDir::removeRecursively()
{
    d.setPath(m_dir);
    if(!d.exists()){
        error("The Dir  that you are trying to remove recursively does not exists");
        return false;
    }
    else if(!d.removeRecursively())
    {
        error("Something wicked happened when trying to rying to remove recursively ? ");
        return false;
    }
    else
    {
        return true;
    }
}


bool QmlDir::remove()
{
    d.setPath(m_dir);
    if(!d.exists()){
        error("The Dir  that you are trying to remove does not exists");
        return false;
    }
    else if(!d.remove(m_dir))
    {
        error("Something wicked happened when trying to rying to remove");
        return false;
    }
    else
    {
        return true;
    }
}

bool QmlDir::isReadable()
{
    d.setPath(m_dir);
    if(!d.exists()){
        error("The Dir  that you are trying to see if it is readeable does not exists");
        return false;
    }
    else if(!d.isReadable())
    {
        return false;
    }
    else
    {
        return true;
    }
}

void QmlDir::copyFile(const QString oldFile, const QString newFile)
{
        QFile m_file;
        m_file.setFileName(oldFile);
        if (m_file.copy(newFile)){
            qDebug() << "Way to go bro ";
        }else{
            error("Could not copy over the file " );
        }

}



void QmlDir::handdelSetQDir(const QString incoming)
{
    d.setPath(incoming);
}

void QmlDir::hanndelError(const QString err)
{
    if(m_errorString == err)
        return;
    m_errorString = err;
    emit errorStringChanged();
}


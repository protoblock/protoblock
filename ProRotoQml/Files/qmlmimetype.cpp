#include "qmlmimetype.h"
QmlMimeType::QmlMimeType(QObject *parent)
    :
      QObject(parent),
      m_matchMode(MatchDefault),
      mm_matchMode(QMimeDatabase::MatchDefault),
      m_databaseType(File),
      m_file("")

{


    // set [file,url,data] -> set db -> set propertys
    connect(this,SIGNAL(fileChanged()),this,SLOT(getTypes()));
    connect(this,SIGNAL(urlChanged()),this,SLOT(getTypes()));
    connect(this,SIGNAL(dataChanged()),this,SLOT(getTypes()));

    connect(this,SIGNAL(databaseSet(QMimeType)),
            this,SLOT(setProperties(QMimeType)));

}

QmlMimeType::DatabaseTypes QmlMimeType::databaseTypes() const
{
    return m_databaseType;
}

void QmlMimeType::setDatabaseTypes(const QmlMimeType::DatabaseTypes &databaseTypes)
{
    if(m_databaseType == databaseTypes){
        return;
    }
    else
    {
        // fixme make if
        // map  QmlMimeType::DatabaseTypes to  other DB types
        switch (databaseTypes) {
        case DataDevice:
            m_databaseType = DataDevice;
            break;
        case DataArray:
            m_databaseType = DataArray;
            break;
        case File:
            m_databaseType = File;
            break;
        case FileAndData:
            m_databaseType = FileAndData;
            break;
        }
        emit databaseTypesChanged();
    }
}

QmlMimeType::MatchMode QmlMimeType::matchMode() const
{
    return m_matchMode;
}

void QmlMimeType::setMatchMode(const QmlMimeType::MatchMode &matchMode)
{
    if(m_matchMode == matchMode){
        return;
    }
    else
    {
        switch (matchMode) {
        case MatchDefault:
            m_matchMode = MatchDefault;
            mm_matchMode = QMimeDatabase::MatchDefault;
            break;
        case MatchExtension:
            m_matchMode = MatchExtension;
            mm_matchMode = QMimeDatabase::MatchExtension;
            break;
        case MatchContent:
            m_matchMode = MatchContent;
            mm_matchMode = QMimeDatabase::MatchContent;
            break;
        }
        emit matchModeChanged();
    }
}

/*!
 * \qmlproperty string  QmlMimeType::file
 * returns the file name that is getting looked at.
 * when this changes the it re
 */
QString QmlMimeType::file() const
{
    return m_file;
}

void QmlMimeType::setFile(const QString &file)
{
    if( m_file == file)
        return;
    m_file = file;
    emit fileChanged();
}

QUrl QmlMimeType::url() const
{
    return m_url;
}

void QmlMimeType::setUrl(const QUrl &url)
{
    if(m_url == url)
        return;
    m_url = url;
    emit urlChanged();

}

QStringList QmlMimeType::data() const
{
    return m_data;
}

void QmlMimeType::setData(const QStringList &data)
{
    if(m_data == data){
        return;
    }else{
        m_data = data;
        //convert the to a QByteArray
        QByteArray bArray;
        bArray.clear();
        for (int i = 0 ; i < data.length(); i++){
            bArray.append(data.at(i));
        }
        mm_data = bArray;
        emit dataChanged();
    }
}

//QString QmlMimeType::device() const
//{
//    return m_device;
//}

//void QmlMimeType::setDevice(const QString &device)
//{
//    if(m_device == device){
//        return;
//    }
//    else
//    {


//        m_device = device;
//        // convert the string to a IODEVICE


//        QIODevice ioDevice;

//        mm_device = ;
//        emit deviceChanged();
//    }
//}

/*!
 * \brief QmlMimeType::iconName
 * \returns the icon name for the file htat is getting viewed
 */
QString QmlMimeType::iconName() const
{
    return m_iconName;
}

void QmlMimeType::setIconName(const QString &iconName)
{
    if(m_iconName == iconName)
        return;
    m_iconName = iconName;
    emit iconNameChanged();
}

/*!
 * \brief QmlMimeType::mimeName
 * \return
 */
QString QmlMimeType::mimeName() const
{
    return m_mimeName;
}

void QmlMimeType::setMimeName(const QString &mimeName)
{
    if(m_mimeName == mimeName)
        return;
    m_mimeName = mimeName;
    emit mimeNameChanged();
}

/*!
 * \brief QmlMimeType::comment
 * \return
 */
QString QmlMimeType::comment() const
{
    return m_comment;
}


/*!
 * \brief QmlMimeType::setComment
 * \param comment
 */
void QmlMimeType::setComment(const QString &comment)
{
    if(m_comment == comment)
        return;
    m_comment = comment;
    emit commentChanged();
}


/*!
 * \brief QmlMimeType::genericIconName
 * \return
 */
QString QmlMimeType::genericIconName() const
{
    return m_genericIconName;
}


void QmlMimeType::setGenericIconName(const QString &genericIconName)
{
    if(m_genericIconName == genericIconName)
        return;
    m_genericIconName = genericIconName ;
    emit genericIconNameChanged();
}

/*!
 * \brief QmlMimeType::preferredSuffix
 * \return
 */
QString QmlMimeType::preferredSuffix() const
{
    return m_preferredSuffix;
}

void QmlMimeType::setPreferredSuffix(const QString &preferredSuffix)
{
    if(m_preferredSuffix == preferredSuffix )
        return;
    m_preferredSuffix = preferredSuffix;
    emit preferredSuffixChanged();
}

/*!
 * \brief QmlMimeType::isVaild
 * \return
 */
bool QmlMimeType::isVaild() const
{
    return m_isValid;
}

void QmlMimeType::setIsVaild(const bool &isVaild)
{
    if(m_isValid == isVaild )
        return;
    m_isValid = isVaild;
    emit isVaildChanged();
}


/*!
 * \brief QmlMimeType::isDefault
 * \return
 */
bool QmlMimeType::isDefault() const
{
    return m_isDefault;
}

void QmlMimeType::setIsDefault(const bool &isDefault)
{
    if(m_isDefault == isDefault)
        return;
    m_isDefault = isDefault;
    emit isDefaultChanged();
}

/*!
 * \brief QmlMimeType::inherits
 * \return
 */
bool QmlMimeType::inherits() const
{
    return m_inherits;
}

void QmlMimeType::setInherits(const bool &inherits)
{
    if(m_inherits == inherits)
        return ;
    m_inherits = inherits;
    emit inheritsChanged();
}

QStringList QmlMimeType::aliases() const
{
    return m_aliases;
}

void QmlMimeType::setAliases(const QStringList &aliases)
{
    if(m_aliases == aliases)
        return;
    m_aliases = aliases;
    emit aliasesChanged();
}

QStringList QmlMimeType::allAncestors() const
{
    return m_allAncestors;
}

void QmlMimeType::setAllAncestors(const QStringList &allAncestors)
{
    if(m_allAncestors == allAncestors)
        return;
    m_allAncestors = allAncestors;
    emit allAncestorsChanged();
}

QStringList QmlMimeType::globPatterns() const
{
    return m_globPatterns;
}

void QmlMimeType::setGlobPatterns(const QStringList &globPatterns)
{
    if(m_globPatterns == globPatterns)
        return;
    m_globPatterns = globPatterns ;
    emit globPatternsChanged();
}

QStringList QmlMimeType::parentMimeTypes() const
{
    return m_parentMimeTypes;
}

void QmlMimeType::setParentMimeTypes(const QStringList &parentMimeTypes)
{
    if(m_parentMimeTypes == parentMimeTypes)
        return;
    m_parentMimeTypes = parentMimeTypes;
    emit  parentMimeTypesChanged();
}

QStringList QmlMimeType::suffixes() const
{
    return m_suffixes;
}

void QmlMimeType::setSuffixes(const QStringList &suffixes)
{
    if(m_suffixes == suffixes)
        return;
    m_suffixes = suffixes;
    emit suffixesChanged();
}

QStringList QmlMimeType::allMimeTypes()
{
    return m_allMimeTypes;
}

void QmlMimeType::setAllMimeTypes(const QStringList &allMimeTypes)
{
    if(m_allMimeTypes == allMimeTypes)
        return;
    m_allMimeTypes = allMimeTypes;
    emit allMimeTypesChanged();
}


/*!
 *  QmlMimeType::getTypes
 */
void QmlMimeType::getTypes()
{



    switch (m_databaseType) {
    //This is kinda stupid and maybe should be removed
    case DataArray:
        m_mimeTypes = m_db.mimeTypeForData(mm_data);
//        setAllMimeTypes(m_db.allMimeTypes());
        break;
    case DataDevice:
        m_mimeTypes = m_db.mimeTypeForFile(m_file,mm_matchMode) ;
//        setAllMimeTypes(m_db.allMimeTypes());
        break;
    case File:
        m_mimeTypes = m_db.mimeTypeForFile(m_file,mm_matchMode) ;
//        setAllMimeTypes(m_db.allMimeTypes());
        break;
    case Url:
        m_mimeTypes = m_db.mimeTypeForUrl(m_url);
//        setAllMimeTypes(m_db.allMimeTypes());
        break;
    case FileAndData:
        m_mimeTypes = m_db.mimeTypeForFileNameAndData(m_file,mm_data) ;
//        m_mimeTypessetAllMimeTypes(m_db.allMimeTypes());
        break;
    }
    //send the signal so that we set the properties
    databaseSet(m_mimeTypes);
}


// sets all the properties
void QmlMimeType::setProperties(const QMimeType mime)
{
    setIconName(mime.iconName());
    setMimeName(mime.name());
    setComment(mime.comment());
    setGenericIconName(mime.genericIconName());
    setPreferredSuffix(mime.preferredSuffix());
    //bools
    setIsVaild(mime.isValid());
    setIsDefault(mime.isDefault());
    setInherits(mime.inherits(mime.name()));
    //stringlists
    setAliases(mime.aliases());
    setAllAncestors(mime.allAncestors());
    setGlobPatterns(mime.globPatterns());
    setParentMimeTypes(mime.parentMimeTypes());
    setSuffixes(mime.suffixes());
    emit doneReading();


//qDebug() << "set";
}

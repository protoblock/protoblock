#ifndef QMLMIMETYPE_H
#define QMLMIMETYPE_H

#include <QObject>
#include <QMimeType>
#include <QMimeDatabase>
#include <QString>
#include <QStringList>
#include <QByteArray>
#include <QUrl>
#include <QIODevice>
#include <QDebug>

class QmlMimeType : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString iconName READ iconName  NOTIFY iconNameChanged)
    Q_PROPERTY(QString mimeName READ mimeName NOTIFY mimeNameChanged)
    Q_PROPERTY(QString comment READ comment NOTIFY commentChanged)
    Q_PROPERTY(QString genericIconName READ genericIconName NOTIFY genericIconNameChanged)
    Q_PROPERTY(QString preferredSuffix READ preferredSuffix NOTIFY preferredSuffixChanged)
    Q_PROPERTY(bool isVaild READ isVaild  NOTIFY isVaildChanged)
    Q_PROPERTY(bool isDefault READ isDefault NOTIFY isDefaultChanged)
    Q_PROPERTY(bool inherits READ inherits NOTIFY inheritsChanged)


    //stringlists
    Q_PROPERTY(QStringList aliases READ aliases NOTIFY aliasesChanged)
    Q_PROPERTY(QStringList allAncestors READ allAncestors NOTIFY allAncestorsChanged)
    Q_PROPERTY(QStringList globPatterns READ globPatterns NOTIFY globPatternsChanged)
    Q_PROPERTY(QStringList parentMimeTypes READ parentMimeTypes  NOTIFY parentMimeTypesChanged)
    Q_PROPERTY(QStringList suffixes READ suffixes NOTIFY suffixesChanged)
    //from QMimeDatabase
    Q_PROPERTY(QStringList allMimeTypes READ allMimeTypes  NOTIFY allMimeTypesChanged)

    //enums
    Q_PROPERTY(MatchMode matchMode READ matchMode WRITE setMatchMode NOTIFY matchModeChanged)
    Q_PROPERTY(DatabaseTypes databaseTypes READ databaseTypes WRITE setDatabaseTypes NOTIFY databaseTypesChanged)

    Q_ENUMS(MatchMode)
    Q_ENUMS(DatabaseTypes)

    // datatypes
    Q_PROPERTY(QString file READ file WRITE setFile NOTIFY fileChanged)
    Q_PROPERTY(QUrl url READ url WRITE setUrl NOTIFY urlChanged)
    Q_PROPERTY(QStringList data READ data WRITE setData NOTIFY dataChanged)
//    Q_PROPERTY(QString device READ device WRITE setDevice NOTIFY deviceChanged)


public:
    explicit QmlMimeType(QObject *parent = 0);
    // FIXME add Database  allMimeTypes()
    enum DatabaseTypes{
        DataDevice = 0,
        DataArray = 1 ,
        File = 2,
        Url = 3 ,
        FileAndData = 4
    };

    enum  MatchMode {
        MatchDefault = 0,
        MatchExtension = 1,
        MatchContent = 2
    };

    //strings
    QString iconName()const;
    void setIconName(const QString &iconName);

    QString mimeName()const;
    void setMimeName(const QString &mimeName);

    QString comment()const;
    void setComment(const QString &comment);

    QString genericIconName()const;
    void setGenericIconName(const QString &genericIconName);

    QString preferredSuffix()const;
    void setPreferredSuffix(const QString &preferredSuffix);

    //bools
    bool isVaild()const;
    void setIsVaild(const bool &isVaild);

    bool isDefault()const;
    void setIsDefault(const bool &isDefault);

    bool inherits()const;
    void setInherits(const bool &inherits);

    //stringLists
    QStringList aliases()const;
    void setAliases(const QStringList &aliases );

    QStringList allAncestors()const;
    void setAllAncestors(const QStringList &allAncestors );

    QStringList globPatterns()const;
    void setGlobPatterns (const QStringList &globPatterns );

    QStringList parentMimeTypes()const;
    void setParentMimeTypes(const QStringList &parentMimeTypes );

    QStringList suffixes()const;
    void setSuffixes (const QStringList &suffixes );

    QStringList allMimeTypes();
    void setAllMimeTypes(const QStringList &allMimeTypes);

    //  enums
    DatabaseTypes databaseTypes()const;
    void setDatabaseTypes(const DatabaseTypes &databaseTypes);

    MatchMode matchMode()const;
    void setMatchMode(const MatchMode &matchMode);


    // DataBase setters and helpers
    QString file()const ;
    void setFile(const QString &file);

    QUrl url()const;
    void setUrl(const QUrl &url);

    QStringList data()const;
    void setData(const QStringList &data);

//Later it is kinda stupid conming from the qml side to open the file and read
//    QString device()const;
//    void setDevice(const QString &device);


signals:
    //strings
    void iconNameChanged();
    void mimeNameChanged();
    void commentChanged();
    void genericIconNameChanged();
    void preferredSuffixChanged();
    //bools
    void isVaildChanged();
    void isDefaultChanged();
    void inheritsChanged();
    //str lists
    void aliasesChanged();
    void allAncestorsChanged();
    void globPatternsChanged();
    void parentMimeTypesChanged();
    void  suffixesChanged();
    void allMimeTypesChanged();
    //enums
    void databaseTypesChanged();
    void matchModeChanged();

    //internal
    void databaseSet(const QMimeType db_type);
    void fileChanged();
    void urlChanged();
    void dataChanged();
    //when everything is set
    void doneReading();


protected slots:
    // [file,url,data]Changed fires this
    void getTypes();
    // after the database For is set this is run to set the properties
    void setProperties(const QMimeType mime);

private :
    QMimeDatabase m_db;
    QMimeType m_mimeTypes;

    //string
    QString m_iconName;
    QString m_mimeName;
    QString m_comment;
    QString m_genericIconName;
    QString m_preferredSuffix;

    //bools
    bool m_isValid;
    bool m_isDefault;
    bool m_inherits;

    //stringlists
    QStringList m_aliases;
    QStringList m_allAncestors ;
    QStringList m_globPatterns;
    QStringList m_parentMimeTypes;
    QStringList m_suffixes;
    QStringList m_allMimeTypes;

    //enums
    MatchMode m_matchMode;
    QMimeDatabase::MatchMode mm_matchMode;

    DatabaseTypes m_databaseType;

    //internal db setter

    QString m_file;

    // need both for converstion.
    QStringList m_data;
    QByteArray mm_data;

// need both for converstion.
//    QString m_device;
//    QIODevice mm_device;

    QUrl m_url;
};

#endif // QMLMIMETYPE_H

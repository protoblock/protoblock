#ifndef QQMLOBJECTLISTMODEL_H
#define QQMLOBJECTLISTMODEL_H

#include <QAbstractListModel>
#include <QByteArray>
#include <QChar>
#include <QDebug>
#include <QHash>
#include <QList>
#include <QMetaMethod>
#include <QMetaObject>
#include <QMetaProperty>
#include <QObject>
#include <QString>
#include <QStringBuilder>
#include <QVariant>
#include <QVector>
#include <QSharedPointer>
#include <QTimer>

template<typename T> QList<T> qListFromVariant( const QVariantList &list )
{
    QList<T> ret;
    ret.reserve( list.size() );
    for( QVariantList::const_iterator it = list.constBegin(); it != list.constEnd(); it++ )
    {
        const QVariant &var = static_cast<QVariant>( * it );
        ret.append( var.value<T> () );
    }
    return ret;
}

template<typename T> QVariantList qListToVariant( const QList<T> &list )
{
    QVariantList ret;
    ret.reserve( list.size() );
    for( typename QList<T>::const_iterator it = list.constBegin(); it != list.constEnd(); it++ )
    {
        const T &val = static_cast<T>( * it );
        ret.append( QVariant::fromValue( val ) );
    }
    return ret;
}

// custom foreach for QList, which uses no copy and check pointer non-null
#define FOREACH_PTR_IN_QLIST(_type_, _var_, _list_) \
    for ( typename QList<_type_ *>::const_iterator it = _list_.begin (); it != _list_.end (); it++) \
        for (_type_ * _var_ = static_cast<_type_ *> (* it); _var_ != Q_NULLPTR; _var_ = Q_NULLPTR)

class QQmlObjectListModelBase : public QAbstractListModel {
    Q_OBJECT
    Q_PROPERTY( int count READ count NOTIFY countChanged )
    Q_PROPERTY( int genericTimerInterval
                READ genericTimerInterval
                WRITE setGenericTimerInterval
                NOTIFY genericTimerIntervalChanged
              )

public:
    explicit QQmlObjectListModelBase( QObject *parent = Q_NULLPTR ) :
        QAbstractListModel( parent )
    {
    }

public slots: // virtual methods API for QML
    virtual int size( void ) const = 0;
    virtual int length( void ) const = 0;
    virtual int count( void ) const = 0;
    virtual int genericTimerInterval( void ) const = 0;
    virtual void setGenericTimerInterval( const int &interval ) = 0;
    virtual bool isEmpty( void ) const = 0;
    virtual bool contains( QObject *item ) const = 0;
    virtual int indexOf( QObject *item ) const = 0;
    virtual int roleForName( const QByteArray &name ) const = 0;
    virtual void clear( void ) = 0;
    virtual void append( QObject *item ) = 0;
    virtual void appendObject( QObject *item ) = 0;
    virtual void prepend( QObject *item ) = 0;
    virtual void insert( int idx, QObject *item ) = 0;
    virtual void move( int idx, int pos ) = 0;
    virtual void remove( QObject *item ) = 0;
    virtual void remove( int idx ) = 0;
    virtual QObject *get( int idx ) const = 0;
    virtual QObject *get( const QString &uid ) const = 0;
    virtual QObject *getFirst( void ) const = 0;
    virtual QObject *getLast( void ) const = 0;
    virtual QVariantList toVarArray( void ) const = 0;

    virtual void startSimulation( int bottom, int top, int heartbeat ) = 0;

    virtual void startGenericTimer() = 0;
    virtual void stopGenericTimer() = 0;
    virtual QSharedPointer<QTimer> getGenericTimer() = 0;
    virtual bool genericTimerActive() = 0;

    virtual int randomNumberBetween( const int &floor, const int &ceil ) = 0;
    virtual double randomNumberBetween( const double &floor, const double &ceil ) = 0;
    virtual float randomNumberBetween( const float &floor, const float &ceil ) = 0;
    virtual quint32 randomNumberBetween( const quint32 &floor, const quint32 &ceil ) = 0;
    virtual quint64 randomNumberBetween( const quint64 &floor, const quint64 &ceil ) = 0;

protected slots: // internal callback
    virtual void onItemPropertyChanged( void ) = 0;

signals: // notifier
    void countChanged( void );
    void genericTimerIntervalChanged( void );
    void timeout();
};




template<class ItemType>
class QQmlObjectListModel : public QQmlObjectListModelBase {
public:
    explicit QQmlObjectListModel(
        QObject *parent = Q_NULLPTR,
        const QByteArray &displayRole = QByteArray(),
        const QByteArray &uidRole = QByteArray()
    )
        : QQmlObjectListModelBase( parent )
        , m_count( 0 )
        , m_uidRoleName( uidRole )
        , m_dispRoleName( displayRole )
        , m_metaObj( ItemType::staticMetaObject )
        , m_interval( 300000 )
        , m_singleShot( true )
    {
        static QSet<QByteArray> roleNamesBlacklist;
        if( roleNamesBlacklist.isEmpty() )
        {
            roleNamesBlacklist << QByteArrayLiteral( "id" )
                               << QByteArrayLiteral( "index" )
                               << QByteArrayLiteral( "class" )
                               << QByteArrayLiteral( "model" )
                               << QByteArrayLiteral( "modelData" );
        }
        static const char *HANDLER = "onItemPropertyChanged()";
        m_handler = metaObject()->method( metaObject()->indexOfMethod( HANDLER ) );
        if( !displayRole.isEmpty() )
        {
            m_roles.insert( Qt::DisplayRole, QByteArrayLiteral( "display" ) );
        }
        m_roles.insert( baseRole(), QByteArrayLiteral( "qtObject" ) );
        const int len = m_metaObj.propertyCount();
        for( int propertyIdx = 0, role = ( baseRole() + 1 ); propertyIdx < len; propertyIdx++, role++ )
        {
            QMetaProperty metaProp = m_metaObj.property( propertyIdx );
            const QByteArray propName = QByteArray( metaProp.name() );
            if( !roleNamesBlacklist.contains( propName ) )
            {
                m_roles.insert( role, propName );
                if( metaProp.hasNotifySignal() )
                {
                    m_signalIdxToRole.insert( metaProp.notifySignalIndex(), role );
                }
            }
            else
            {
                static const QByteArray CLASS_NAME = ( QByteArrayLiteral( "QQmlObjectListModel<" ) % m_metaObj.className() % '>' );
                qWarning() << "Can't have" << propName << "as a role name in" << qPrintable( CLASS_NAME );
            }
        }

        giveGenericTimerData();

    }
    ~QQmlObjectListModel()
    {
        if( this->count() > 0 )
        {
            this->clear();
        }
        if( m_genericTimer.data() )
        {
            m_genericTimer.clear();
        }

    }

    bool setData( const QModelIndex &index, const QVariant &value, int role )
    {
        bool ret = false;
        ItemType *item = at( index.row() );
        const QByteArray rolename = ( role != Qt::DisplayRole ? m_roles.value( role, emptyBA() ) : m_dispRoleName );
        if( item != Q_NULLPTR && role != baseRole() && !rolename.isEmpty() )
        {
            ret = item->setProperty( rolename, value );
        }
        return ret;
    }
    QVariant data( const QModelIndex &index, int role ) const
    {
        QVariant ret;
        ItemType *item = at( index.row() );
        const QByteArray rolename = ( role != Qt::DisplayRole ? m_roles.value( role, emptyBA() ) : m_dispRoleName );
        if( item != Q_NULLPTR && !rolename.isEmpty() )
        {
            ret.setValue( role != baseRole() ? item->property( rolename ) : QVariant::fromValue( static_cast<QObject *>( item ) ) );
        }
        return ret;
    }
    QHash<int, QByteArray> roleNames( void ) const
    {
        return m_roles;
    }
    typedef typename QList<ItemType *>::const_iterator const_iterator;
    const_iterator begin( void ) const
    {
        return m_items.begin();
    }
    const_iterator end( void ) const
    {
        return m_items.end();
    }
    const_iterator constBegin( void ) const
    {
        return m_items.constBegin();
    }
    const_iterator constEnd( void ) const
    {
        return m_items.constEnd();
    }

public: // C++ API

    ItemType *at( int idx ) const
    {
        ItemType *ret = Q_NULLPTR;
        if( idx >= 0 && idx < m_items.size() )
        {
            ret = m_items.value( idx );
        }
        return ret;
    }
    ItemType *getByUid( const QString &uid ) const
    {
        return m_indexByUid.value( uid, Q_NULLPTR );
    }
    int roleForName( const QByteArray &name ) const
    {
        return m_roles.key( name, -1 );
    }
    int count( void ) const
    {
        return m_count;
    }

    int genericTimerInterval( void ) const
    {
        return m_interval;
    }
    void setGenericTimerInterval( const int &interval )
    {
        if( m_interval != interval )
        {
            m_interval = interval;
            emit genericTimerIntervalChanged();
        }
    }

    int size( void ) const
    {
        return m_count;
    }
    int length( void )const
    {
        return m_count;
    }
    bool isEmpty( void ) const
    {
        return m_items.isEmpty();
    }
    bool contains( ItemType *item ) const
    {
        return m_items.contains( item );
    }
    int indexOf( ItemType *item ) const
    {
        return m_items.indexOf( item );
    }
    void clear( void )
    {
        if( !m_items.isEmpty() )
        {
            beginRemoveRows( noParent(), 0, m_items.count() - 1 );
            FOREACH_PTR_IN_QLIST( ItemType, item, m_items )
            {
                dereferenceItem( item );
            }
            m_items.clear();
            endRemoveRows();
            updateCounter();
        }
    }
    void append( ItemType *item )
    {
        if( item != Q_NULLPTR )
        {
            const int pos = m_items.count();
            beginInsertRows( noParent(), pos, pos );
            m_items.append( item );
            referenceItem( item );
            endInsertRows();
            updateCounter();
        }
    }
    //FIXME use the metaobject of the obj and a obj
    // from a model to auto create a object to insert
    // into this
    virtual void appendObject( QObject *item )
    {
        Q_UNUSED( item )
        qDebug() << "QQmlObjectListModel::appendObject is virtual";
    }
    virtual void startSimulation( int bottom, int top, int heartbeat )
    {
        Q_UNUSED( bottom )
        Q_UNUSED( top )
        Q_UNUSED( heartbeat )
        qDebug() << "QQmlObjectListModel::startSimulation is virtual";
    }


    void prepend( ItemType *item )
    {
        if( item != Q_NULLPTR )
        {
            beginInsertRows( noParent(), 0, 0 );
            m_items.prepend( item );
            referenceItem( item );
            endInsertRows();
            updateCounter();
        }
    }
    void insert( int idx, ItemType *item )
    {
        if( item != Q_NULLPTR )
        {
            beginInsertRows( noParent(), idx, idx );
            m_items.insert( idx, item );
            referenceItem( item );
            endInsertRows();
            updateCounter();
        }
    }

    void append( const QList<ItemType *> &itemList )
    {
        if( !itemList.isEmpty() )
        {
            const int pos = m_items.count();
            beginInsertRows( noParent(), pos, pos + itemList.count() - 1 );
            m_items.reserve( m_items.count() + itemList.count() );
            m_items.append( itemList );
            FOREACH_PTR_IN_QLIST( ItemType, item, itemList )
            {
                referenceItem( item );
            }
            endInsertRows();
            updateCounter();
        }
    }
    void prepend( const QList<ItemType *> &itemList )
    {
        if( !itemList.isEmpty() )
        {
            beginInsertRows( noParent(), 0, itemList.count() - 1 );
            m_items.reserve( m_items.count() + itemList.count() );
            int offset = 0;
            FOREACH_PTR_IN_QLIST( ItemType, item, itemList )
            {
                m_items.insert( offset, item );
                referenceItem( item );
                offset++;
            }
            endInsertRows();
            updateCounter();
        }
    }
    void insert( int idx, const QList<ItemType *> &itemList )
    {
        if( !itemList.isEmpty() )
        {
            beginInsertRows( noParent(), idx, idx + itemList.count() - 1 );
            m_items.reserve( m_items.count() + itemList.count() );
            int offset = 0;
            FOREACH_PTR_IN_QLIST( ItemType, item, itemList )
            {
                m_items.insert( idx + offset, item );
                referenceItem( item );
                offset++;
            }
            endInsertRows();
            updateCounter();
        }
    }
    void move( int idx, int pos )
    {
        if( idx != pos )
        {
            const int lowest  = qMin( idx, pos );
            const int highest = qMax( idx, pos );
            beginMoveRows( noParent(), highest, highest, noParent(), lowest );
            m_items.move( highest, lowest );
            endMoveRows();
        }
    }
    void remove( ItemType *item )
    {
        if( item != Q_NULLPTR )
        {
            const int idx = m_items.indexOf( item );
            remove( idx );
        }
    }
    void remove( int idx )
    {
        if( idx >= 0 && idx < m_items.size() )
        {
            beginRemoveRows( noParent(), idx, idx );
            ItemType *item = m_items.takeAt( idx );
            dereferenceItem( item );
            endRemoveRows();
            updateCounter();
        }
    }
    ItemType *first( void ) const
    {
        return m_items.first();
    }
    ItemType *last( void ) const
    {
        return m_items.last();
    }
    const QList<ItemType *> &toList( void ) const
    {
        return m_items;
    }

    const QList<ItemType * > &toListFromId( const QString &roleName, const QString &whereClause )
    {
        QList<ItemType *> ret;
        char *buff = roleName.toLocal8Bit().data();
        for( int i = 0; i < this->count(); i++ )
        {
            if( this->get( i )->property( buff ).toString() == whereClause )
            {
                ret.append( this->get( i ) );
            }
        }
        return ret;
    }
    const QVariantList toVarListFromRoleName( const QString &roleName )
    {
        QVariantList ret = QVariantList();
        char *re_role = roleName.toLocal8Bit().data();
        for( int i = 0; i < this->count(); i++ )
        {
            ret.append( this->get( i )->property( re_role ) );
        }
        return ret;
    }

    ItemType *getItemByRoleName( const QString &roleName, const QString &whereClause )
    {
        ItemType *ret = Q_NULLPTR;
        char *buff = roleName.toLocal8Bit().data();
        for( int i = 0; i < this->count(); i++ )
        {
            if( this->get( i )->property( buff ).toString() == whereClause )
            {
                ret = this->at( i );
            }
        }
        return ret;
    }

    void replaceItem( ItemType *oldItem, ItemType *newItem )
    {
        int lastPlace = indexOf( oldItem );
        remove( oldItem );
        insert( lastPlace , newItem );
    }

public: // QML slots implementation
    void append( QObject *item )
    {
        append( qobject_cast<ItemType *> ( item ) );
    }
    void prepend( QObject *item )
    {
        prepend( qobject_cast<ItemType *> ( item ) );
    }
    void insert( int idx, QObject *item )
    {
        insert( idx, qobject_cast<ItemType *> ( item ) );
    }
    void remove( QObject *item )
    {
        remove( qobject_cast<ItemType *> ( item ) );
    }
    bool contains( QObject *item ) const
    {
        return contains( qobject_cast<ItemType *> ( item ) );
    }
    int indexOf( QObject *item ) const
    {
        return indexOf( qobject_cast<ItemType *> ( item ) );
    }
    int indexOf( const QString &uid ) const
    {
        return indexOf( get( uid ) );
    }
    QObject *get( int idx ) const
    {
        return static_cast<QObject *>( at( idx ) );
    }
    QObject *get( const QString &uid ) const
    {
        return static_cast<QObject *>( getByUid( uid ) );
    }
    QObject *getFirst( void ) const
    {
        return static_cast<QObject *>( first() );
    }
    QObject *getLast( void ) const
    {
        return static_cast<QObject *>( last() );
    }
    QVariantList toVarArray( void ) const
    {
        return qListToVariant<ItemType *> ( m_items );
    }
    Q_INVOKABLE QVariantList toVarArrayFromId( const QString &r, const QString &wR )
    {
        return qListToVariant<ItemType *>( toListFromId( r, wR ) );
    }

    Q_INVOKABLE QList<ItemType *> listFromId( const QString &r, const QString &wR )
    {
        return toListFromId( r, wR );
    }



    Q_INVOKABLE void clearModel()
    {
        if( !m_items.isEmpty() )
        {
            beginRemoveRows( noParent(), 0, m_items.count() - 1 );
            FOREACH_PTR_IN_QLIST( ItemType, item, m_items )
            {
                dereferenceItem( item );
            }
            m_items.clear();
            endRemoveRows();
            updateCounter();
        }
    }

    Q_INVOKABLE void startGenericTimer()
    {
        m_genericTimer.data()->start( m_interval );
    }
    Q_INVOKABLE void stopGenericTimer()
    {
        m_genericTimer.data()->stop();
    }
    QSharedPointer<QTimer> getGenericTimer()
    {
        return m_genericTimer;
    }
    Q_INVOKABLE bool genericTimerActive()
    {
        bool ret = false;
        if( m_genericTimer.data() )
        {
            ret = m_genericTimer.data()->isActive();
        }
        return ret;
    }



    int randomNumberBetween( const int &floor, const int &ceil )
    {
        qsrand( qrand() );
        return floor + static_cast<int>( qrand() ) /
               static_cast<int>( RAND_MAX / ( ceil - floor ) );
    }
    double randomNumberBetween( const double &floor, const double &ceil )
    {
        qsrand( qrand() );
        return floor + static_cast<double>( qrand() ) /
               static_cast<double>( RAND_MAX / ( ceil - floor ) );
    }
    float randomNumberBetween( const float &floor, const float &ceil )
    {
        qsrand( qrand() );
        return floor + static_cast<float>( qrand() ) /
               static_cast<float>( RAND_MAX / ( ceil - floor ) );
    }
    quint32 randomNumberBetween( const quint32 &floor, const quint32 &ceil )
    {
        qsrand( qrand() );
        return floor + static_cast<quint32>( qrand() ) /
               static_cast<quint32>( RAND_MAX / ( ceil - floor ) );
    }
    quint64 randomNumberBetween( const quint64 &floor, const quint64 &ceil )
    {
        qsrand( qrand() );
        return floor + static_cast<quint64>( qrand() ) /
               static_cast<quint64>( RAND_MAX / ( ceil - floor ) );
    }

protected: // internal stuff
    static const QString &emptyStr( void )
    {
        static const QString ret = QStringLiteral( "" );
        return ret;
    }
    static const QByteArray &emptyBA( void )
    {
        static const QByteArray ret = QByteArrayLiteral( "" );
        return ret;
    }
    static const QModelIndex &noParent( void )
    {
        static const QModelIndex ret = QModelIndex();
        return ret;
    }
    static const int &baseRole( void )
    {
        static const int ret = Qt::UserRole;
        return ret;
    }
    int rowCount( const QModelIndex &parent = QModelIndex() ) const
    {
        Q_UNUSED( parent );
        return m_items.count();
    }
    void referenceItem( ItemType *item )
    {
        if( item != Q_NULLPTR )
        {
            if( item->parent() == Q_NULLPTR )
            {
                item->setParent( this );
            }
            const QList<int> signalsIdxList = m_signalIdxToRole.keys();
            for( QList<int>::const_iterator it = signalsIdxList.constBegin(); it != signalsIdxList.constEnd(); it++ )
            {
                const int signalIdx = static_cast<int>( * it );
                QMetaMethod notifier = item->metaObject()->method( signalIdx );
                connect( item, notifier, this, m_handler, Qt::UniqueConnection );
            }
            if( !m_uidRoleName.isEmpty() )
            {
                const QString key = m_indexByUid.key( item, emptyStr() );
                if( !key.isEmpty() )
                {
                    m_indexByUid.remove( key );
                }
                const QString value = item->property( m_uidRoleName ).toString();
                if( !value.isEmpty() )
                {
                    m_indexByUid.insert( value, item );
                }
            }
        }
    }
    void dereferenceItem( ItemType *item )
    {
        if( item != Q_NULLPTR )
        {
            disconnect( this, Q_NULLPTR, item, Q_NULLPTR );
            disconnect( item, Q_NULLPTR, this, Q_NULLPTR );
            if( !m_uidRoleName.isEmpty() )
            {
                const QString key = m_indexByUid.key( item, emptyStr() );
                if( !key.isEmpty() )
                {
                    m_indexByUid.remove( key );
                }
            }
            if( item->parent() == this )
            {
                // FIXME : maybe that's not the best way to test ownership ?
                item->deleteLater();
            }
        }
    }
    void onItemPropertyChanged( void )
    {
        ItemType *item = qobject_cast<ItemType *> ( sender() );
        const int row = m_items.indexOf( item );
        const int sig = senderSignalIndex();
        const int role = m_signalIdxToRole.value( sig, -1 );
        if( row >= 0 && role >= 0 )
        {
            QModelIndex index = QAbstractListModel::index( row, 0, noParent() );
            QVector<int> rolesList;
            rolesList.append( role );
            if( m_roles.value( role ) == m_dispRoleName )
            {
                rolesList.append( Qt::DisplayRole );
            }
            emit dataChanged( index, index, rolesList );
        }
        if( !m_uidRoleName.isEmpty() )
        {
            const QByteArray roleName = m_roles.value( role, emptyBA() );
            if( !roleName.isEmpty() && roleName == m_uidRoleName )
            {
                const QString key = m_indexByUid.key( item, emptyStr() );
                if( !key.isEmpty() )
                {
                    m_indexByUid.remove( key );
                }
                const QString value = item->property( m_uidRoleName ).toString();
                if( !value.isEmpty() )
                {
                    m_indexByUid.insert( value, item );
                }
            }
        }
    }
    inline void updateCounter( void )
    {
        if( m_count != m_items.count() )
        {
            m_count = m_items.count();
            emit countChanged();
        }
    }


    void giveGenericTimerData()
    {
        if( m_genericTimer.data() )
        {
            m_genericTimer.clear();
        }

        m_genericTimer = QSharedPointer<QTimer>( new QTimer() );
        m_genericTimer.data()->setSingleShot( m_singleShot );

        connect( m_genericTimer.data(), SIGNAL( timeout() )
                 , this, SIGNAL( timeout() ) );
    }


private: // data members
    int m_count;
    QByteArray m_uidRoleName;
    QByteArray m_dispRoleName;
    QMetaObject m_metaObj;
    QMetaMethod m_handler;
    QHash<int, QByteArray> m_roles;
    QHash<int, int> m_signalIdxToRole;
    QList<ItemType *> m_items;
    QHash<QString, ItemType *> m_indexByUid;

    // This is the generic timers.  They can be used
    // for database timing or simulation ect
    int m_interval;
    bool m_singleShot;
    QSharedPointer<QTimer> m_genericTimer;
};

#define QML_OBJMODEL_PROPERTY(type, name) \
    protected: Q_PROPERTY (QQmlObjectListModelBase * name READ get_##name CONSTANT) \
    private: QQmlObjectListModel<type> * m_##name; \
    public: QQmlObjectListModel<type> * get_##name (void) const { return m_##name; } \
    private:

#endif // QQMLOBJECTLISTMODEL_H

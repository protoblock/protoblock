#include "pnlmodel.h"
#include "../tempapi.h"


// Set the propertys for the roles of the model
PnlPropertys::PnlPropertys(
        const QString &fantasyName,
        const QString &playerId,
        const qint32 &qty,
        const qint32 &price,
        const qint32 &pnl,
        const int &week,
        const QString &gameId
        ):
        m_fantasyName(fantasyName),
        m_playerId(playerId),
        m_qty(qty),
        m_price(price),
        m_pnl(pnl),
        m_week(week),
        m_gameId(gameId)
{


}



//Model Side of things
PNLModel::PNLModel(QObject *parent)
    : QAbstractListModel(parent),
      m_gameId("NULL")
{
    connect (this,SIGNAL (weekChanged ()) , this,SLOT(initModel()));
    connect (this,SIGNAL (gameIdChanged ()) , this,SLOT(initModel()));
}

QString PNLModel::gameId() const
{
    return m_gameId;
}

void PNLModel::setGameId(const QString &gameId)
{
    if( m_gameId == gameId)
        return;
    m_gameId = gameId;
    emit gameIdChanged ();

}

int PNLModel::week() const
{
    return m_week;
}

void PNLModel::setWeek(const int &week)
{
    if( m_week == week)
        return;
    m_week = week;
    weekChanged ();

}

void PNLModel::clearModel()
{
    beginResetModel ();
    m_pnlPropertys.clear ();
    endResetModel ();
}

void PNLModel::initModel()
{
    if(m_pnlPropertys.length () >= 1 ){
        clearModel ();
    }
    emit running();
    for (auto &it :fantasybit::TempApi::m_pnlItem) {
        if (it.gameId  == m_gameId)
        {
            addToModel (
                        PnlPropertys(
                            it.fantasyName,
                            it.playerId,
                            it.qty,
                            it.price,
                            it.pnl,
                            it.week,
                            it.gameId
                            )
                        ) ;
        }
    }
    emit done ();
}

// maybe we should pass this in from the api utils
void PNLModel::addToModel(const PnlPropertys &pnlPropertys)
{
    beginInsertRows (QModelIndex() , rowCount (),rowCount () );
    m_pnlPropertys << pnlPropertys;
    endInsertRows ();
}

int PNLModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_pnlPropertys.count ();
}

QVariant PNLModel::data(const QModelIndex &index, int role) const
{
    if (index.row () < 0 || index.row () >= m_pnlPropertys.count ())
        return QVariant();
    const PnlPropertys &pnlProp = m_pnlPropertys[index.row()];
    if (role == FantasyName)
        return pnlProp.fantasyName ();
    else if (role == PlayerId)
        return pnlProp.playerId ();
    else if ( role == Qty)
        return pnlProp.qty ();
    else if ( role == Price)
        return pnlProp.price ();
    else if ( role == Pnl)
        return pnlProp.pnl ();
    else if ( role == Week)
        return pnlProp.week ();
    else if ( role == GameId )
        return pnlProp.gameId ();
    return QVariant();
}

QHash<int, QByteArray> PNLModel::roleNames() const
{
    QHash<int,QByteArray> roles;
    roles[FantasyName] = "fantasyName";
    roles[PlayerId] = "playerId";
    roles[Qty] =  "qty";
    roles[Price] = "price";
    roles[Pnl] = "pnl";
    roles[Week] = "week";
    roles[GameId] = "gameId";
    return roles;
}

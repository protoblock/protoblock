#include "leaderboardmodel.h"
#include "../tempapi.h"

// Set the propertys for the roles of the model
LeaderBoardItems::LeaderBoardItems(
        const QString &fantasyName,
        const QString &publicKey,
        const quint64 &stakeBalance,
        const qint64 &skillBalance):
    m_fantasyName(fantasyName),
    m_publicKey(publicKey),
    m_stakeBalance(stakeBalance),
    m_skillBalance(skillBalance)
{

}

QString LeaderBoardItems::fantasyName() const
{
    return m_fantasyName;
}

QString LeaderBoardItems::publicKey() const
{
    return m_publicKey;
}

quint64 LeaderBoardItems::stakeBalance() const
{
    return m_stakeBalance;
}

qint64 LeaderBoardItems::skillBalance() const
{
    return m_skillBalance;
}


//Model Side of things
LeaderBoardModel::LeaderBoardModel(QObject *parent)
    : QAbstractListModel(parent)

{
    // Init here
    initModel ();
}

void LeaderBoardModel::initModel()
{

#ifdef JAY_TRY
    beginInsertRows (QModelIndex() , rowCount (),rowCount () );
    //m_leaderboardItem << leaderBoardItem;
    endInsertRows ();
    return;
#else
    for (auto &it :fantasybit::TempApi::mLeaderBoard) {
      addToModel (LeaderBoardItems(
                      it.fantasyName,
                      it.publicKey,
                      it.stakeBalance,
                      it.skillBalance)) ;
    }
#endif
}

// maybe we should pass this in from the api utils
void LeaderBoardModel::addToModel(const LeaderBoardItems &leaderBoardItem)
{
#ifndef JAY_TRY
    beginInsertRows (QModelIndex() , rowCount (),rowCount () );
    m_leaderboardItem << leaderBoardItem;
    endInsertRows ();
#endif
}

int LeaderBoardModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
#ifdef JAY_TRY
    return fantasybit::TempApi::mLeaderBoard.count();
#else
    return m_leaderboardItem.count ();
#endif
}

QVariant LeaderBoardModel::data(const QModelIndex &index, int role) const
{

#ifdef JAY_TRY
    if (index.row () < 0 || index.row () >= rowCount())
        return QVariant();
    const fantasybit::LeaderBoardItem &leaderBoardItem = fantasybit::TempApi::mLeaderBoard[index.row()];
    if (role == FantasyName)
        return leaderBoardItem.fantasyName;
    else if (role == PublicKey)
        return leaderBoardItem.publicKey;
    else if (role == StakeBalance)
        return leaderBoardItem.stakeBalance;
    else if (role == SkillBalance)
        return leaderBoardItem.skillBalance;

    return QVariant();
#else

    if (index.row () < 0 || index.row () >= m_leaderboardItem.count ())
        return QVariant();
    const LeaderBoardItems &leaderBoardItem = m_leaderboardItem[index.row()];
    if (role == FantasyName)
        return leaderBoardItem.fantasyName();
    else if (role == PublicKey)
        return leaderBoardItem.publicKey ();
    else if (role == StakeBalance)
        return leaderBoardItem.stakeBalance ();
    else if (role == SkillBalance)
        return leaderBoardItem.skillBalance ();

    return QVariant();
#endif
}

QHash<int, QByteArray> LeaderBoardModel::roleNames() const
{
    QHash<int,QByteArray> roles;
    roles[FantasyName] = "fantasyName";
    roles[PublicKey] = "pubKey";
    roles[StakeBalance] = "stateBal";
    roles[SkillBalance] = "skillBal";
    return roles;
}

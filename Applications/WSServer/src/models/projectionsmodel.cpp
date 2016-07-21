#include "projectionsmodel.h"
#include "../tempapi.h"


// Set the propertys for the roles of the model
ProjectionsItems::ProjectionsItems(
        const QString &fantasyName,
        const QString &playerId,
        const qint32 &projections
        ):
    m_fantasyName(fantasyName),
    m_playerId(playerId),
    m_projection(projections)
{

}

QString ProjectionsItems::fantasyName() const { return m_fantasyName; }
QString ProjectionsItems::playerId () const { return m_playerId; }
qint32 ProjectionsItems::projection ()const { return m_projection; }




//Model Side of things
ProjectionsModel::ProjectionsModel(QObject *parent)
    : QAbstractListModel(parent)

{
    // Init here
    initModel ();
}

void ProjectionsModel::initModel()
{
    for (auto &it :fantasybit::TempApi::mProjections) {
      addToModel (ProjectionsItems(
                      it.fantasyName,
                      it.playerId,
                      it.projection)) ;
    }
}

// maybe we should pass this in from the api utils
void ProjectionsModel::addToModel(const ProjectionsItems &projectionItem)
{
    beginInsertRows (QModelIndex() , rowCount (),rowCount () );
    m_projectionItem << projectionItem;
    endInsertRows ();
}

int ProjectionsModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_projectionItem.count ();
}

QVariant ProjectionsModel::data(const QModelIndex &index, int role) const
{
    if (index.row () < 0 || index.row () >= m_projectionItem.count ())
        return QVariant();
    const ProjectionsItems &projectionItem = m_projectionItem[index.row()];
    if (role == FantasyName)
        return projectionItem.fantasyName ();
    else if (role == PlayerId)
        return projectionItem.playerId ();
    else if (role == Projection)
        return projectionItem.projection ();

    return QVariant();
}

QHash<int, QByteArray> ProjectionsModel::roleNames() const
{
    QHash<int,QByteArray> roles;
    roles[FantasyName] = "fantasyName";
    roles[PlayerId] = "playerId";
    roles[Projection] = "projection";
    return roles;
}

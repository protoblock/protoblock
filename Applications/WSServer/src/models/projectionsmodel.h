#include <QObject>
#include <QAbstractListModel>
#include <QString>


class ProjectionsItems
{
public:
        ProjectionsItems(
                const QString &fantasyName,
                const QString &playerId,
                const qint32 &projection
                );

        QString fantasyName() const;
        QString playerId()const;
        qint32 projection()const;

private:
        QString m_fantasyName;
        QString m_playerId;
        qint32 m_projection;
};

class ProjectionsModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit ProjectionsModel(QObject *parent = 0);

    enum ProjectionsRoles{
        FantasyName,
        PlayerId,
        Projection,
    };

    void initModel();
    void addToModel(const ProjectionsItems &projectionsItem) ;
    int rowCount (const QModelIndex &parent = QModelIndex()) const ;
    QVariant data(const QModelIndex &index , int role = Qt::DisplayRole) const;


protected:
  QHash<int, QByteArray> roleNames()const ;
private:
    QList<ProjectionsItems> m_projectionItem;
};

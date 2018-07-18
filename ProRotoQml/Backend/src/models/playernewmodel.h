#ifndef PLAYERNEWMODEL_H
#define PLAYERNEWMODEL_H
#include <QObject>
#include <QString>
#include "../QmlSupermacros/QQmlConstRefPropertyHelpers.h"
//#include "src/tempapi.h"
#include "../QmlModels/QQmlObjectListModel.h"

class PlayerNewModelItem : public QObject {
    Q_OBJECT
    QML_CONSTANT_CSTREF_PROPERTY (QString, firstName)
    QML_CONSTANT_CSTREF_PROPERTY (QString, lastName)
    QML_CONSTANT_CSTREF_PROPERTY (QString, position)
    QML_CONSTANT_CSTREF_PROPERTY (QString, team)
    QML_CONSTANT_CSTREF_PROPERTY (QString, playerStatus)
    QML_CONSTANT_CSTREF_PROPERTY (QString, playerId)

public:

    explicit PlayerNewModelItem(
            const QString &firstName,
            const QString &lastName,
            const QString &position,
            const QString &team,
            const QString &playerStatus,
            const QString &playerId,
            QObject * parent = Q_NULLPTR
            ):
        QObject    (parent),
        m_firstName(firstName),
        m_lastName(lastName),
        m_position(position),
        m_team(team),
        m_playerStatus(playerStatus),
        m_playerId(playerId)

    {

    }
};


class PlayerNewModel : public QQmlObjectListModel<PlayerNewModelItem>{};
//{


//public:
//    explicit PlayerNewModel (QObject *          parent      = Q_NULLPTR,
//                                  const QByteArray & displayRole = QByteArray (),
//                                  const QByteArray & uidRole     = QByteArray ())
//                    : QQmlObjectListModel<PlayerNewModelItem>(parent,displayRole,uidRole) {
//        fillAll();
//    }



//    void fillAll(){
////        for (auto &iter : fantasybit::TempApi::mPlayers) {
//            this->append (
//                        new PlayerNewModelItem (
//                            iter.firstName,
//                            iter.lastName,
//                            iter.position,
//                            iter.team,
//                            iter.playerStatus,
//                            iter.playerId
//                            )
////                        ) ;
////        }
//    }
//};
#endif // PLAYERNEWMODEL_H

#ifndef PLAYERSYMBOLSMODEL_H
#define PLAYERSYMBOLSMODEL_H

#include <QObject>
#include <QString>
#include "QQmlConstRefPropertyHelpers.h"
#include "QQmlObjectListModel.h"
#include "fbutils.h"

namespace pb {
using namespace fantasybit;

class PlayerSymbolsModelItem : public QObject {
    Q_OBJECT

    QML_READONLY_CSTREF_PROPERTY (QString, playerid)
    QML_READONLY_CSTREF_PROPERTY (QString, symbol)

    QML_READONLY_CSTREF_PROPERTY (QString, pos)
    QML_READONLY_CSTREF_PROPERTY (QString, lastname)
    QML_READONLY_CSTREF_PROPERTY (QString, firstname)
    QML_READONLY_CSTREF_PROPERTY (QString, teamid)
    QML_READONLY_CSTREF_PROPERTY (QString, fullname)
public:
    explicit PlayerSymbolsModelItem(const QString &in, const QString &in2)
        : m_playerid(in), m_symbol(in2), QObject(nullptr) {

        m_firstname = "first";
        m_lastname = "last";
    }

    void Update(const PlayerBase &pb) {
        m_pos = pb.position().data();
        m_firstname = pb.first().data();
        m_lastname = pb.last().data();
        m_fullname =  QString("%1, %2")
                .arg ( pb.last().data() )
                .arg ( pb.first().data());
    }

    void Update(const PlayerDetail &pd) {
        m_teamid = pd.team.data();
        Update(pd.base);
    }

};

class PlayerSymbolsModel : public QQmlObjectListModel<PlayerSymbolsModelItem>{
    Q_OBJECT

public:
    explicit PlayerSymbolsModel (QObject *          parent      = Q_NULLPTR,
                                 const QByteArray & displayRole = {"symbol"},
                                  const QByteArray & uidRole     = {"symbol"})
        : QQmlObjectListModel (parent,displayRole,uidRole) {}

    QString Update(const QString &syb, const PlayerDetail &pd) {
        auto *item = this->getByUid(syb);
        item->Update(pd);
        return item->get_fullname();
    }


    void Update(const std::vector<pb::GameRoster> &inrosters) {
        for(const pb::GameRoster & game  : inrosters) {
            //add home players
            for (auto ha : {QString("home"),QString("away")}) {
                for(const auto& player : (ha == QString("home")) ? game.homeroster : game.awayroster) {
                    if ( player.second.symbol == "" ) {
                        qWarning() << " bad data" << player.second.symbol.data() << player.first.data();
                        continue;
                    }
                    auto *item = this->getByUid(player.second.symbol.data());
                    if ( !item ) {
                        qWarning() << " extected to have this already" << player.second.symbol.data() << player.first.data();
                        item = new PlayerSymbolsModelItem(player.first.data(),
                                                          player.second.symbol.data());
                        append(item);
                    }
                    item->Update(player.second);
                }
            }
        }
    }

};

}

using namespace pb;
Q_DECLARE_METATYPE(PlayerSymbolsModel*)
Q_DECLARE_METATYPE(PlayerSymbolsModelItem*)

#endif

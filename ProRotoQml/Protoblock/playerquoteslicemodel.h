#ifndef PLAYERQUOTESLICEMODEL_H
#define PLAYERQUOTESLICEMODEL_H

#include <QObject>
#include <QString>
#include "../QmlSupermacros/QQmlConstRefPropertyHelpers.h"
#include "../QmlModels/QQmlObjectListModel.h"
#include "StateData.pb.h"

class PlayerQuoteSliceModelItem : public QObject {
    Q_OBJECT
    QML_CONSTANT_CSTREF_PROPERTY (qint32, lastprice)
    QML_CONSTANT_CSTREF_PROPERTY (QString, position)
    QML_CONSTANT_CSTREF_PROPERTY (QString, lastname)
    QML_CONSTANT_CSTREF_PROPERTY (QString, firstname)
    QML_CONSTANT_CSTREF_PROPERTY (QString, team_id)
    QML_CONSTANT_CSTREF_PROPERTY (qint32, bidsize)
    QML_CONSTANT_CSTREF_PROPERTY (qint32, bid)
    QML_CONSTANT_CSTREF_PROPERTY (qint32, ask)
    QML_CONSTANT_CSTREF_PROPERTY (qint32, asksize)
    QML_CONSTANT_CSTREF_PROPERTY (qint32, volume)
    QML_CONSTANT_CSTREF_PROPERTY (qint32, change)
    QML_CONSTANT_CSTREF_PROPERTY (qint32, updown)
    QML_CONSTANT_CSTREF_PROPERTY (qint32, hi)
    QML_CONSTANT_CSTREF_PROPERTY (qint32, lo)


public:

    explicit PlayerQuoteSliceModelItem(const fantasybit::ROWMarket &in) :  QObject(nullptr) {
        m_lastprice = in.quote().l();
        m_position = in.playerdata().player_base().position().data();
        m_firstname = in.playerdata().player_base().first().data();
        m_lastname = in.playerdata().player_base().last().data();
        m_team_id = in.playerdata().player_status().teamid().data();
        m_bidsize = in.quote().bs();
        m_bid = in.quote().b();
        m_ask = in.quote().a();
        m_asksize = in.quote().as();
        m_volume = in.ohlc().volume();
        m_change = in.ohlc().change();
        m_updown = in.quote().udn();
        m_hi = in.ohlc().high();
        m_lo = in.ohlc().low();
    }
//            const QString &firstName,
//            const QString &lastName,
//            const QString &position,
//            const QString &team,
//            const QString &playerStatus,
//            const QString &playerId,
//            QObject * parent = Q_NULLPTR
//            ):
//        QObject    (parent),
//        m_firstName(firstName),
//        m_lastName(lastName),
//        m_position(position),
//        m_team(team),
//        m_playerStatus(playerStatus),
//        m_playerId(playerId)

};


class PlayerQuoteSliceModel : public QQmlObjectListModel<PlayerQuoteSliceModelItem>{};

Q_DECLARE_METATYPE(PlayerQuoteSliceModel*)

//message ContractOHLC {
//    optional string symbol = 10;
//    optional int32 open = 20;
//    optional int32 high = 30;
//    optional int32 low = 40;
//    optional int32 close = 50;
//    optional int32 volume = 60;
//    optional int32 change = 70;
//}


//message MarketQuote {
//    optional int32 bs = 10;
//    optional int32 b = 20;
//    optional int32 a = 30;
//    optional int32 as = 40;
//    optional int32 l = 50;
//    optional int32 ls = 60;
//    optional int32 udn = 70;
//}



//if( column ==i++)
//    return data->propertyValue<PropertyNames::LAST>();
//if( column ==  i++)
//    return data->propertyValue<PropertyNames::Position>();
//if( column == i++)
//    return data->propertyValue<PropertyNames::Player_Name>();
//if( column == i++)
//    return data->propertyValue<PropertyNames::Team_ID>();
//if( column ==i++)
//    return data->propertyValue<PropertyNames::BIDSIZE>();
//if( column ==i++)
//    return data->propertyValue<PropertyNames::BID>();
//if( column ==i++)
//    return data->propertyValue<PropertyNames::ASK>();
//if( column ==i++)
//    return data->propertyValue<PropertyNames::ASKSIZE>();
//if( column ==i++)
//    return data->propertyValue<PropertyNames::VOLUME>();
//if( column == i++)
//    return data->propertyValue<PropertyNames::CHANGE>();


//#endif // PLAYERNEWMODEL_H


#endif // PLAYERQUOTESLICEMODEL_H

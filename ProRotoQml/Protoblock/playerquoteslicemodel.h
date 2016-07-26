#ifndef PLAYERQUOTESLICEMODEL_H
#define PLAYERQUOTESLICEMODEL_H

#include <QObject>
#include <QString>
#include "../QmlSupermacros/QQmlConstRefPropertyHelpers.h"
#include "../QmlModels/QQmlObjectListModel.h"

class PlayerQuoteSliceModelItem : public QObject {
    Q_OBJECT
    QML_CONSTANT_CSTREF_PROPERTY (QString, LAST)
    QML_CONSTANT_CSTREF_PROPERTY (QString, Position)
    QML_CONSTANT_CSTREF_PROPERTY (QString, Player_Name)
    QML_CONSTANT_CSTREF_PROPERTY (QString, Team_ID)
    QML_CONSTANT_CSTREF_PROPERTY (QString, BIDSIZE)
    QML_CONSTANT_CSTREF_PROPERTY (QString, BID)
    QML_CONSTANT_CSTREF_PROPERTY (QString, ASK)
    QML_CONSTANT_CSTREF_PROPERTY (QString, ASKSIZE)
    QML_CONSTANT_CSTREF_PROPERTY (QString, VOLUME)
    QML_CONSTANT_CSTREF_PROPERTY (QString, CHANGE)
    QML_CONSTANT_CSTREF_PROPERTY (QString, UPDOWN)
    QML_CONSTANT_CSTREF_PROPERTY (QString, HI)
    QML_CONSTANT_CSTREF_PROPERTY (QString, LO)


public:

    explicit PlayerQuoteSliceModelItem() {
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


    }
};


class PlayerQuoteSliceModel : public QQmlObjectListModel<PlayerQuoteSliceModelItem>{};



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

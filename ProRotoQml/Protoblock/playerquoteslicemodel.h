#ifndef PLAYERQUOTESLICEMODEL_H
#define PLAYERQUOTESLICEMODEL_H

#include <QObject>
#include <QString>
#include "../QmlSupermacros/QQmlConstRefPropertyHelpers.h"
#include "../QmlModels/QQmlObjectListModel.h"
#include "StateData.pb.h"

class PlayerQuoteSliceModelItem : public QObject {
    Q_OBJECT
    QML_READONLY_CSTREF_PROPERTY (qint32, lastprice)
    QML_READONLY_CSTREF_PROPERTY (QString, position)
    QML_READONLY_CSTREF_PROPERTY (QString, lastname)
    QML_READONLY_CSTREF_PROPERTY (QString, firstname)
    QML_READONLY_CSTREF_PROPERTY (QString, team_id)
    QML_READONLY_CSTREF_PROPERTY (qint32, bidsize)
    QML_READONLY_CSTREF_PROPERTY (QString , fullname)
    QML_READONLY_CSTREF_PROPERTY (qint32, bid)
    QML_READONLY_CSTREF_PROPERTY (qint32, ask)
    QML_READONLY_CSTREF_PROPERTY (qint32, asksize)
    QML_READONLY_CSTREF_PROPERTY (qint32, volume)
    QML_READONLY_CSTREF_PROPERTY (qint32, change)
    QML_READONLY_CSTREF_PROPERTY (qint32, updown)
    QML_READONLY_CSTREF_PROPERTY (qint32, hi)
    QML_READONLY_CSTREF_PROPERTY (qint32, lo)
    QML_READONLY_CSTREF_PROPERTY (QString, playerid)
    QML_READONLY_CSTREF_PROPERTY (QString, symbol)


public:

    explicit PlayerQuoteSliceModelItem(const fantasybit::ROWMarket &in) :  QObject(nullptr) {
        m_lastprice = in.quote().l();
        m_position = in.playerdata().player_base().position().data();
        m_firstname = in.playerdata().player_base().first().data();
        m_lastname = in.playerdata().player_base().last().data();
        m_team_id = in.playerdata().player_status().teamid().data();
        m_fullname =  QString("%1 %2")
                .arg ( in.playerdata ().player_base ().first ().data () )
                .arg ( in.playerdata ().player_base ().last ().data () );
        m_bidsize = in.quote().bs();
        m_bid = in.quote().b();
        m_ask = in.quote().a();
        m_asksize = in.quote().as();
        m_volume = in.ohlc().volume();
        m_change = in.ohlc().change();
        m_updown = in.quote().udn();
        m_hi = in.ohlc().high();
        m_lo = in.ohlc().low();
        m_playerid = in.pid().data();
        m_symbol = in.pid().data(); //TODO
    }

    void setProperties(const fantasybit::ROWMarket &in)   {
       setplayerid(in.pid().data());
       setsymbol(in.pid().data()); //TODO
       setlastprice(in.quote().l());
       setposition(in.playerdata().player_base().position().data());

       setlastname(in.playerdata().player_base().last().data());
       setfirstname(in.playerdata().player_base().first().data());
       setteam_id(in.playerdata().player_status().teamid().data());
       setbidsize(in.quote().bs());
       setfullname(QString("%1 %2")
                   .arg ( in.playerdata ().player_base ().first ().data () )
                   .arg ( in.playerdata ().player_base ().last ().data () ));
       setbid(in.quote().b());
       setask(in.quote().a());
       setasksize(in.quote().as());
       setvolume(in.ohlc().volume());
       setchange(in.ohlc().change());
       setupdown(in.quote().udn());
       sethi(in.ohlc().high());
       setlo(in.ohlc().low());

    }
};


class PlayerQuoteSliceModel : public QQmlObjectListModel<PlayerQuoteSliceModelItem>{};

Q_DECLARE_METATYPE(PlayerQuoteSliceModel*)
Q_DECLARE_METATYPE(PlayerQuoteSliceModelItem*)

//https://github.com/mkawserm/ModelsTest/blob/master/main.cpp


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


#endif // PLAYERQUOTESLICEMODEL_H

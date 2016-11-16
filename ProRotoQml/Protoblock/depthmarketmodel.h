#ifndef DEPTHMARKETMODEL_H
#define DEPTHMARKETMODEL_H

#include <QObject>
#include <QString>
#include "../QmlSupermacros/QQmlConstRefPropertyHelpers.h"
#include "../QmlModels/QQmlObjectListModel.h"
#include "../QmlSuperMacros/QQmlHelpersCommon.h"
#include "StateData.pb.h"
#include <QQmlHelpersCommon.h>
#include "QQmlPtrPropertyHelpers.h"
#include "playerquoteslicemodel.h"

namespace pb {
using namespace fantasybit;

class DepthMarketModelItem : public QObject {
    Q_OBJECT
    QML_CONSTANT_CSTREF_PROPERTY (qint32, bidsize)
    QML_CONSTANT_CSTREF_PROPERTY (qint32, bid)
    QML_CONSTANT_CSTREF_PROPERTY (qint32, ask)
    QML_CONSTANT_CSTREF_PROPERTY (qint32, asksize)

public:

    explicit DepthMarketModelItem(const fantasybit::DepthItem &in) :  QObject(nullptr) {
        m_bidsize = in.bs();
        m_bid = in.b();
        m_ask = in.a();
        m_asksize = in.as();
    }
};


class DepthMarketModel : public QQmlObjectListModel<DepthMarketModelItem> {
    Q_OBJECT
    QML_READONLY_CSTREF_PROPERTY(QString, playerid)
    QML_READONLY_PTR_PROPERTY(PlayerQuoteSliceModelItem, pPlayerQuoteSliceModelItem)

public:
    explicit DepthMarketModel() :  m_pPlayerQuoteSliceModelItem(nullptr) {}
    void updateFullDepth(const GetDepthRep &depthrep) {
        setplayerid(depthrep.pid().data());
        if ( depthrep.has_rowmarket() ) {
            if ( m_pPlayerQuoteSliceModelItem == nullptr)
                update_pPlayerQuoteSliceModelItem(new PlayerQuoteSliceModelItem(depthrep.rowmarket()) );
            else {
                m_pPlayerQuoteSliceModelItem->setProperties(depthrep.rowmarket());
//                emit pPlayerQuoteSliceModelItemChanged(m_pPlayerQuoteSliceModelItem);
            }
//            setplayerQuoteSliceModelItem(PlayerQuoteSliceModelItem(depthrep.rowmarket()));
        }
        clear();

        for ( auto &di : depthrep.depthitems())
            append(new DepthMarketModelItem(di));
    }
};

Q_DECLARE_METATYPE(DepthMarketModel*)

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

}
#endif // DEPTHMARKETMODEL_H

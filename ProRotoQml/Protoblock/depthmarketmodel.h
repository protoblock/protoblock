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
//#include "playerquoteslicemodel.h"

namespace pb {
using namespace fantasybit;

class DepthMarketModelItem : public QObject {
    Q_OBJECT
    QML_READONLY_CSTREF_PROPERTY_INIT0 (qint32, bidsize)
    QML_READONLY_CSTREF_PROPERTY_INIT0 (qint32, bid)
    QML_READONLY_CSTREF_PROPERTY_INIT0 (qint32, ask)
    QML_READONLY_CSTREF_PROPERTY_INIT0 (qint32, asksize)

public:
    DepthMarketModelItem() :  QObject(nullptr) {}

    explicit DepthMarketModelItem(const fantasybit::DepthItem &in) :  QObject(nullptr) {
        m_bidsize = in.bs();
        m_bid = in.b();
        m_ask = in.a();
        m_asksize = in.as();
    }

    explicit DepthMarketModelItem(int bs, int b, int a, int as) :  QObject(nullptr) {
        m_bidsize = bs;
        m_bid = b;
        m_ask = a;
        m_asksize = as;
    }
};


class DepthMarketModel : public QQmlObjectListModel<DepthMarketModelItem> {
    Q_OBJECT
//    QML_READONLY_CSTREF_PROPERTY(QString, playerid)
//    QML_READONLY_PTR_PROPERTY(PlayerQuoteSliceModelItem, pPlayerQuoteSliceModelItem)

public:
    explicit DepthMarketModel (QObject *          parent      = Q_NULLPTR,
                                  const QByteArray & displayRole = QByteArray (),
                                  const QByteArray & uidRole     = QByteArray ())
        : QQmlObjectListModel (parent,displayRole,uidRole) {}
//    explicit DepthMarketModel() :  m_pPlayerQuoteSliceModelItem(nullptr) {}
//    void updateFullDepth(const GetDepthRep &depthrep) {
//        setplayerid(depthrep.pid().data());
//        if ( depthrep.has_rowmarket() ) {
//            if ( m_pPlayerQuoteSliceModelItem == nullptr)
//                update_pPlayerQuoteSliceModelItem(new PlayerQuoteSliceModelItem(depthrep.rowmarket()) );
//            else {
//                m_pPlayerQuoteSliceModelItem->setProperties(depthrep.rowmarket());
////                emit pPlayerQuoteSliceModelItemChanged(m_pPlayerQuoteSliceModelItem);
//            }
////            setplayerQuoteSliceModelItem(PlayerQuoteSliceModelItem(depthrep.rowmarket()));
//        }
//        clear();

//        for ( auto &di : depthrep.depthitems())
//            append(new DepthMarketModelItem(di));
//    }
    void snapDepth(const fantasybit::MarketSnapshot* mt) {
        clear();
        for ( auto &di : mt->depth() ) {
            append(new DepthMarketModelItem(di));
        }
    }

    void Update(fantasybit::DepthFeedDelta* dfd) {
        const QList<DepthMarketModelItem *> &book = toList();
        auto bsize = count();
    //    depths->depthitems().iterator
//        this->count()
        if ( dfd->isbid() ) {
            bool nopush = false;
            if ( bsize > 0 ) {
                if ( book[bsize-1]->get_bid() == 0)
                    nopush = true;;
            }

            for ( int i =0; i<bsize; i++) {
                if ( book[i]->get_bid() == 0 ) {
                    if ( dfd->size() > 0 ) {
                        book[i]->setbid(dfd->price());
                        book[i]->setbidsize(dfd->size());
                    }
                    break;
                }
                else if ( dfd->price() < book[i]->get_bid())  {
                    if ( i < bsize-1)
                        continue;

                    if ( dfd->size() > 0) {
                        DepthMarketModelItem *bi = new DepthMarketModelItem();
                        append(bi);
                        //book = toList();
                        bsize = count();
                        bi->setask(0);
                        bi->setasksize(0);
                        bi->setbid(dfd->price());
                        bi->setbidsize(dfd->size());
                    }
                    break;
                }
                else if ( dfd->price() > book[i]->get_bid() ) {
                    if ( dfd->size() > 0) {
                        int end = bsize-1;
                        if ( nopush )
                            ;//end = bsize-1;
                        else {
                            //end = bsize-2;
                            DepthMarketModelItem *bi = new DepthMarketModelItem();
                            append(bi);
                            //book = toList();
                            bsize = count();
                            bi->setask(0);
                            bi->setasksize(0);
                            bi->setbid(book[bsize-2]->get_bid());
                            bi->setbidsize(book[bsize-2]->get_bidsize());
                        }
                        for (int j=end;j > i;--j) {
                            if ( nopush && book[j-1]->get_bid() != 0 )
                                nopush = false;

                            if ( !nopush ) {
                                book[j]->setbid(book[j-1]->get_bid());
                                book[j]->setbidsize(book[j-1]->get_bidsize());
                            }
                        }

                        book[i]->setbid(dfd->price());
                        book[i]->setbidsize(dfd->size());
                    }

                    break;
                }
                else {
                    if ( dfd->size() > 0 )
                        book[i]->setbidsize(dfd->size());
                    else {
                        int j=i;
                        for (;
                              j<bsize-1 && book[j]->get_bid() > 0;
                              ++j) {
                            book[j]->setbid(book[j+1]->get_bid());
                            book[j]->setbidsize(book[j+1]->get_bidsize());
                        }
                        book[j]->setbid(0);
                        book[j]->setbidsize(0);
                    }

                    break;
                }
            }
        }
        else //!isbid
        {
            bool nopush = false;
            if ( bsize > 0 ) {
                if ( book[bsize-1]->get_ask() == 0)
                    nopush = true;
            }

            for ( int i =0; i<bsize; i++) {
                if ( book[i]->get_ask()== 0 ) {
                    if ( dfd->size() > 0 ) {
                        book[i]->setask(dfd->price());
                        book[i]->setasksize(dfd->size());
                    }
                    break;
                }
                else if ( dfd->price() > book[i]->get_ask())  {
                    if ( i < bsize-1)
                        continue;

                    if ( dfd->size() > 0) {
                        DepthMarketModelItem *bi = new DepthMarketModelItem();
                        append(bi);
                        //book = toList();
                        bsize = count();
                        bi->setbid(0);
                        bi->setbidsize(0);
                        bi->setask(dfd->price());
                        bi->setasksize(dfd->size());
                    }
                    break;
                }
                else if ( dfd->price() < book[i]->get_ask()) {
                    if ( dfd->size() > 0) {
                        int end = bsize-1;
                        if ( nopush )
                            ;//end = dfd->size()-1;
                        else {
                            //end = dfd->size()-2;
                            DepthMarketModelItem *bi = new DepthMarketModelItem();
                            append(bi);
                            //book = toList();
                            bsize = count();
                            bi->setbid(0);
                            bi->setbidsize(0);
                            bi->setask(book[bsize-2]->get_ask());
                            bi->setasksize(book[bsize-2]->get_asksize());
                        }
                        for (int j=end;j > i;--j) {
                            if ( nopush && book[j-1]->get_ask()!= 0 )
                                nopush = false;

                            if ( !nopush ) {
                                book[j]->setask(book[j-1]->get_ask());
                                book[j]->setasksize(book[j-1]->get_asksize());
                            }
                        }

                        book[i]->setask(dfd->price());
                        book[i]->setasksize(dfd->size());
                    }
                    break;
                }
                else {
                    if ( dfd->size() > 0 )
                        book[i]->setasksize(dfd->size());
                    else {
                        int j=i;
                        for (;
                              j<bsize-1 && book[j]->get_ask()> 0;
                              ++j) {
                           book[j]->setask(book[j+1]->get_ask());
                           book[j]->setasksize(book[j+1]->get_asksize());
                        }
                        book[j]->setask(0);
                        book[j]->setasksize(0);
                    }
                    break;
                }
            }
        }

        if (bsize == 0 && dfd->size() >0 && dfd->price() > 0) {
            DepthMarketModelItem *bi = new DepthMarketModelItem();
            append(bi);
            //book = toList();
            bsize = count();
            if ( dfd->isbid() ) {
                bi->setask(0);
                bi->setasksize(0);
                bi->setbid(dfd->price());
                bi->setbidsize(dfd->size());
            }
            else {
                bi->setbid(0);
                bi->setbidsize(0);
                bi->setask(dfd->price());
                bi->setasksize(dfd->size());
            }
        }
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

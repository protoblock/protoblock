#ifndef OPENORDERSMODEL_H
#define OPENORDERSMODEL_H

#include <QObject>
#include <QString>
#include "../QmlSupermacros/QQmlConstRefPropertyHelpers.h"
#include "../QmlModels/QQmlObjectListModel.h"
#include "../QmlSuperMacros/QQmlHelpersCommon.h"
#include "StateData.pb.h"
#include <QQmlHelpersCommon.h>
#include "QQmlPtrPropertyHelpers.h"
#include "ExchangeData.h"

using namespace fantasybit;

class OpenOrdersModelItem : public QObject {
    Q_OBJECT
    QML_READONLY_CSTREF_PROPERTY (QString, symbol)
    QML_CONSTANT_CSTREF_PROPERTY (qint32, refnum)
    QML_CONSTANT_CSTREF_PROPERTY (bool,  isbuy)
    QML_CONSTANT_CSTREF_PROPERTY (qint32, size)
    QML_CONSTANT_CSTREF_PROPERTY (qint32, price)

public:

    explicit OpenOrdersModelItem(const fantasybit::Order &in,  QObject *parent = Q_NULLPTR)
                    :  QObject(parent) {
//        m_symbol = in.
        m_refnum = in.refnum();
        m_isbuy = in.core().buyside();
        m_size = in.core().size();
        m_price = in.core().price();
    }
};


class OpenOrdersModel : public QQmlObjectListModel<OpenOrdersModelItem> {
    Q_OBJECT
    QML_READONLY_CSTREF_PROPERTY(QString, pidsymbol)

public:
    explicit OpenOrdersModel (QObject *  parent  = Q_NULLPTR,
                              const QByteArray & displayRole = QByteArray (),
                              const QByteArray & uidRole     = {"refnum"}) :
                              QQmlObjectListModel<OpenOrdersModelItem>(parent,displayRole,uidRole)
    {}

//    void updateAllOrders(const AllOdersFname &allordersfn) {
//        setfantasyname(allordersfn.fname().data());

//        clear();

//        for ( auto &pio : allordersfn.pidorders())
//            for ( auto &ord : pio.orders() )
//                append(new OpenOrdersModelItem(pio.symbol(),ord));
//    }

    void Update(const std::vector<Order> &vo) {
        for ( auto &ord : vo) {
            auto *it = new OpenOrdersModelItem(ord,this);
            it->setsymbol(m_pidsymbol);
            append(it);
        }
    }
};


Q_DECLARE_METATYPE(OpenOrdersModel*)



class TradingPositionsModelItem : public QObject {
    Q_OBJECT

    OpenOrdersModel mOpenOrdersModel;
    QML_READONLY_CSTREF_PROPERTY (QString, symbol)
    QML_READONLY_CSTREF_PROPERTY (qint32, netprice)
    QML_READONLY_CSTREF_PROPERTY (qint32, netqty)
    QML_READONLY_CSTREF_PROPERTY (double, openpnl)
    QML_READONLY_CSTREF_PROPERTY (double, avgprice)

    QML_READONLY_PTR_PROPERTY(OpenOrdersModel, pOpenOrdersModel)
public:
    explicit TradingPositionsModelItem(const AllOdersSymbol &allordersymbol,
                                       QObject *parent = Q_NULLPTR)
                : m_symbol{allordersymbol.symbol().data()},
                  m_netprice{allordersymbol.netprice()},
                  m_netqty{allordersymbol.netqty()},
                  m_openpnl{allordersymbol.pnl()},
//                  mOpenOrdersModel(),
                  m_pOpenOrdersModel{nullptr},
                  m_avgprice(0.0),
                  QObject{parent}
    {
//        mOpenOrdersModel.setfantasyname(allordersymbol.fname().data());
//        for ( auto &pio : allordersymbol())

        if ( m_pOpenOrdersModel == nullptr )
            m_pOpenOrdersModel = new OpenOrdersModel();
        else
            m_pOpenOrdersModel->clear();

        m_pOpenOrdersModel->setpidsymbol(allordersymbol.symbol().data());
        for ( auto &ord : allordersymbol.orders() )
            if ( ord.refnum() != 0 )
                m_pOpenOrdersModel->append(new OpenOrdersModelItem(ord));

        if (allordersymbol.has_avg() ) setavgprice(allordersymbol.avg());

    }

    explicit TradingPositionsModelItem(const ordsnap_t::value_type &pidppvc,
                                 QObject *parent = Q_NULLPTR) :
                                mOpenOrdersModel{},
                                m_pOpenOrdersModel{&mOpenOrdersModel},
                                QObject{parent} {

        m_symbol = pidppvc.first.data();
        auto &mypair = pidppvc.second;
        auto &myorders = mypair.second;
        Update(mypair.first);
        mOpenOrdersModel.setpidsymbol(m_symbol);
        mOpenOrdersModel.Update(myorders);
    }


//    OpenOrdersModel mOpenOrdersModel;

    void Update(const Position &pos) {
        setnetqty(pos.netqty);
        setnetprice(pos.netprice);
        if ( pos.netqty == 0 ) {
            setopenpnl(pos.netqty * 100.0);
            setavgprice(0);
        }
        else {
            setopenpnl(0);
            setavgprice(pos.netprice / (pos.netqty * -1));
        }
    }
};


class TradingPositionsModel : public QQmlObjectListModel<TradingPositionsModelItem> {
    Q_OBJECT
//    QML_READONLY_CSTREF_PROPERTY(QString, symbol)
    QML_READONLY_CSTREF_PROPERTY(QString, fantasyname)
    QML_READONLY_CSTREF_PROPERTY (double, totalopenpnl)

public:
    explicit TradingPositionsModel (QObject *          parent      = Q_NULLPTR,
                                    const QByteArray & displayRole = QByteArray (),
                                    const QByteArray & uidRole     = QByteArray ()) :
                                    QQmlObjectListModel<TradingPositionsModelItem>
                                                    (parent,displayRole,uidRole)
    {}

    void updateAllOrders(const AllOdersFname &allordersfn) {
        clear();

        setfantasyname(allordersfn.fname().data());

        for ( auto &aos : allordersfn.pidorders() )
           append(new TradingPositionsModelItem(aos,this));
    }

    void updateAllOrders(const std::string &fname, const ordsnap_t &myorderpositions) {
        clear();
        m_totalopenpnl = 0.0;
        setfantasyname(fname.data());

        for ( auto p : myorderpositions ) {
          //  qDebug() << "level2 Trading SetMyPositions" << p.first << p.second;
            auto &mypair = p.second;
            auto &myorders = mypair.second;

            auto it = new TradingPositionsModelItem(p,this);
            append(it);
            m_totalopenpnl += it->get_openpnl();
        }
    }

//    void UpdatePnl(TradingPositionsModelItem *tit,PlayerQuoteSliceModelItem)

};

Q_DECLARE_METATYPE(TradingPositionsModel*)

#endif // OPENORDERSMODEL_H



























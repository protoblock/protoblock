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

using namespace fantasybit;

class OpenOrdersModelItem : public QObject {
    Q_OBJECT
    QML_CONSTANT_CSTREF_PROPERTY (QString, symbol)
    QML_CONSTANT_CSTREF_PROPERTY (qint32, refnum)
    QML_CONSTANT_CSTREF_PROPERTY (bool,  isbuy)
    QML_CONSTANT_CSTREF_PROPERTY (qint32, size)
    QML_CONSTANT_CSTREF_PROPERTY (qint32, price)

public:

    explicit OpenOrdersModelItem(const std::string &pid, const fantasybit::Order &in,  QObject *parent = Q_NULLPTR)
                    :  QObject(parent) {
        m_symbol = pid.data();
        m_refnum = in.refnum();
        m_isbuy = in.core().buyside();
        m_size = in.core().size();
        m_price = in.core().price();
    }
};


class OpenOrdersModel : public QQmlObjectListModel<OpenOrdersModelItem> {
    Q_OBJECT


public:
    explicit OpenOrdersModel (QObject *  parent  = Q_NULLPTR,
                              const QByteArray & displayRole = QByteArray (),
                              const QByteArray & uidRole     = QByteArray ()) :
                              QQmlObjectListModel<OpenOrdersModelItem>(parent,displayRole,uidRole)
    {}

//    void updateAllOrders(const AllOdersFname &allordersfn) {
//        setfantasyname(allordersfn.fname().data());

//        clear();

//        for ( auto &pio : allordersfn.pidorders())
//            for ( auto &ord : pio.orders() )
//                append(new OpenOrdersModelItem(pio.symbol(),ord));
//    }
};


Q_DECLARE_METATYPE(OpenOrdersModel*)



class TradingPositionsModelItem : public QObject {
    Q_OBJECT
    QML_CONSTANT_CSTREF_PROPERTY (QString, symbol)
    QML_CONSTANT_CSTREF_PROPERTY (qint32, netprice)
    QML_CONSTANT_CSTREF_PROPERTY (qint32, netqty)
    QML_CONSTANT_CSTREF_PROPERTY (double, openpnl)

    QML_READONLY_PTR_PROPERTY(OpenOrdersModel, pOpenOrdersModel)
public:
    explicit TradingPositionsModelItem(const AllOdersSymbol &allordersymbol,
                                       QObject *parent = Q_NULLPTR)
                : m_symbol{allordersymbol.symbol().data()},
                  m_netprice{allordersymbol.netprice()},
                  m_netqty{allordersymbol.netqty()},
                  m_openpnl{0.0},
                  mOpenOrdersModel(parent,"display","refnum"),
                  m_pOpenOrdersModel{&mOpenOrdersModel},
                  QObject{parent}
    {
//        mOpenOrdersModel.setfantasyname(allordersymbol.fname().data());
//        for ( auto &pio : allordersymbol())
            for ( auto &ord : allordersymbol.orders() )
                mOpenOrdersModel.append(new OpenOrdersModelItem(allordersymbol.symbol(),ord));
    }

    OpenOrdersModel mOpenOrdersModel;
};


class TradingPositionsModel : public QQmlObjectListModel<TradingPositionsModelItem> {
    Q_OBJECT
//    QML_READONLY_CSTREF_PROPERTY(QString, symbol)
    QML_READONLY_CSTREF_PROPERTY(QString, fantasyname)

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
};

Q_DECLARE_METATYPE(TradingPositionsModel*)

#endif // OPENORDERSMODEL_H



























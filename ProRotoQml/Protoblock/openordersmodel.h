#ifndef OPENORDERSMODEL_H
#define OPENORDERSMODEL_H

#include <QObject>
#include <QString>
#include "../QmlSupermacros/QQmlConstRefPropertyHelpers.h"
#include "../QmlModels/QQmlObjectListModel.h"
#include "../QmlSuperMacros/QQmlHelpersCommon.h"
#include "StateData.pb.h"
#include <QQmlHelpersCommon.h>

using namespace fantasybit;

class OpenOrdersModelItem : public QObject {
    Q_OBJECT
    QML_CONSTANT_CSTREF_PROPERTY (QString, symbol)
    QML_CONSTANT_CSTREF_PROPERTY (qint32, refnum)
    QML_CONSTANT_CSTREF_PROPERTY (bool,  isbuy)
    QML_CONSTANT_CSTREF_PROPERTY (qint32, size)
    QML_CONSTANT_CSTREF_PROPERTY (qint32, price)

public:

    explicit OpenOrdersModelItem(const std::string &pid, const fantasybit::Order &in) :  QObject(nullptr) {
        m_symbol = pid.data();
        m_refnum = in.refnum();
        m_isbuy = in.core().buyside();
        m_size = in.core().size();
        m_price = in.core().price();
    }
};


class OpenOrdersModel : public QQmlObjectListModel<OpenOrdersModelItem> {
    Q_OBJECT
    QML_READONLY_CSTREF_PROPERTY(QString, fantasyname)

public:
    void updateAllOrders(const AllOdersFname &allordersfn) {
        setfantasyname(allordersfn.fname().data());

        clear();

        for ( auto &pio : allordersfn.pidorders())
            for ( auto &ord : pio.orders() )
                append(new OpenOrdersModelItem(pio.symbol(),ord));
    }
};


Q_DECLARE_METATYPE(OpenOrdersModel*)

#endif // OPENORDERSMODEL_H

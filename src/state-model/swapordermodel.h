/*
//  swapstatemodel.h
//
//  Protoblock
//
//  Created by Jay Berg on 12/04/18
//
*/
#ifndef SWAPORDERMODEL_H
#define SWAPORDERMODEL_H

#include <QObject>
#include <QString>
#include <QQmlConstRefPropertyHelpers.h>
#include <QQmlObjectListModel.h>
#include <QQmlHelpersCommon.h>
#include "NameData.pb.h"
#include "FantasyName.h"
#include "Commissioner.h"
#include <pbgateways.h>
#include <SwapData.pb.h>

using namespace fantasybit;

class SwapOrderModelItem : public QObject {
    Q_OBJECT
    QML_WRITABLE_CSTREF_PROPERTY (QString, name)
    QML_WRITABLE_CSTREF_PROPERTY (quint64, rate)
    QML_WRITABLE_CSTREF_PROPERTY (QString, status)
    QML_WRITABLE_CSTREF_PROPERTY (quint64, qty)
    QML_WRITABLE_CSTREF_PROPERTY (quint64, sat_min)
//    QML_WRITABLE_CSTREF_PROPERTY (quint64, sat_max)



public:
    SwapOrderModelItem() :  QObject(nullptr) {
        m_name = "noname";
        m_rate = 0;
        m_qty = 0;
        m_sat_min = 0;
    }

    explicit SwapOrderModelItem(const fantasybit::SwapOrder &so) : QObject(nullptr) {
        m_name = so.fname().data();
        m_rate = so.rate();
        if ( so.isask() )
            m_qty = so.openq();
        else if ( so.rate() > 0 )
            m_qty = so.satoshi_max() / so.rate();

        m_sat_min = so.satoshi_min();
    }

};


class SwapOrderModel : public QQmlObjectListModel<SwapOrderModelItem> {

public:
    explicit SwapOrderModel (QObject *parent= Q_NULLPTR,
                                    const QByteArray & displayRole = QByteArray (),
                                    const QByteArray & uidRole     = "name") :
                                    QQmlObjectListModel<SwapOrderModelItem>
                                                    (parent,displayRole,uidRole)
    {}

    void add(const fantasybit::SwapOrder &so) {
        auto it = this->getByUid(so.fname().data());
        if ( it == nullptr ) {
            append(new SwapOrderModelItem(so));
            qDebug() << "swap adding " << so.DebugString().data();
        }
        else {
            it->set_rate(so.rate());
            it->set_qty(so.openq());
            it->set_sat_min(so.satoshi_min());
            it->set_status("");
        }
    }

    void update(const fantasybit::SwapOrder &so) {
        auto it = this->getByUid(so.fname().data());
        if ( it == nullptr )
            return;

        QString st("msg(%1) ref(%2) directed(%3) ");
        st = st.arg(so.msg().data()).arg(so.ref().data()).arg(so.directed().data());
        if ( so.double_spent() )
            st.append(" double spent");
        it->set_status(st);
    }
};

Q_DECLARE_METATYPE(SwapOrderModel*)
Q_DECLARE_METATYPE(SwapOrderModelItem*)

#endif // SWAPORDERMODEL_H

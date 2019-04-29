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

public:
    SwapOrderModelItem() :  QObject(nullptr) {
        m_name = "noname";
        m_rate = 0;
//        m_bits = 0;
//        m_thisweek = 0;
//        m_lastweek = 0;
//        m_leaders20XX = 0;
//        m_numberproj = m_lastupdate = 0;
//        m_pnl = 0;
//        m_net = 0;
    }

    explicit SwapOrderModelItem(const QString &name, quint64 rate = 0)
                               : QObject(nullptr),
                                 m_name(name) {
        m_rate = rate;
    }

};


class SwapOrderModel : public QQmlObjectListModel<SwapOrderModelItem> {

public:
    explicit SwapOrderModel (QObject *parent= Q_NULLPTR,
                                    const QByteArray & displayRole = QByteArray (),
                                    const QByteArray & uidRole     = "name") :
                                    QQmlObjectListModel<SwapOrderModelItem>
                                                    (parent,displayRole,uidRole)
    {
//        append(new SwapOrderModelItem("bob0",1000));
//        append(new SwapOrderModelItem("other bob",1100));
    }

    void add(const fantasybit::SwapOrder &so) {
        auto it = this->getByUid(so.fname().data());
        if ( it == nullptr ) {
            append(new SwapOrderModelItem(so.fname().data(),so.rate()));
            qDebug() << "swap adding " << so.DebugString().data();
        }
        else {
            it->set_rate(so.rate());
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

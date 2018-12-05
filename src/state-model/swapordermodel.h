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
#include "../QmlModels/QQmlObjectListModel.h"
#include "../QmlSuperMacros/QQmlHelpersCommon.h"
#include "NameData.pb.h"
#include <QQmlHelpersCommon.h>
#include "FantasyName.h"
#include "Commissioner.h"
#include <pbgateways.h>

using namespace fantasybit;

class SwapOrderModelItem : public QObject {
    Q_OBJECT
    QML_WRITABLE_CSTREF_PROPERTY (QString, name)
//    QML_WRITABLE_CSTREF_PROPERTY (QString, pk)
//    QML_WRITABLE_CSTREF_PROPERTY (qint64, stake)
    QML_WRITABLE_CSTREF_PROPERTY (quint64, rate)
//    QML_WRITABLE_CSTREF_PROPERTY (qint64, pnl)
//    QML_WRITABLE_CSTREF_PROPERTY (qint64, net)
//    QML_WRITABLE_CSTREF_PROPERTY (quint64, thisweek)
//    QML_WRITABLE_CSTREF_PROPERTY (quint64, lastweek)
//    QML_WRITABLE_CSTREF_PROPERTY (quint64, leaders20XX)
//    QML_WRITABLE_CSTREF_PROPERTY (int, numberproj)
//    QML_WRITABLE_CSTREF_PROPERTY (int, lastupdate)

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
                                    const QByteArray & uidRole     = QByteArray ()) :
                                    QQmlObjectListModel<SwapOrderModelItem>
                                                    (parent,displayRole,uidRole)
    {
        append(new SwapOrderModelItem("bob0",1000));
        append(new SwapOrderModelItem("other bob",1100));
    }

};

Q_DECLARE_METATYPE(SwapOrderModel*)
Q_DECLARE_METATYPE(SwapOrderModelItem*)

#endif // SWAPORDERMODEL_H

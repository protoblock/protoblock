#ifndef FANTASYNAMEMODEL_H
#define FANTASYNAMEMODEL_H

#include <QObject>
#include <QString>
#include "../QmlSupermacros/QQmlConstRefPropertyHelpers.h"
#include "../QmlModels/QQmlObjectListModel.h"
#include "../QmlSuperMacros/QQmlHelpersCommon.h"
#include "NameData.pb.h"
#include <QQmlHelpersCommon.h>

using namespace fantasybit;

class FantasyNameBalModelItem : public QObject {
    Q_OBJECT
    QML_CONSTANT_CSTREF_PROPERTY (QString, name)
    QML_CONSTANT_CSTREF_PROPERTY (QString, pk)
    QML_CONSTANT_CSTREF_PROPERTY (qint64, stake)
    QML_CONSTANT_CSTREF_PROPERTY (quint64, bits)

public:

    explicit FantasyNameBalModelItem(const fantasybit::FantasyNameBal &in) :  QObject(nullptr) {
        m_name = in.name().data();
        m_pk = in.public_key().data();
        m_stake = in.stake() + in.bits();
        m_bits = in.bits();
    }
};


class FantasyNameBalModel : public QQmlObjectListModel<FantasyNameBalModelItem> {};

Q_DECLARE_METATYPE(FantasyNameBalModel*)
#endif // FANTASYNAMEMODEL_H

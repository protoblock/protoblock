#ifndef FANTASYNAMEMODEL_H
#define FANTASYNAMEMODEL_H

#include <QObject>
#include <QString>
#include "../QmlSupermacros/QQmlConstRefPropertyHelpers.h"
#include "../QmlModels/QQmlObjectListModel.h"
#include "../QmlSuperMacros/QQmlHelpersCommon.h"
#include "NameData.pb.h"
#include <QQmlHelpersCommon.h>
#include "FantasyName.h"

using namespace fantasybit;

class FantasyNameBalModelItem : public QObject {
    Q_OBJECT
    QML_CONSTANT_CSTREF_PROPERTY (QString, name)
    QML_CONSTANT_CSTREF_PROPERTY (QString, pk)
    QML_CONSTANT_CSTREF_PROPERTY (qint64, stake)
    QML_CONSTANT_CSTREF_PROPERTY (quint64, bits)
    QML_CONSTANT_CSTREF_PROPERTY (quint64, chash)

public:

    explicit FantasyNameBalModelItem(const fantasybit::FantasyNameBal &in) :  QObject(nullptr) {
        m_name = in.name().data();
        m_pk = in.public_key().data();
        m_stake = in.stake() + in.bits();
        m_bits = in.bits();
        m_chash = in.chash();
    }

    explicit FantasyNameBalModelItem(fantasybit::FantasyName &in) :  QObject(nullptr) {
        m_name = in.alias().data();
//        m_pk = in.pubkey().data();
        m_stake = in.getStakeBalance();
        m_bits = in.getBalance();
        m_chash = in.hash();
    }
};


class FantasyNameBalModel : public QQmlObjectListModel<FantasyNameBalModelItem> {

public:
    explicit FantasyNameBalModel (QObject *          parent      = Q_NULLPTR,
                                    const QByteArray & displayRole = QByteArray (),
                                    const QByteArray & uidRole     = QByteArray ()) :
                                    QQmlObjectListModel<FantasyNameBalModelItem>
                                                    (parent,displayRole,uidRole)
    {}

    void updateleaders(std::vector<std::shared_ptr<fantasybit::FantasyName>> &in) {
        clear();
        for(std::shared_ptr<fantasybit::FantasyName> fPlayer  : in) {
            append(new FantasyNameBalModelItem(*fPlayer));
        }
    }
};

Q_DECLARE_METATYPE(FantasyNameBalModel*)
#endif // FANTASYNAMEMODEL_H

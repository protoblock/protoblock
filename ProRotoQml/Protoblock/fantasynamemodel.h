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
#include "Commissioner.h"

using namespace fantasybit;

class FantasyNameBalModelItem : public QObject {
    Q_OBJECT
    QML_WRITABLE_CSTREF_PROPERTY (QString, name)
    QML_WRITABLE_CSTREF_PROPERTY (QString, pk)
    QML_WRITABLE_CSTREF_PROPERTY (qint64, stake)
    QML_WRITABLE_CSTREF_PROPERTY (quint64, bits)
    QML_WRITABLE_CSTREF_PROPERTY (int, numberproj)
    QML_WRITABLE_CSTREF_PROPERTY (int, lastupdate)

public:

    explicit FantasyNameBalModelItem(QString &name) :  QObject(nullptr) {
        m_name = name;
        m_stake = 0;
        m_bits = 0;
    }
    explicit FantasyNameBalModelItem(const fantasybit::FantasyNameBal &in) :  QObject(nullptr) {
        m_name = in.name().data();
        m_pk = in.public_key().data();
        m_stake = in.stake() + in.bits();
        m_bits = in.bits();
//        m_chash = in.chash();
    }

    explicit FantasyNameBalModelItem(fantasybit::FantasyName &in) :  QObject(nullptr) {
        m_name = in.alias().data();
        m_pk = Commissioner::pk2str(in.pubkey()).data();
        m_stake = in.getStakeBalance();
        m_bits = in.getBalance();
//        m_chash = in.hash();
        set_numberproj(in.numberproj);
        set_lastupdate(in.lastupdate);
    }

    explicit    FantasyNameBalModelItem(const FantasyNameBalModelItem &in) : QObject(nullptr) {
        set_name( in.get_name());
        set_pk(in.get_pk());
        set_stake(in.get_stake());
        set_bits(in.get_bits());
//        set_chash(in.get_chash());
    }

    void    update(const fantasybit::FantasyNameBal &in) {
        set_name(in.name().data());
        set_pk (in.public_key().data());
        set_stake ( in.stake() + in.bits());
        set_bits ( in.bits());
//        set_chash(in.chash());
    }

    void    update(const FantasyNameBalModelItem &in) {
        set_name( in.get_name());
        set_pk(in.get_pk());
        set_stake(in.get_stake());
        set_bits(in.get_bits());
//        set_chash(in.get_chash());
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

    void update (FantasyNameBalModelItem *in) {
        FantasyNameBalModelItem *my = (FantasyNameBalModelItem *)get(in->get_name());
        if ( my )
            my->update(*in);
        else {
            FantasyNameBalModelItem *n = new FantasyNameBalModelItem(*in);
//            n->update(*in);
            append(n);
        }

    }
};

Q_DECLARE_METATYPE(FantasyNameBalModel*)
#endif // FANTASYNAMEMODEL_H

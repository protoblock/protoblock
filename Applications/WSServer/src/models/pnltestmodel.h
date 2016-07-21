#ifndef PNLTESTMODEL_H
#define PNLTESTMODEL_H
#include <QObject>
#include <QString>

#include "../QmlSuperMacros/QQmlVarPropertyHelpers.h"
#include "../QmlSupermacros/QQmlConstRefPropertyHelpers.h"
#include "../QmlSuperMacros/QQmlAutoPropertyHelpers.h"

#include "src/tempapi.h"
namespace fantasybit {
class PnlTestModel : public QObject {
    Q_OBJECT
    QML_CONSTANT_CSTREF_PROPERTY (QString, fantasyName)
    QML_CONSTANT_CSTREF_PROPERTY (QString, playerId)
    QML_CONSTANT_VAR_PROPERTY    (qint32, qty)
    QML_CONSTANT_VAR_PROPERTY (qint32, price)
    QML_CONSTANT_VAR_PROPERTY (qint32, pnl)
    QML_CONSTANT_VAR_PROPERTY (int, week)
    QML_CONSTANT_CSTREF_PROPERTY (QString, gameId)

public:
explicit PnlTestModel (
                            const QString &fantasyName,
                            const QString &playerId,
                            const qint32 &qty,
                            const qint32 &price,
                            const qint32 &pnl,
                            const int &week,
                            const QString &gameId,
                            QObject * parent = Q_NULLPTR
                            )
                            : QObject    (parent),
                            m_fantasyName(fantasyName),
                            m_playerId(playerId),
                            m_qty(qty),
                            m_price(price),
                            m_pnl(pnl),
                            m_week(week),
                            m_gameId(gameId)
{ }
};
}

#endif // PNLTESTMODEL_H

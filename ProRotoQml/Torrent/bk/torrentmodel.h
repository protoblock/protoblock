#ifndef TORRENTMODEL_H
#define TORRENTMODEL_H
#include <QObject>
#include <QString>
#include "QQmlConstRefPropertyHelpers.h"
#include "QQmlObjectListModel.h"

class TorrentModelItem : public QObject {
    Q_OBJECT
    QML_CONSTANT_CSTREF_PROPERTY (QString, fileName)
    QML_CONSTANT_CSTREF_PROPERTY (QString, tooltip)
    QML_CONSTANT_CSTREF_PROPERTY (QString, peers)
    QML_CONSTANT_CSTREF_PROPERTY (QString , progress)
    QML_CONSTANT_CSTREF_PROPERTY (QString, downloadRate)
    QML_CONSTANT_CSTREF_PROPERTY (QString, status)
public:
    explicit TorrentModelItem(
            const QString &fileName,
            const QString &tooltip,
            const QString &peers,
            const QString &progress,
            const QString &downloadRate,
            const QString &status,
            QObject * parent = Q_NULLPTR
            ):
        QObject    (parent),
        m_fileName(fileName),
        m_tooltip(tooltip),
        m_peers(peers),
        m_progress(progress),
        m_downloadRate(downloadRate),
        m_status(status)
    {
    }
};

class TorrentModel : public QQmlObjectListModel<TorrentModelItem>
{};

#endif

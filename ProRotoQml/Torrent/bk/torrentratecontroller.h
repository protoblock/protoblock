#ifndef TORRENTRATECONTROLLER_H
#define TORRENTRATECONTROLLER_H

#include <QObject>

class TorrentRateController : public QObject
{
    Q_OBJECT
public:
    explicit TorrentRateController(QObject *parent = 0);

signals:

public slots:
};

#endif // TORRENTRATECONTROLLER_H
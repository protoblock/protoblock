#ifndef QAUDIOLEVEL_H
#define QAUDIOLEVEL_H

#include <QObject>

class QAudioLevel : public QObject
{
    Q_OBJECT
public:
    explicit QAudioLevel(QObject *parent = 0);
    void setLevel(qreal level);

signals:
    void levelChanged();
private:
    qreal m_level;
};

#endif // QAUDIOLEVEL_H

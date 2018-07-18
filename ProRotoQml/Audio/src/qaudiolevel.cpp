#include "qaudiolevel.h"

QAudioLevel::QAudioLevel(QObject *parent)
    : QObject(parent),
       m_level(0.0)
{
}
void QAudioLevel::setLevel(qreal level)
{
    if (m_level != level) {
        m_level = level;
        levelChanged();
    }
}

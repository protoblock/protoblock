#include "formfactor.h"

FormFactor::FormFactor(QObject *parent) :
    QObject(parent)
//  ,
//    m_factor(Desktop)
{
//    QGuiApplication m_guiApp(this);
//    connect(&m_guiApp,SIGNAL(screenAdded(QScreen*)),
//            this,SLOT(handleScreenAdded(QScreen*)));
//    connect(&m_guiApp,SIGNAL(screenRemoved(QScreen*)),
//            this,SLOT(handleScreenRemoved(QScreen*)));
}

FormFactor::Factor FormFactor::factor() const
{
    return m_factor;
}

void FormFactor::setFormFactor(const FormFactor::Factor &factor)
{
    if ( m_factor == factor )
        return;
    m_factor = factor ;
    formFactorChanged();
    setCurrentFactor();
}

QStringList FormFactor::availableFactors() const
{
    return m_availableFactors;
}

void FormFactor::setAvailableFactors()
{
    m_availableFactors.clear();
    QList<QString> li;
    li <<  "Desktop" <<  "TV" <<  "Phone" <<  "Tablet";
    m_availableFactors = li;
}

QString FormFactor::currentFactor() const
{
    return m_currentFactor;
}

void FormFactor::setCurrentFactor()
{
    switch (m_factor) {
    case Desktop:
        m_currentFactor = "Desktop";
        break;
    case TV:
        m_currentFactor = "TV";
        break;
    case Phone:
        m_currentFactor = "Phone";
        break;
    case Tablet:
        m_currentFactor = "Tablet";
    }
}

void FormFactor::handleScreenAdded(QScreen *screen)
{
    qDebug() << "Screen added named "  <<  screen->name();
}

void FormFactor::handleScreenRemoved(QScreen *screen)
{
    qDebug() << "Screen removed named " << screen->name();
}

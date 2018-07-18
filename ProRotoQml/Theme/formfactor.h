#ifndef FORMFACTOR_H
#define FORMFACTOR_H

#include <QObject>
#include <QScreen>
#include <QGuiApplication>
#include <QDebug>
#include <QString>
#include <QStringList>
#include <QList>

class FormFactor : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString currentFactor READ currentFactor NOTIFY currentFactorChanged)

    Q_PROPERTY(QStringList availableFactors READ availableFactors  NOTIFY availableFactorsChanged)
    Q_PROPERTY(Factor factor READ factor WRITE setFormFactor NOTIFY formFactorChanged)
    Q_ENUMS(Factor)

public:
    explicit FormFactor(QObject *parent = 0);
    enum Factor{
        Desktop,
        TV,
        Phone,
        Tablet
    };
    Factor factor()const;
    void setFormFactor(const Factor &factor);
    QStringList availableFactors()const;
    void setAvailableFactors();
    QString currentFactor()const;
    void setCurrentFactor();



signals:
    void formFactorChanged();
    void currentFactorChanged();
    void availableFactorsChanged();
public slots:
    //void handelFactor(const QScreen screen);
    void handleScreenAdded(QScreen *screen);
    void handleScreenRemoved(QScreen *screen);

private:
    Factor m_factor;
    QStringList m_availableFactors;
    QString m_currentFactor;

//    QScreen m_screen;
//    QGuiApplication m_guiApp;

};

#endif // FORMFACTOR_H

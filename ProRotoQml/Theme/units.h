#ifndef UNITS_H
#define UNITS_H

#include <QObject>
#include <QHash>
#include <QUrl>

class Units : public QObject
{
    Q_OBJECT
    Q_PROPERTY(float gridUnit READ gridUnit WRITE setGridUnit NOTIFY gridUnitChanged)

public:
    static Units& instance() {
        static Units instance;
        return instance;
    }

    explicit Units(QObject *parent = 0);
    Q_INVOKABLE float dp(float value);
    Q_INVOKABLE float gu(float value);
    QString resolveResource(const QUrl& url);

    // getters
    float gridUnit();

    // setters
    void setGridUnit(float gridUnit);

signals:
    void gridUnitChanged();

protected:
    QString suffixForGridUnit(float gridUnit);
    float gridUnitSuffixFromFileName(const QString &fileName);

private:
    float m_gridUnit;
};


#endif // UNITS_H

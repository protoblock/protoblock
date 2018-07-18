#ifndef QMLDESTTINGS_H
#define QMLDESTTINGS_H

#include <QObject>
#include <QSettings>
#include <QDebug>

class QmlSesttings : public QObject
{
    Q_OBJECT
//    Q_PROPERTY(QString fileLocation READ fileLocation WRITE setFileLocation NOTIFY fileLocationChanged)
//    Q_PROPERTY(QStringList keys READ keys WRITE setKeys NOTIFY keysChanged)

public:
    explicit QmlSesttings(QObject *parent = 0);


    enum  Status {
        NoError,
        AccessError,
        FormatError
    };

    enum  Scope {
        UserScope,
        SystemScope
    };

    enum  Format {
        NativeFormat,
        IniFormat,
        InvalidFormat
    };



    QStringList allKeys() const;
    QString fileName() const;
    QString group() const;
    Q_INVOKABLE void  clear();

    void setIniFile();

signals:

public slots:
private:
    QSettings m_settings ;
};

#endif // QMLDESTTINGS_H

#ifndef PASSWORDSTRENGTH_H
#define PASSWORDSTRENGTH_H

#include <QObject>
#include <QRegExp>
#include <QSize>
#include <QString>
#include <QDebug>

class PasswordStrength : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int pwdStrength READ pwdStrength NOTIFY pwdStrengthChanged)
    Q_PROPERTY(QString errorString READ errorString NOTIFY errorStringChanged)
    Q_PROPERTY(QString strenghtString READ strenghtString NOTIFY strenghtStringChanged)
    Q_PROPERTY(Error enumError READ enumError NOTIFY errorChanged)

    Q_ENUMS(Error)
public:
    explicit PasswordStrength(QObject *parent = 0);

    enum Error{
        NotEnoughtCharacters,
        TooWeak,
        NoError
    };


    Error enumError()const;

    QString errorString()const ;
    void setErrorString(const QString errorString);

    int pwdStrength()const;
    void setPwdStrength(const int pwdStrength);
    QString strenghtString()const;
    void setStrenghtString(const int sS);

signals:
    void update(QString);
    void error(Error);
    void pwdStrengthChanged();
    void errorStringChanged();
    void errorChanged();
    void strenghtStringChanged();

protected slots:
    void updateError(const Error enumError);
    void handelUpdate(const QString upString);

private:
    int m_pwdStrength;
    int m_minimumPasswordLength;
//    int m_passwordStrengthWarningLevel;
    int m_reasonablePasswordLength;
    Error m_error;
    QString m_errorString;
    QString m_strenghtString;
    int m_sS;

};

#endif // PASSWORDSTRENGTH_H

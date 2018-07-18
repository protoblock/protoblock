#include "passwordstrength.h"

PasswordStrength::PasswordStrength(QObject *parent) :
    QObject(parent),
    m_minimumPasswordLength(0),
//    m_passwordStrengthWarningLevel(1),
    m_reasonablePasswordLength(8),
    m_error(NoError),
    m_sS(0)
{

    connect(this,SIGNAL(update(QString)),this,SLOT(handelUpdate(QString)));
    connect(this,SIGNAL(error(Error)),this,SLOT(updateError(Error)));

}

PasswordStrength::Error PasswordStrength::enumError() const
{
    return m_error;
}

QString PasswordStrength::errorString()const
{
    return m_errorString;
}

void PasswordStrength::setErrorString(const QString errorString)
{
    if(m_errorString == errorString)
        return;
    m_errorString = errorString;
    emit errorStringChanged();
}

int PasswordStrength::pwdStrength() const
{
    return m_pwdStrength;
}


//FIXME lets add warning levels to this
// IE Patitic ,Weak , OK, Good , Great ect
void PasswordStrength::setPwdStrength(const int pwdStrength)
{
    if(m_pwdStrength == pwdStrength)
        return;
    m_pwdStrength = pwdStrength;
    emit pwdStrengthChanged();
}

QString PasswordStrength::strenghtString() const
{
    return m_strenghtString;
}

void PasswordStrength::setStrenghtString(const int sS)
{
    if ( m_sS == sS ){
        return;
    }else{
        if(sS <= 20  ){
            m_strenghtString = "Extreamly Weak";
            m_sS = sS;
        }else if (sS >= 20 && sS <= 40){
            m_strenghtString = "Weak";
            m_sS = sS;
        }else if (sS >= 41 && sS <= 60 ){
            m_strenghtString = "OK";
            m_sS = sS;
        }else if (sS >= 61 && sS <= 80){
            m_strenghtString = "Good";
            m_sS = sS;
        }else if (sS >= 85){
            m_strenghtString = "Great";
            m_sS = sS;
        }
        m_sS = sS;
        emit strenghtStringChanged();
    }
}

void PasswordStrength::updateError(const PasswordStrength::Error enumError)
{
    if(m_error == enumError){
        return ;

    }else{
       int r = enumError;
       switch (r) {
       case 0:
           m_error = NotEnoughtCharacters;
           setErrorString("Not Enough Characters") ;
           break;
       case 1:
           m_error = TooWeak;
           setErrorString("The Password is to weak");
           break;
       case 2:
           m_error = NoError;
           setErrorString("No Know Error");
           break;
       }
       emit errorStringChanged();
    }
}

void PasswordStrength::handelUpdate(const QString upString)
{
    if(upString.length() < 1 ){
        emit error(NotEnoughtCharacters);
        return;
    }

    const double lengthFactor = m_reasonablePasswordLength / 8.0;
    int pwlength = (int) ( upString.length()/ lengthFactor);
    if (pwlength > 5) {
        pwlength = 5;
    }

    const QRegExp numRxp("[0-9]", Qt::CaseSensitive, QRegExp::RegExp);
    int numeric = (int) (upString.count(numRxp) / lengthFactor);
    if (numeric > 3) {
        numeric = 3;
    }

    const QRegExp symbRxp("\\W", Qt::CaseInsensitive, QRegExp::RegExp);
    int numsymbols = (int) (upString.count(symbRxp) / lengthFactor);
    if (numsymbols > 3) {
        numsymbols = 3;
    }

    const QRegExp upperRxp("[A-Z]", Qt::CaseSensitive, QRegExp::RegExp);
    int upper = (int) (upString.count(upperRxp) / lengthFactor);
    if (upper > 3) {
        upper = 3;
    }

    int pwstrength=((pwlength*10)-20) + (numeric*10) + (numsymbols*15) + (upper*10);


//    qDebug() << pwstrength;

    if ( pwstrength < 0 ) {
        setPwdStrength(0);
        setStrenghtString(pwstrength);

        return;
    }

    if ( pwstrength > 100 ) {
        setPwdStrength(100);
        setStrenghtString(pwstrength);

        return;
    }

    if (pwstrength > 1 && pwstrength < 99){
    setPwdStrength(pwstrength);
    setStrenghtString(pwstrength);

    }
}

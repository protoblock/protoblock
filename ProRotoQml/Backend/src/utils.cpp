#include "utils.h"
//#include <queue>

#include <QString>
#include <QCryptographicHash>
using namespace std;
namespace fantasybit {

std::string hashit(const std::string &in){
    QString passin = QString::fromStdString (in);
    QByteArray results = QCryptographicHash::hash( passin.toLatin1() , QCryptographicHash::Sha256 );
    QString hashResult(results.toHex());
    return hashResult.toStdString ();
}

std::string hashit(const GOOGLE_NAMESPACE::protobuf::Message  &in)
{
    std::string ss = in.SerializeAsString();
    QString s = QString::fromStdString (ss);
    QString passin = s ;//QString::fromStdString (s);
    QByteArray results = QCryptographicHash::hash( passin.toLatin1() , QCryptographicHash::Sha256 );
    QString hashResult(results.toHex());
    return hashResult.toStdString ();
}
}

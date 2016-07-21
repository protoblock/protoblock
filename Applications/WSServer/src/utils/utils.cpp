#include "utils.h"
//#include <fc/crypto/sha256.hpp>
#include <queue>

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



//std::string hashit(const std::string &in) {
//    return fc::sha256::hash(in).str();
//}

std::string hashit(const google::protobuf::Message  &in)
{
    std::string ss = in.SerializeAsString();
    QString s = QString::fromStdString (ss);
    QString passin = s ;//QString::fromStdString (s);
    QByteArray results = QCryptographicHash::hash( passin.toLatin1() , QCryptographicHash::Sha256 );
    QString hashResult(results.toHex());
    return hashResult.toStdString ();
  //  return fc::sha256::hash(in.SerializeAsString()).str();
}


//// Three-way comparison function:
////   if a < b: negative result
////   if a > b: positive result
////   else: zero result
//int Int32Comparator::Compare(const leveldb::Slice& a, const leveldb::Slice& b) const {
//    int32_t ia = *(reinterpret_cast<const int32_t *>(a.data()));
//    int32_t ib = *(reinterpret_cast<const int32_t *>(b.data()));

//    //qDebug() << ia << ib << "yoyo";
//    if (ia < ib) return -1;
//    if (ia > ib) return +1;
//    return 0;
//}

//// Ignore the following methods for now:
//const char* Int32Comparator::Name() const { return "Int32Comparator"; }
//void Int32Comparator::FindShortestSeparator(std::string*, const leveldb::Slice&) const { }
//void Int32Comparator::FindShortSuccessor(std::string*) const { }

//std::string makeMerkleRoot(decltype(fantasybit::MerkleTree::default_instance().leaves()) &in) {
//    std::queue<fc::sha256> merkle;

//    for (const auto elem : in ) {
//        fc::sha256 digest = fc::sha256::hash(elem);

//        merkle.push(digest);
//    }

//    if (merkle.empty())
//        return "";//(fc::sha256) NULL;

//    int rows = 0;
//    while (merkle.size() > 1) {
//       /*
//        if (merkle.size() % 2 != 0) {
//            merkle.push(merkle.back());
//        }
//       */
//        queue<fc::sha256> new_merkle;

//        rows++;
//        int j =  merkle.size();

//        while (merkle.size() > 1) {
//            j =  merkle.size();

//            fc::sha256 first = merkle.front();
//            merkle.pop();

//            j =  merkle.size();

//            fc::sha256 second = merkle.front();
//            merkle.pop();

//            j =  merkle.size();

//            string concat;

//            concat = (string) first + (string) second;

//            new_merkle.push(fc::sha256::hash(concat));
//        }
//        if ( merkle.size() == 1) {
//            new_merkle.push(merkle.front());
//            merkle.pop();
//        }

//        j =  merkle.size();

//        merkle = new_merkle;
//    }

//    return merkle.front();

//    return "";
//}


}

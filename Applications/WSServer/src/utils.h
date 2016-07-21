#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <leveldb/db.h>
#include <leveldb/comparator.h>
#include "ProtoData.pb.h"
#include "StateData.pb.h"
#include <string>

//using namespace std;
namespace fantasybit {
class Int32Comparator : public leveldb::Comparator {
public:
    std::string hashit(const std::string &in);
    std::string hashit(const GOOGLE_NAMESPACE::protobuf::Message  &in);
};
}
#endif // UTILS_H

#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <leveldb/db.h>
#include <leveldb/comparator.h>
#include <generated/ProtoData.pb.h>
#include <generated/StateData.pb.h>
#include <string>

//using namespace std;
namespace fantasybit {
class Int32Comparator : public leveldb::Comparator {
public:
    std::string hashit(const std::string &in);
    std::string hashit(const google::protobuf::Message  &in);
}
#endif // UTILS_H

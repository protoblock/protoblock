//
//  NameData.cpp
//  fantasybit
//
//  Created by Jay Berg on 8/17/2015
//
//

#include <QDebug>
#include <vector>
#include "NameData.h"
#include "fbutils.h"
#include "FantasyName.h"
#include "leveldb/slice.h"


using namespace std;
using namespace fantasybit;

void NameData::init() {
    write_sync.sync = true;
    leveldb::Options options;
    options.create_if_missing = true;
    leveldb::Status status;

    status = leveldb::DB::Open(options, filedir("namestore"), &namestore);
}

void NameData::AddNewName(const std::string &pubkey,const std::string &name) {
    FantasyNameBal fn{};
    fn.set_name(name);
    fn.set_public_key(pubkey);
    fn.set_bits(0);

    auto hash = FantasyName::name_hash(name);

    leveldb::Slice hkey((char*)&hash, sizeof(hash_t));
    namestore->Put(write_sync, hkey, fn.SerializeAsString());

    qDebug() << fn.DebugString();
}

void NameData::AddBalance(const std::string name, uint64_t amount) {
    auto hash = FantasyName::name_hash(name);

    string temp;
    leveldb::Slice hkey((char*)&hash, sizeof(hash_t));
    if ( !namestore->Get(leveldb::ReadOptions(), hkey, &temp).ok() ) {
        qWarning() << "cant name to add balance" << name.c_str();
        return;
    }
    FantasyNameBal fn{};
    fn.ParseFromString(temp);
    fn.set_bits(fn.bits() + amount);
    namestore->Put(write_sync, hkey, fn.SerializeAsString());

    qDebug() << fn.DebugString();
}

void NameData::AddProjection(const string &name, const string &player,
                             uint32_t proj) {
    leveldb::Slice bval((char*)&proj, sizeof(uint32_t));
    string key(name + ":" + player);
    projstore->Put(leveldb::WriteOptions(), key, bval);
    auto m = FantasyNameProjections[name];
    m[player] = proj;
    m = PlayerIDProjections[player];
    m[name] = proj;
    qDebug() << "proj: " << key << ":" << proj;
}


std::string NameData::filedir(const std::string &in) {
    return GET_ROOT_DIR() + "index/" + in;
}

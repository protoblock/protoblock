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
#include "Commissioner.h"

using namespace std;
using namespace fantasybit;

void NameData::init() {
    write_sync.sync = true;
    leveldb::Options options;
    options.create_if_missing = true;
    leveldb::Status status;

    status = leveldb::DB::Open(options, filedir("namestore"), &namestore);
    if ( !status.ok() ) {
        qCritical() << " cant open " + filedir("namestore");
        //todo emit fatal
        return;
    }

    auto *it = namestore->NewIterator(leveldb::ReadOptions());
    for (it->SeekToFirst(); it->Valid(); it->Next()) {
        FantasyNameBal fn;
        fn.ParseFromString(it->value().ToString());
        auto fnp = Commissioner::AddName(fn.name(),fn.public_key());
        if ( fnp != nullptr )
            fnp->addBalance(fn.bits());
    }
}

void NameData::AddNewName(std::string name,std::string pubkey) {
    FantasyNameBal fn{};
    fn.set_name(name);
    fn.set_public_key(pubkey);
    fn.set_bits(0);

    auto hash = FantasyName::name_hash(name);

    leveldb::Slice hkey((char*)&hash, sizeof(hash_t));
    namestore->Put(write_sync, hkey, fn.SerializeAsString());

    qDebug() << fn.DebugString();

    Commissioner::AddName(name,pubkey);
    OnFantasyName(name);
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
    OnFantasyNameBalance(fn);
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
    OnProjection(name,player,proj);
}

std::unordered_map<std::string,int> NameData::GetProjById(const std::string &pid) {
    return PlayerIDProjections[pid];
}

std::unordered_map<std::string,int> NameData::GetProjByName(const std::string &nm) {
    return FantasyNameProjections[nm];
}

void NameData::Subscribe(std::string in) {
    mSubscribed.insert(in);
}

void NameData::UnSubscribe(std::string in) {
    mSubscribed.erase(in);
}

void NameData::OnProjection(const std::string &name, const std::string &player,
                            uint32_t proj) {
    if ( !amlive )
        return;

    if ( mSubscribed.find(name) == end(mSubscribed))
        return;

    FantasyBitProj fpj{};
    fpj.set_name(name);
    fpj.set_playerid(player);
    fpj.set_proj(proj);

    emit ProjectionLive(fpj);
}

void NameData::OnFantasyName(std::string &name) {
    //if ( !amlive )
    //    return;

    if ( mSubscribed.find(name) != end(mSubscribed))
        emit FantasyNameFound(name);
}

void NameData::OnFantasyNameBalance(FantasyNameBal &fn) {
    if ( !amlive )
        return;

    if ( mSubscribed.find(fn.name()) != end(mSubscribed))
        emit FantasyNameBalance(fn);
}

void NameData::OnWeekOver(int in) {
    FantasyNameProjections.clear();
    PlayerIDProjections.clear();

    auto *it = projstore->NewIterator(leveldb::ReadOptions());
    for (it->SeekToFirst(); it->Valid(); it->Next())
        projstore->Delete(leveldb::WriteOptions(), it->key());

    qDebug() << " clearProjections ";
}

void NameData::OnWeekStart(int in) {
    week = in;
}

std::string NameData::filedir(const std::string &in) {
    return GET_ROOT_DIR() + "index/" + in;
}

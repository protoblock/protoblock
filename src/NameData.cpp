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

void FantasyNameData::init() {
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

void FantasyNameData::AddNewName(std::string name,std::string pubkey) {
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

void FantasyNameData::AddBalance(const std::string name, uint64_t amount) {
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
    auto fnp = Commissioner::getName(hash);
    if ( fnp != nullptr)
        fnp->addBalance(amount);

    qDebug() << fn.DebugString();
    OnFantasyNameBalance(fn);
}

void FantasyNameData::AddProjection(const string &name, const string &player,
                             uint32_t proj) {
    leveldb::Slice bval((char*)&proj, sizeof(uint32_t));
    string key(name + ":" + player);
    projstore->Put(leveldb::WriteOptions(), key, bval);
    {
        std::lock_guard<std::recursive_mutex> lockg{ data_mutex };
        auto m = FantasyNameProjections[name];
        m[player] = proj;
        m = PlayerIDProjections[player];
        m[name] = proj;
    }
    qDebug() << "proj: " << key << ":" << proj;
    OnProjection(name,player,proj);
}

std::unordered_map<std::string,int> FantasyNameData::GetProjById(const std::string &pid) {
    std::lock_guard<std::recursive_mutex> lockg{ data_mutex };
    return PlayerIDProjections[pid];
}

std::unordered_map<std::string,int> FantasyNameData::GetProjByName(const std::string &nm) {
    std::lock_guard<std::recursive_mutex> lockg{ data_mutex };
    return FantasyNameProjections[nm];
}

void FantasyNameData::Subscribe(std::string in) {
    mSubscribed.insert(in);
}

void FantasyNameData::UnSubscribe(std::string in) {
    mSubscribed.erase(in);
}

void FantasyNameData::OnProjection(const std::string &name, const std::string &player,
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

void FantasyNameData::OnFantasyName(std::string &name) {
    //if ( !amlive )
    //    return;

    if ( mSubscribed.find(name) != end(mSubscribed))
        emit FantasyNameFound(name);
}

void FantasyNameData::OnFantasyNameBalance(FantasyNameBal &fn) {
    if ( !amlive )
        return;

    if ( mSubscribed.find(fn.name()) != end(mSubscribed))
        emit FantasyNameBalance(fn);
}

void FantasyNameData::OnWeekOver(int in) {
    {
        std::lock_guard<std::recursive_mutex> lockg{ data_mutex };
        FantasyNameProjections.clear();
        PlayerIDProjections.clear();
    }
    auto *it = projstore->NewIterator(leveldb::ReadOptions());
    for (it->SeekToFirst(); it->Valid(); it->Next())
        projstore->Delete(leveldb::WriteOptions(), it->key());

    qDebug() << " clearProjections ";
}

void FantasyNameData::OnWeekStart(int in) {
    week = in;
}

std::string FantasyNameData::filedir(const std::string &in) {
    return GET_ROOT_DIR() + "index/" + in;
}

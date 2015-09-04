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
#include "ApiData.pb.h"
#include "RestFullCall.h"

using namespace std;
using namespace fantasybit;

void FantasyNameData::init() {
    write_sync.sync = true;
    leveldb::Options options;
    options.create_if_missing = true;
    leveldb::Status status;

    leveldb::DB *db1;
    status = leveldb::DB::Open(options, filedir("namestore"), &db1);
    namestore.reset(db1);
    if ( !status.ok() ) {
        qCritical() << " cant open " + filedir("namestore");
        //todo emit fatal
        return;
    }
    else {
        auto *it = namestore->NewIterator(leveldb::ReadOptions());
        for (it->SeekToFirst(); it->Valid(); it->Next()) {
            FantasyNameBal fn;
            fn.ParseFromString(it->value().ToString());
            auto fnp = Commissioner::AddName(fn.name(),fn.public_key());
            if ( fnp != nullptr )
                fnp->addBalance(fn.bits());
        }
        delete it;
    }

    leveldb::DB *db2;
    status = leveldb::DB::Open(options, filedir("projstore"), &db2);
    projstore.reset(db2);
    if ( !status.ok() ) {
        qCritical() << " cant open " + filedir("projstore");
        //todo emit fatal shit
        return;
    }
    else {
        auto *it = projstore->NewIterator(leveldb::ReadOptions());
        for (it->SeekToFirst(); it->Valid(); it->Next()) {
            auto str = it->key().ToString();
            int pos =  str.find_first_of(':');
            auto fantasyname = str.substr(0, pos);
            auto nflplayer = str.substr(pos + 1);
            uint32_t proj = *(reinterpret_cast<const uint32_t *>(it->value().data()));

            FantasyNameProjections[fantasyname].insert(make_pair(nflplayer,proj));
            PlayerIDProjections[nflplayer].insert(make_pair(fantasyname,proj));
        }
        delete it;
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

    auto fnp = Commissioner::AddName(name,pubkey);
    if ( fnp  != nullptr)
        OnFantasyName(fnp);
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
    if (!projstore->Put(leveldb::WriteOptions(), key, bval).ok())
        qWarning() << " error writing proj" << player << name << proj;
    else
    {
        std::lock_guard<std::recursive_mutex> lockg{ data_mutex };
        FantasyNameProjections[name].insert(make_pair(player,proj));
        PlayerIDProjections[player].insert(make_pair(name,proj));
    }
    qDebug() << "proj: " << key << ":" << proj;
    OnProjection(name,player,proj);

    /*
    dump(FantasyNameProjections);
    qDebug() << " ============== ";
    dump(PlayerIDProjections);
    qDebug() << " ============== ";
    dump(GetProjByName(name));
    qDebug() << " ============== ";
    dump(GetProjById(player));
    */
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

    fantasybit::FantasyBitProj fpj{};
    fpj.set_name(name);
    fpj.set_playerid(player);
    fpj.set_proj(proj);

    emit ProjectionLive(fpj);
}

void FantasyNameData::OnFantasyName(std::shared_ptr<FantasyName> fn) {
    //if ( !amlive )
    //    return;

    auto name = fn->alias();

#ifdef DATAAGENTGUI
    FantasyNameHash fnh{};
    fnh.set_name(name);
    fnh.set_hash(fn->hash());
    emit new_dataFantasyNameHash(fnh);

    auto fnhstr = fnh.SerializeAsString();
    RestfullClient rest(QUrl("https://api.trading.football:9854"));
    rest.postRawData("fantasy/name","shit",fnhstr.data(),((size_t)fnhstr.size()),true);

#endif

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
    delete it;
    qDebug() << " clearProjections ";
}

void FantasyNameData::OnWeekStart(int in) {
    std::lock_guard<std::recursive_mutex> lockg{ data_mutex };
    week = in;
}

std::string FantasyNameData::filedir(const std::string &in) {
    return GET_ROOT_DIR() + "index/" + in;
}

void FantasyNameData::dump(mapmapi &mm) {
    for ( auto fn : mm ) {
        qDebug() << fn.first;
        dump ( fn.second );
    }
}

void FantasyNameData::dump(std::unordered_map<std::string,int> &m) {
    for ( auto pa : m)
        qDebug() << pa.first << pa.second;

}

void FantasyNameData::dumpProj() {
    qDebug() << "--FantasyNameProjections--";
    dump(FantasyNameProjections);
    qDebug() << "--PlayerIDProjections--";
    dump(PlayerIDProjections);
}

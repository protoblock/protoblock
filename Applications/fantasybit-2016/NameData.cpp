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
#include "fbutils.h"
#include "globals.h"

#include "DataPersist.h"

#ifdef DATAAGENTWRITENAMES
#include "../../../fantasybit-2015/tradingfootball/playerloader.h"
#endif
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
        qCritical() << " cant open " << filedir("namestore");
        //todo emit fatal
        return;
    }
    else {
#ifdef WRITE_BOOTSTRAP
        Writer<FantasyNameBal> writer{ GET_ROOT_DIR() + "bootstrap/FantasyNameBal.txt" };
#endif
        auto *it = namestore->NewIterator(leveldb::ReadOptions());
        for (it->SeekToFirst(); it->Valid(); it->Next()) {
            FantasyNameBal fn;
            if ( !fn.ParseFromString(it->value().ToString())) {
                 qCritical() << "NameData error reading names";
                 break;
            }
#ifdef WRITE_BOOTSTRAP
            writer(fn);
#endif
            auto fnp = Commissioner::AddName(fn.name(),fn.public_key());
            if ( fnp != nullptr ) {
                fnp->initBalance(fn.bits());
                fnp->initStakePNL(fn.stake());
                fnp->setBlockNump (0,0);
#ifdef TRACE2
                qDebug() << fnp->ToString().data();
#endif
#ifdef DATAAGENTWRITENAMES_SPECIAL
                auto name = fnp->alias();
                if ( name != Commissioner::FantasyAgentName())
                {
                    FantasyNameHash fnh2{};
                    SqlStuff sql("satoshifantasy","OnFantasyName");

                    fnh2.set_name(name);
                    fnh2.set_hash(fnp->hash());
//                    emit new_dataFantasyNameHash(fnh);

//                    auto fnhstr = fnh.SerializeAsString();
                    sql.fantasyname(fnh2);

                    //RestfullClient rest(QUrl(LAPIURL.data()));
                    //rest.postRawData("fantasy/name","oc",fnhstr.data(),((size_t)fnhstr.size()));
                }
#endif
            }
        }
        delete it;
    }

    leveldb::DB *db2;
    status = leveldb::DB::Open(options, filedir("projstore"), &db2);
    projstore.reset(db2);
    if ( !status.ok() ) {
        qCritical() << " cant open " << filedir("projstore");
        //todo emit fatal shit
        return;
    }
    else {
        auto *it = projstore->NewIterator(leveldb::ReadOptions());
        for (it->SeekToFirst(); it->Valid(); it->Next()) {
            auto str = it->key().ToString();
            int pos =  str.find_last_of(':');
            if ( pos != string::npos) {
                auto fantasyname = str.substr(0, pos);
                auto nflplayer = str.substr(pos+1);
                uint32_t proj = *(reinterpret_cast<const uint32_t *>(it->value().data()));

                FantasyNameProjections[fantasyname].insert(make_pair(nflplayer,proj));
                PlayerIDProjections[nflplayer].insert(make_pair(fantasyname,proj));
//                qDebug() << "read sumplusproj" <<  PlayerIDSumProj[nflplayer] << " + " << proj;
                PlayerIDSumProj[nflplayer] += proj;
            }
            else {
                if ( 0 == strncmp(str.c_str(), "time^", 5) ) {
                    int32_t ts = *(reinterpret_cast<const int32_t *>(it->value().data()));
                    auto fnp = Commissioner::getName(str.substr(5));
                    if ( fnp && ts)
                        fnp->setBlockNum(ts);
                }
            }
        }
        delete it;
    }

    for ( auto fff : FantasyNameProjections) {
        auto fnp = Commissioner::getName(fff.first);
        fnp->setNump (fff.second.size());
    }
}

void FantasyNameData::closeAll() {
    Commissioner::clearAll();
    std::lock_guard<std::recursive_mutex> lockg{ data_mutex };
    bool amlive = false;
    int week = 0;
    mSubscribed.clear();
    PlayerIDProjections.clear();
    FantasyNameProjections.clear();
    PlayerIDSumProj.clear();
    namestore.reset();
    projstore.reset();
    //reset num for week todo:
}

void FantasyNameData::AddNewName(std::string name,std::string pubkey, int32_t blocknum) {
    FantasyNameBal fn{};
    fn.set_name(name);
    fn.set_public_key(pubkey);
    fn.set_bits(0);

    auto hash = FantasyName::name_hash(name);

    leveldb::Slice hkey((char*)&hash, sizeof(hash_t));
    namestore->Put(write_sync, hkey, fn.SerializeAsString());

    qDebug() << fn.DebugString().data();

    auto fnp = Commissioner::AddName(name,pubkey);
    fnp->setBlockNump (blocknum,0);
    if ( fnp  != nullptr) {
        OnFantasyName(fnp);
        if ( amlive ) {
            emit NewFantasyName(fn);
//            fantasybit::FantasyBitProj fpj{};
//            fpj.set_name(name);
//            fpj.set_playerid(0);
//            fpj.set_proj(0);
//            fpj.set_count(0);
//            fpj.set_block(blocknum);
//            emit ProjectionLive(fpj);
        }
    }

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
    if ( fnp != nullptr) {
        fnp->addBalance(amount);

    //if ( name == "Windo")
//    qDebug() << "adding award" << amount << " :: " << fn.DebugString() << fnp->ToString();
        OnFantasyNameBalance(fn);
        if ( amlive )
            emit AnyFantasyNameBalance(fn);
    }
}

void FantasyNameData::AddPnL(const std::string name, int64_t pnl) {
    auto hash = FantasyName::name_hash(name);

    qDebug() << "adding pnl " << name << pnl;
    string temp;
    leveldb::Slice hkey((char*)&hash, sizeof(hash_t));
    if ( !namestore->Get(leveldb::ReadOptions(), hkey, &temp).ok() ) {
        qWarning() << "cant name to add balance" << name.c_str();
        return;
    }

    FantasyNameBal fn{};
    fn.ParseFromString(temp);
    fn.set_stake(fn.stake() + pnl);
    namestore->Put(write_sync, hkey, fn.SerializeAsString());
    auto fnp = Commissioner::getName(hash);
    if ( fnp != nullptr) {
        fnp->addProfitLoss(pnl);
    }
    //if ( name == "Windo")
    //    qDebug() << "abcdefg" << fn.DebugString();
    qDebug() << "adding pnl" << pnl << " :: " << name.data ()<< fn.public_key().data ()<< fn.stake() << fn.bits();
    OnFantasyNamePnl(fn);
}

void FantasyNameData::AddProjection(const string &name, const string &player,
                             uint32_t proj, int32_t blocknum) {

    leveldb::Slice bval((char*)&proj, sizeof(uint32_t));
    string key(name + ":" + player);

    if (!projstore->Put(write_sync, key, bval).ok())
        qWarning() << " error writing proj" << player.data ()<< name.data ()<< proj;
    else
    {
        std::lock_guard<std::recursive_mutex> lockg{ data_mutex };
        int prev = FantasyNameProjections[name][player];
        FantasyNameProjections[name][player] = proj;
        PlayerIDProjections[player][name] = proj;
        int sum;
        if ( PlayerIDSumProj.find (player) == end(PlayerIDSumProj))
            sum = 0;
        else
            sum =  PlayerIDSumProj[player];

//        qDebug() << "sumplusproj" <<  sum << " + " << proj << " -  " << prev;
        sum += proj - prev;
        PlayerIDSumProj[player] =  sum;

        auto fnp = Commissioner::getName(name);
        if ( fnp != nullptr) {
            int nump = FantasyNameProjections[name].size();
            fnp->setBlockNump (blocknum,nump);

            key = "time^" + name;
            leveldb::Slice bval((char*)&blocknum, sizeof(int32_t));
            projstore->Put(write_sync, key, bval);

            OnProjection(name,player,proj,blocknum,nump);

        }
    }



    if ( name == "The Savages" && player == "1122" && proj == 5)
        qDebug() << "";

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

void FantasyNameData::OnGameStart(std::string gid,
                 std::vector<std::string> &home,
                 std::vector<std::string> &away
                 ) {
    std::lock_guard<std::recursive_mutex> lockg{ data_mutex };
    GameFantasyBitProj gfp{};

    gfp.set_gameid(gid);
    for ( auto pid : home) {
        fantasybit::FantasyBitProj fpj{};
        fpj.set_playerid(pid);

        //qDebug() << "yoyoyo home" << pid;
        for ( auto fnp : PlayerIDProjections[pid] ) {
            fpj.set_name(fnp.first);
            fpj.set_proj(fnp.second);
            gfp.add_home()->CopyFrom(fpj);
            //qDebug() << "yoyoyo" << fnp.first << fnp.second << fpj.DebugString();

        }
    }

    for ( auto pid : away) {
        fantasybit::FantasyBitProj fpj{};
        fpj.set_playerid(pid);

        //qDebug() << "yoyoyo away" << pid;
        for ( auto fnp : PlayerIDProjections[pid] ) {
            fpj.set_name(fnp.first);
            fpj.set_proj(fnp.second);
            gfp.add_away()->CopyFrom(fpj);
            //qDebug() << "yoyoyo" << fnp.first << fnp.second << fpj.DebugString();

        }
    }

    if (!projstore->Put(write_sync, gid, gfp.SerializeAsString()).ok())
        qWarning() << "error writing proj" << gid;
    else
        qInfo() << "OnGameStart " << gid;

}

GameFantasyBitProj FantasyNameData::GetGameProj(const std::string &gid) {
    GameFantasyBitProj gfp;

    string temp;
    if ( !projstore->Get(leveldb::ReadOptions(), gid, &temp).ok() ) {
        qWarning() << "cant GetGameProj" << gid.c_str();
        return gfp;
    }
    else
        qInfo() << "GetGameProj" << gid.data();


    gfp.ParseFromString(temp);
    return gfp;
}

void FantasyNameData::Subscribe(std::string in) {
    mSubscribed.insert(in);
}

void FantasyNameData::UnSubscribe(std::string in) {
    mSubscribed.erase(in);
}


void FantasyNameData::OnProjection(const std::string &name, const std::string &player,
                            uint32_t proj, int32_t blocknum, int nump) {
    if ( !amlive )
        return;

//    if ( mSubscribed.find(name) == end(mSubscribed))
//        return;

    fantasybit::FantasyBitProj fpj{};
    fpj.set_name(name);
    fpj.set_playerid(player);
    fpj.set_proj(proj);
    fpj.set_count(nump);
    fpj.set_block(blocknum);

    emit ProjectionLive(fpj);
}

void FantasyNameData::OnFantasyName(std::shared_ptr<FantasyName> fn) {
    //if ( !amlive )
    //    return;

    auto name = fn->alias();

#ifdef DATAAGENTWRITENAMES
#if !defined(DATAAGENTWRITENAMES_FORCE) || defined(DATAAGENTWRITENAMES_FORCE_NONAMES)
    if ( amlive )
#endif
    if ( name != Commissioner::FantasyAgentName())
    {
        FantasyNameHash fnh{};
        SqlStuff sql("satoshifantasy","OnFantasyName");

        fnh.set_name(name);
        fnh.set_hash(fn->hash());
        emit new_dataFantasyNameHash(fnh);

        auto fnhstr = fnh.SerializeAsString();
        sql.fantasyname(fnh);

        //RestfullClient rest(QUrl(LAPIURL.data()));
        //rest.postRawData("fantasy/name","oc",fnhstr.data(),((size_t)fnhstr.size()));
    }
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

void FantasyNameData::OnFantasyNamePnl(FantasyNameBal &fn) {
    if ( !amlive )
        return;

    if ( mSubscribed.find(fn.name()) != end(mSubscribed))
        emit FantasyNamePnl(fn);
}


void FantasyNameData::OnWeekOver(int in) {
    {
        std::lock_guard<std::recursive_mutex> lockg{ data_mutex };
        FantasyNameProjections.clear();
        PlayerIDProjections.clear();
        PlayerIDSumProj.clear();
    }
    auto *it = projstore->NewIterator(leveldb::ReadOptions());
    for (it->SeekToFirst(); it->Valid(); it->Next())
        projstore->Delete(write_sync, it->key());

    for ( auto fnb : Commissioner::GetFantasyNames() )
        fnb->setBlockNump(0,0);

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
        qDebug() << fn.first.data ();
        dump ( fn.second );
    }
}

void FantasyNameData::dump(std::unordered_map<std::string,int> &m) {
    for ( auto pa : m)
        qDebug() << pa.first.data ()<< pa.second;

}

void FantasyNameData::dumpProj() {
    qDebug() << "--FantasyNameProjections--";
    dump(FantasyNameProjections);
    qDebug() << "--PlayerIDProjections--";
    dump(PlayerIDProjections);
}

/*
void FantasyNameData::OnFantasyNameOrder(fantasybit::Order& ord) {
    if ( mSubscribed.find(ord.co) == end(mSubscribed))
        return;

    emit NewFantasyNameOrder(ord);
}
*/

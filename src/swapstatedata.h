/*
//  swapstatedata.h
//
//  Protoblock
//
//  Created by Jay Berg on 11/23/18
//
*/
#ifndef SWAPSTATEDATA_H
#define SWAPSTATEDATA_H


#include <QObject>
#include <leveldb/db.h>
#include <mutex>
#include <memory>
#include "ProtoData.pb.h"
#include <QDir>
#include "ldbwriter.h"
#include <unordered_map>
#include <fbutils.h>
#include <SwapData.pb.h>

//#include <set>
//#include "ApiData.pb.h"
//#include "FantasyName.h"
//#include "StateData.pb.h"
//#include "PeerNode.h"
//#include "pbutils.h"

namespace fantasybit {

struct SwapSeller {
    std::string fname;
    SwapAsk offer;
    uint64_t filled;
    uint64_t pending_fill;
    uint64_t open;

    SwapSeller() {}
    SwapSeller( const std::string &ifn, SwapAsk in)
                : fname(ifn), offer(in),
                  filled(0), pending_fill(0) {
        open = in.satoshi_max();
    }

};

struct SwapSells {
    uint64_t rate;
    std::list<SwapSeller> ssellers;
    int qty = 0;

    SwapSells(uint64_t irate) : rate(irate) {}
    void Add(const SwapSeller &inseller) {
        //ToDo: deal with multiple same buyers
        ssellers.push_back(inseller);
    }

};

struct SwapBuyer {
    SwapBuyer() {}
    SwapBuyer( const std::string &ifn, SwapBid in) : fname(ifn), bid(in) {}
    std::string fname;
    SwapBid bid;
//    uint64_t filled;
//    uint64_t pending_fill;
//    uint64_t open;
    bool is_pending = false;
};

struct SwapBuys {
    uint64_t rate;
    std::list<SwapBuyer> sbuyers;
    int qty = 0;

    SwapBuys(uint64_t irate) : rate(irate) {}

    void Add(const SwapBuyer &inbuyer) {
        //ToDo: deal with multiple same buyers
        sbuyers.push_back(inbuyer);
    }
};

struct SwapOrderBook {
    std::map<uint64_t, SwapBuys> bids;
    std::map<uint64_t, SwapSells> asks;
    void Add(SwapBuyer &inbuyer) {
        auto rate = inbuyer.bid.rate();
        auto it = bids.find(rate);
        if ( it == end(bids)) {
            auto pret = bids.insert( { rate ,  SwapBuys(rate) } );
            if ( !pret.second ) return;
            it = pret.first;
        }

        it->second.Add( inbuyer );
    }

    void Add(SwapSeller &inseller) {
        auto rate = inseller.offer.rate();
        auto it = asks.find(rate);
        if ( it == end(asks)) {
            auto pret = asks.insert( { rate ,  SwapSells(rate) } );
            if ( !pret.second ) return;
            it = pret.first;
        }

        it->second.Add( inseller );
    }

    void clear() {
        bids.clear();
        asks.clear();
    }
};

class SwapStateData : public QObject {
    Q_OBJECT

    //leveldb
    std::shared_ptr<leveldb::DB> swapbids, swapasks;
    leveldb::WriteOptions write_sync{};

    std::recursive_mutex data_mutex{};

//    std::unordered_map<std::string,SwapBid> mSwapNameMap;

    SwapOrderBook mOrderBook;
    bool amlive = false;
    std::string filedir(const std::string &in) {
        return fantasybit::GET_ROOT_DIR() + "swap/" + in;
    }

public:
    SwapStateData();
    void init();
    void closeAll();
    void AddNewSwapOrder(const SwapBid &inbid, const std::string &fname );
    void AddNewSwapOrder(const SwapAsk &inbid, const std::string &fname );
    void OnNewSwapTx(const SwapBid &inbid, const std::string &fname,const std::string &txid);
    void OnNewSwapTx(const SwapAsk &inoffer, const std::string &fname,const std::string &txid);

    std::vector<SwapOrder> GetCurrentSwapSnaps();
    SwapBuyer GetSwapBid(const QString &);
    SwapSeller GetSwapAsk(const QString &);


    void removeAll();
public slots:
    void OnLive(bool) {
        amlive = true;
    }

signals:
    void NewSwapData(fantasybit::SwapOrder);


};

}
#endif // SWAPSTATEDATA_H

/*

//    typedef std::unordered_map<std::string,std::unordered_map<std::string,int>> mapmapi;

//    mapmapi FantasyNameProjections;
//    std::unordered_map<std::string,
//            std::unordered_map<std::string,int>> PlayerIDProjections;

//    std::unordered_map<std::string,long> PlayerIDSumProj;


//    std::set<std::string> mSubscribed{};
//    bool amlive = false;
//    int week = 0;



    void clearProjCounts();
signals:
    void ProjectionLive(fantasybit::FantasyBitProj);
    void FantasyNameFound(string);
    void FantasyNameBalance(fantasybit::FantasyNameBal);
    void FantasyNamePnl(fantasybit::FantasyNameBal);
    void new_dataFantasyNameHash(fantasybit::FantasyNameHash);
    void NewFantasyNameOrder(fantasybit::Order&);
    void NewFantasyName(fantasybit::FantasyNameBal);

    void AnyFantasyNameBalance(fantasybit::FantasyNameBal);

public slots:
    void OnLive(bool subscribe) {
        amlive = true;
    }

    //void OnFantasyNameOrder(fantasybit::Order&);

public:
    void OnWeekOver(int week);
    void OnWeekStart(int week);
    int GetAvgProj(const string &playerid) {
        std::lock_guard<std::recursive_mutex> lockg{ data_mutex };
        int count = PlayerIDProjections[playerid].size();
        long sum = PlayerIDSumProj[playerid];

//        qDebug() << " sum " << sum << " count " << count;
        if  (count > 0 && sum > 0) {
            int ret = sum / count;
            if ( ret > count * 40)
                return 20;//PlayerIDProjections[playerid][0];
            else
                return ret;
        }
        else {
            if ( count == 0 && sum > 0 )
                PlayerIDSumProj[playerid] = 0;

            return 0;
        }
    }


public:

    FantasyNameData() {}
    void init();
    void closeAll();

    void DoTransfer(const std::string &from,const std::string &to,
                    const uint64_t amount, int openpnl, uint64_t nonce = 0);


    void AddNewName(std::string name, std::string pubkey,int32_t blocknum = 0);
    void AddBalance(const std::string name,uint64_t amount);
    void AddPnL(const std::string name, int64_t pnl);
    void AddProjection(const std::string &name, const std::string &player, uint32_t proj,int32_t);
    void OnProjection(const std::string &name, const std::string &player, uint32_t pro,int32_t,int);
    void OnFantasyName(std::shared_ptr<FantasyName> fn);

    void OnFantasyNameBalance(FantasyNameBal &fn);

    void OnFantasyNamePnl(FantasyNameBal &fn);

    //ToDo: the place to store game time IN/OUt status of NFL player
    //void UpdatePlayerGameStatus(const std::string &player, PlayerGameStatus status);

    std::unordered_map<std::string,int> GetProjById(const std::string &pid);
    std::unordered_map<std::string,int> GetProjByName(const std::string &nm);

    void Subscribe(std::string );
    void UnSubscribe(std::string );

    void OnGameStart(std::string gid,
                     std::vector<std::string> &home,
                     std::vector<std::string> &away
                     );

    GameFantasyBitProj GetGameProj(const std::string &gid);

    std::string filedir(const std::string &in);

    void dump(mapmapi &mm);

    void dump(std::unordered_map<std::string,int> &m);

    void dumpProj();

    void OnSeasonStart(int season) {
//        bootStrap(season,1);
    }

    void OnSeasonEnd(int season) {
        seasonFreeze(season);
    }

    void addBootStrap(Bootstrap *in,bool writehead = false) {
        LdbWriter ldb;
        ldb.init(Node::bootstrap.get());
        Bootstrap &bs = *in;

        bs.set_fnamemetaroot(BootStrapFantasyName(ldb));

        ldb.write(in->key(),ldb.write(bs));
        if ( writehead )
            ldb.write("head",in->key());

        return;
    }

    std::string BootStrapFantasyName(LdbWriter &ldb) {
        auto *it = namestore->NewIterator(leveldb::ReadOptions());
        FantasyNameBalMeta fnm;
        MerkleTree tree;

        for (it->SeekToFirst(); it->Valid(); it->Next()) {
            FantasyNameBal fn;
            fn.ParseFromArray(it->value().data(),it->value().size());
            fnm.set_name(fn.name());
            fnm.set_public_key(fn.public_key());
            fnm.set_bits(fn.bits());
            fnm.set_stake(fn.stake());
            tree.add_leaves(ldb.write(fnm));
        }
        delete it;

        tree.set_root(pb::makeMerkleRoot(tree.leaves()));
        return ldb.write(tree.root(),tree.SerializeAsString());
    }

    void seasonFreeze(int season) {
        return;
        closeAll();
        string moveto = GET_ROOT_DIR() + "freeze-" + std::to_string(season);
        QDir dir(moveto.data());
        if ( !dir.exists() )
            dir.mkdir(moveto.data());
        dir.rename(filedir("namestore").data(), (moveto + "/namestore").data());
        init();
    }
    bool DeltaTransfer(const std::string name, int64_t openpnl, int64_t amount, bool = false);
};
*/

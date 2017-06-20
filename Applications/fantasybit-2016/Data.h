//
//  Data.h
//  fantasybit
//
//  Created by Jay Berg on 3/25/14.
//
//
#ifndef __fantasybit__Data__
#define __fantasybit__Data__

#include <QObject>

#include "StaticData.pb.h"
#include "StatusData.pb.h"
#include <unordered_set>
#include <unordered_map>
#include <mutex>
#include <leveldb/db.h>
#include <utility>
#include <memory>
#include <QDir>
#include "fbutils.h"
#include "StateData.pb.h"
#include "ldbwriter.h"
#include "PeerNode.h"
#include "pbutils.h"

using namespace std;

namespace fantasybit
{


class NFLStateData : public QObject {

    Q_OBJECT

    std::shared_ptr<leveldb::DB> playerstore;
    std::shared_ptr<leveldb::DB> staticstore;
    std::shared_ptr<leveldb::DB> statusstore;
    std::shared_ptr<leveldb::DB> statsstore;

    leveldb::WriteOptions write_sync{};
    //int week = 0;
    int amlive = false;

    std::recursive_mutex data_mutex{};


    std::map<std::string, std::unordered_set<std::string>> MyTeamRoster;
    std::map<std::string, PlayerStatus> MyPlayerStatus;
    std::map<std::string, GameInfo> MyGameInfo;

    //std::map<std::string, bool> TeamLocked;

    void removePlayerTeam(const std::string &pid,const std::string &tid) {       

        MyTeamRoster[tid].erase(pid);
    }

    void addPlayerTeam(const std::string &pid,const std::string &tid) {

        MyTeamRoster[tid].insert(pid);
    }


signals:
    void PlayerStatusChange(pair<string,fantasybit::PlayerStatus>);
    void PlayerAdd(PlayerBase);
    void GameStart(string);
    void GlobalStateChange(fantasybit::GlobalState);
    void NewGameResult(string);

public slots:
    void OnLive(bool subscribe) {
        amlive = true;
    }
public:
    void OnWeekOver(int week);
    void OnWeekStart(int week);

public:
    NFLStateData() {}

#ifdef CHECKPOINTS_2015
    static void InitCheckpoint();
#endif

#ifndef NOUSE_GENESIS_BOOT
    static void InitCheckpoint(bool = false);
#endif

    void init();
    void closeAll();

    void AddNewPlayer(const std::string playerid, const PlayerBase &);
    void AddNewWeeklySchedule(int season, int week, const WeeklySchedule &);
    void AddGameResult(const string &gameid, const GameResult&);
    void ProcessAddGameResult(const string &gameid, const GameResult&);

    GameResult GetGameResult(const std::string &gameid);

    PlayerStatus GetPlayerStatus(const std::string &playerid);
    PlayerResult GetPlayerStats(const std::string &);

    PlayerBase GetPlayerBase(std::string playerid);

    //void AddTeamDepth(const TeamDepth &);
    void UpdatePlayerStatus(const std::string &playerid, const PlayerStatus &);
    //void UpdatePlayerStatus(std::string playerid, const PlayerGameStatus &);
    void UpdatePlayerStats( const PlayerResult &);


    void UpdateGameStatus(const std::string &gameid, const GameStatus &gs, bool force = false);

    void OnGameStart(const std::string &gameid, const GameStatus &gs);
    bool GetGameResult(const std::string &gameid, GameResult &result);

    std::vector<GameRoster> GetCurrentWeekGameRosters();
    std::vector<fantasybit::GameResult> GetPrevWeekGameResults(int season,int week);

    std::unordered_map<std::string,PlayerDetail>
            GetTeamRoster(const std::string &teamid);

    /*
    mUniqueSymbol["AB"]
            [0] - QB
            [1] - RB
            [2] - WR
            [3] - TE
            [4] - KI
    */
    std::map<std::string, vector<char>> mUniqueSymbol;
    static map<std::string,int> PosIndexMap;

    std::unordered_map<std::string,std::string> mSym2Pid;

    GameStatus GetUpdatedGameStatus(std::string id);
    WeeklySchedule GetWeeklySchedule(int season,int week);

    GlobalState GetGlobalState();
    void OnGlobalState(GlobalState &gs);

    GameInfo GetGameInfo(string gameid);

    std::string GenerateTicker(const std::string &pid,const PlayerStatus &ps) {
        std::string result;
        PlayerBase pba = GetPlayerBase(pid);
        if ( pba.position() == "DEF") {
            result = ps.teamid();
            if ( result.size() == 2 )
                result += (pba.last().at(0));
            result += 'D';
        }
        else {
            result += (pba.first().at(0));
            result += (pba.last().at(0));

//            if ( result == "JB")
//                qDebug() << "jbjbjbj" << pba.DebugString().data();

            auto it = mUniqueSymbol.find(result);
            if ( it == mUniqueSymbol.end()) {
                vector<char> ar(5,0);
//                char nc = ++(ar[PosIndexMap[pba.position()]]);
                ar[PosIndexMap[pba.position()]] = 'a';
                mUniqueSymbol.insert({result,ar});
//                result += nc;
                result += pba.position();
                if ( pba.position() == "K") result += 'I';
            }
            else {
                result += pba.position();
                if ( pba.position() == "K") result += 'I';
                char nxt = it->second[PosIndexMap[pba.position()]];
                if ( nxt == 0 )
                    it->second[PosIndexMap[pba.position()]] = 'a';
                else {
                    result += nxt;
                    ++(it->second[PosIndexMap[pba.position()]]);
                }
            }
        }

        qDebug() << result.data() << "GenerateTicker" << pid.data() << pba.DebugString().data() << ps.DebugString().data();
        return result;
    }

    void FromTicker(const std::string &ticker) {
        if ( ticker.size() < 4 ) {
            qWarning() << " bad ticker" << ticker.data();
            return;
        }

        if ( ticker[3] == 'D' ) return;
        int index;
        std::string pos = ticker.substr(2,2);
        if ( pos == "KI") index = 4;
        else
            index = PosIndexMap[pos];

        std::string initials = ticker.substr(0,2);
//        if ( initials == "JB")
//            qDebug() << "jbjbjbj" << ticker.data();
        auto it = mUniqueSymbol.find(initials);
        if ( it == mUniqueSymbol.end()) {
            vector<char> ar(5,0);

            ar[index] = (ticker.size() < 5) ? 'a' : ticker[4]+1;
            mUniqueSymbol.insert({initials,ar});
        }
        else {
            if (ticker.size() < 5)
                it->second[index] = 'a';
            else if ( it->second[index] <= ticker[4] )
                it->second[index] = ticker[4]+1;
        }
    }

    void TeamNameChange(const std::string &playerid, const PlayerBase &pb, const PlayerStatus &ps);

    void OnSeasonStart(int season) {
//        makeBootStrap(season,1,0);
    }

    void OnSeasonEnd(int season);

    Bootstrap makeBootStrap(int season, int week, int blocknum) {
        LdbWriter ldb;
        ldb.init(Node::bootstrap.get());
        Bootstrap bs;
        bs.set_blocknum(blocknum);
        bs.set_week(week);
        bs.set_season(season);
        bs.set_gamemetaroot(BootStrapSchedule(ldb));
        bs.set_playermetaroot(BootStrapPlayer(ldb));
        bs.set_gameresultroot(BootStrapResult(ldb));

        return bs;
    }

    std::unordered_map<int,pair<bool,string>> myKnownPlayerStatus() {
        std::unordered_map<int,pair<bool,string>> ret;

        auto *it = playerstore->NewIterator(leveldb::ReadOptions());

        for (it->SeekToFirst(); it->Valid(); it->Next()) {
            PlayerBase pb;
            pb.ParseFromArray(it->value().data(),it->value().size());
//            pm.set_allocated_player_base(&pb);

            string temp;
            PlayerStatus ps;
            if ( !statusstore->Get(leveldb::ReadOptions(), it->key(), &temp).ok() )
               qDebug() << "no status  " << it->key().ToString().data();
            else {
                !ps.ParseFromString(temp);
            }

            ret.insert({std::stoi(it->key().ToString()),{ps.status() == PlayerStatus_Status_ACTIVE,ps.teamid()}});
        }
        return  ret;
    }


    std::string BootStrapPlayer(LdbWriter &ldb) {
        auto *it = playerstore->NewIterator(leveldb::ReadOptions());
        PlayerMeta pm;
        MerkleTree tree;

        for (it->SeekToFirst(); it->Valid(); it->Next()) {
            PlayerBase pb;
            pb.ParseFromArray(it->value().data(),it->value().size());
            pm.set_allocated_player_base(&pb);

            string temp;
            PlayerStatus ps;
            if ( !statusstore->Get(leveldb::ReadOptions(), it->key(), &temp).ok() )
               qDebug() << "no status  " << it->key().ToString().data();
            else {
                !ps.ParseFromString(temp);
            }

            pm.set_playerid(it->key().ToString());
            pm.set_allocated_player_status(&ps);
            tree.add_leaves(ldb.write(pm));
            pm.release_player_base();
            pm.release_player_status();
            pm.Clear();
        }
        delete it;

        tree.set_root(pb::makeMerkleRoot(tree.leaves()));
        return ldb.write(tree.root(),tree.SerializeAsString());
    }

    std::string BootStrapSchedule(LdbWriter &ldb) {
        GameStatusMeta gsm;
        WeeklySchedule ws;
        MerkleTree tree;
        string temp;
        for ( int s = 2014; s<=theSeason();s++)
        for (int i=1; i<=17;i++) {
            string key = to_string(s) + "scheduleweek:" + to_string(i);

            if ( !staticstore->Get(leveldb::ReadOptions(), key, &temp).ok() ) {
                qWarning() << "BootStrapSchedule cant find schedule " << key.data();
                break;
            }

            if ( !ws.ParseFromString(temp) ) {
                qCritical() << "bad read WeeklySchedule ";
                continue;
            }

            for ( auto game : *ws.mutable_games()) {
//                GameInfo &game = *gm;
                gsm.set_allocated_gameinfo(&game);
                gsm.set_week(i);
                gsm.set_id(game.id());
                gsm.set_season(s);

                string key = "gamestatus:" + game.id();
                if ( statusstore->Get(leveldb::ReadOptions(), key, &temp).ok() ) {
                    gsm.mutable_gamesatus()->ParseFromString(temp);
                }
                else {
                    qDebug() << " no game status " << key.data() << gsm.has_gamesatus();
                }

                tree.add_leaves(ldb.write(gsm));
                gsm.release_gameinfo();
                gsm.Clear();
            }
        }
        tree.set_root(pb::makeMerkleRoot(tree.leaves()));
        return ldb.write(tree.root(),tree.SerializeAsString());
    }

    std::string BootStrapResult(LdbWriter &ldb) {
        WeeklySchedule ws;
        MerkleTree tree;
        string temp;
        for ( int s = 2014; s<=theSeason();s++)

        for (int i=1; i<=16;i++) {
            string key = to_string(s) + "scheduleweek:" + to_string(i);

            if ( !staticstore->Get(leveldb::ReadOptions(), key, &temp).ok() ) {
                qWarning() << "BootStrapResult cant find schedule " << key.data();
                break;
            }

            if ( !ws.ParseFromString(temp) ) {
                qCritical() << "bad read WeeklySchedule ";
                continue;
            }

            for ( auto game : ws.games()) {
                GameResult gr;
                if ( GetGameResult(game.id(),gr) ) {
                   tree.add_leaves(ldb.write(gr));
                }
            }
        }
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
        dir.rename(filedir("staticstore").data(), (moveto + "/staticstore").data());
        dir.rename(filedir("statusstore").data(), (moveto + "/statusstore").data());
//        dir.rename(filedir("playerstore").data(), (moveto + "/playerstore").data());
        init();
    }

    void OnGameClosed(const std::string &gameid) {
        GameStatus gs = GetUpdatedGameStatus(gameid);
        gs.set_status(GameStatus::CLOSED);
        UpdateGameStatus(gameid,gs, true);
    }

private:
    void OnNewPlayer(const std::string &pid);
    void OnPlayerTrade(const std::string &pid, const std::string &tid, const std::string &ntid);
    void OnPlayerRelease(const std::string &pid, const std::string &tid);
    void OnPlayerSign(const std::string &pid, const std::string &tid);
    void OnPlayerStatus(const std::string &pid,PlayerStatus ps);

    WeeklySchedule getWeeklyStaticSchedule(int season,int week);
    int week();
    int theSeason();

    static std::string filedir(const std::string &in);


};

}

#endif /* defined(__fantasybit__Data__) */

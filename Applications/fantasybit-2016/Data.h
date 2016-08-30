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

struct PlayerDetail {
    PlayerBase base;
    PlayerStatus::Status team_status;
    PlayerGameStatus game_status;
};

struct GameRoster {
    GameInfo info;
    GameStatus::Status  status;
    std::unordered_map<std::string,PlayerDetail> homeroster;
    std::unordered_map<std::string,PlayerDetail> awayroster;

};

class NFLStateData : public QObject {

    Q_OBJECT

    std::shared_ptr<leveldb::DB> playerstore;
    std::shared_ptr<leveldb::DB> staticstore;
    std::shared_ptr<leveldb::DB> statusstore;

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

#ifdef CHECKPOINTS
    static void InitCheckpoint();
#endif

    void init();
    void closeAll();

    void AddNewPlayer(const std::string playerid, const PlayerBase &);
    void AddNewWeeklySchedule(int week, const WeeklySchedule &);
    void AddGameResult(const string &gameid, const GameResult&);
    GameResult GetGameResult(const std::string &gameid);

    PlayerBase GetPlayerBase(std::string playerid);

    //void AddTeamDepth(const TeamDepth &);
    void UpdatePlayerStatus(const std::string &playerid, const PlayerStatus &);
    //void UpdatePlayerStatus(std::string playerid, const PlayerGameStatus &);

    void UpdateGameStatus(const std::string &gameid, const GameStatus &gs);

    void OnGameStart(const std::string &gameid, const GameStatus &gs);
    bool GetGameResult(const std::string &gameid, GameResult &result);

    std::vector<GameRoster> GetCurrentWeekGameRosters();
    std::vector<fantasybit::GameResult> GetPrevWeekGameResults(int week);

    std::unordered_map<std::string,PlayerDetail>
            GetTeamRoster(const std::string &teamid);

    GameStatus GetUpdatedGameStatus(std::string id);
    WeeklySchedule GetWeeklySchedule(int week);

    GlobalState GetGlobalState();
    void OnGlobalState(GlobalState &gs);

    GameInfo GetGameInfo(string gameid);

    void TeamNameChange(const std::string &playerid, const PlayerBase &pb, const PlayerStatus &ps);

    void OnSeasonStart(int season) {
//        makeBootStrap(season,1,0);
    }

    void OnSeasonEnd(int season) {
        seasonFreeze(season);
//        makeBootStrap(season,18,0);
    }

    Bootstrap makeBootStrap(int season, int week, int blocknum) {
        LdbWriter ldb;
        ldb.init(Node::bootstrap.get());
        Bootstrap bs;
        bs.set_blocknum(blocknum);
        bs.set_week(week);
        bs.set_season(season);
        bs.set_gamemetaroot(BootStrapSchedule(ldb));
        bs.set_playermetaroot(BootStrapPlayer(ldb));

        return bs;
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
            if ( !statusstore->Get(leveldb::ReadOptions(), it->key(), &temp).ok() )
                continue;

            PlayerStatus ps;
            if (!ps.ParseFromString(temp) )
                continue;

            pm.set_playerid(it->key().ToString());

            tree.add_leaves(ldb.write(pm));
            pm.release_player_base();
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
        for (int i=1; i<=17;i++) {
            string key = "scheduleweek:" + to_string(i);

            if ( !staticstore->Get(leveldb::ReadOptions(), key, &temp).ok() ) {
                qWarning() << "cant find schedule " << key.c_str();
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

                string key = "gamestatus:" + game.id();
                if ( statusstore->Get(leveldb::ReadOptions(), key, &temp).ok() )
                    gsm.mutable_gamesatus()->ParseFromString(temp);

                tree.add_leaves(ldb.write(gsm));
                gsm.release_gameinfo();
                gsm.Clear();
            }
        }
        tree.set_root(pb::makeMerkleRoot(tree.leaves()));
        return ldb.write(tree.root(),tree.SerializeAsString());
    }

    void seasonFreeze(int season) {
        closeAll();
        string moveto = GET_ROOT_DIR() + "freeze-" + std::to_string(season);
        QDir dir(moveto.data());
        if ( !dir.exists() )
            dir.mkdir(moveto.data());
        dir.rename(filedir("staticstore").data(), (moveto + "/staticstore").data());
        dir.rename(filedir("statusstore").data(), (moveto + "/statusstore").data());
        dir.rename(filedir("playerstore").data(), (moveto + "/playerstore").data());
        init();
    }
private:
    void OnNewPlayer(const std::string &pid);
    void OnPlayerTrade(const std::string &pid, const std::string &tid, const std::string &ntid);
    void OnPlayerRelease(const std::string &pid, const std::string &tid);
    void OnPlayerSign(const std::string &pid, const std::string &tid);
    void OnPlayerStatus(const std::string &pid,PlayerStatus ps);

    WeeklySchedule getWeeklyStaticSchedule(int week);
    int week();

    static std::string filedir(const std::string &in);


};

}

#endif /* defined(__fantasybit__Data__) */

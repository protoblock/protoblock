#ifndef COREAPI_H
#define COREAPI_H

#include "ProtoData.pb.h"

namespace fantasybit
{

//class CoreApi
//{
//public:
//    CoreApi();


//    int GetHeight(); //get current blockchain height from API
//    Block GetBlock(int height); //hget block from API


//    //Get all local name with private keys - can be done before Islive
//    std::vector<MyFantasyName> GetMyNames();

//    //choose which name to sign projections with
//    bool BeMyName(std::string name);

//    //OnLive() called , first time blockchina is in sync
//    void OnLive(DeltaData &snap);

//// only after OnLive()

//    //check if name avai -
//    bool IsNameAvail(std::string name);
//    bool ClaimName(std::string name);


//    bool MakeProjection(std::string player_id, int points);

//    // subscribe to get updates
//    // OnMyNameStatus
//    // OnMyNewProjection1111
//    void SubscribeMyName(std::string name);
//    void OnMyNameStatus(std::string name, MyNameStatus status);
//    void OnMySeenProjection(std::string name, std::pair<std::string,int> proj);


//    std::unordered_map<std::string,int> GetProjections(std::string name);

//    //OnReset when in bad state - out of sync
//    //void OnReset();

//    //On* called after OnLive()
//    // for each balance, team, global
//    // week, player  ------- change
//    //
//    void OnBalanceUpdate(std::string name, double balance);
//    void OnTeamState(TeamState &ts);
//    void OnGlobalState(GlobalState &gs);
//    void OnWeekChange(int newweek);
//    void OnPlayerData(std::string playerid,std::string teamid);
//    //std::unordered_map<std::string,int> GetTeamStates();

//    //int GetWeek();

//    //GetJson* calls PAPI for NFL and Fantasy Data
//    std::string GetJsonSchedule(int week);
//    std::string GetJsonFantasyPlayer(std::string playerid);
//    std::string GetJsonTeamPlayers(std::string teamid);
//    std::string GetJsonGameStats(std::string gameid);

//    std::string GetJsonResultAward(int week, std::string playerid);
//    std::string GetJsonBalanceDepth(int week, std::string fantasyname);

//};

}
#endif // COREAPI_H

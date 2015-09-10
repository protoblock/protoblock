#ifndef PLAYERLOADER
#define PLAYERLOADER

#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QFile>
#include "ProtoData.pb.h"
#include <QDateTime>
#include "RestFullCall.h"
#include <QByteArray>
#include "Commissioner.h"
#include <QDebug>
#include "StaticData.pb.h"
#include "DistributionAlgo.h"
#include "dataservice.h"
#include <QtSql/QSql>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlDriver>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>


/*
void print_hex_memory(void *mem,int size) {
  int i;
  unsigned char *p = (unsigned char *)mem;
  for (i=0;i<size;) {
    printf("0x%02x,", p[i]);
    if (++i%16==0)
      printf("\n");
  }
  printf("\n");
}
*/

using namespace std;
using namespace fantasybit;



struct SqlStuff {
    QSqlDatabase db{};

    SqlStuff() {
        db = QSqlDatabase::addDatabase("QMYSQL");

        db.setHostName(DBIP.data());
        db.setPort(3306);
        db.setDatabaseName(DBNAME.data());
        db.setUserName("root");
        db.setPassword("fantasyf00tball!");
        //bool success = true;
        if (!db.open()) {
             qDebug() << "Database error occurred :" << db.lastError().databaseText();
             //LogIt(db.lastError().databaseText().toStdString());
            return;
        }

    }

    void teams() {

        map<int,string> teamIdKey{};
        for (int i=0;i< Commissioner::GENESIS_NFL_TEAMS.size();i++) {
            teamIdKey[i+1] = Commissioner::GENESIS_NFL_TEAMS[i];
        }

        for ( auto tk : teamIdKey ) {
//        QSqlQuery query;
            QSqlQuery insertQuery(db);
            insertQuery.prepare("INSERT INTO team (tid, tkey) VALUES(:mid,:mkey)");
            insertQuery.bindValue(":mid",tk.first);
            insertQuery.bindValue(":mkey",QString::fromStdString(tk.second));
            //insertQuery.exec();
            if ( !insertQuery.exec() )
            {
                qDebug() << " exec ret " << insertQuery.lastError().databaseText();
                break;
            }

        }

        //for ()
        /*
         * CALL usp_GetPlayerID 'acf68a6a-3439-4ad6-8937-2562f4eba62b'

        while (query.next()) {
             QString name = query.value(0).toString();
             int salary = query.value(1).toInt();
             qDebug() << name << salary;
         }
         */
    }

    int getpidT(string tpid) {
        QSqlQuery query(db);

        query.prepare("SELECT playerid FROM pid p WHERE p.id = :tpid");
        query.bindValue(":tpid",QString::fromStdString(tpid));

        if ( ! query.exec() ) {
            qDebug() << " exec ret " << query.lastError().databaseText();

            return -1;
        }

        qDebug() << query.isActive() << query.isValid() <<
                    query.first();;

        auto ppid =  query.value(0);
        int pid = ppid.toInt();
        return pid;
    }

    int maxPid() {
        QSqlQuery query(db);

        query.prepare("SELECT max(playerid) FROM player");

        if ( ! query.exec() ) {
            qDebug() << " exec ret " << query.lastError().databaseText();

            return -1;
        }

        qDebug() << query.isActive() << query.isValid() <<
                    query.first();;

        auto ppid =  query.value(0);
        int pid = ppid.toInt();
        return pid;

    }

    pair<bool,string> getPlayer(int pid) {
        QSqlQuery query(db);

        query.prepare("select team, roster_status from player where playerid = :pid");
        query.bindValue(":pid",pid);
        if ( ! query.exec() ) {
            qDebug() << " exec ret " << query.lastError().databaseText();
            return make_pair(false,"");
        }

        qDebug() << query.isActive() << query.isValid() <<
                    query.first();;

        auto myteam = query.value(0).toString();
        //auto status = query.value(1).toChar().toString();
        auto sv = query.value("roster_status");
        auto svs = sv.toString();

        bool active = svs == "A";
        return make_pair(active,myteam.toStdString());

    }

    void gamemap(string gameid, string id) {
        QSqlQuery insertQuery(db);
        insertQuery.prepare("INSERT INTO gid (feed, id, gameid) VALUES(:feed, :mid,:mgameid)");
        insertQuery.bindValue(":feed","TRDR");

        insertQuery.bindValue(":mgameid",QString::fromStdString(gameid));
        insertQuery.bindValue(":mid",QString::fromStdString(id));
        //insertQuery.exec();
        if ( !insertQuery.exec() )
        {
            qDebug() << " exec ret " << insertQuery.lastError().databaseText();
        }

    }

    void playermap(int playerid, string id) {
        QSqlQuery insertQuery(db);
        insertQuery.prepare("INSERT INTO pid (feed, id, playerid) VALUES(:feed, :mid,:mplayerid)");
        insertQuery.bindValue(":feed","TRDR");

        insertQuery.bindValue(":mplayerid",playerid);
        insertQuery.bindValue(":mid",QString::fromStdString(id));
        //insertQuery.exec();
        if ( !insertQuery.exec() )
        {
            qDebug() << " exec ret " << insertQuery.lastError().databaseText();
        }

    }

    void player(PlayerData pd) {
        QSqlQuery insertQuery(db);

        if ( pd.has_player_base() ) {
            insertQuery.prepare
                ("INSERT INTO player (playerid,first,last,team,pos,roster_status)"
                 "VALUES(:pid,:f,:l,:t,:pos,:rs)");

            insertQuery.bindValue(":pid",std::stoi(pd.playerid()));
            insertQuery.bindValue(":f",QString::fromStdString(pd.player_base().first()));
            insertQuery.bindValue(":l",QString::fromStdString(pd.player_base().last()));
            insertQuery.bindValue(":pos",QString::fromStdString(pd.player_base().position()));
//            insertQuery.bindValue(":pid",std::stoi(pd.playerid()));
            insertQuery.bindValue(":t",QString::fromStdString(pd.player_status().teamid()));

            if ( pd.player_status().status() == PlayerStatus::ACTIVE  ||
                 pd.player_base().position() == "DEF"
                 )
                insertQuery.bindValue(":rs", QChar('A'));
            else
                insertQuery.bindValue(":rs", QChar('I'));

        }
        else if (pd.has_player_status()) {
            insertQuery.prepare
                ("UPDATE player set team = :t, roster_status = :rs where playerid = :pid");
            insertQuery.bindValue(":pid",std::stoi(pd.playerid()));
            insertQuery.bindValue(":t",QString::fromStdString(pd.player_status().teamid()));

            if ( pd.player_status().status() == PlayerStatus::ACTIVE )
                insertQuery.bindValue(":rs", QChar('A'));
            else
                insertQuery.bindValue(":rs", QChar('I'));
        }
        else return;

        //insertQuery.exec();
        if ( !insertQuery.exec() )
        {
            qDebug() << " exec ret " << insertQuery.lastError().databaseText();
        }

    }

    void playertest() {
        //playermap(1,"ARI");

        fantasybit::PlayerData pd{};
        pd.set_playerid(to_string(1));
        //pid[i+1] = team;

        fantasybit::PlayerBase pb{};
        pb.set_position("DEF");
        pb.set_first("Arizona");
        pb.set_last("Cardnals");

        fantasybit::PlayerStatus ps{};
        ps.set_teamid("ARI");

        pd.mutable_player_base()->CopyFrom(pb);
        pd.mutable_player_status()->CopyFrom(ps);

        player(pd);
    }

    ~SqlStuff() {
        db.close();
    }
};

class PlayerLoader
{

public:
    PlayerLoader(){}
    ~PlayerLoader(){}

    std::vector<fantasybit::PlayerData> loadPlayersFromJsonFile() { std::vector<fantasybit::PlayerData> result;

        //hard coded resources
        QFile jsonFile(":/Fantasy2015/Player.2015.json");
        if (!jsonFile.open(QIODevice::ReadOnly)){
            //errorMessage = "Can't open the player file !";
            return result;
        }
        QByteArray jsonData   = jsonFile.readAll();
        jsonFile.close();
        QJsonParseError * error = NULL;
        QJsonDocument doc = QJsonDocument::fromJson(jsonData,error);

        if (error != NULL){
            //errorMessage = error->errorString();
            return result;
        }
        if (doc.isEmpty()) {
            //errorMessage = "File seems to be empty.";
            return result;
        }

        QJsonArray playerJsonData = doc.array();

        for (int i=0;i< playerJsonData.size();i++  ) {
            QJsonValueRef data = playerJsonData[i];
            //we are supposed to encounter a list of objects so skip values
            if (!data.isObject()) continue;
            QJsonObject playerData = data.toObject();
            QString errorParsingObject;
            fantasybit::PlayerData player =
                    getPlayerFromJsonObject(playerData,errorParsingObject);
            if (!errorParsingObject.isEmpty()) {
                //errorMessage += "\n Error parsing json object : "+ errorParsingObject;
                continue;
            }

//            QFile outfile("GenesisPLayer.txt");
//            outfile.open(QIODevice::WriteOnly);

            //outfile.writeData(player.ser)
            std::string ps = player.SerializeAsString();
            //print_hex_memory((void *)ps.data(),ps.size());
            result.push_back(player);
        }
        return result;
    }

   fantasybit::PlayerData getPlayerFromJsonObject(QJsonObject & jsonObject,QString & errorParsingObject) const{
      fantasybit::PlayerData pd{};

      auto pos = jsonObject.value("FantasyPosition").toString();
      if   (! (    pos == "QB"
           || pos == "RB"
           || pos == "WR"
           || pos == "TE"
           || pos == "K")) {
          errorParsingObject = pos;
          return pd;
      }

      pd.set_playerid(jsonObject.value("PlayerID").toString().toStdString());

      fantasybit::PlayerBase pb{};
      pb.set_position(jsonObject.value("FantasyPosition").toString().toStdString());
      pb.set_first(jsonObject.value("FirstName").toString().toStdString());
      pb.set_last(jsonObject.value("LastName").toString().toStdString());

      fantasybit::PlayerStatus ps{};
      ps.set_teamid(jsonObject.value("Team").toString().toStdString());

      pd.mutable_player_base()->CopyFrom(pb);
      pd.mutable_player_status()->CopyFrom(ps);

      return pd;
   }

};

class ScheduleLoader
{

public:
    ScheduleLoader(){}
    ~ScheduleLoader(){}
    std::map<string,string> gid{};

    map<string,int> teamIdKey{};
    SqlStuff sqls{};
    void Dump() {
        for ( auto p : gid ) {
            sqls.gamemap(p.first,p.second);
        }
    }
    std::vector<fantasybit::ScheduleData> loadScheduleFromJsonFile() {
        std::vector<fantasybit::ScheduleData> result;

        for (int i=0;i< Commissioner::GENESIS_NFL_TEAMS.size();i++) {
            teamIdKey[Commissioner::GENESIS_NFL_TEAMS[i]] = i+1;
        }

        //hard coded resources
        QFile jsonFile(":/Schedule2015/Schedule.2015-reg.json");
        if (!jsonFile.open(QIODevice::ReadOnly)){
            //errorMessage = "Can't open the player file !";
            return result;
        }
        QByteArray jsonData   = jsonFile.readAll();
        jsonFile.close();
        QJsonParseError * error = NULL;
        QJsonDocument doc = QJsonDocument::fromJson(jsonData,error);

        if (error != NULL){
            //errorMessage = error->errorString();
            return result;
        }
        if (doc.isEmpty()) {
            //errorMessage = "File seems to be empty.";
            return result;
        }  

        //if (doc.isObject())
        QJsonObject jo = doc.object();
        auto vals = jo.value("weeks");


        QJsonArray wkarr = vals.toArray();
        for (int j=0;j< wkarr.size();j++  ) {
            fantasybit::ScheduleData sd{};
            QJsonValueRef wkdata = wkarr[j];
            auto wkobj = wkdata.toObject();
            int wk = wkobj.value("number").toInt();
            sd.set_week(wk);

            fantasybit::WeeklySchedule ws{};
            auto games = wkobj.value("games");
            QJsonArray gmarr = games.toArray();
            for (int i=0;i< gmarr.size();i++  ) {
                QJsonValueRef data = gmarr[i];
                //we are supposed to encounter a list of objects so skip values
                if (!data.isObject()) continue;
                QJsonObject playerData = data.toObject();
                QString errorParsingObject;
                fantasybit::GameInfo gi =
                        getScheduleFromJsonObject(playerData,wk,errorParsingObject);
                if (!errorParsingObject.isEmpty()) {
                    //errorMessage += "\n Error parsing json object : "+ errorParsingObject;
                    continue;
                }

    //            QFile outfile("GenesisPLayer.txt");
    //            outfile.open(QIODevice::WriteOnly);

                ws.add_games()->CopyFrom(gi);

                //outfile.writeData(player.ser)
                //std::string ps = player.SerializeAsString();
                //print_hex_memory((void *)ps.data(),ps.size());
            }

            sd.mutable_weekly()->CopyFrom(ws);

            result.push_back(sd);
        }

        Dump();
        return result;
    }

   fantasybit::GameInfo getScheduleFromJsonObject(QJsonObject & jsonObject, int wk,
                                                  QString & errorParsingObject) {
      fantasybit::GameInfo pd{};

      /*
1500122
year season_type(0) week(01) homeis(22)
   */
      string swk = to_string(wk);
      if ( swk.size() == 1)
          swk = "0" + swk;

      string home = jsonObject.value("home").toString().toStdString();

      string team = to_string(teamIdKey[home]);
      if ( team.size() == 1)
          team = "0" + team;

      string myid("20150");
      myid = myid.append(swk).append(team);
      gid[myid] =  jsonObject.value("id").toString().toStdString();

      pd.set_id(myid);
      pd.set_home(home);
      pd.set_away(jsonObject.value("away").toString().toStdString());

      auto datetime = jsonObject.value("scheduled").toString();

      auto qdt = QDateTime::fromString(datetime,Qt::ISODate);

      pd.set_time(qdt.toTime_t());

      qDebug() << pd.DebugString();

      return pd;

    }

};

class PlayerLoaderTR {
    QString uribase = "http://api.sportradar.us/nfl-b1/teams";
    QString uritail = "/roster.json?api_key=2uqzuwdrgkpzkhbfumzrg8gn";

    QString makeroute(std::string team) {
        return QString::fromStdString(team) + uritail;
    }


public:
    int start = 1000;
    std::map<int,string> pid{};

    std::vector<fantasybit::PlayerData> result;
    std::set<fantasybit::PlayerData> nochange;
    SqlStuff sqls{};

    std::unordered_map<std::string,pair<string,bool>> myknownplayerstatus;

    //std::map<string,std::unordered_map<std::string,PlayerDetail> *>
    //teamrosters;
    void dump() {
        for ( auto p : pid ) {
            sqls.playermap(p.first,p.second);
        }

        for ( auto pd : result) {
           // if ( pd.has_player_base())
           sqls.player(pd);
        }
    }

    std::vector<fantasybit::PlayerData> loadPlayersFromTradeRadar(bool isgenesis = false) {

        if ( !isgenesis ) {
            start = sqls.maxPid();

            std::vector<fantasybit::GameRoster>
            gameroster =  DataService::instance()->GetCurrentWeekGameRosters();

            for ( auto games : gameroster ) {
                for ( auto p : games.homeroster) {
                    bool isactive
                    = (p.second.team_status == PlayerStatus_Status_ACTIVE);
                    myknownplayerstatus[p.first] = make_pair(games.info.home(),isactive);
                }
                for ( auto p : games.awayroster ) {
                    bool isactive
                    = (p.second.team_status == PlayerStatus_Status_ACTIVE);
                    myknownplayerstatus[p.first] = make_pair(games.info.away(),isactive);
                }

            }

        }

        RestfullClient rest(QUrl("http://api.sportradar.us/nfl-b1/teams/"));

        for ( int i = 0; i < fantasybit::Commissioner::GENESIS_NFL_TEAMS.size(); i++) {
            auto team = fantasybit::Commissioner::GENESIS_NFL_TEAMS[i];
            QThread::currentThread()->msleep(1000);
            auto route = makeroute(team);
            QMap<QString,QString>  headers;
            QMap<QString,QVariant> params;
            //params.insert("api_key","2uqzuwdrgkpzkhbfumzrg8gn");

            qDebug() << uribase << route;
            rest.getData(route,params,headers);
            auto resp = rest.lastReply();

            qDebug() << resp;
            QJsonDocument ret = QJsonDocument::fromJson(resp);
            qDebug() << ret.isNull() << ret.isEmpty() << ret.isArray() << ret.isObject();

            QJsonObject jo = ret.object();
            //auto vals = jo.value("name").toString();
            if ( isgenesis ) {
                fantasybit::PlayerData pd{};
                pd.set_playerid(to_string(i+1));
                pid[i+1] = team;

                fantasybit::PlayerBase pb{};
                pb.set_position("DEF");
                pb.set_first(jo.value("market").toString().toStdString());
                pb.set_last(jo.value("name").toString().toStdString());

                fantasybit::PlayerStatus ps{};
                ps.set_teamid(team);

                pd.mutable_player_base()->CopyFrom(pb);
                pd.mutable_player_status()->CopyFrom(ps);

                sqls.player(pd);
                result.push_back(pd);
            }

            auto vals = jo.value("players");
            QJsonArray parr = vals.toArray();

            for (int i=0;i< parr.size();i++  ) {
                QJsonValueRef data = parr[i];

                QJsonObject playerData = data.toObject();
                QString errorParsingObject;
                fantasybit::PlayerData player =
                        getPlayerFromJsonObject(playerData,team,isgenesis,errorParsingObject);

                if ( errorParsingObject != "" ) continue;
                result.push_back(player);
            }

        }

        //dump();
        return result;
    }

    fantasybit::PlayerData getPlayerFromJsonObject(QJsonObject & jsonObject,std::string team,bool isgenesis,
                                                   QString & errorParsingObject) {

       auto tid = jsonObject.value("id").toString().toStdString();

       bool isnew = false;
       int mypid;
       if ( isgenesis )
           isnew = true;
       else {
           mypid = sqls.getpidT(tid);
           if ( mypid <= 0)
               isnew = true;
       }

       fantasybit::PlayerData pd{};

       auto pos = jsonObject.value("position").toString();
       if   (! (pos == "QB"
            || pos == "RB"
            || pos == "WR"
            || pos == "TE"
            || pos == "K"
            || pos == "FB")) {
           errorParsingObject = pos;
           return pd;
       }

       if ( pos == "FB")
           pos = "RB";

       fantasybit::PlayerStatus ps{};
       auto status = jsonObject.value("status").toString();
       bool isnowactive = status == "ACT";

       if ( isnew ) {
            start++;
            pd.set_playerid(to_string(start));
            pid[start] = jsonObject.value("id").toString().toStdString();


           fantasybit::PlayerBase pb{};
           pb.set_position(pos.toStdString());
           pb.set_first(jsonObject.value("name_first").toString().toStdString());
           pb.set_last(jsonObject.value("name_last").toString().toStdString());

           ps.set_teamid(team);
           ps.set_status(isnowactive ? fantasybit::PlayerStatus_Status_ACTIVE : fantasybit::PlayerStatus_Status_INACTIVE);

    /*
           enum PlayerStatus_Status {
             PlayerStatus_Status_ACTIVE = 0,
             PlayerStatus_Status_INACTIVE = 1,
             PlayerStatus_Status_OTHER = 3,
             PlayerStatus_Status_FA = 4
           };
    */

           pd.mutable_player_base()->CopyFrom(pb);
           pd.mutable_player_status()->CopyFrom(ps);

           return pd;
           }

       else {
           bool changed = false;

           //know team
           string smypid = to_string(mypid);
           auto iter = myknownplayerstatus.find(smypid);
           if ( iter == myknownplayerstatus.end()  ) {
                auto pde = DataService::instance()->GetPlayerBase(smypid);
                if ( !pde.has_first() || pde.first() == "") {
                    errorParsingObject = "know player but not on roster?? " ;
                    return pd;
                }

                changed = true;
           }
           else {
               if ( iter->second.second != isnowactive ||
                    iter->second.first != team )
                   changed = true;

               myknownplayerstatus.erase(iter);
           }


           if ( changed ) {
               ps.set_teamid(team);
               ps.set_status(isnowactive ? fantasybit::PlayerStatus_Status_ACTIVE : fantasybit::PlayerStatus_Status_INACTIVE);
               pd.mutable_player_status()->CopyFrom(ps);
           }
           else
               errorParsingObject = pos;

           return pd;
       }

    }

};
 
class GameStatsLoader {
    QString uribase = "http://api.sportradar.us/nfl-b1/2015";
    QString uritail = "/statistics.json?api_key=2uqzuwdrgkpzkhbfumzrg8gn";
        
    
    QString makeroute(int week, string home, string away) {
        string inr = preregpost 
                        + "/" + to_string(week) 
                        + "/" + away 
                        + "/" + home; 
        
                
        return QString::fromStdString(inr) + uritail;
                
    }

    string preregpost;
public:
    GameStatsLoader(string seasontype) {
        preregpost = seasontype;
    }
    
    GameStatsLoader() {
        preregpost = "REG";
    }
    
    std::vector<fantasybit::GameResult> 
                loadGameStatsFromTradeRadar(int week,vector<GameInfo> &games) {
        std::vector<fantasybit::GameResult> result;

        RestfullClient rest(QUrl("http://api.sportradar.us/nfl-b1/2015"));

        for ( auto game : games) {

            auto route = makeroute(week,game.home(),game.away());
            QMap<QString,QString>  headers;
            QMap<QString,QVariant> params;
            //params.insert("api_key","2uqzuwdrgkpzkhbfumzrg8gn");

            qDebug() << uribase << route;
            rest.getData(route,params,headers);
            auto resp = rest.lastReply();

            qDebug() << resp;
            QJsonDocument ret = QJsonDocument::fromJson(resp);
            qDebug() << ret.isNull() << ret.isEmpty() << ret.isArray() << ret.isObject();

            QJsonObject jo = ret.object();

            //std::map<QString,Stats> homestats{};
            //std::map<QString,Stats> awaystats{};
            
            GameResult gr{};
            gr.set_gameid(jo.value("id").toString().toStdString());
            for ( auto tm : {QString("home_team"), QString("away_team")}) {

                auto playerresults = ( tm == "home_team" ) ?
                            gr.mutable_home_result() : gr.mutable_away_result();

                auto vals = jo.value(tm).toObject().value("statistics");
                auto sobj = vals.toObject();
                QJsonArray rush = sobj.value("rushing")
                        .toObject().value("players").toArray();

                QJsonArray pass = sobj.value("passing")
                        .toObject().value("players").toArray();

                QJsonArray rec = sobj.value("receiving")
                        .toObject().value("players").toArray();

                std::map<QString,Stats> tstats{};

                //for ( auto tstats : {homestats, awaystats}) {
                map<string,QJsonArray> mp = { {"rush", rush}, {"pass", pass}, {"rec",rec}};
                for ( auto rpsp : mp ) {
                    auto rps = rpsp.second;
                    for ( int i=0;i<rps.size();i++) {
                        QJsonValueRef data = rps[i];
                        QJsonObject odata = data.toObject();
                        auto id = odata.value("id").toString();
                        auto it = tstats.find(id);
                        if ( it == end(tstats))
                            tstats[id] = Stats{};

                        Ostats * ostat = tstats[id].mutable_ostats();
                        getStats(ostat,rpsp.first,odata);
                    }
                }

                for ( auto pstats : tstats) {
                    PlayerResult pr{};
                    pr.set_playerid(pstats.first.toStdString());
                    pr.mutable_stats()->CopyFrom(pstats.second);
                    pr.set_result(CalcResults(pstats.second));
                    playerresults->Add()->CopyFrom(pr);
                }
            }
            result.push_back(gr);
        }

        return result;
    }

    static double CalcResults(const Stats &stats) {
        int ret = 0;
        double iret = 0;

        if ( stats.has_ostats() ) {
            auto os = stats.ostats();
            if ( os.has_passtd())
                ret += 400 * os.passtd(); //PAssing Yards (QB)


            if ( os.has_rushtd() )
                ret += 600 * os.rushtd();

            if ( os.has_rectd() )
                ret += 600 *  os.rectd();

            if ( os.has_passyds() )
               ret += 5 *  os.passyds();

            if ( os.has_recyds() )
                ret += 10 *  os.recyds();

            if (  os.has_rushyds() )
                ret += 10 *  os.rushyds();

            if ( os.has_rec() )
                ret += 100 * os.rec();

            if ( os.has_pint() )
                ret += -100 * os.pint();

            if ( os.has_twopt() )
                ret += 200 * os.twopt();

            if (os.has_onept())
                ret += 200 *  os.onept();
        }
        if ( stats.has_kstats() ) {
            auto ks = stats.kstats();
            if ( ks.has_pa() )
                ret += 100 * ks.pa();
            for ( auto f : ks.fg())
                ret += 300 + 10 * ((f > 30) ? f : 0);
        }
        if ( stats.has_dstats() ) {
            auto ds = stats.dstats();
            if ( ds.has_deftd())
                ret += 600 * ds.deftd();
            if ( ds.has_onept())
                ret += 200 * ds.onept();
            if ( ds.has_ptsa()) {
                if ( ds.ptsa() == 0)
                    ret += 1200;
                else if ( ds.ptsa() < 7)
                    ret += 100;
                else if ( ds.ptsa() < 11)
                    ret += 800;
            }
            if ( ds.has_sacks())
                ret += 100 * ds.sacks();
            if ( ds.has_sfty())
                ret += 500 * ds.sfty();
            if ( ds.has_turnovers())
                ret += 200 * ds.turnovers();
            if ( ds.has_twopt())
                ret += 200 * ds.twopt();
        }

        if ( ret == 0) return 0.0;

        iret = (double)ret / 100.0;
        return iret;
    }

    void getStats(Ostats *ostat,std::string type,QJsonObject & jsonObject) const {

       auto yds = jsonObject.value("yds");
       int y = yds.toInt();
       if (y != 0) {
           if ( type == "pass")
               ostat->set_passyds(y);
           else if ( type == "rush")
               ostat->set_rushyds(y);
           else if ( type == "rec")
               ostat->set_recyds(y);
       }

       auto td = jsonObject.value("td");
       int t = td.toInt();
       if (t > 0) {
           if ( type == "pass")
               ostat->set_passtd(t);
           else if ( type == "rush")
               ostat->set_rushtd(t);
           else if ( type == "rec")
               ostat->set_rectd(t);
       }

       if ( type == "rec" ) {
           auto rec = jsonObject.value("rec");
           int r = rec.toInt();
           if ( r > 0 )
               ostat->set_rec(r);
       }
       else if ( type == "pass" ) {
           auto rec = jsonObject.value("int");
           int r = rec.toInt();
           if ( r > 0 )
               ostat->set_pint(r);
       }

    }
};

#endif // PLAYERLOADER


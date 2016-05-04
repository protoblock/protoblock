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
   //  db{};

    SqlStuff(string dbname) {
        init(dbname);
    }

    SqlStuff(const string &dbname, const string &connectionName) {
        init(dbname, connectionName);
    }

    SqlStuff(const char* dbname, const char* connectionName) {
        init(dbname, connectionName);
    }

    SqlStuff() {
        init(DBNAME);
    }

    SqlStuff(bool useDefault, const string &connectionName) {
        init(DBNAME,connectionName);
    }


    void init(string dbname) {
        init(dbname,dbname + "_defaultConnection");
    }

    QString conname;
    void init(string dbname, string connectionName) {
        qDebug() << "sql init " << dbname << connectionName;
        conname = QString::fromStdString(connectionName);
        QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL",conname);

        db.setHostName(DBIP.data());
        db.setPort(3306);
        db.setDatabaseName(dbname.data());
        db.setUserName("root");
        db.setPassword("fantasyf00tball!");
        //bool success = true;
        if (!db.open()) {
             qDebug() << "Database error occurred :" << db.lastError().databaseText();
             //LogIt(db.lastError().databaseText().toStdString());
            return;
        }

    }


    int lastSeq() {
        QSqlDatabase db = QSqlDatabase::database (conname);
        QSqlQuery query(db);

        query.prepare("SELECT max(seqnum) FROM trades  WHERE isprocessed = 1");
        //order by t.seqnum desc limit 1);

        //query.bindValue(":seqn",seqnum);
        if ( ! query.exec() ) {
            qDebug() << " exec ret " << query.lastError().databaseText();
            db.close();
            return 0;
        }
        db.close();


        qDebug() << query.executedQuery() << query.isValid() <<
                    query.first();


        auto ppid =  query.value(0);
        int pid = ppid.toInt();
        return pid;


    }

    void dumpTx(int seqnum) {
        QSqlDatabase db = QSqlDatabase::database (conname);
        QSqlQuery query(db);

        query.prepare("SELECT Transactions FROM transactions t WHERE t.SeqNum = 6236");

        //query.bindValue(":seqn",seqnum);
        if ( ! query.exec() ) {
            qDebug() << " exec ret " << query.lastError().databaseText();
            db.close();
            return;
        }
        db.close();


        qDebug() << query.executedQuery() << query.isValid() <<
                    query.isActive();


        auto tx =  query.value(0);

        auto btx = tx.toByteArray();

        SignedTransaction st{};
        st.ParseFromArray(btx.data(),btx.size());

        qDebug() << st.DebugString();

    }

    void teams() {

        map<int,string> teamIdKey{};
        for (int i=0;i< Commissioner::GENESIS_NFL_TEAMS.size();i++) {
            teamIdKey[i+1] = Commissioner::GENESIS_NFL_TEAMS[i];
        }

        for ( auto tk : teamIdKey ) {
//        QSqlQuery query;
            QSqlDatabase db = QSqlDatabase::database (conname);
            QSqlQuery insertQuery(db);
            insertQuery.prepare("INSERT INTO team (tid, tkey) VALUES(:mid,:mkey)");
            insertQuery.bindValue(":mid",tk.first);
            insertQuery.bindValue(":mkey",QString::fromStdString(tk.second));
            //insertQuery.exec();
            bool good = insertQuery.exec();
            db.close();

            if ( !good  ) {
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
        return getpid("TRDR",tpid);
    }

    int getpid(string feed, string tpid) {
        QSqlDatabase db = QSqlDatabase::database (conname);
        QSqlQuery query(db);

        query.prepare("SELECT playerid FROM pid p WHERE p.id = :tpid and p.feed = :feedid");
        query.bindValue(":feedid",QString::fromStdString(feed));
        query.bindValue(":tpid",QString::fromStdString(tpid));

        bool good = query.exec();
        db.close();

        if ( !good  ) {
            qDebug() << " exec ret " << query.lastError().databaseText();
            return -1;
        }


        qDebug() << query.isActive() << query.isValid() <<
                    query.first();;

        auto ppid =  query.value(0);
        int pid = ppid.toInt();
        return pid;
    }

    unordered_map<string,int> getallpid(string feed) {
        unordered_map<string,int> ret;

        QSqlDatabase db = QSqlDatabase::database (conname);
        QSqlQuery query(db);

        query.prepare("SELECT id,playerid FROM pid p WHERE p.feed = :feedid");
        query.bindValue(":feedid",QString::fromStdString(feed));

        bool good = query.exec();
        db.close();

        if ( !good  ) {
            qDebug() << " exec ret " << query.lastError().databaseText();
            return ret;
        }


        qDebug() << query.isActive() << query.isValid() <<
                    query.first();

        int idf = 0;//query.rec.indexOf("id");
        int playeridf = 1;//query.rec.indexOf("playerid");

        while (query.next()) {
            QString id = query.value(idf).toString();
            int pid = query.value(playeridf).toInt();
            ret[id.toStdString()] = pid;
        }

        return ret;
    }

    string getgidT(string tgid) {
        QSqlDatabase db = QSqlDatabase::database (conname);
        QSqlQuery query(db);

        query.prepare("SELECT gameid FROM gid g WHERE g.id = :tgid");
        query.bindValue(":tgid",QString::fromStdString(tgid));

        bool good = query.exec();
        db.close();

        if ( ! good ) {
            qDebug() << " exec ret " << query.lastError().databaseText();

            return "";
        }

        qDebug() << query.isActive() << query.isValid() <<
                    query.first();;

        return query.value(0).toString().toStdString();
        //int pid = ppid.toInt();
        //return pid;
    }

    int maxPid() {
        QSqlDatabase db = QSqlDatabase::database (conname);
        QSqlQuery query(db);

        query.prepare("SELECT max(playerid) FROM player");

        bool good = query.exec();
        db.close();

        if ( ! good ) {
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
        QSqlDatabase db = QSqlDatabase::database (conname);
        QSqlQuery query(db);

        query.prepare("select team, roster_status from player where playerid = :pid");
        query.bindValue(":pid",pid);

        bool good = query.exec();
        db.close();

        if ( ! good ) {
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
        QSqlDatabase db = QSqlDatabase::database (conname);
        QSqlQuery insertQuery(db);
        insertQuery.prepare("INSERT INTO gid (feed, id, gameid) VALUES(:feed, :mid,:mgameid)");
        insertQuery.bindValue(":feed","TRDR");

        insertQuery.bindValue(":mgameid",QString::fromStdString(gameid));
        insertQuery.bindValue(":mid",QString::fromStdString(id));
        //insertQuery.exec();
        bool good = insertQuery.exec();
        db.close();

        if ( !good ) {
            qDebug() << " exec ret " << insertQuery.lastError().databaseText();
        }

    }

    void playermap(int playerid, string id) {
        playermapFeed("TRDR",playerid,id);
    }

    void distribute(Distribution &dist) {

        QSqlDatabase db = QSqlDatabase::database (conname); // Open Connection
        QSqlQuery insertQuery(db);

        insertQuery.prepare
            ("INSERT INTO distribution "
             "(fantasynameid,gameid,playerid,teamid,season,week,projection,award,result)"
             "VALUES(:fnid, :gid, :pid,:tid,:s,:w,:proj,:award, :res)");

        insertQuery.bindValue(":fnid",dist.fantasy_nameid());
        insertQuery.bindValue(":gid",QString::fromStdString(dist.gameid()));
        insertQuery.bindValue(":pid",QString::fromStdString(dist.playerid()));
        insertQuery.bindValue(":tid",QString::fromStdString(dist.teamid()));
        insertQuery.bindValue(":s",dist.season());
        insertQuery.bindValue(":w",dist.week());
        insertQuery.bindValue(":proj",dist.proj());
        insertQuery.bindValue(":award",dist.award());
        insertQuery.bindValue(":res",dist.result());


        bool good = insertQuery.exec();
        //db.close();

        if ( ! good ) {
            qDebug() << " exec ret " << insertQuery.lastError().databaseText();
            qDebug() << dist.DebugString();
        }
    }

    void profit(Profits &dist) {

        QSqlDatabase db = QSqlDatabase::database (conname); // Open Connection
        QSqlQuery insertQuery(db);

        insertQuery.prepare
            ("INSERT INTO profits "
             "(fantasynameid,gameid,playerid,teamid,season,week,qty,price,pnl,result)"
             "VALUES(:fnid, :gid, :pid,:tid,:s,:w,:qty,:price,:pnl,:res)");

        insertQuery.bindValue(":fnid",dist.fantasy_nameid());
        insertQuery.bindValue(":gid",QString::fromStdString(dist.gameid()));
        insertQuery.bindValue(":pid",QString::fromStdString(dist.playerid()));
        insertQuery.bindValue(":tid",QString::fromStdString(dist.teamid()));
        insertQuery.bindValue(":s",dist.season());
        insertQuery.bindValue(":w",dist.week());
        insertQuery.bindValue(":qty",dist.qty());
        insertQuery.bindValue(":price",dist.price());
        insertQuery.bindValue(":pnl",dist.pnl());
        insertQuery.bindValue(":res",dist.result());


        bool good = insertQuery.exec();
        //db.close();

        if ( ! good ) {
            qDebug() << " exec ret " << insertQuery.lastError().databaseText();
            qDebug() << dist.DebugString();
        }
    }

    void quote(const string &inplayerid, const ContractOHLC &ohlc) {

        QSqlDatabase db = QSqlDatabase::database (conname); // Open Connection
        QSqlQuery insertQuery(db);


        insertQuery.prepare
            ("REPLACE into player_contract_quote "
             "(playerid,price,volume,`change`) "
             " VALUES(:pid, :pr, :vol, :chg)");
        /*
             "ON DUPLICATE KEY "
             "UPDATE player_contract_quote "
              "set price = :pr, volume = :vol, `change` = :chg where playerid = :pid");

        /*
        insertQuery.prepare
            ("UPDATE player_contract_quote "
             "set price = :pr, volume = :vol, `change` = :chg where playerid = :pid");
        /*
        insertQuery.prepare
        ("UPDATE player_contract_quote "
        "set price = :pr, volume = :vol, `change` = :chg where playerid = :pid"
        "IF @@ROWCOUNT=0"
            "INSERT into player_contract_quote "
            "(playerid,price,volume,`change`) "
            " VALUES(:pid, :pr, :vol, :chg)");
            */

        insertQuery.bindValue(":pid",std::stoi(inplayerid));
        insertQuery.bindValue(":pr",ohlc.close());
        insertQuery.bindValue(":vol",ohlc.volume());
        insertQuery.bindValue(":chg",ohlc.change());

        bool good = insertQuery.exec();
        db.close();

        qDebug() << " sql query " << insertQuery.executedQuery();

        if ( ! good ) {
            qDebug() << " exec ret " << insertQuery.lastError().databaseText();
            qDebug() << ohlc.DebugString();
        }
    }

    void mdlevel1(const string &inplayerid, const MarketQuote &mq) {
        QSqlDatabase db = QSqlDatabase::database (conname); // Open Connection
        QSqlQuery insertQuery(db);


        insertQuery.prepare
            ("REPLACE into md_level1"
             "(playerid, bidsize, bid, ask, asksize, `last`, lastsize, updownind, lastupdate) "
             " VALUES(:pid, :bs, :b, :a, :as, :l, :ls, :ud, :timestamp)");
        /*
             "ON DUPLICATE KEY "
             "UPDATE player_contract_quote "
              "set price = :pr, volume = :vol, `change` = :chg where playerid = :pid");

        /*
        insertQuery.prepare
            ("UPDATE player_contract_quote "
             "set price = :pr, volume = :vol, `change` = :chg where playerid = :pid");
        /*
        insertQuery.prepare
        ("UPDATE player_contract_quote "
        "set price = :pr, volume = :vol, `change` = :chg where playerid = :pid"
        "IF @@ROWCOUNT=0"
            "INSERT into player_contract_quote "
            "(playerid,price,volume,`change`) "
            " VALUES(:pid, :pr, :vol, :chg)");
            */

        insertQuery.bindValue(":pid",inplayerid.data());
        insertQuery.bindValue(":bs",mq.bs());
        insertQuery.bindValue(":b",mq.b());
        insertQuery.bindValue(":a",mq.a());
        insertQuery.bindValue(":as",mq.as());
        insertQuery.bindValue(":l",mq.l());
        insertQuery.bindValue(":ls",mq.ls());
        insertQuery.bindValue(":ud",mq.udn());
        insertQuery.bindValue(":timestamp",currentNewYorkTime());

        bool good = insertQuery.exec();
        db.close();

        qDebug() << " sql query " << insertQuery.executedQuery();

        if ( ! good ) {
            qDebug() << " exec ret " << insertQuery.lastError().databaseText();
            qDebug() << mq.DebugString();
        }
    }


    void fill(const string playerid,
              TradeTic *tt,
              int week,
              uint64_t timestamp,
              int32_t seqnum) {

        QString ds = QDateTime::currentDateTimeUtc().toString("yyyy-mm-dd-hh.mm.ss.nnnnnn");
        qDebug() << " sql fill timestamp " << ds;

        QSqlDatabase db = QSqlDatabase::database (conname); // Open Connection
        QSqlQuery insertQuery(db);

        insertQuery.prepare
            ("INSERT INTO ticker "
             "(playerid,week,price,qty,tictime,seqnum)"
             "VALUES(:pid, :w, :tic,:qty,:tictime,:seq)");

        insertQuery.bindValue(":pid",QString::fromStdString(playerid));
        insertQuery.bindValue(":w", week);
        insertQuery.bindValue(":tic",tt->price());
        insertQuery.bindValue(":qty",tt->size());
        insertQuery.bindValue(":tictime",QDateTime::currentDateTimeUtc());
        insertQuery.bindValue(":seq",seqnum);

        bool good = insertQuery.exec();
        db.close();

        qDebug() << " sql query " << insertQuery.executedQuery();

        if ( ! good ) {
            qDebug() << " exec ret " << insertQuery.lastError().databaseText();
            //qDebug() << dist.DebugString();
        }

    }

    void openprice(const string &inplayerid, int price, int week) {

        QSqlDatabase db = QSqlDatabase::database (conname); // Open Connection
        QSqlQuery insertQuery(db);


        insertQuery.prepare(
                "INSERT into player_contract_quote "
                "(playerid,price) "
                " VALUES(:pid, :pr)");

        insertQuery.bindValue(":pid",std::stoi(inplayerid));
        insertQuery.bindValue(":pr",price);

        bool good = insertQuery.exec();
        db.close();

        qDebug() << " sql query " << insertQuery.executedQuery();

        if ( ! good ) {
            qDebug() << " exec ret " << insertQuery.lastError().databaseText();
            qDebug() << price;
            return;
        }

        TradeTic tt;
        tt.set_price(price);
        tt.set_size(0);

        fill(inplayerid,&tt,week,0,0);
        /*
            ("REPLACE into player_contract_quote "
             "(playerid,price,`change`) "
             " VALUES(:pid, :pr, :vol, :chg)");
        /*
             "ON DUPLICATE KEY "
             "UPDATE player_contract_quote "
              "set price = :pr, volume = :vol, `change` = :chg where playerid = :pid");

        /*
        insertQuery.prepare
            ("UPDATE player_contract_quote "
             "set price = :pr, volume = :vol, `change` = :chg where playerid = :pid");
        /*
        insertQuery.prepare
        ("UPDATE player_contract_quote "
        "set price = :pr, volume = :vol, `change` = :chg where playerid = :pid"
        "IF @@ROWCOUNT=0"
            "INSERT into pla    yer_contract_quote "
            "(playerid,price,volume,`change`) "
            " VALUES(:pid, :pr, :vol, :chg)");
            */


    }

    void fantasyname(FantasyNameHash &fnh) {
        QSqlDatabase db = QSqlDatabase::database (conname);
        QSqlQuery insertQuery(db);

         insertQuery.prepare
            ("INSERT INTO fantasyname "
             "(fantasynameid,fantasyname)"
             "VALUES(:fnid, :fn)");

        insertQuery.bindValue(":fnid",fnh.hash());
        insertQuery.bindValue(":fn",QString::fromStdString(fnh.name()));

        bool good = insertQuery.exec();
        db.close();

        if ( ! good ) {
            qDebug() << " exec ret " << insertQuery.lastError().databaseText();
        }


    }

    void playermapFeed(string feed, int playerid, string id) {
        QSqlDatabase db = QSqlDatabase::database (conname);
        QSqlQuery insertQuery(db);

        insertQuery.prepare("INSERT INTO pid (feed, id, playerid) VALUES(:feed, :mid,:mplayerid)");
        insertQuery.bindValue(":feed",feed.data());

        insertQuery.bindValue(":mplayerid",playerid);
        insertQuery.bindValue(":mid",QString::fromStdString(id));
        //insertQuery.exec();

        bool good = insertQuery.exec();
        db.close();

        if ( ! good ) {
            qDebug() << " exec ret " << insertQuery.lastError().databaseText();
        }

    }

    void player(PlayerData pd) {
        QSqlDatabase db = QSqlDatabase::database (conname);
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
            if ( pd.player_status().has_teamid())
                insertQuery.bindValue(":t",QString::fromStdString(pd.player_status().teamid()));
            else
                insertQuery.bindValue(":t",QVariant(QVariant::String));

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
            if ( pd.player_status().has_teamid())
                insertQuery.bindValue(":t",QString::fromStdString(pd.player_status().teamid()));
            else
                insertQuery.bindValue(":t",QVariant(QVariant::String));

            if ( pd.player_status().status() == PlayerStatus::ACTIVE )
                insertQuery.bindValue(":rs", QChar('A'));
            else
                insertQuery.bindValue(":rs", QChar('I'));
        }
        else return;

        bool good = insertQuery.exec();
        db.close();

        if ( ! good ) {
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
        {
            QSqlDatabase db = QSqlDatabase::database(conname, false);
            if (db.isOpen())
            db.close();
        }
        QSqlDatabase::removeDatabase(conname);
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
    SqlStuff sqls;
public:
    ScheduleLoader() : sqls(true,"ScheduleLoader") {}
    ~ScheduleLoader(){}
    std::map<string,string> gid{};

    map<string,int> teamIdKey{};
    void Dump() {
        for ( auto p : gid ) {
            sqls.gamemap(p.first,p.second);
        }
    }

    std::vector<fantasybit::ScheduleData> loadScheduleFromJsonFile() {
        std::vector<fantasybit::ScheduleData> result;

        //hard coded resources
        QFile jsonFile(":/Schedule2015/Schedule.2015-reg.json");
        if (!jsonFile.open(QIODevice::ReadOnly)){
            //errorMessage = "Can't open the player file !";
            return result;
        }
        QByteArray jsonData   = jsonFile.readAll();
        jsonFile.close();

        result = getScheduleDatafromJSON(jsonData,1);
        Dump();
        return result;
    }

    std::vector<fantasybit::ScheduleData> loadScheduleMovingFwdFromTR(int startweek) {
        std::vector<fantasybit::ScheduleData> result;
        QString uribase = "http://api.sportradar.us/nfl-b1/2015/REG/";
        QString uritail = "schedule.json?api_key=2uqzuwdrgkpzkhbfumzrg8gn";
        RestfullClient rest(QUrl("http://api.sportradar.us/nfl-b1/2015/REG"));

        QMap<QString,QString>  headers;
        QMap<QString,QVariant> params;

        qDebug() << uribase << uritail;
        rest.getData(uritail,params,headers);
        auto resp = rest.lastReply();

        result =  getScheduleDatafromJSON(resp,startweek);
        return result;
    }

    std::vector<fantasybit::ScheduleData> getScheduleDatafromJSON(QByteArray jsonData, int startweek) {
        std::vector<fantasybit::ScheduleData> result;

        for (int i=0;i< Commissioner::GENESIS_NFL_TEAMS.size();i++) {
            teamIdKey[Commissioner::GENESIS_NFL_TEAMS[i]] = i+1;
        }

        QJsonParseError * error = NULL;
        QJsonDocument doc = QJsonDocument::fromJson(jsonData,error);
        qDebug() << doc.isNull() << doc.isEmpty() << doc.isArray() << doc.isObject();

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
            if ( wk < startweek )
                continue;

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

        return result;
    }

    /*
    fantasybit::WeeklySchedule updatedGameInfoFromTR(int startweek) {
        QString uribase = "http://api.sportradar.us/nfl-t1/2015/REG/";
        QString uritail = "/schedule.json?api_key=2uqzuwdrgkpzkhbfumzrg8gn";
        QString route = uritail;//QString::number(week) + uritail;


        fantasybit::WeeklySchedule ws{};

        for (int i=0;i< Commissioner::GENESIS_NFL_TEAMS.size();i++) {
            teamIdKey[Commissioner::GENESIS_NFL_TEAMS[i]] = i+1;
        }

        RestfullClient rest(QUrl("http://api.sportradar.us/nfl-t1/2015/REG/"));

        QMap<QString,QString>  headers;
        QMap<QString,QVariant> params;

        qDebug() << uribase << route;
        rest.getData(route,params,headers);
        auto resp = rest.lastReply();

        qDebug() << resp;
        QJsonDocument ret = QJsonDocument::fromJson(resp);
        qDebug() << ret.isNull() << ret.isEmpty() << ret.isArray() << ret.isObject();

        //if (doc.isObject())
        QJsonObject jo = ret.object();
        int wk = jo.value("number").toInt();
        if ( wk != week ) {
            qCritical() << " wring week" << wk << week;
            return ws;
        }

        auto games = jo.value("games");
        QJsonArray gmarr = games.toArray();
        for (int i=0;i< gmarr.size();i++  ) {
            QJsonValueRef data = gmarr[i];
            //we are supposed to encounter a list of objects so skip values
            if (!data.isObject()) continue;
            QJsonObject gameData = data.toObject();
            QString errorParsingObject;
            fantasybit::GameInfo gi =
                    getScheduleFromJsonObject(gameData,wk,errorParsingObject);
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
        return ws;
    }
*/
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
    PlayerLoaderTR() : sqls(true,"PlayerLoaderTR") {}
    static map<int,vector<string> > byes;

    int start = 1000;
    std::map<int,string> pid{};

    std::vector<fantasybit::PlayerData> result;
    std::set<fantasybit::PlayerData> nochange;
    SqlStuff sqls;


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

    std::vector<fantasybit::PlayerData> loadPlayersFromTradeRadar(int week, bool makeSymbols,
                                                                  bool isgenesis = false) {

        if ( !isgenesis ) {
            start = sqls.maxPid();

            std::vector<fantasybit::GameRoster>
            gameroster =  DataService::instance()->GetCurrentWeekGameRosters();

            for ( auto games : gameroster ) {
                for ( auto p : games.homeroster) {
                    if ( p.second.base.position() == "DEF") continue;
                    bool isactive
                    = (p.second.team_status == PlayerStatus_Status_ACTIVE);
                    myknownplayerstatus[p.first] = make_pair(games.info.home(),isactive);
                }

                for ( auto p : games.awayroster ) {
                    if ( p.second.base.position() == "DEF") continue;
                    bool isactive
                    = (p.second.team_status == PlayerStatus_Status_ACTIVE);
                    myknownplayerstatus[p.first] = make_pair(games.info.away(),isactive);
                }
            }

            for ( auto t : byes[week]) {
                std::unordered_map<std::string,PlayerDetail>
                teamroster = DataService::instance()->GetTeamRoster(t);
                for ( auto p : teamroster) {
                    if ( p.second.base.position() == "DEF") continue;
                    bool isactive
                    = (p.second.team_status == PlayerStatus_Status_ACTIVE);
                    myknownplayerstatus[p.first] = make_pair(t,isactive);
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

        for ( auto dp : myknownplayerstatus ) {
            PlayerData pd{};
            pd.set_playerid(dp.first);
            PlayerStatus ps{};
            ps.set_status(PlayerStatus::INACTIVE);
            pd.mutable_player_status()->CopyFrom(ps);
            result.push_back(pd);
        }

        //dump();
        return result;
    }

    fantasybit::PlayerData getPlayerFromJsonObject(QJsonObject & jsonObject,std::string team,bool isgenesis,
                                                   QString & errorParsingObject) {

       auto tid = jsonObject.value("id").toString().toStdString();

       auto pos = jsonObject.value("position").toString();

       fantasybit::PlayerData pd{};

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

       bool isnew = false;
       int mypid;
       if ( isgenesis )
           isnew = true;
       else {
           mypid = sqls.getpidT(tid);
           if ( mypid <= 0)
               isnew = true;
       }



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
               pd.set_playerid(smypid);
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

        SqlStuff sqls(true,"GameStatsLoader");
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
            int homepoints = 0;
            int awaypoints = 0;
            QString homeid;
            QString awayid;
            Dstats dhome{};
            Dstats daway{};
            GameResult gr{};
            auto tgid = jo.value("id").toString().toStdString();
            auto mygid = sqls.getgidT(tgid);
            if ( mygid == "") {
                qCritical() << " bas sql return" << tgid << mygid;
                result.clear();
                return result;
            }
            qInfo() << " using gid" << mygid;
            gr.set_gameid(mygid);
            for ( auto tm : {QString("home_team"), QString("away_team")}) {

                auto playerresults = ( tm == "home_team" ) ?
                            gr.mutable_home_result() : gr.mutable_away_result();

                auto vals = jo.value(tm).toObject();
                auto teamid = vals.value("id").toString();

                auto points= vals.value("points");

                Dstats *dstat;
                if ( tm == "home_team" ) {
                   homepoints = points.toInt();
                   homeid = teamid;
                   dstat = &dhome;
                }
                else {
                   awaypoints = points.toInt();
                   awayid = teamid;
                   dstat = &daway;
                }


                auto sobj = vals.value("statistics").toObject();

                QJsonArray rush = sobj.value("rushing")
                        .toObject().value("players").toArray();

                QJsonArray pass = sobj.value("passing")
                        .toObject().value("players").toArray();

                QJsonArray rec = sobj.value("receiving")
                        .toObject().value("players").toArray();

                QJsonArray ep = sobj.value("extra_point")
                        .toObject().value("players").toArray();

                QJsonArray fg = sobj.value("field_goal")
                        .toObject().value("players").toArray();

                QJsonObject tdef = sobj.value("defense")
                        .toObject().value("team").toObject();

                QJsonObject td = sobj.value("touchdowns")
                        .toObject().value("team").toObject();

                QJsonArray two = sobj.value("two_point_conversion")
                        .toObject().value("players").toArray();



  /*
   *                 "team": {
                    "tackle": 48,
                    "ast": 14,
                    "comb": 62,
                    "tlost": 3,
                    "sack": 3.0,
                    "sack_yds": 21.0,
                    "sfty": 0,
                    "int": 1,
                    "int_yds": 0,
                    "int_lg": 0,
                    "int_td": 0,
                    "force_fum": 0,
                    "fum_rec": 0,
                    "fum_td": 0,
                    "qh": 5,
                    "pd": 1,
                    "bk": 0,
                    "sfty_1pt": 0,
                    "sp_tackle": 2,
                    "sp_ast": 1,
                    "sp_comb": 3,
                    "sp_force_fum": 0,
                    "sp_fum_rec": 0,
                    "misc_tackle": 0,
                    "misc_ast": 0,
                    "misc_comb": 0,
                    "misc_force_fum": 0,
                    "misc_fum_rec": 1

                    */
                std::map<QString,Stats> tstats{};
                //tstats[defteam] = Stats{};

                //Dstats * dstat = tstats[defteam].mutable_dstats();
                getStats(dstat,"defense",tdef);
                getStats(dstat,"td",td);

                //for ( auto tstats : {homestats, awaystats}) {
                map<string,QJsonArray> mp = { {"rush", rush}, {"pass", pass}, {"rec",rec} , {"two", two} };
                for ( auto rpsp : mp ) {
                    auto rps = rpsp.second;
                    for ( int i=0;i<rps.size();i++) {
                        QJsonValueRef data = rps[i];
                        QJsonObject odata = data.toObject();
                        auto id = odata.value("id").toString();
                        auto it = tstats.find(id);
                        if ( it == end(tstats)) {
                            tstats[id] = Stats{};
                        }

                        Ostats * ostat = tstats[id].mutable_ostats();
                        getStats(ostat,rpsp.first,odata);
                    }
                }

                map<string,QJsonArray> kp = { {"fg", fg}, {"ep", ep}};
                for ( auto rpsp : kp ) {
                    auto rps = rpsp.second;
                    for ( int i=0;i<rps.size();i++) {
                        QJsonValueRef data = rps[i];
                        QJsonObject odata = data.toObject();
                        auto id = odata.value("id").toString();
                        auto it = tstats.find(id);
                        if ( it == end(tstats))
                            tstats[id] = Stats{};

                        Kstats * kstat = tstats[id].mutable_kstats();
                        getStats(kstat,rpsp.first,odata);
                    }
                }
                for ( auto pstats : tstats) {
                    PlayerResult pr{};
                    int dbpid = sqls.getpidT(pstats.first.toStdString());
                    if ( dbpid <=0 ) {
                        qWarning() << "cant get player id form id ignoring result" << pstats.first;
                        continue;
                    }
                    pr.set_playerid(to_string(dbpid));
                    pr.mutable_stats()->CopyFrom(pstats.second);
                    pr.set_result(CalcResults(pstats.second));
                    playerresults->Add()->CopyFrom(pr);
                }
            }

            {
                PlayerResult pr{};
                int dbpid = sqls.getpidT(homeid.toStdString());
                if ( dbpid <=0 ) {
                    qWarning() << "cant get player id form id ignorming result" << homeid;
                    continue;
                }
                pr.set_playerid(to_string(dbpid));
                dhome.set_ptsa(awaypoints);
                pr.mutable_stats()->mutable_dstats()->CopyFrom(dhome);
                pr.set_result(CalcResults(pr.stats()));
                gr.add_home_result()->CopyFrom(pr);
            }

            {
                PlayerResult pr{};
                int dbpid = sqls.getpidT(awayid.toStdString());
                if ( dbpid <=0 ) {
                    qWarning() << "cant get player id form id ignorming result" << awayid;
                    continue;
                }
                pr.set_playerid(to_string(dbpid));
                daway.set_ptsa(homepoints);
                pr.mutable_stats()->mutable_dstats()->CopyFrom(daway);
                pr.set_result(CalcResults(pr.stats()));
                gr.add_away_result()->CopyFrom(pr);
                }


            result.push_back(gr);
        }

        return result;
    }

    double CalcResults(const Stats &stats) {
        return BlockProcessor::CalcResults(stats);
    }

    void getStats(Ostats *ostat,std::string type,QJsonObject & jsonObject) const {

       if ( type == "two") {
           int made = 0;
           auto p = jsonObject.value("pass");
           made += p.toInt();
           auto r = jsonObject.value("rush");
           made += r.toInt();
           auto re = jsonObject.value("rec");
           made += re.toInt();
           ostat->set_twopt(made);
           return;
       }

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
    void getStats(Kstats *kstat,std::string type,QJsonObject & jsonObject) const {

       if ( type == "fg") {
           auto keys  = jsonObject.keys();


           for ( auto k : keys ) {
               string sk = k.toStdString();
               int pos =  sk.find_first_of('_');
               if ( pos == string::npos) continue;


               auto made = sk.substr(0, pos);
               if ( made != "made") continue;

               auto syards = sk.substr(pos + 1);
               int yards = std::stoi(syards);

               auto num = jsonObject.value(k);
               int n = num.toInt();

               for (int i=0;i<n;i++)
                    kstat->add_fg(yards);

           }
       }
       else if ( type == "ep") {
           auto made = jsonObject.value("made");
           int m = made.toInt();
           kstat->set_pa(m);

       }
    }
    void getStats(Dstats *dstat,std::string type,QJsonObject & jsonObject) const {

       if ( type == "defense" ) {
           auto sack = jsonObject.value("sack");
           int s = sack.toInt();
           dstat->set_sacks(s);

           auto sfty = jsonObject.value("sfty");
           int sf = sfty.toInt();
           dstat->set_sfty(sf);

           auto pint = jsonObject.value("int");
           int pi = pint.toInt();
           dstat->set_turnovers(pi);

           auto fr = jsonObject.value("fum_rec");
           int fri = fr.toInt();
           dstat->set_turnovers(dstat->turnovers()+fri);
       }
       else if ( type == "td") {
           int tds = 0;
           for ( auto tdt : {"int", "fum_ret", "punt_ret","kick_ret","fg_ret"}) {
                auto its = jsonObject.value(tdt);
                int it = its.toInt();
                tds = tds + it;
                //qDebug() << "xxxx" << tdt << it;
           }

           dstat->set_deftd(tds);
       }

    }

};

class FFNerdLoader {

    QString nerdbase = "http://www.fantasyfootballnerd.com/service/weekly-projections/json/tfzx83sdg348/";
    
    QString makeroute(std::string pos) {
        return QString::fromStdString(pos);
    }
    
public:
    FFNerdLoader() : sqls(true,"FFNerdLoader") {}

    static map<string,int> import;

    SqlStuff sqls;
    std::vector<fantasybit::PlayerPoints> loadProj() {

        for ( auto x : import ) {
            sqls.playermapFeed("FFNERD",x.second,x.first);
        }

        std::vector<fantasybit::PlayerPoints> result;

        for ( auto pos : {"QB", "WR", "RB", "TE", "K", "DEF"}) {
            auto url = QUrl(nerdbase);
            RestfullClient rest(url);
            rest.getData(makeroute(pos));
            auto resp = rest.lastReply();
            qDebug() << resp;
            QJsonDocument ret = QJsonDocument::fromJson(resp);
            qDebug() << ret.isNull() << ret.isEmpty() << ret.isArray() << ret.isObject();
            QJsonObject jo = ret.object();
            auto vals = jo.value("Projections");
            QJsonArray parr = vals.toArray();
            for (int i=0;i< parr.size();i++  ) {
                QJsonValueRef data = parr[i];

                QJsonObject projData = data.toObject();
                QString errorParsingObject;

                PlayerPoints pp = getProjFromJson(projData,errorParsingObject);
                if ( errorParsingObject != "" ) continue;
                result.push_back(pp);

            }


        }

        return result;
    }
    fantasybit::PlayerPoints getProjFromJson(QJsonObject & jsonObject,QString & errorParsingObject) {

       PlayerPoints pp{};

       ProjStats s{};
       getStats(&s,jsonObject);
       double proj = CalcProj(s);
       pp.set_points(ceil(proj));

       auto playerid = jsonObject.value("playerId").toString();
       int pid = sqls.getpid("FFNERD",playerid.toStdString());
       if ( pid == 0) {
           if (pp.points() > 0.0001 )
                qDebug() << " FFNERD id not found" << playerid
                         << jsonObject.value("displayName").toString()
                         << jsonObject.value("team").toString()
                         << jsonObject.value("position").toString();

           errorParsingObject = playerid;
           return pp;
       }

       pp.set_playerid(to_string(pid));
       return pp;
    }

    void getStats(ProjStats *stat,QJsonObject & jsonObject) const {

       ProjOstats os{};

       QString doublestr;
       float i;
       doublestr = jsonObject.value("passYds").toString();
       i = doublestr.toFloat();
       os.set_passyds(i);
       doublestr = jsonObject.value("rushYds").toString();
       i = doublestr.toFloat();
       os.set_rushyds(i);
       doublestr = jsonObject.value("recYds").toString();
       i = doublestr.toFloat();
       os.set_recyds(i);
       doublestr = jsonObject.value("passTD").toString();
       i = doublestr.toFloat();
       os.set_passtd(i);
       doublestr = jsonObject.value("rushTD").toString();
       i = doublestr.toFloat();
       os.set_rushtd(i);
       doublestr = jsonObject.value("recTD").toString();
       i = doublestr.toFloat();
       os.set_rectd(i);
       doublestr = jsonObject.value("receptions").toString();
       i = doublestr.toFloat();
       os.set_rec(i);
       doublestr = jsonObject.value("passInt").toString();
       i = doublestr.toFloat();
       os.set_pint(i);
       doublestr = jsonObject.value("fumblesLost").toString();
       i = doublestr.toFloat();
       os.set_fumble(i);
       stat->mutable_ostats()->CopyFrom(os);

       ProjDstats ds{};
       doublestr = jsonObject.value("defSack").toString();
       i = doublestr.toFloat();
       ds.set_sacks(i);
       doublestr = jsonObject.value("defSafety").toString();
       i = doublestr.toFloat();
       ds.set_sfty(i);
       doublestr = jsonObject.value("defInt").toString();
       i = doublestr.toFloat();
       doublestr = jsonObject.value("defFF").toString();
       i += doublestr.toFloat();
       ds.set_turnovers(i);
       doublestr = jsonObject.value("defTD").toString();
       i = doublestr.toFloat();
       doublestr = jsonObject.value("defRetTD").toString();
       i += doublestr.toFloat();
       ds.set_deftd(i);
       doublestr = jsonObject.value("defPA").toString();
       i = doublestr.toFloat();
       ds.set_ptsa(i);
       stat->mutable_dstats()->CopyFrom(ds);

       ProjKstats ks{};
       doublestr = jsonObject.value("xp").toString();
       i = doublestr.toFloat();
       ks.set_pa(i);
       doublestr = jsonObject.value("fg").toString();
       i = doublestr.toFloat();
       ks.set_fg40(i);
       stat->mutable_kstats()->CopyFrom(ks);
    }

    static double CalcProj(const ProjStats &stats) {
        double ret = 0;

        if ( stats.has_ostats() ) {
            auto os = stats.ostats();
            if ( os.has_passtd())
                ret += 4.0 * os.passtd(); //PAssing Yards (QB)


            if ( os.has_rushtd() )
                ret += 6.0 * os.rushtd();

            if ( os.has_rectd() )
                ret += 6.0 *  os.rectd();

            if ( os.has_passyds() )
               ret += .05 *  os.passyds();

            if ( os.has_recyds() )
                ret += .1 *  os.recyds();

            if (  os.has_rushyds() )
                ret += .1 *  os.rushyds();

            if ( os.has_rec() )
                ret += 1.0 * os.rec();

            if ( os.has_pint() )
                ret += -1.0 * os.pint();

            if ( os.has_twopt() )
                ret += 2.0 * os.twopt();

            if (os.has_onept())
                ret += 2.0 *  os.onept();
        }
        if ( stats.has_kstats() ) {
            auto ks = stats.kstats();
            if ( ks.has_pa() )
                ret += 1.0 * ks.pa();
            if ( ks.has_fg30() )
                ret += 3.0 * ks.fg30();
            if ( ks.has_fg40() )
                ret += 4.0 * ks.fg40();
            if ( ks.has_fg50() )
                ret += 5.0 * ks.fg50();
            if ( ks.has_fg60() )
                ret += 6.0 * ks.fg60();
        }

        if ( stats.has_dstats() ) {
            auto ds = stats.dstats();
            if ( ds.has_deftd())
                ret += 6.0 * ds.deftd();
            if ( ds.has_onept())
                ret += 2.0 * ds.onept();
            if ( ds.has_ptsa() && ds.ptsa() > 0.0) {
                if ( ds.ptsa() < 1.0)
                    ret += 12.0;
                else if ( ds.ptsa() < 7.0)
                    ret += 10.0;
                else if ( ds.ptsa() < 11.0)
                    ret += 8.0;
                else if ( ds.ptsa() < 14.0)
                    ret += 4.0;

            }
            if ( ds.has_sacks())
                ret += 1.0 * ds.sacks();
            if ( ds.has_sfty())
                ret += 5.0 * ds.sfty();
            if ( ds.has_turnovers())
                ret += 2.0 * ds.turnovers();
            if ( ds.has_twopt())
                ret += 2.0 * ds.twopt();
        }

        return ret;
    }


};

class MikeClayLoader {

public:
    static map<string,int> headers;
    static map<string,int> import;
    bool mDirect;
    MikeClayLoader(bool direct =false) : sqls(true,"MikeClayLoader") , mDirect(direct) {}

    int start = 3;
    SqlStuff sqls;

    std::unordered_map<string, int> mIds;
    std::vector<fantasybit::PlayerPoints> loadProjFromLink(int week) {
        mIds = sqls.getallpid("STATS");
        QString pfflink("https://www.profootballfocus.com/");
        QString route("toolkit/export/Satoshi/?password=a90kvc294gh9jmc9je98du932fj39&week=%1");

        QMap<QString,QString>  headers;
        QMap<QString,QVariant> params;

        QUrl url;
        url.setUrl(pfflink);
        RestfullClient rest (url);
        rest.getData(route.arg(week));
        auto resp = rest.lastReply();

        std::vector<fantasybit::PlayerPoints> result;

        QTextStream in(resp);
        QStringList listdata;
        QString line;

        for ( int i=0;i<start;i++)
            line = in.readLine();
        while (!in.atEnd()) {
            listdata.clear();
            line = in.readLine();
            //qDebug() << line;
            listdata.append(line.split(','));
            QString errorParsingObject;

            PlayerPoints pp = getProjFromList(listdata,errorParsingObject);
            if ( errorParsingObject != "" ) continue;
            result.push_back(pp);

        }
        qDebug() << "out loop";

        return result;
    }

    std::vector<fantasybit::PlayerPoints> loadProjFromFile(string infile = "SatoshiFantasy.csv") {

        for ( auto x : import ) {
            sqls.playermapFeed("STATS",x.second,x.first);
        }

        std::vector<fantasybit::PlayerPoints> result;
        string filename = GET_ROOT_DIR() + infile;

        //qDebug() << " ccccc " << filename;
        QFile file(filename.data());
        QTextStream in(&file);
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QStringList listdata;
            QString line;

            for ( int i=0;i<start;i++)
                line = in.readLine();
            while (!in.atEnd()) {
                listdata.clear();
                line = in.readLine();
                //qDebug() << line;
                listdata.append(line.split(','));
                QString errorParsingObject;

                PlayerPoints pp = getProjFromList(listdata,errorParsingObject);
                if ( errorParsingObject != "" ) continue;
                result.push_back(pp);

            }
            qDebug() << "out loop" << file.atEnd();

        }

        return result;
    }

    int getpid(string &in) {
        auto it = mIds.find(in);
        if ( it != end(mIds))
            return it->second;
        else
            return 0;
    }

    fantasybit::PlayerPoints getProjFromList(QStringList & list,QString & errorParsingObject) {

       PlayerPoints pp{};

       auto statsid = list[headers["statsid"]];
       auto pos = list[headers["pos"]];
       //qDebug() << "ccccc" << statsid;
       if (!(
               pos == "QB" ||
               pos == "WR" ||
                              pos == "TE" ||
               pos == "HB" ||
                              pos == "FB" ||
                              pos == "K" ||
                              pos == "DST"
               )) {
           errorParsingObject = pos;
           return pp;
       }
       int pid = getpid(statsid.toStdString());
       if ( pid == 0) {
           qDebug() << " stats id not found" << statsid << list[headers["name"]]
                   << list[headers["team"]] << pos;
           errorParsingObject = statsid;
           return pp;
       }

       ProjStats s{};
       getStats(&s,list);
       double proj = CalcProj(s);
       pp.set_points(ceil(proj));
       pp.set_playerid(to_string(pid));

       return pp;
    }

    void getStats(ProjStats *stat,QStringList & list) const {

       ProjOstats os{};
       float i = 0.0;
       i = list[headers["pyds"]].toFloat();
       os.set_passyds(i);
       i = list[headers["ruyds"]].toFloat();
       os.set_rushyds(i);
       i = list[headers["reyds"]].toFloat();
       os.set_recyds(i);
       i = list[headers["ptd"]].toFloat();
       os.set_passtd(i);
       i = list[headers["rutd"]].toFloat();
       os.set_rushtd(i);
       i = list[headers["retd"]].toFloat();
       os.set_rectd(i);
       i = list[headers["rec"]].toFloat();
       os.set_rec(i);
       i = list[headers["pint"]].toFloat();
       os.set_pint(i);
       i = list[headers["fumble"]].toFloat();
       os.set_fumble(i);
       stat->mutable_ostats()->CopyFrom(os);

       ProjDstats ds{};
       i = list[headers["sack"]].toFloat();
       ds.set_sacks(i);
       i = list[headers["sfty"]].toFloat();
       ds.set_sfty(i);
       i = list[headers["int"]].toFloat();
       i += list[headers["fumble"]].toFloat();
       ds.set_turnovers(i);
       i = list[headers["dtd"]].toFloat();
       ds.set_deftd(i);
       i = list[headers["pa0"]].toFloat();
       if ( i == 1)
           ds.set_ptsa(0.1);
       else {
           i = list[headers["pa6"]].toFloat();
           if ( i == 1)
               ds.set_ptsa(6);
           else {
               i = list[headers["pa13"]].toFloat();
               if ( i == 1)
                   ds.set_ptsa(13);

           }

       }

       stat->mutable_dstats()->CopyFrom(ds);

       ProjKstats ks{};
       i = list[headers["xp"]].toFloat();
       ks.set_pa(i);
       i = list[headers["fg19"]].toFloat();
       ks.set_fg30(i);
       i = list[headers["fg29"]].toFloat();
       ks.set_fg30(i + ks.fg30());
       i = list[headers["fg39"]].toFloat();
       ks.set_fg40(i);
       i = list[headers["fg49"]].toFloat();
       ks.set_fg50(i);
       i = list[headers["fg50p"]].toFloat();
       ks.set_fg60(i);

       stat->mutable_kstats()->CopyFrom(ks);

    }

    static double CalcProj(const ProjStats &stats) {
        double ret = 0;

        if ( stats.has_ostats() ) {
            auto os = stats.ostats();
            if ( os.has_passtd())
                ret += 4.0 * os.passtd(); //PAssing Yards (QB)


            if ( os.has_rushtd() )
                ret += 6.0 * os.rushtd();

            if ( os.has_rectd() )
                ret += 6.0 *  os.rectd();

            if ( os.has_passyds() )
               ret += .05 *  os.passyds();

            if ( os.has_recyds() )
                ret += .1 *  os.recyds();

            if (  os.has_rushyds() )
                ret += .1 *  os.rushyds();

            if ( os.has_rec() )
                ret += 1.0 * os.rec();

            if ( os.has_pint() )
                ret += -1.0 * os.pint();

            if ( os.has_twopt() )
                ret += 2.0 * os.twopt();

            if (os.has_onept())
                ret += 2.0 *  os.onept();
        }
        if ( stats.has_kstats() ) {
            auto ks = stats.kstats();
            if ( ks.has_pa() )
                ret += 1.0 * ks.pa();
            if ( ks.has_fg30() )
                ret += 3.0 * ks.fg30();
            if ( ks.has_fg40() )
                ret += 4.0 * ks.fg40();
            if ( ks.has_fg50() )
                ret += 5.0 * ks.fg50();
            if ( ks.has_fg60() )
                ret += 6.0 * ks.fg60();
        }

        if ( stats.has_dstats() ) {
            auto ds = stats.dstats();
            if ( ds.has_deftd())
                ret += 6.0 * ds.deftd();
            if ( ds.has_onept())
                ret += 2.0 * ds.onept();
            if ( ds.has_ptsa() && ds.ptsa() > 0.0) {
                if ( ds.ptsa() < 1.0)
                    ret += 12.0;
                else if ( ds.ptsa() < 7.0)
                    ret += 10.0;
                else if ( ds.ptsa() < 11.0)
                    ret += 8.0;
                else if ( ds.ptsa() < 14.0)
                    ret += 4.0;

            }
            if ( ds.has_sacks())
                ret += 1.0 * ds.sacks();
            if ( ds.has_sfty())
                ret += 5.0 * ds.sfty();
            if ( ds.has_turnovers())
                ret += 2.0 * ds.turnovers();
            if ( ds.has_twopt())
                ret += 2.0 * ds.twopt();
        }

        return ret;
    }


};

struct pmatch {
    pmatch() = default;
    std::string pos;
    //std::string jersey;
    std::string last;
    bool operator ==(const pmatch &b) const {
        return b.pos + b.last == pos + last;
    }
};

struct ids {
    std::map<std::string,std::string> lid{};
};

namespace std {
  template <> struct hash<pmatch>
  {
    size_t operator()(const pmatch & x) const
    {
        return std::hash<std::string>()(x.pos + x.last);
      /* your code here, e.g. "return hash<int>()(x.value);" */
    }
  };
}
class FantasyDataNerdMapAll {
    QString nerdbase = "http://www.fantasyfootballnerd.com/service/players/json/tfzx83sdg348/";
    QString truribase = "http://api.sportradar.us/nfl-b1/teams";
    QString truritail = "/roster.json?api_key=2uqzuwdrgkpzkhbfumzrg8gn";

    QString fduribase = "https://api.fantasydata.net/nfl/v2/JSON/Players/";
    QMap<QString,QString>  fdheaders;

    QString makeroute(std::string team) {
        return QString::fromStdString(team) + truritail;
    }

public:
    std::map<std::string, std::unordered_map<pmatch,ids> > nerddata{};
    std::map<std::string,std::string> fd2fa{};

    void loadPlayers() {
        fdheaders["Ocp-Apim-Subscription-Key"] = "e9941289786443bd983c79a6c9f0b6cf";

        SqlStuff sqls(true,"FantasyDataNerdMapAll");

        loadNerds();

        for ( auto t : fantasybit::Commissioner::GENESIS_NFL_TEAMS) {

            auto fdmap = loadFantasyData(t);
            for ( auto p : fdmap ) {
            //    qDebug() << p.first.pos << p.first.jersey << p.first.last << p.second;
            }
            auto nerdmap = nerddata[t];
            auto trurl = QUrl(truribase);

            RestfullClient trrest(trurl);
            auto route = makeroute(t);
            qDebug() << truribase << route;
            trrest.getData(route);
            auto resp = trrest.lastReply();

            qDebug() << resp;
            QJsonDocument ret = QJsonDocument::fromJson(resp);
            qDebug() << ret.isNull() << ret.isEmpty() << ret.isArray() << ret.isObject();

            QJsonObject jo = ret.object();
            auto vals = jo.value("players");
            QJsonArray parr = vals.toArray();

            for (int i=0;i< parr.size();i++  ) {
                QJsonValueRef data = parr[i];

                QJsonObject playerData = data.toObject();
                auto tid = playerData.value("id").toString().toStdString();

                auto pos = playerData.value("position").toString();

                if   (! (pos == "QB"
                     || pos == "RB"
                     || pos == "WR"
                     || pos == "TE"
                     || pos == "K"
                     || pos == "FB")) { continue; }

                if ( pos == "FB") pos = "RB";

                auto jersey = playerData.value("jersey_number");
                auto name_last = playerData.value("name_last").toString();

                auto name_first = playerData.value("name_first").toString().toStdString();

                int jersey_number = jersey.toInt();

                pmatch p{pos.toStdString(),name_last.toStdString()};

                auto myjersey = to_string(jersey_number);

                auto myids = nerdmap[p];
                std::string holdid = "";
                std::string npid = "";
                for ( auto ids : myids.lid) {
                    if ( ids.first == myjersey) {
                        npid = ids.second;
                        break;
                    }
                    else if ( ids.first == name_first) {
                        holdid = ids.second;
                    }
                }
                if ( npid == "" ) {
                    if ( holdid != "" && myids.lid.size() %2 == 0)
                        npid = holdid;
                    else if ( myids.lid.size() == 2)
                       npid = myids.lid.begin()->second;
                }
                if ( npid == "")
                    qDebug() << "error" << p.pos << p.last;


                myids = fdmap[p];
                holdid = "";
                std::string fpid = "";
                for ( auto ids : myids.lid) {
                    if ( ids.first == myjersey) {
                        fpid = ids.second;
                        break;
                    }
                    else if ( ids.first == name_first) {
                        holdid = ids.second;
                    }
                }
                if ( fpid == "" ) {
                    if ( holdid != "" && myids.lid.size() %2 == 0)
                        fpid = holdid;
                    else if ( myids.lid.size() == 2)
                       fpid = myids.lid.begin()->second;
                }
                if ( fpid == "")
                    qDebug() << "error" << p.pos << p.last;

                auto mypid = sqls.getpidT(tid);
                if ( mypid <= 0)
                    continue;

                if ( npid != "")
                    sqls.playermapFeed("FFNERD",mypid,npid);

                if ( fpid != "") {
                    sqls.playermapFeed("FNDATA",mypid,fpid);
                    auto faid = fd2fa[fpid];
                    if ( faid != "")
                        sqls.playermapFeed("FALARM",mypid,faid);

                }

            }


        }

    }

    std::unordered_map<pmatch,ids> loadFantasyData(std::string team) {
        std::unordered_map<pmatch,ids> fddata{};
        auto myteam = team;
        if ( myteam == "JAC")
            myteam = "JAX";

        auto url = QUrl(fduribase);

        RestfullClient trrest(url);
        auto route = QString::fromStdString(myteam);
        qDebug() << truribase << route;
        trrest.getData(route,QMap<QString,QVariant>(),fdheaders);
        auto resp = trrest.lastReply();

        qDebug() << resp;
        QJsonDocument ret = QJsonDocument::fromJson(resp);
        qDebug() << ret.isNull() << ret.isEmpty() << ret.isArray() << ret.isObject() << ret.isArray();
        QJsonArray parr = ret.array();
        for (int i=0;i< parr.size();i++  ) {
            QJsonValueRef data = parr[i];

            QJsonObject playerData = data.toObject();
            auto PlayerID = playerData.value("PlayerID");
            int pid = PlayerID.toInt();

            auto pos = playerData.value("FantasyPosition").toString();

            if   (! (pos == "QB"
                 || pos == "RB"
                 || pos == "WR"
                 || pos == "TE"
                 || pos == "K"
                 )) { continue; }

            auto jersey = playerData.value("Number");
            auto name_last = playerData.value("LastName").toString();
            auto name_first = playerData.value("FirstName").toString().toStdString();

            auto FantasyAlarmPlayerID = playerData.value("FantasyAlarmPlayerID");
            auto faplayerid = FantasyAlarmPlayerID.toInt();

            int jersey_number = jersey.toInt();
            if ( jersey_number <= 0 ) continue;

            pmatch p{pos.toStdString(), name_last.toStdString()};

            auto myid = fddata[p];
            myid.lid[to_string(jersey_number)] = to_string(pid);
            myid.lid[name_first] = to_string(pid);

            fddata[p] = myid;

            if ( faplayerid > 0)
                fd2fa[to_string(pid)] = to_string(faplayerid);
        }

        return fddata;

    }

    void loadNerds() { //nerds

        for ( auto t : fantasybit::Commissioner::GENESIS_NFL_TEAMS) {
            nerddata.insert(make_pair(t,std::unordered_map<pmatch,ids>()));
        }

        QString route = "";
        //do nerd
        auto url = QUrl(nerdbase);
        RestfullClient rest(url);
        rest.getData(route);
        auto resp = rest.lastReply();
        qDebug() << resp;
        QJsonDocument ret = QJsonDocument::fromJson(resp);
        qDebug() << ret.isNull() << ret.isEmpty() << ret.isArray() << ret.isObject();
        QJsonObject jo = ret.object();
        auto vals = jo.value("Players");
        QJsonArray parr = vals.toArray();
        for (int i=0;i< parr.size();i++  ) {
            QJsonValueRef data = parr[i];

            QJsonObject playerData = data.toObject();
            auto jersey = playerData.value("jersey").toString();
            if ( jersey == "0")
                continue;

            auto position = playerData.value("position").toString();
            auto lname = playerData.value("lname").toString();
            auto fname = playerData.value("fname").toString().toStdString();



            auto playerid = playerData.value("playerId").toString();
            auto team = playerData.value("team").toString();
            pmatch p{position.toStdString(), lname.toStdString()};
            auto myid = nerddata[team.toStdString()][p];
            myid.lid[jersey.toStdString()] = playerid.toStdString();
            myid.lid[fname] = playerid.toStdString();

            nerddata[team.toStdString()][p] = myid;
        }
    }


};



#endif // PLAYERLOADER


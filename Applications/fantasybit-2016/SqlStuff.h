#ifndef SQLSTUFF_H
#define SQLSTUFF_H

#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QFile>
#include "ProtoData.pb.h"
#include <QDateTime>
#include "RestFullCall.h"
#include <QtSql/QSql>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlDriver>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include "ApiData.pb.h"
#include "Commissioner.h"
#include "platform.h"
#include "appsettings.h"


namespace fantasybit {

struct SqlStuff {
   //  db{};

    SqlStuff(std::string dbname) {
        init(dbname);
    }

    SqlStuff(const std::string &dbname, const std::string &connectionName) {
        init(dbname, connectionName);
    }

    SqlStuff(const char* dbname, const char* connectionName) {
        init(dbname, connectionName);
    }

    SqlStuff() {
        init(DBNAME);
    }

    SqlStuff(bool useDefault, const std::string &connectionName) {
        init(DBNAME,connectionName);
    }


    void init(std::string dbname) {
        init(dbname,dbname + "_defaultConnection");
    }

    QString conname;
    void init(std::string dbname, std::string connectionName) {
        qDebug() << "sql init " << dbname.data ()<< connectionName.data ();
        conname = QString::fromStdString(connectionName);
        QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL",conname);

        db.setHostName(DBIP.data());
        db.setPort(3306);
        db.setDatabaseName(dbname.data());

        QString value = Platform::instance()->settings()->getSetting("dbUserName").toString();
        if (  value == "" )
            value = "root";
        qDebug() << " user " << value;
        db.setUserName(value);
        value = Platform::instance()->settings()->getSetting("dbPassword").toString();
        if (  value == "" )
            value = "fantasyf00tball!";
        qDebug() << " password " << value;
        db.setPassword(value);

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

        qDebug() << st.DebugString().data ();

    }

    void teams() {

        std::map<int,std::string> teamIdKey{};
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

    int getpidT (std::string tpid) {
        return getpid("TRDR",tpid);
    }

    int getpid (std::string feed, std::string tpid) {
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

    std::unordered_map<std::string,int> getallpid (std::string feed) {
        std::unordered_map<std::string,int> ret;

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


        qDebug() << "getallpid" << query.isActive() << query.isValid() <<
                    query.first();

        int idf = 0;//query.rec.indexOf("id");
        int playeridf = 1;//query.rec.indexOf("playerid");

        do {
            QString id = query.value(idf).toString();
            int pid = query.value(playeridf).toInt();
            ret[id.toStdString()] = pid;
            qDebug() << feed.data() << id << pid;
        } while (query.next());


        return ret;
    }

    std::string getgidT (std::string tgid) {
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

    std::string getidfromgameidT (std::string gameid, std::string feed) {
        QSqlDatabase db = QSqlDatabase::database (conname);
        QSqlQuery query(db);

        query.prepare("SELECT id FROM gid g WHERE g.gameid = :gameid and g.feed = :feed");
        query.bindValue(":gameid",QString::fromStdString(gameid));
        query.bindValue(":feed",QString::fromStdString(feed));

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

    std::pair<bool,std::string> getPlayer(int pid) {
        QSqlDatabase db = QSqlDatabase::database (conname);
        QSqlQuery query(db);

        query.prepare("select team, roster_status from player where playerid = :pid");
        query.bindValue(":pid",pid);

        bool good = query.exec();
        db.close();

        if ( ! good ) {
            qDebug() << " exec ret " << query.lastError().databaseText();
            return std::make_pair(false,"");
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

    std::unordered_map<int,std::pair<bool,std::string>> getAllPlayer() {
        std::unordered_map<int,std::pair<bool,std::string>> ret;
        QSqlDatabase db = QSqlDatabase::database (conname);
        QSqlQuery query(db);

        query.prepare("select team, playerid, roster_status from player");
//        query.bindValue(":pid",1000);
//        query.bindValue(":pid",pid);

        bool good = query.exec();
        db.close();

        if ( ! good ) {
            qDebug() << " exec ret " << query.lastError().databaseText();
            return ret;
        }

        qDebug() << query.isActive() << query.isValid() <<
                    query.first();;

        int teamf = 0;//query.rec.indexOf("id");
        int playeridf = 1;//query.rec.indexOf("playerid");
        int rosterf = 2;



        for ( ;query.next();) {
            QString team = query.value(teamf).toString();
            int pid = query.value(playeridf).toInt();
            QString roster = query.value("roster_status").toString();
            ret[pid] = {roster == "A", team.toStdString()};
        }

//        auto myteam = query.value(0).toString();
//        //auto status = query.value(1).toChar().toString();
//        auto sv = query.value("roster_status");
//        auto svs = sv.toString();

//        bool active = svs == "A";
//        return make_pair(active,myteam.toStdString());

        return ret;

    }

    void gamemap (std::string gameid, std::string id,std::string feed = "TRDR") {
        QSqlDatabase db = QSqlDatabase::database (conname);
        QSqlQuery insertQuery(db);
        insertQuery.prepare("INSERT INTO gid (feed, id, gameid) VALUES(:feed, :mid,:mgameid)");
        insertQuery.bindValue(":feed",QString::fromStdString(feed));

        insertQuery.bindValue(":mgameid",QString::fromStdString(gameid));
        insertQuery.bindValue(":mid",QString::fromStdString(id));
        //insertQuery.exec();
        bool good = insertQuery.exec();
        db.close();

        if ( !good ) {
            qDebug() << " exec ret " << insertQuery.lastError().databaseText();
        }

    }

    void playermap(int playerid, std::string id) {
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
            qDebug() << dist.DebugString().data ();
        }
    }

    void distribute(std::vector<Distribution> &dists) {

        QSqlDatabase db = QSqlDatabase::database (conname); // Open Connection
        QSqlQuery insertQuery(db);

        for ( auto &dist : dists) {
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
                qDebug() << dist.DebugString().data ();
            }
        }
    }

    void profit(std::vector<Profits> &profs) {
        QSqlDatabase db = QSqlDatabase::database (conname); // Open Connection
        QSqlQuery insertQuery(db);

        for ( auto &dist : profs) {
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
                qDebug() << dist.DebugString().data ();
            }

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
            qDebug() << dist.DebugString().data ();
        }
    }

    void fills(Fills &dist) {

        QSqlDatabase db = QSqlDatabase::database (conname); // Open Connection
        QSqlQuery insertQuery(db);

        insertQuery.prepare("INSERT INTO fills "
                "(fantasynamebuyer,fantasynameseller,playerid,season,week,qty,price,isbuyaggressive)"
                "VALUES(:buyer, :seller, :pid,:s, :w,:qty,:price, :isba)");

        insertQuery.bindValue(":buyer",QString::fromStdString(dist.buyer()));
        insertQuery.bindValue(":seller",QString::fromStdString(dist.seller()));
//        insertQuery.bindValue(":gid",QString::fromStdString(dist.gameid()));
//        insertQuery.bindValue(":tid",QString::fromStdString(dist.teamid()));
        insertQuery.bindValue(":pid",QString::fromStdString(dist.playerid()));
        insertQuery.bindValue(":s",dist.season());
        insertQuery.bindValue(":w",dist.week());
        insertQuery.bindValue(":qty",dist.qty());
        insertQuery.bindValue(":price",dist.price());
        insertQuery.bindValue(":isba",dist.passivebuy() ? 0 : 1);


        bool good = insertQuery.exec();
        //db.close();

        if ( ! good ) {
            qDebug() << " exec ret " << insertQuery.lastError().databaseText();
            qDebug() << dist.DebugString().data ();
        }
    }

    void quote(const std::string &inplayerid, const ContractOHLC &ohlc) {

        QSqlDatabase db = QSqlDatabase::database (conname); // Open Connection
        QSqlQuery insertQuery(db);


        insertQuery.prepare
            ("REPLACE into player_contract_quote "
             "(symbol ,price,volume,`change`) "
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

        insertQuery.bindValue(":pid",inplayerid.data());
        insertQuery.bindValue(":pr",ohlc.close());
        insertQuery.bindValue(":vol",ohlc.volume());
        insertQuery.bindValue(":chg",ohlc.change());

        bool good = insertQuery.exec();
        db.close();

        qDebug() << " sql query " << insertQuery.executedQuery();

        if ( ! good ) {
            qDebug() << " exec ret " << insertQuery.lastError().databaseText();
            qDebug() << ohlc.DebugString().data ();
        }
    }

    void mdlevel1(const std::string &inplayerid, const MarketQuote &mq) {
        QSqlDatabase db = QSqlDatabase::database (conname); // Open Connection
        QSqlQuery insertQuery(db);


        insertQuery.prepare
            ("REPLACE into md_level1"
             "(symbol, bidsize, bid, ask, asksize, `last`, lastsize, updownind, lastupdate) "
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
            qDebug() << mq.DebugString().data();
        }
    }

    void fill(const std::string symbol,
              TradeTic *tt,
//              int week,
              uint64_t timestamp,
              int32_t seqnum) {

//        QString ds = QDateTime::currentDateTimeUtc().toString("yyyy-mm-dd-hh.mm.ss.ff");
//        qDebug() << " sql fill timestamp " << ds;

        QDateTime dt =  QDateTime::fromTime_t(timestamp);
        qDebug() << " fill DateTime: " << dt;

        QSqlDatabase db = QSqlDatabase::database (conname); // Open Connection
        QSqlQuery insertQuery(db);

        insertQuery.prepare
            ("INSERT INTO ticker "
             "(symbol,price,qty,tictime,seqnum)"
             "VALUES(:pid,:tic,:qty,:tictime,:seq)");

        insertQuery.bindValue(":pid",QString::fromStdString(symbol));
//        insertQuery.bindValue(":w", week);
        insertQuery.bindValue(":tic",tt->price());
        insertQuery.bindValue(":qty",tt->size());
        insertQuery.bindValue(":tictime",dt);
        insertQuery.bindValue(":seq",seqnum);

        bool good = insertQuery.exec();
        db.close();

        qDebug() << " sql query " << insertQuery.executedQuery();

        if ( ! good ) {
            qDebug() << " exec ret " << insertQuery.lastError().databaseText();
            //qDebug() << dist.DebugString();
        }

    }

    void openprice(const std::string &inplayerid, int price, int week) {

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

        fill(inplayerid,&tt,0,0);
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

    void playermapFeed (std::string feed, int playerid, std::string id) {
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
            if ( pd.player_status().has_symbol() && pd.player_status().symbol() != "" ) {
                insertQuery.prepare
                    ("INSERT INTO player (playerid,first,last,team,pos,roster_status, symbol)"
                     "VALUES(:pid,:f,:l,:t,:pos,:rs, :sy)");

                insertQuery.bindValue(":sy", QString::fromStdString(pd.player_status().symbol()));
            }
            else
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
            if ( pd.player_status().has_symbol() && pd.player_status().symbol() != "" ) {
                insertQuery.prepare
                    ("UPDATE player set team = :t, roster_status = :rs, symbol = :sy where playerid = :pid");
                insertQuery.bindValue(":sy", QString::fromStdString(pd.player_status().symbol()));
            }
            else
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

        qDebug() << " sql query " << insertQuery.executedQuery() << pd.DebugString().data();
        if ( ! good ) {
            qDebug() << " exec ret " << insertQuery.lastError().databaseText();
        }

    }

    void playerStatus(const PlayerStatus &ps, const std::string playerid) {
        QSqlDatabase db = QSqlDatabase::database (conname);
        QSqlQuery insertQuery(db);

        if ( ps.has_symbol() && ps.symbol() != "" ) {
            insertQuery.prepare
                ("UPDATE player set team = :t, roster_status = :rs, symbol = :sy where playerid = :pid");

            insertQuery.bindValue(":sy", QString::fromStdString(ps.symbol()));
        }
        else {
            insertQuery.prepare
                ("UPDATE player set team = :t, roster_status = :rs where playerid = :pid");
        }

        insertQuery.bindValue(":pid",std::stoi(playerid));
        if ( ps.has_teamid())
            insertQuery.bindValue(":t",QString::fromStdString(ps.teamid()));
        else
            insertQuery.bindValue(":t",QVariant(QVariant::String));

        if ( ps.status() == PlayerStatus::ACTIVE )
            insertQuery.bindValue(":rs", QChar('A'));
        else
            insertQuery.bindValue(":rs", QChar('I'));


//        else
//            insertQuery.bindValue(":sy", QString::fromStdString(playerid));


        bool good = insertQuery.exec();
        db.close();

        qDebug() << " sql query " << insertQuery.executedQuery() << ps.DebugString().data() << playerid.data();

        if ( ! good )
            qDebug() << " exec ret " << insertQuery.lastError().databaseText();

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

}

#endif // PLAYERLOADER_H


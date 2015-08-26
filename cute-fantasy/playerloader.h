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

    std::vector<fantasybit::ScheduleData> loadScheduleFromJsonFile() {
        std::vector<fantasybit::ScheduleData> result;

        //hard coded resources
        QFile jsonFile(":/Schedule2015/Schedule.2015-pre.json");
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
                        getScheduleFromJsonObject(playerData,errorParsingObject);
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

   fantasybit::GameInfo getScheduleFromJsonObject(QJsonObject & jsonObject,QString & errorParsingObject) const{
      fantasybit::GameInfo pd{};


      pd.set_id(jsonObject.value("id").toString().toStdString());
      pd.set_home(jsonObject.value("home").toString().toStdString());
      pd.set_away(jsonObject.value("away").toString().toStdString());

      auto datetime = jsonObject.value("scheduled").toString();

      auto qdt = QDateTime::fromString(datetime,Qt::ISODate);

      pd.set_time(qdt.toTime_t());

      qDebug() << pd.DebugString();

      return pd;

      /*
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
*/
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
    std::vector<fantasybit::PlayerData> loadPlayersFromTradeRadar() {
        std::vector<fantasybit::PlayerData> result;

        RestfullClient rest(QUrl("http://api.sportradar.us/nfl-b1/teams/"));

        for ( auto team : fantasybit::Commissioner::GENESIS_NFL_TEAMS) {
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
            auto vals = jo.value("players");
            QJsonArray parr = vals.toArray();

            for (int i=0;i< parr.size();i++  ) {
                QJsonValueRef data = parr[i];

                QJsonObject playerData = data.toObject();
                QString errorParsingObject;
                fantasybit::PlayerData player =
                        getPlayerFromJsonObject(playerData,team,errorParsingObject);

                if ( errorParsingObject != "" ) continue;
                result.push_back(player);
            }

        }

        return result;
    }

    fantasybit::PlayerData getPlayerFromJsonObject(QJsonObject & jsonObject,std::string team,QString & errorParsingObject) const{
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

       pd.set_playerid(jsonObject.value("id").toString().toStdString());

       fantasybit::PlayerBase pb{};
       pb.set_position(pos.toStdString());
       pb.set_first(jsonObject.value("name_first").toString().toStdString());
       pb.set_last(jsonObject.value("name_last").toString().toStdString());

       fantasybit::PlayerStatus ps{};
       ps.set_teamid(team);
/*
       enum PlayerStatus_Status {
         PlayerStatus_Status_ACTIVE = 0,
         PlayerStatus_Status_INACTIVE = 1,
         PlayerStatus_Status_OTHER = 3,
         PlayerStatus_Status_FA = 4
       };
*/
       auto status = jsonObject.value("status").toString();
       ps.set_status(status == "ACT" ? fantasybit::PlayerStatus_Status_ACTIVE : fantasybit::PlayerStatus_Status_INACTIVE);

       pd.mutable_player_base()->CopyFrom(pb);
       pd.mutable_player_status()->CopyFrom(ps);

       return pd;
    }

};

#endif // PLAYERLOADER


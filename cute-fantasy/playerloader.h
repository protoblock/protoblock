#ifndef PLAYERLOADER
#define PLAYERLOADER

#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QFile>
#include "ProtoData.pb.h"

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

    std::vector<fantasybit::PlayerData> loadPlayersFromJsonFile() {
        std::vector<fantasybit::PlayerData> result;

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

    std::vector<fantasybit::WeeklySchedule> loadScheduleFromJsonFile() {
        std::vector<fantasybit::WeeklySchedule> result;

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

        QJsonArray playerJsonData = doc.array();

        for (int i=0;i< playerJsonData.size();i++  ) {
            QJsonValueRef data = playerJsonData[i];
            //we are supposed to encounter a list of objects so skip values
            if (!data.isObject()) continue;
            QJsonObject playerData = data.toObject();
            QString errorParsingObject;
            fantasybit::WeeklySchedule player =
                    getScheduleFromJsonObject(playerData,errorParsingObject);
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

   fantasybit::WeeklySchedule getScheduleFromJsonObject(QJsonObject & jsonObject,QString & errorParsingObject) const{
      fantasybit::WeeklySchedule pd{};

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



#endif // PLAYERLOADER


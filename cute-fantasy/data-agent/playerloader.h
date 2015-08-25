#ifndef PLAYERLOADER
#define PLAYERLOADER

#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QFile>
#include "ProtoData.pb.h"

class PlayerLoader
{

public:
    PlayerLoader(){}
    ~PlayerLoader(){}

    bool loadPlayersFromJsonFile(QList<fantasybit::PlayerData> & result,
                               QString & errorMessage){
        //hard coded resources
        QFile jsonFile("C:\Users\User\Documents\work\fantasybit-2015\cute-fantasy\resources\Player.2015.json");
        if (!jsonFile.open(QIODevice::ReadOnly)){
            errorMessage = "Can't open the player file !";
            return false;
        }
        QByteArray jsonData   = jsonFile.readAll();
        jsonFile.close();
        QJsonParseError * error = NULL;
        QJsonDocument doc = QJsonDocument::fromJson(jsonData,error);

        if (error != NULL){
            errorMessage = error->errorString();
            return false;
        }
        if (doc.isEmpty()) {
            errorMessage = "File seems to be empty.";
            return false;
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
                errorMessage += "\n Error parsing json object : "+ errorParsingObject;
                continue;
            }

            QFile outfile("GenesisPLayer.txt");
            outfile.open(QIODevice::WriteOnly);

            //outfile.writeData(player.ser)

            result.append(player);
        }
        return true;
    }

   fantasybit::PlayerData getPlayerFromJsonObject(QJsonObject & jsonObject,QString & errorParsingObject) const{
      fantasybit::PlayerData pd{};

      auto pos = jsonObject.value("FantasyPosition").toString();
      if   (! (    player.FantasyPosition == "QB"
           || player.FantasyPosition == "RB"
           || player.FantasyPosition == "WR"
           || player.FantasyPosition == "TE"
           || player.FantasyPosition == "K")) {
          errorParsingObject = pos;
          return pd;
      }

      pd.set_playerid(jsonObject.value("PlayerID").toString());

      fantasybit::PlayerBase pb{};
      pb.set_position(jsonObject.value("FantasyPosition").toString());
      pb.set_first(jsonObject.value("FirstName").toString());
      pb.set_last(jsonObject.value("LastName").toString());

      fantasybit::PlayerStatus ps{};
      ps.set_teamid(jsonObject.value("Team").toString());

      pd.mutable_player_base()->CopyFrom(pb);
      pd.mutable_player_status()->CopyFrom(ps);

      return pd;
   }

};




#endif // PLAYERLOADER


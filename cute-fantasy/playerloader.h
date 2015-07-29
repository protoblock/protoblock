#ifndef PLAYERLOADER
#define PLAYERLOADER

#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include "models.h"
#include <QFile>

class PlayerLoader
{

public:
    static QPair<QString,QString> getPlayerInfo(const QString & playerId)  {
        QPair<QString,QString> info = PlayerLoader::myPlayerInfoCache.value(playerId);
        if (info.first =="")  info.first = "Unknown Player";
        return info;
    }

    static QList<QString> getTeamPlayers(const QString& teamID)   {
        return PlayerLoader::myTeamsPlayers.values(teamID);
    }

    struct JsonPlayer {
        QString PlayerID;//  ARI,
        QString Team; //: 1,
        QString FantasyPosition;// : 1,
        QString Name; // Arizona,
        QString PhotoUrl;// Cardinals,
        QString DepthPosition;// West,
        int DepthOrder;// Arizona Cardinals,
    };

    PlayerLoader(){}
    ~PlayerLoader(){}

    bool loadPlayersFromJsonFile(QList<JsonPlayer> & result,
                               QString & errorMessage){
        //hard coded resources
        QFile jsonFile(":/fantasy2015/Player.2015.json");
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
            JsonPlayer player = getPlayerFromJsonObject(playerData,errorParsingObject);
            if (!errorParsingObject.isEmpty()) {
                errorMessage += "\n Error parsing json object : "+ errorParsingObject;
                continue;
            }

            if (    player.FantasyPosition == "QB"
                 || player.FantasyPosition == "RB"
                 || player.FantasyPosition == "WR"
                 || player.FantasyPosition == "TE"
                 || player.FantasyPosition == "K"
                 //|| player.FantasyPosition == "DEF"//not in this file
                    )
                //if ( player.DepthOrder > 0 && player.DepthOrder < 3)
                {
                    result.append(player);
                    CachePlayer(player);
                    //qDebug() << "{" << player.PlayerID << "," << player.Team << "},";
                }
        }
        return true;
    }


   static void CachePlayer(JsonPlayer player) {
       myPlayerInfoCache[player.PlayerID] =
               QPair<QString,QString>(player.Name,player.FantasyPosition);
       myTeamsPlayers.insert(player.Team,player.PlayerID);
   }


   JsonPlayer getPlayerFromJsonObject(QJsonObject & jsonObject,QString & /*errorParsingObject*/) const{
      JsonPlayer jsonPlayer;
      jsonPlayer.PlayerID=jsonObject.value("PlayerID").toString();
      jsonPlayer.Team=jsonObject.value("Team").toString();
      jsonPlayer.FantasyPosition=jsonObject.value("FantasyPosition").toString();
      jsonPlayer.Name=jsonObject.value("Name").toString();
      jsonPlayer.PhotoUrl=jsonObject.value("PhotoUrl").toString();
      jsonPlayer.DepthPosition=jsonObject.value("DepthPosition").toString();
      QString str = jsonObject.value("DepthOrder").toString();
      jsonPlayer.DepthOrder=str.toInt();
      return jsonPlayer;
   }

private:
   static QHash<QString, QPair<QString,QString> > myPlayerInfoCache;
   static QMultiHash<QString,QString> myTeamsPlayers;   
};




#endif // PLAYERLOADER


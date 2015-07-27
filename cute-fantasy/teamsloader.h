#ifndef TEAMLOADER_H
#define TEAMLOADER_H

#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include "models.h"
#include <QFile>


class TeamLoader
{
public:

    struct JsonTeam {
        QString myKey;//  ARI,
        QString myTeamID; //: 1,
        QString myPlayerID;// : 1,
        QString myCity; // Arizona,
        QString myName;// Cardinals,
        QString myConference;// NFC,
        QString myDivision;// West,
        QString myFullName;// Arizona Cardinals,
        QString myStadiumID;// 29,
        quint32 myByeWeek;// 9,
        qreal myAverageDraftPosition;// 137.90,
        qreal myAverageDraftPositionPPR;// 136.20,
        QString myHeadCoach;// Bruce Arians,
        QString myOffensiveCoordinator;// Harold Goodwin,
        QString myDefensiveCoordinator;// James Bettcher,
        QString mySpecialTeamsCoach;// Amos Jones,
        QString myOffensiveScheme;// 2TE,
        QString myDefensiveScheme;// 3-4,
        QVariant myUpcomingSalary;// null,
        quint32 myUpcomingOpponentRank;// 15,
        quint32 myUpcomingOpponentPositionRank;// 15
    };


    TeamLoader(){}
    ~TeamLoader(){}

    bool loadTeamsFromJsonFile(QList<JsonTeam> & result,
                               QString & errorMessage){
        //hard coded resources
        QFile jsonFile(":/team2015/Team.2015.json");
        if (!jsonFile.open(QIODevice::ReadOnly)){
            errorMessage = "Can't open the team file !";
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

        QJsonArray teamsJsonData = doc.array();

        for (int i=0;i< teamsJsonData.size();i++  ) {
            QJsonValueRef data = teamsJsonData[i];
            //we are supposed to encounter a list of objects so skip values
            if (!data.isObject()) continue;
            QJsonObject teamData = data.toObject();
            QString errorParsingObject;
            JsonTeam team = getTeamFromJsonObject(teamData,errorParsingObject);
            if (errorParsingObject.isEmpty()) {
                result.append(team);
                //qDebug() << "{" << team.myPlayerID << "," << team.myKey << "},";
            }
            else
                errorMessage += "\n Error parsing json object : "+ errorParsingObject;
        }
        return true;
    }


   JsonTeam getTeamFromJsonObject(QJsonObject & jsonObject,QString & errorParsingObject) const{
      JsonTeam jsonTeam;
      jsonTeam.myKey=jsonObject.value("Key").toString();
      jsonTeam.myTeamID=jsonObject.value("TeamID").toString();
      jsonTeam.myPlayerID=jsonObject.value("PlayerID").toString();
      jsonTeam.myCity=jsonObject.value("City").toString();
      jsonTeam.myName=jsonObject.value("Name").toString();
      jsonTeam.myConference=jsonObject.value("Conference").toString();
      jsonTeam.myDivision=jsonObject.value("Division").toString();
      jsonTeam.myFullName=jsonObject.value("FullName").toString();
      jsonTeam.myStadiumID=jsonObject.value("StadiumID").toString();
      jsonTeam.myByeWeek=jsonObject.value("ByeWeek").toInt();
      jsonTeam.myAverageDraftPosition=jsonObject.value("AverageDraftPosition").toDouble();
      jsonTeam.myAverageDraftPositionPPR=jsonObject.value("AverageDraftPositionPPR").toDouble();
      jsonTeam.myHeadCoach=jsonObject.value("HeadCoach").toString();
      jsonTeam.myOffensiveCoordinator=jsonObject.value("OffensiveCoordinator").toString();
      jsonTeam.myDefensiveCoordinator=jsonObject.value("DefensiveCoordinator").toString();
      jsonTeam.mySpecialTeamsCoach=jsonObject.value("SpecialTeamsCoach").toString();
      jsonTeam.myOffensiveScheme=jsonObject.value("OffensiveScheme").toString();
      jsonTeam.myDefensiveScheme=jsonObject.value("DefensiveScheme").toString();
      jsonTeam.myUpcomingSalary=jsonObject.value("UpcomingSalary").toString();
      jsonTeam.myUpcomingOpponentRank=jsonObject.value("UpcomingOpponentRank").toInt();
      jsonTeam.myUpcomingOpponentPositionRank=jsonObject.value("UpcomingOpponentPositionRank").toInt();
      return jsonTeam;
   }

};

#endif // TEAMLOADER_H

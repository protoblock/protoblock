#ifndef SCHEDULELOADER
#define SCHEDULELOADER
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include "models.h"
#include <QFile>
#include <QDateTime>

class ScheduleLoader
{
public:


    struct JsonSchedule {
        QString GameKey;//  ARI,
        quint32 SeasonType; //: 1,
        quint32 Week;// : 1,
        QDateTime Date; // Arizona,
        QString AwayTeam;// Cardinals,
        QString HomeTeam;// West,
    };

    ScheduleLoader(){}
    ~ScheduleLoader(){}

    bool loadSchedulesFromJsonFile(QMultiMap<int,JsonSchedule> & result,
                               QString & errorMessage){

        qDebug() << " loading~";
        //hard coded resources
        QFile jsonFile(":/team2015/Schedule.2015.json");
        if (!jsonFile.open(QIODevice::ReadOnly)){
            errorMessage = "Can't open the Schedule file !";
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

        QJsonArray ScheduleJsonData = doc.array();

        for (int i=0;i< ScheduleJsonData.size();i++  ) {
            QJsonValueRef data = ScheduleJsonData[i];
            //we are supposed to encounter a list of objects so skip values
            if (!data.isObject()) continue;
            QJsonObject ScheduleData = data.toObject();
            QString errorParsingObject;
            JsonSchedule Schedule = getScheduleFromJsonObject(ScheduleData,errorParsingObject);
            if (!errorParsingObject.isEmpty()) {
                errorMessage += "\n Error parsing json object : "+ errorParsingObject;
                continue;
            }

            if (Schedule.SeasonType == 1)
                result.insert(Schedule.Week,Schedule);
        }
        return true;
    }


   JsonSchedule getScheduleFromJsonObject(QJsonObject & jsonObject,QString & errorParsingObject) const{
      JsonSchedule jsonSchedule;
      jsonSchedule.GameKey=jsonObject.value("GameKey").toString();

      QString str = jsonObject.value("SeasonType").toString();
      jsonSchedule.SeasonType=str.toInt();

      str = jsonObject.value("Week").toString();
      jsonSchedule.Week=str.toInt();

      str = jsonObject.value("Date").toString();
      jsonSchedule.Date=QDateTime::fromString(str,"M/dd/yyyy h:mm:ss AP");

      jsonSchedule.AwayTeam=jsonObject.value("AwayTeam").toString();
      jsonSchedule.HomeTeam=jsonObject.value("HomeTeam").toString();
      return jsonSchedule;
   }

};

#endif // SCHEDULELOADER


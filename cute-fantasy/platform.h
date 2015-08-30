#ifndef PLATFORM_H
#define PLATFORM_H

#include "genericsingleton.h"
#in

class AppSettings;
class Platform : GenericSingleton<Platform>
{
    friend class GenericSingleton<Platform>;
    Platform();

public:
    ~Platform();
    static AppSettings * settings();
    static std::string getRootDir();

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
    static bool runQuery(){
        QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");

            db.setHostName("162.254.27.178");
            db.setPort(3306);
            db.setDatabaseName("satoshifantasy");
            db.setUserName("datafeed");
            db.setPassword("s@tof@nt@sy6#1");
            bool success = true;
            if (!db.open()) {
                 qWarning() << "Database error occurred :" << db.lastError().databaseText();
                 //LOG(lg,error) << "db errror :"<< db.lastError().databaseText().toStdString();
                return false;
            }


            QSqlQuery insertQuery(db);
            insertQuery.prepare("INSERT INTO fantasyteam_copy (fantasyteam, fantasybits, stake) VALUES(:player_name,:bits,:bits)");
            insertQuery.bindValue(":player_name",viewModel->playerName());
            insertQuery.bindValue(":bits",viewModel->bits());

            if ( !insertQuery.exec() )
            {
                QSqlQuery updateQuery(db);
                updateQuery.prepare("UPDATE fantasyteam_copy set fantasybits= :bits, stake= :bits where fantasyteam= :player_name");
                updateQuery.bindValue(":player_name",viewModel->playerName());
                updateQuery.bindValue(":bits",viewModel->bits());
                if (!updateQuery.exec()) {
                    //LOG(lg,info) << " exec ret " << updateQuery.lastError().databaseText();
                    success= false;
                }
            }
            db.close();
            return success;
        }
    };
#endif // PLATFORM_H

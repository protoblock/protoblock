#include "teammap.h"

TeamMap::TeamMap(QObject *parent) : QObject(parent)
{
    setupMap ();
}

void TeamMap::setupMap()
{
            QMap<QString,QString> longNames;
            longNames.insert ("SF","San Francisco 49ers");
            longNames.insert ("CHI","Chicago Bears");
            longNames.insert ("CIN","Cincinnati Bengals");
            longNames.insert ("BUF","Buffalo Bills" );
            longNames.insert ("DEN","Denver Broncos" );
            longNames.insert ("CLE","Cleveland Browns" );
            longNames.insert ("TB","Tampa Bay Buccaneers" );
            longNames.insert ("ARI","Arizona Cardinals" );
            longNames.insert ("SD","San Diego Chargers");
            longNames.insert ("KC","Kansas City Chiefs");
            longNames.insert ("IND","Indianapolis Colts" );
            longNames.insert ("DAL","Dallas Cowboys" );
            longNames.insert ("MIA","Miami Dolphins" );
            longNames.insert ("PHI","Philadelphia Eagles");
            longNames.insert ("ATL","Atlanta Falcons" );
            longNames.insert ("NYG","New York Giants" );
            longNames.insert ("JAC","Jacksonville Jaguars");
            longNames.insert ("NYJ","New York Jets" );
            longNames.insert ("DET","Detroit Lions");
            longNames.insert ("GB","Green Bay Packers");
            longNames.insert ("CAR","Carolina Panthers" );
            longNames.insert ("NE","New England Patriots");
            longNames.insert ("OAK","Oakland Raiders" );
            longNames.insert ("STL","St.Louis Rams" );
            longNames.insert ("BAL","Baltimore Ravens");
            longNames.insert ("WAS","Washington Redskins");
            longNames.insert ("NO","New Orlean Saints" );
            longNames.insert ("SEA","Seattle Seahawks" );
            longNames.insert ("PIT","Pittsburgh Steelers");
            longNames.insert ("HOU","Houston Texans" );
            longNames.insert ("TEN","Tennesse Titans");
            longNames.insert ("MIN","Minnesota Vikings");
            m_longNameMap = longNames;
            longNames.clear ();
}

QString TeamMap::getTeamLongName(const QString shortName)
{
    return m_longNameMap.value (shortName);
}



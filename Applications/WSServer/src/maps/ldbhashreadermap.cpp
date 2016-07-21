#include "ldbhashreadermap.h"

LdbHashReaderMap::LdbHashReaderMap(QObject *parent) : QObject(parent)
{
    setMap ();
}

QString LdbHashReaderMap::getKey(const QString key)
{

    if( m_map.contains(key) )
    {
//        qDebug() << key << " Found Value = " << m_map.value(key) << " !!";
        return m_map.value (key);
    }
    else if (key.contains ("root",Qt::CaseSensitive))
    {
        return "MerkleTree";
    }
    else
    {
        return "NULL";
    }

}

void LdbHashReaderMap::setMap()
{

    m_map.clear ();

    QMap<QString,QString> theMap;



    theMap.insert ("NULL","BlockMeta");




    theMap.insert ("NULL" ,"TxMeta");




    theMap.insert ("NULL" ,"TrMeta");




    theMap.insert ("NULL" ,"GameMeta");

    theMap.insert ("NULL" ,"DataMeta");

    theMap.insert ("leaderboardstateid" ,"LeaderBoardMeta");

//    theMap.insert ("fnbalmetaroot" ,"FantasyNameBalMeta");

    theMap.insert ("NULL" ,"FantasyNameState");

    theMap.insert ("NULL" ,"WeekGameStatusMeta");

    theMap.insert ("NULL" ,"InGameProjMeta");

    theMap.insert ("NULL" ,"GameResultsMeta");

    theMap.insert ("NULL" ,"GameStatusMeta");

    theMap.insert ("NULL" ,"TeamProjMeta");

    theMap.insert ("NULL" ,"TeamResultMeta");

    theMap.insert ("NULL" ,"GamePlayerProjMeta");

//    theMap.insert ("projstateid" ,"ProjMeta");

    theMap.insert ("NULL" ,"PosMeta");

    theMap.insert ("NULL" ,"PlayerResultMeta");

    theMap.insert ("NULL" ,"AwardMeta");

    theMap.insert ("NULL" ,"PnlMeta");

    theMap.insert ("NULL" ,"PlayerMarketState");

    theMap.insert ("NULL" ,"LimitBookMeta");

    theMap.insert ("NULL" ,"InsideBookMeta");

    theMap.insert ("NULL" ,"MarketTicMeta");

    theMap.insert ("NULL" ,"OrderMeta");

    theMap.insert ("NULL" ,"OrderFillMeta");

    theMap.insert ("globalstateid" ,"GlobalStateMeta");

    theMap.insert ("NULL" ,"ScheduleStateId");

    theMap.insert ("NULL" ,"PlayerMeta");

    theMap.insert ("NULL" ,"TeamMeta");

    theMap.insert ("pbstateid","pbstate");



    // MERKLES
    theMap.insert ( "teamstatemid" ,"MerkleTree" );
    theMap.insert ( "schedulestateid" , "MerkleTree" );
    theMap.insert ("projstateid" ,"MerkleTree");





    theMap.insert ( "", "NULL");
    m_map = theMap;

}

#include "teamcolors.h"
TeamColors::TeamColors(QObject *parent) :
    QObject(parent)
{
    setPrimaryMap();
}

void TeamColors::setPrimaryMap()
{
    // just in case
    m_map.clear();
    QMap<QString,QString> &map = m_map;
    map.insert("ARI" , "#97233F");
    map.insert("ATL" , "#C60C30");
    map.insert("BAL" , "#241773");
    map.insert("BUF" , "#00338D");
    map.insert("CAR" , "#0088CE");
    map.insert("CHI" , "#DD4814");
    map.insert("CIN" , "#F9461C");
    map.insert("CLE" , "#F9461C");
    map.insert("DAL" , "#002147");
    map.insert("DEN" , "#FF6319");
    map.insert("DET" , "#85888B");
    map.insert("GB" , "#FFB612");
    map.insert("IND" , "#002395" );
    map.insert("JAC" , "#006983");
    map.insert("KC" , "#C60C30");
    map.insert("HOU" , "#AE162B");
    map.insert("MIA" , "#006265");
    map.insert("MIN" , "#4B306A");
    map.insert("NE" , "#002244");
    map.insert("NO" , "#968252");
    map.insert("NYG" , "#0B2265");
    map.insert("NYJ" , "#2C5E4F");
    map.insert("OAK" , "#85888B");
    map.insert("LV" , "#85888B");
    map.insert("PHI" , "#004953");
    map.insert("PIT" , "#111C24");
    map.insert("SD" , "#002244");
    map.insert("LAC" , "#002244");
    map.insert("SEA" , "#00338D");
    map.insert("SF" , "#97233F");
    map.insert("STL", "#002147");
    map.insert("LA", "#002147");
    map.insert("LAR", "#002147");
    map.insert("TB" , "#A71930");
    map.insert("TEN" , "#002147");
    map.insert("WAS", "#822433" );
//    m_map = map;


    //2nd colors
    QMap<QString,QString> &map2 = m_secondMap;
    map2.insert("ARI" , "#FFFFFF");
    map2.insert("ATL" , "#111C24");
    map2.insert("BAL" , "#111C24");
    map2.insert("BUF" , "#C60C30");
    map2.insert("CAR" , "#111C24");
    map2.insert("CHI" , "#031E2F");
    map2.insert("CIN" , "#111C24");
    map2.insert("CLE" , "#332B2A");
    map2.insert("DAL" , "#828A87");
    map2.insert("DEN" , "#002147");
    map2.insert("DET" , "#2A6EBB");
    map2.insert("GB" , "#2C5E4F");
    map2.insert("IND" , "#FFFFFF" );
    map2.insert("JAC" , "#111C24");
    map2.insert("KC" , "#FFB612");
    map2.insert("HOU" , "#0F1F2C");
    map2.insert("MIA" , "#F9461C");
    map2.insert("MIN" , "#FFB612");
    map2.insert("NE" , "#C60C30");
    map2.insert("NO" , "#111C24");
    map2.insert("NYG" , "#A71930");
    map2.insert("NYJ" , "#FFFFFF");
    map2.insert("OAK" , "#111C24");
    map2.insert("LV" , "#111C24");
    map2.insert("PHI" , "#111C24");
    map2.insert("PIT" , "#FFB612");
    map2.insert("SD" , "#FFB612");
    map2.insert("LAC" , "#FFB612");
    map2.insert("SEA" , "#008542");
    map2.insert("SF" , "#8E6E4D");
    //
    map2.insert("STL", "#95774D");
    map2.insert("LA", "#95774D");
    map2.insert("LAR", "#95774D");
    map2.insert("TB" , "#665C4F");
    map2.insert("TEN" , "#4B92DB");
    map2.insert("WAS", "#FFB612" );
//    m_secondMap = map2;

    m_pos.insert("RB", "#BCFAAD");
        m_pos.insert("WR", "#FEFBB6");
        m_pos.insert("TE", "#CCB4F0");
        m_pos.insert("DEF","#AFE1FF");
        m_pos.insert("QB", "#F8ADAA");
        m_pos.insert("K", "#FBD580");

}



QString TeamColors::getPrimaryAt(const QString &team)
{
    return m_map.value(team);
}

QString TeamColors::getSecondaryAt(const QString &team)
{
    return m_secondMap.value(team);
}

QString TeamColors::getPosColor(const QString &team){
    return m_pos.value(team);
}

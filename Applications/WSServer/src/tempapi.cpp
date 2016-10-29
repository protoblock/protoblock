#include "tempapi.h"
//#include "ldbwriter.h"
//#include "pbjson.hpp"
#include "QStringList"


namespace fantasybit {
TempApi::TempApi(QObject *parent) :
    QObject(parent),
    ldb(*LdbHashReaderTool::instance()),
    m_pnm(&m_PlayerNewModel)
{
    //ldb.init();
    qDebug() << " tempapi" ;
    BlockMeta bm;
    ldb.read(ldb.read("blocknum_100"),bm);
        qDebug() << " tempapi2" << bm.DebugString().data();
    initState(bm.pbstateid());
    emit open();
        qDebug() << " tempapi over" ;
}

QString TempApi::fromTime_t_toFantasyString(uint dtt) {
    QByteArray zone = "America/New_York";
    QTimeZone tz(zone);

    return QDateTime::fromTime_t(dtt,tz).toString("MMMM dd , h:mm a yyyy"); //.remove(" pm");
}

//PlayerNewModel * TempApi::pnm() {
//    return &m_pnm;
//}

QString TempApi::getFantasyIdByName(const QString fName)
{
//    return fname2Id[fName].
    return "FUCKYOU ";
}

QString TempApi::getPlayerNameById(const QString playerId)
{
    QString ret = QString("%1 %2")
        .arg (pid2Playeritem[playerId].firstName)
        .arg(pid2Playeritem[playerId].lastName);
    return ret;
}

QString TempApi::getPlayerPostionById(const QString playerId)
{
    return pid2Playeritem[playerId].position;
}

QString TempApi::getTeamsByGameId(const QString gameId)
{
    QString ret = QString("%1 vs %2")
            .arg (gid2Game[gameId].hometeam)
            .arg (gid2Game[gameId].awayteam);
    return ret;
}

void TempApi::setAllStats()
{
    //    QString playerId;
    StatsItems m_stats;
    AwardItem m_aws;
    PnlItem m_pnl;


    for (auto  df : mPrevWeeks)
    {
        for (auto it : df.closedGames ){
            // AWAY Game
            for (auto i : it.awayRes.playResults){
                m_stats.week = df.week;
                m_stats.gameid = it.gameid;
                m_stats.homeName = QString("NotHome");
                m_stats.isHome = false;
                m_stats.awayName =  it.awayRes.team ;
                m_stats.isAway = true;
                //FIXME
                m_stats.playerId =  i.playerId ;
                m_stats.passtd = i.stats.ostats ().passtd ();
                m_stats.passyds = i.stats.ostats ().passyds ();
                m_stats.rushtd  = i.stats.ostats ().rushtd ();
                m_stats.rushyds = i.stats.ostats ().rushyds ();
                m_stats.rectd = i.stats.ostats ().rectd ();
                m_stats.recyds = i.stats.ostats ().recyds ();
                m_stats.rec = i.stats.ostats ().rec ();
                m_stats.pint = i.stats.ostats ().pint ();
                m_stats.fumble = i.stats.ostats ().fumble ();
                m_stats.twopt = i.stats.ostats ().twopt ();
                //FIXME
                m_stats.fg = 0 ;// i.stats.kstats ().fg (1);
                m_stats.pa =i.stats.kstats ().pa ();
                m_stats.ptsa = i.stats.dstats ().ptsa ();
                m_stats.deftd = i.stats.dstats ().deftd ();
                m_stats.sacks = i.stats.dstats ().sacks ();
                m_stats.turnovers = i.stats.dstats ().turnovers ();
                m_stats.sfty = i.stats.dstats ().sfty ();
                m_stats.dtwopt = i.stats.dstats ().twopt ();

                m_statsItem.append (m_stats);

                for (auto aI : i.fnameProjections )
                {
                    m_aws.playerId = aI.playerId;
                    m_aws.award = aI.award;
                    m_aws.fantasyName = aI.fantasyName;
                    m_aws.projection = aI.projection;
                    m_aws.week = df.week;
                    m_aws.gameId = it.gameid;

                    //m_awards.append (m_aws);

                }
                //awards
                for(auto fnPos : i.fnamePositions)
                {
                    m_pnl.week = df.week;
                    m_pnl.fantasyName = fnPos.fantasyName ;
                    m_pnl.playerId =  fnPos.playerId ;
                    m_pnl.pnl = fnPos.pnl;
                    m_pnl.price =  fnPos.price ;
                    m_pnl.qty =  fnPos.qty;
                    m_pnl.gameId = it.gameid;

                    m_pnlItem.append (m_pnl);
                }
            }

                // HOME
            for (auto i : it.homeRes.playResults){
                m_stats.week = df.week;
                m_stats.gameid = it.gameid;
                m_stats.homeName =  it.homeRes.team ;
                m_stats.isHome = true;
                m_stats.awayName = QString("NotAway");
                m_stats.isAway = false;
                m_stats.playerId = i.playerId;
                m_stats.passtd = i.stats.ostats ().passtd ();
                m_stats.passyds = i.stats.ostats ().passyds ();
                m_stats.rushtd  = i.stats.ostats ().rushtd ();
                m_stats.rushyds = i.stats.ostats ().rushyds ();
                m_stats.rectd = i.stats.ostats ().rectd ();
                m_stats.recyds = i.stats.ostats ().recyds ();
                m_stats.rec = i.stats.ostats ().rec ();
                m_stats.pint = i.stats.ostats ().pint ();
                m_stats.fumble = i.stats.ostats ().fumble ();
                m_stats.twopt = i.stats.ostats ().twopt ();
                //FIXME
                m_stats.fg = 0 ; //i.stats.kstats ().fg (2);
                m_stats.pa =i.stats.kstats ().pa ();
                m_stats.ptsa = i.stats.dstats ().ptsa ();
                m_stats.deftd = i.stats.dstats ().deftd ();
                m_stats.sacks = i.stats.dstats ().sacks ();
                m_stats.turnovers = i.stats.dstats ().turnovers ();
                m_stats.sfty = i.stats.dstats ().sfty ();
                m_stats.dtwopt = i.stats.dstats ().twopt ();

                m_statsItem.append (m_stats);

                for (auto aI : i.fnameProjections )
                {
                    m_aws.playerId = aI.playerId;
                    m_aws.award = aI.award;
                    m_aws.fantasyName = aI.fantasyName;
                    m_aws.projection = aI.projection;
                    m_aws.week = df.week;
                    m_aws.gameId = it.gameid;

                    //m_awards.append (m_aws);

                }
                for(auto fnPos : i.fnamePositions)
                {
                    m_pnl.week  = df.week;
                    m_pnl.fantasyName = fnPos.fantasyName ;
                    m_pnl.playerId =  fnPos.playerId ;
                    m_pnl.pnl = fnPos.pnl;
                    m_pnl.price =  fnPos.price ;
                    m_pnl.qty =  fnPos.qty;
                    m_pnl.gameId = it.gameid;

                    m_pnlItem.append (m_pnl);

                }
            }
        }
    }

    for (auto it : mCurrentWeek.closedGames ){
        // AWAY Game
        for (auto i : it.awayRes.playResults){
            m_stats.week = mCurrentWeek.week;
            m_stats.gameid = it.gameid;
            m_stats.homeName = QString("NotHome");
            m_stats.isHome = false;
            m_stats.awayName =  it.awayRes.team ;
            m_stats.isAway = true;
            //FIXME
            m_stats.playerId =  i.playerId ;
            m_stats.passtd = i.stats.ostats ().passtd ();
            m_stats.passyds = i.stats.ostats ().passyds ();
            m_stats.rushtd  = i.stats.ostats ().rushtd ();
            m_stats.rushyds = i.stats.ostats ().rushyds ();
            m_stats.rectd = i.stats.ostats ().rectd ();
            m_stats.recyds = i.stats.ostats ().recyds ();
            m_stats.rec = i.stats.ostats ().rec ();
            m_stats.pint = i.stats.ostats ().pint ();
            m_stats.fumble = i.stats.ostats ().fumble ();
            m_stats.twopt = i.stats.ostats ().twopt ();
            //FIXME
            m_stats.fg = 0 ;// i.stats.kstats ().fg (1);
            m_stats.pa =i.stats.kstats ().pa ();
            m_stats.ptsa = i.stats.dstats ().ptsa ();
            m_stats.deftd = i.stats.dstats ().deftd ();
            m_stats.sacks = i.stats.dstats ().sacks ();
            m_stats.turnovers = i.stats.dstats ().turnovers ();
            m_stats.sfty = i.stats.dstats ().sfty ();
            m_stats.dtwopt = i.stats.dstats ().twopt ();

            m_statsItem.append (m_stats);

            for (auto aI : i.fnameProjections )
            {
                m_aws.playerId = aI.playerId;
                m_aws.award = aI.award;
                m_aws.fantasyName = aI.fantasyName;
                m_aws.projection = aI.projection;
                m_aws.week = mCurrentWeek.week;
                m_aws.gameId = it.gameid;

                //m_awards.append (m_aws);

            }
            //awards
            for(auto fnPos : i.fnamePositions)
            {
                m_pnl.week = mCurrentWeek.week;
                m_pnl.fantasyName = fnPos.fantasyName ;
                m_pnl.playerId =  fnPos.playerId ;
                m_pnl.pnl = fnPos.pnl;
                m_pnl.price =  fnPos.price ;
                m_pnl.qty =  fnPos.qty;
                m_pnl.gameId = it.gameid;

                m_pnlItem.append (m_pnl);
            }
        }

            // HOME
        for (auto i : it.homeRes.playResults){
            m_stats.week = mCurrentWeek.week;
            m_stats.gameid = it.gameid;
            m_stats.homeName = it.homeRes.team ;
            m_stats.isHome = true;
            m_stats.awayName = QString("NotAway");
            m_stats.isAway = false;
            m_stats.playerId = i.playerId;
            m_stats.passtd = i.stats.ostats ().passtd ();
            m_stats.passyds = i.stats.ostats ().passyds ();
            m_stats.rushtd  = i.stats.ostats ().rushtd ();
            m_stats.rushyds = i.stats.ostats ().rushyds ();
            m_stats.rectd = i.stats.ostats ().rectd ();
            m_stats.recyds = i.stats.ostats ().recyds ();
            m_stats.rec = i.stats.ostats ().rec ();
            m_stats.pint = i.stats.ostats ().pint ();
            m_stats.fumble = i.stats.ostats ().fumble ();
            m_stats.twopt = i.stats.ostats ().twopt ();
            //FIXME
            m_stats.fg = 0 ; //i.stats.kstats ().fg (2);
            m_stats.pa =i.stats.kstats ().pa ();
            m_stats.ptsa = i.stats.dstats ().ptsa ();
            m_stats.deftd = i.stats.dstats ().deftd ();
            m_stats.sacks = i.stats.dstats ().sacks ();
            m_stats.turnovers = i.stats.dstats ().turnovers ();
            m_stats.sfty = i.stats.dstats ().sfty ();
            m_stats.dtwopt = i.stats.dstats ().twopt ();

            m_statsItem.append (m_stats);

            for (auto aI : i.fnameProjections )
            {
                m_aws.playerId = aI.playerId;
                m_aws.award = aI.award;
                m_aws.fantasyName = aI.fantasyName;
                m_aws.projection = aI.projection;
                m_aws.week = mCurrentWeek.week;
                m_aws.gameId = it.gameid;

                //m_awards.append (m_aws);

            }
            for(auto fnPos : i.fnamePositions)
            {
                m_pnl.week  = mCurrentWeek.week;
                m_pnl.fantasyName = fnPos.fantasyName ;
                m_pnl.playerId =  fnPos.playerId ;
                m_pnl.pnl = fnPos.pnl;
                m_pnl.price =  fnPos.price ;
                m_pnl.qty =  fnPos.qty;
                m_pnl.gameId = it.gameid;

                m_pnlItem.append (m_pnl);

            }
        }
    }


    for (auto it : mCurrentWeek.lockedGames ){
        // AWAY Game
        for (auto i : it.awayProj.playProjPos){
            for (auto aI : i.fnameProjections )
            {
                m_aws.playerId = aI.playerId;
                m_aws.award = 0;
                m_aws.fantasyName = aI.fantasyName;
                m_aws.projection = aI.projection;
                m_aws.week = mCurrentWeek.week;
                m_aws.gameId = it.gameid;

                //m_awards.append (m_aws);

            }
            //awards
            for(auto fnPos : i.fnamePositions)
            {
                m_pnl.week = mCurrentWeek.week;
                m_pnl.fantasyName = fnPos.fantasyName ;
                m_pnl.playerId =  fnPos.playerId ;
                m_pnl.pnl = 0;
                m_pnl.price =  fnPos.price ;
                m_pnl.qty =  fnPos.qty;
                m_pnl.gameId = it.gameid;

                m_pnlItem.append (m_pnl);
            }
        }

            // HOME
        for (auto i : it.homeProj.playProjPos){
            for (auto aI : i.fnameProjections )
            {
                m_aws.playerId = aI.playerId;
                m_aws.award = 0;
                m_aws.fantasyName = aI.fantasyName;
                m_aws.projection = aI.projection;
                m_aws.week = mCurrentWeek.week;
                m_aws.gameId = it.gameid;

                //m_awards.append (m_aws);

            }
            for(auto fnPos : i.fnamePositions)
            {
                m_pnl.week  = mCurrentWeek.week;
                m_pnl.fantasyName = fnPos.fantasyName ;
                m_pnl.playerId =  fnPos.playerId ;
                m_pnl.pnl = 0;
                m_pnl.price =  fnPos.price ;
                m_pnl.qty =  fnPos.qty;
                m_pnl.gameId = it.gameid;

                m_pnlItem.append (m_pnl);

            }
        }
    }

}

int TempApi::season() const {
    return m_globalstatemeta.globalstate().season();
}

int TempApi::week() const {
    if ( season() == 2016 ) return 17;
    return m_globalstatemeta.globalstate().week();
}

QString TempApi::globalstate() const {
    return GlobalState::State_Name(m_globalstatemeta.globalstate().state()).data();

}

QString TempApi::fnamebalrootid() const {
    return m_leaderboardmeta.fnbalmetaroot().data();
}

QString TempApi::dString() const {
    QString debugString;
    //return "teas";
    debugString.append("\n----------------------\nPB State\n----------------------\n")
            .append(m_pbstate.DebugString().data())
            .append("\n----------------------\nLeaderboard\n----------------------\n")
            .append( m_leaderboardmeta.DebugString().data())
            .append("\n----------------------\nSchedule ID\n----------------------\n")
            .append(m_weekgamestatusmetatree.DebugString().data())
            .append("\n----------------------\nProjection ID\n----------------------\n")
            .append(m_projectionstatetree.DebugString().data())
            .append("\n----------------------\nGlobal State\n----------------------\n")
            .append(m_globalstatemeta.DebugString().data())
            .append("\n----------------------\n Team state id\n----------------------\n")
            .append(m_teamstatetree.DebugString().data());
    return debugString;
}

void TempApi::initState(const std::string &stateid) {
    ldb.read(stateid,m_pbstate);

    qDebug() << m_pbstate.leaderboardstateid ().data ();

    ldb.read(m_pbstate.globalstateid(),m_globalstatemeta);
    ldb.read(m_pbstate.leaderboardstateid(),m_leaderboardmeta);
    ldb.read(m_pbstate.teamstatemid(),m_teamstatetree);
    ldb.read(m_pbstate.projstateid(),m_projectionstatetree);
    ldb.read(m_pbstate.posstateid(),m_positionsstatetree);

    ldb.read(m_pbstate.schedulestateid(),m_weekgamestatusmetatree);
    ldb.read(m_pbstate.marketstateid(),m_marketstatetree);
    ldb.read(m_pbstate.orderstateid(),m_orderstatetree);



    // for all of our models
    initLeaderboard();
    initPlayers();
    initProjections();
    initPositions();
    initWeeklyGames();
    initMarket();
    initOrders();
    setAllStats ();


    //        ldb.read(m_pbstate.schedulestateid(),m_scheduleStateId);
    //        ldb.read(m_pbstate.projstateid(),m_projectionStatId);
    //        ldb.read(m_pbstate.teamstatemid(),m_teamstatemid);
    //FIXME where is the close functions ?
//    ldb.close()
}

void TempApi::initMarket() {
    for ( auto nodestr : m_marketstatetree.leaves()) {
        PlayerMarketState pms;
        ldb.read(nodestr,pms);
        PlayerMarketItem pmi;
        pmi.playerid = pms.playerid().data();
        pmi.limitbook = getMarketLimitBook(pms.limitbookmetaid());
        pmi.tics = getMarketTics(pms.marketticmetaid());
        mPlayerMarkets.append(pmi);
    }
}

QList<MarketTicItem> TempApi::getMarketTics(const std::string &marketticmetaid)  {
    MerkleTree tree;
    ldb.read(marketticmetaid,tree);
    QList<MarketTicItem> ret;
    for( auto mt : tree.leaves()) {
        MarketTicMeta lbm;
        MarketTicItem lbi;
        ldb.read(mt,lbm);
        lbi.price = lbm.price();
        lbi.size = lbm.size();
        lbi.lastFill = getOrderFillItem(lbm.orderfillhead());
        ret.append(lbi);
    }

    return ret;
}

QList<LimitBookItem> TempApi::getMarketLimitBook(const std::string &limitbookmetaid)  {
    MerkleTree tree;
    ldb.read(limitbookmetaid,tree);
    QList<LimitBookItem> ret;
    for( auto lb : tree.leaves()) {
        LimitBookMeta lbm;
        ldb.read(lb,lbm);
        LimitBookItem lbi;
        lbi.bid = lbm.bid();
        lbi.bidsize = lbm.bidsize();
        lbi.ask = lbm.ask();
        lbi.asksize = lbm.asksize();
        MerkleTree tree;
        ldb.read(lbm.bidordermetaroot(), tree);
        lbi.bidorders = getOrders(tree);
        ldb.read(lbm.askordermetaroot(), tree);
        lbi.askorders = getOrders(tree);
        ret.append(lbi);
    }

    return ret;
}

void TempApi::initOrders() {
    mOrders = getOrders(m_orderstatetree);
}

QList<OrderItem> TempApi::getOrders(const MerkleTree &tree) {
    QList<OrderItem> ret;
    for ( auto nodestr : tree.leaves()) {
        OrderMeta om;
        ldb.read(nodestr,om);
        ret.append(getOrderItem(om));
    }

    return ret;
}

OrderItem TempApi::getOrderItem(const OrderMeta &om) {
    OrderItem oi;
    oi.fname = om.fname().data();
    oi.isBuy = om.buyside();
    oi.playerid = om.playerid().data();
    oi.price = om.price();
    oi.timestamp = om.timestamp();
    oi.orderFills = getOrderFillItems(om.orderfillmetaid().data());
    return oi;
}

OrderFillItem TempApi::getOrderFillItem(const OrderFillMeta &om) {
    OrderFillItem oi;
    oi.refnum = om.refnum();
    oi.fname = om.fname().data();
    oi.isBuy = om.buyside();
    oi.playerid = om.playerid().data();
    oi.fillprice = om.fillprice();
    oi.fillsize = om.fillsize();
    oi.timestamp = om.timestamp();
    return oi;
}

QList<OrderFillItem> TempApi::getOrderFillItems(const QString &ofillsroot) {
    MerkleTree tree;
    ldb.read(ofillsroot.toStdString(),tree);
    QList<OrderFillItem> ret;
    for (auto &fid : tree.leaves()) {
        ret.append(getOrderFillItem(fid));
    }
    return ret;
}

OrderFillItem TempApi::getOrderFillItem(const std::string &ofillsroot) {
    OrderFillMeta ofi;

    ldb.read(ofillsroot,ofi);
    return getOrderFillItem(ofi);
}


/*!
 * \brief TempApi::initLeaderboard

    This is used to set upo the models that we are going to use.
*/
void TempApi::initLeaderboard() {
    MerkleTree mtree;
    ldb.read(m_leaderboardmeta.fnbalmetaroot(),mtree);
    for ( auto nodestr : mtree.leaves()) {
        FantasyNameBalMeta fnbm;
        ldb.read(nodestr,fnbm);
        fnbm.GetTypeName();
        LeaderBoardItem lbi;
        lbi.fantasyName = fnbm.name().data();
        lbi.publicKey = fnbm.public_key().data();
        //            lbi.nameHash =
        lbi.stakeBalance = fnbm.stake();
        lbi.skillBalance = fnbm.bits();
        mLeaderBoard.append(lbi);
    }
}




/*!
 * \brief TempApi::TempApi::initPlayers
    Method that is used to set up the players models
*/
void TempApi::initPlayers() {
    for ( auto nodestr : m_teamstatetree.leaves()) {
        TeamMeta tm;
        ldb.read(nodestr,tm);
        MerkleTree pmtree;
        ldb.read(tm.playermetaidroot(),pmtree);
        for ( auto nodestrp : pmtree.leaves()) {
            PlayerMeta pm;
            ldb.read(nodestrp,pm);
            mPlayerModel.add_players()->CopyFrom(pm);
            PlayerItem pi;
            //qDebug() << "initPlayers" << mPlayerModel.DebugString().data ();
            pi.firstName = pm.player_base().first().data();
            pi.lastName = pm.player_base().last().data();
            pi.position = pm.player_base().position().data();
            pi.playerId =  pm.playerid().data() ;
            if (pm.player_status ().status () == PlayerStatus::ACTIVE)
                pi.playerStatus = "Active";
            else
                pi.playerStatus = "InAcitve";

            pi.team = pm.player_status().teamid().data();

            //qDebug() << "pi initPlayers" << pi.team << pi.playerId;

            mPlayers.append(pi);
            pid2Playeritem[pi.playerId] = pi;
            m_PlayerNewModel.append (new PlayerNewModelItem (
                               pi.firstName,
                               pi.lastName,
                               pi.position,
                               pi.team,
                               pi.playerStatus,
                               pi.playerId
                               ));
        }
    }
}

/*!
 * \brief TempApi::initProj

    This is used to set upo the models that we are going to use.
*/
void TempApi::initProjections() {
    for ( auto nodestr : m_projectionstatetree.leaves()) {
        ProjMeta pjm;
        ldb.read(nodestr,pjm);
        ProjItem pi;
        pi.fantasyName = pjm.name().data();
        pi.playerId = pjm.playerid().data();
        pi.projection = pjm.proj();
        mProjections.append(pi);
    }
}

/*!
 * \brief TempApi::initProj

    This is used to set upo the models that we are going to use.
*/
void TempApi::initPositions() {
    for ( auto nodestr : m_positionsstatetree.leaves()) {
        PosMeta pjm;
        ldb.read(nodestr,pjm);
        PosItem pi;
        pi.fantasyName = pjm.name().data();
        pi.playerId = pjm.playerid().data();
        pi.qty = pjm.qty();
        pi.price = pjm.price();
        mPositions.append(pi);
    }
}

void TempApi::initStats()
{


}


/*!
 * \brief TempApi::initWeeklyGames

    This is used to set upo the models that we are going to use.
*/
void TempApi::initWeeklyGames() {
    for ( auto nodestr : m_weekgamestatusmetatree.leaves()) {
        WeekGameStatusMeta wsm;
        ldb.read(nodestr,wsm);
//        qDebug() << "initWeeklyGames" << wsm.DebugString().data ();

        if ( wsm.week() > week()) {
            NextWeekItem next;
            next.week = wsm.week();
            next.openGames = getOpenGamesList(wsm.opengamestatusroot());
            mNextWeeks.append(next);
        }
        else if ( wsm.week() < week()) {
            PrevWeekItem prev;
            prev.week = wsm.week();
            prev.closedGames = getClosedGamesList(wsm.gameresultmetaroot());
            mPrevWeeks.append(prev);
        }
        else if ( wsm.week() == week() ) {
            mCurrentWeek.week = week();
            mCurrentWeek.openGames = getOpenGamesList(wsm.opengamestatusroot());
            mCurrentWeek.closedGames = getClosedGamesList(wsm.gameresultmetaroot());
            mCurrentWeek.lockedGames = getGamesList(wsm.ingameprojmetaroot());
        }
    }
}

QList<ScheduledGamesItem> TempApi::getOpenGamesList(const std::string &opengamestatusroot) {
    QList<ScheduledGamesItem> ret;

    MerkleTree gstree;
    ldb.read(opengamestatusroot,gstree);
    ScheduledGamesItem sgi;
    for  (auto &gl : gstree.leaves()) {
        sgi.gameStatus = getGameItemFromStatusMeta(gl);
        ret.append(sgi);
    }

    return ret;

}

QList<LockedGameItem> TempApi::getGamesList(const std::string &ingameprojmetaroot) {
    QList<LockedGameItem> ret;

    MerkleTree gstree;
    ldb.read(ingameprojmetaroot,gstree);
    LockedGameItem lgi;
    InGameProjMeta igm{};
    for  (auto &gl : gstree.leaves()) {
        ldb.read(gl,igm);
//        qDebug() << " InGame " << igm.DebugString().data();
        lgi.gameStatus = getGameItemFromStatusMeta(igm.gamestatusmetaid());
        lgi.gameid = lgi.gameStatus.gameid;
        lgi.homeProj = getTeamProj(igm.homeprojmeta());
        lgi.awayProj = getTeamProj(igm.awayprojmeta());

//        qDebug() << lgi.gameStatus.gameid;
        ret.append(lgi);
    }

    return ret;

}

TeamProjPos TempApi::getTeamProj(const std::string &teamprojmeta) {
    TeamProjPos tm;
    TeamProjMeta tpm{};
    ldb.read(teamprojmeta,tpm);
//    qDebug() << tpm.DebugString().data();
    tm.playProjPos = getGamePlayerProj(tpm.gameplayerprojmetaroot());

    return tm;
}

QList<GamePlayerProjPos> TempApi::getGamePlayerProj(const std::string &gameplayerprojmetaroot) {
    MerkleTree mtree{};
    ldb.read(gameplayerprojmetaroot,mtree);

    QList<GamePlayerProjPos> lgpp;
    for ( auto &gppmid : mtree.leaves()) {
        GamePlayerProjMeta gppm{};
        GamePlayerProjPos gpp;
        ldb.read(gppmid,gppm);

//        qDebug() << gppm.DebugString().data();

        gpp.playerId = gppm.playerid().data();
        gpp.fnameProjections = getProjItems(gppm.projmetaplayerroot());
        gpp.fnamePositions = getPosItems(gppm.posmetaplayerroot());
        lgpp.append(gpp);
    }

    return lgpp;
}

QList<ProjItem> TempApi::getProjItems(const std::string &projmetaplayerroot) {
    MerkleTree mtree{};
    ldb.read(projmetaplayerroot,mtree);

    QList<ProjItem> projret{};
    for ( auto nodestr : mtree.leaves()) {
        ProjMeta pjm;
        ldb.read(nodestr,pjm);
        ProjItem pi;
        pi.fantasyName = pjm.name().data();
        pi.playerId = pjm.playerid().data();
        pi.projection = pjm.proj();
        projret.append(pi);
    }

    return projret;
}

QList<PosItem> TempApi::getPosItems(const std::string &posmetaplayerroot) {
    MerkleTree mtree{};
    ldb.read(posmetaplayerroot,mtree);

    QList<PosItem> posret{};
    for ( auto nodestr : mtree.leaves()) {
        PosMeta pjm;
        ldb.read(nodestr,pjm);
        PosItem pi;
        pi.fantasyName = pjm.name().data();
        pi.playerId = pjm.playerid().data();
        pi.qty = pjm.qty();
        pi.price = pjm.price();
        posret.append(pi);
    }

    return posret;
}


GameStatusItem TempApi::getGameItemFromStatusMeta(const std::string &gamestatusmetaid) {
    GameStatusMeta gsm;
    GameStatusItem gsi;
    gsm.ParseFromString(ldb.read(gamestatusmetaid));
//    qDebug() << "getGameItemFromStatusMeta" << gsm.DebugString().data ();

    setGameItem(gsm,gsi);
    return gsi;
}

void TempApi::setGameItem(const GameStatusMeta &gsm, GameStatusItem &gsi) {
//    ClosedGameItem cgi;
    GameResultsMeta grm;
   // GameStatusItem gsi;
   gsi.gameid = gsm.gameinfo().id().data();
   gsi.week = gsm.week();

   gsi.hometeam = gsm.gameinfo().home().data();

//    qDebug() << gsi.hometeam;

   gsi.awayteam = gsm.gameinfo().away().data();
   gsi.time =   fromTime_t_toFantasyString( gsm.gameinfo ().time () );
   gsi.eTime = gsm.gameinfo ().time ();
    if (gsm.gamesatus ().status () == GameStatus::SCHEDULED )
    {
         gsi.status  = "SCHEDULED";
    }
    else if (gsm.gamesatus ().status ()  == GameStatus::CLOSED)
    {
        gsi.status = "Closed";
    }
    else if (gsm.gamesatus ().status () == GameStatus::INGAME)
    {
      gsi.status = "InGame";
    }
    else if (gsm.gamesatus ().status () == GameStatus::PREGAME)
    {
        gsi.status = "PreGame";
    }
    else if (gsm.gamesatus ().status () == GameStatus::POSTGAME)
    {
        gsi.status = "PostGame";
    }

    gid2Game[gsi.gameid] = gsi;
   mGameStatusItem.append (gsi);
}

QList<ClosedGameItem> TempApi::getClosedGamesList(const std::string &gameresultmetaroot) {

    QList<ClosedGameItem> ret;
    MerkleTree gstree;
    ldb.read(gameresultmetaroot,gstree);
    ClosedGameItem cgi;
    GameResultsMeta grm;
//    qDebug() << grm.DebugString ().data ();
    for  (auto &gl : gstree.leaves()) {
        ldb.read(gl,grm);
        cgi.gameStatus = getGameItemFromStatusMeta(grm.gamestatusmetaid());
        cgi.gameid = cgi.gameStatus.gameid;
        cgi.homeRes = getTeamRes(grm.homeresultmeta());
        cgi.homeRes.gameid = cgi.gameid;
        cgi.homeRes.team = cgi.gameStatus.hometeam;
//        qDebug () << cgi.homeRes.team;
        cgi.awayRes = getTeamRes(grm.awayresultmeta());
        cgi.awayRes.gameid = cgi.gameid;
        cgi.awayRes.team = cgi.gameStatus.awayteam;

        ret.append(cgi);
    }

    return ret;
}

TeamGameResults TempApi::getTeamRes(const std::string &teamprojmeta) {
    TeamGameResults tm;
    TeamResultMeta tpm{};
    ldb.read(teamprojmeta,tpm);
//    qDebug() << tpm.DebugString().data();
    tm.playResults = getGamePlayerResult(tpm.playerresultmetaroot(),tpm.week());

    return tm;
}

QList<GamePlayerResultAwards> TempApi::getGamePlayerResult(const std::string &playerresultmetaroot,qint32 week) {
    MerkleTree mtree{};
    ldb.read(playerresultmetaroot,mtree);

    QList<GamePlayerResultAwards> lgpp;
    for ( auto &gppmid : mtree.leaves()) {
        PlayerResultMeta gppm{};
        fantasybit::PlayerGameStats pgm;
        GamePlayerResultAwards gpp;
        ldb.read(gppmid,gppm);
        ldb.read(gppm.playergamestatsid(),pgm);
        gpp.playerId = pgm.playerid().data();
        gpp.result = pgm.result();
        gpp.stats = pgm.stats();
        gppm.gamestatusmetaid ();
        gpp.fnameProjections = getAwardItems(gppm.awardmetaplayerroot(),gpp.playerId.toStdString(),
                                             week,gpp.result,pgm.gameid().data());
        gpp.fnamePositions = getPnlItems(gppm.pnlmetaplayerroot(),gpp.playerId.toStdString());
        lgpp.append(gpp);
    }

    return lgpp;
}

QList<AwardItem> TempApi::getAwardItems(const std::string &awardmetaplayerroot,
                                        const std::string &pid,
                                        qint32 week,
                                        qint32 result,
                                        QString gameId) {
    MerkleTree mtree{};
    ldb.read(awardmetaplayerroot,mtree);

    QList<AwardItem> projret{};
    AwardMeta pjm{};
    for ( auto nodestr : mtree.leaves()) {
        ldb.read(nodestr,pjm);
        AwardItem pi;
        pi.fantasyName = pjm.name().data();
        pi.playerId = pid.data();
        pi.projection = pjm.proj();
        pi.award = pjm.award();
        pi.result = result;
        pi.week = week;
        pi.gameId = gameId,
        pjm.playergamestatsid();
        projret.append(pi);
        m_awards.append(pi);
    }

    return projret;
}

QList<PnlItem> TempApi::getPnlItems(const std::string &pnlmetaplayerroot,
                                        const std::string &pid) {
    MerkleTree mtree{};
    ldb.read(pnlmetaplayerroot,mtree);

    QList<PnlItem> projret{};
    PnlMeta pjm{};
    for ( auto nodestr : mtree.leaves()) {
        ldb.read(nodestr,pjm);
        PnlItem pi;
        pi.fantasyName = pjm.name().data();
        pi.playerId = pid.data();
        pi.qty = pjm.qty();
        pi.price = pjm.price();
        pi.pnl = pjm.pnl();
        projret.append(pi);
    }

    return projret;
}

//QString TempApi::getPlayerInfoById(const QString id , TempApi::PlayerEnum playerEnum)
//{

//    QString m_ret;
//    for (auto &it : fantasybit::TempApi::mPlayers)
//    {
//        if ( it.playerId == id){
//            switch (playerEnum) {
//            case Name:
//                m_ret.append (it.firstName).append (" ").append (it.lastName);
//                break;
//            case  Pos:
//                m_ret = it.position;
//            break;
//            }
//        }
//    }
//    return m_ret;
//}

//QMap<QString,FantasyPlayer>TempApi::fname2Id{};'
QMap<QString,PlayerItem> TempApi::pid2Playeritem{};
QMap<QString,GameStatusItem> TempApi::gid2Game{};

QList<LeaderBoardItem>  TempApi::mLeaderBoard{};
QList<PlayerItem>  TempApi::mPlayers{};
QList<ProjItem>  TempApi::mProjections{};
QList<NextWeekItem> TempApi::mNextWeeks{};
QList<PrevWeekItem> TempApi::mPrevWeeks{};
QList<GameStatusItem> TempApi::mGameStatusItem{};

QList<PosItem>  TempApi::mPositions{};

//PlayerNewModel TempApi::m_pnm{};

CurrentWeek TempApi::mCurrentWeek{};

QList<OrderItem> TempApi::mOrders;
QList<PlayerMarketItem> TempApi::mPlayerMarkets;
QList<StatsItems>TempApi::m_statsItem;
QList<AwardItem>TempApi::m_awards;
QList<PnlItem> TempApi::m_pnlItem;
NewPlayerModel TempApi::mPlayerModel;

}




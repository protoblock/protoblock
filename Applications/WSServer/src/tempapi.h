#ifndef TEMPAPI_H
#define TEMPAPI_H
#include "StateData.pb.h"
//#include "leaderboardmodel.h"
#include "ldbhashreadertool.h"
#include "models/playernewmodel.h"

#include <QObject>
#include <QDebug>
#include <QDateTime>
#include <QTimeZone>
#include <QMap>
#include <QSettings>

#include "../QmlSuperMacros/QQmlPtrPropertyHelpers.h"

namespace fantasybit {
struct OrderFillItem {
    qint32 refnum;
    QString fname;
    QString playerid;
    bool isBuy;
    qint32 fillsize;
    qint32 fillprice;
    qint32 timestamp;
};


struct StatsItems{
    qint32 week;
    QString gameid;
    QString homeName;
    bool isHome;
    QString awayName;
    bool isAway;
    QString playerId;
    qint32 passtd;
    qint32 passyds ;
    qint32 rushtd  ;
    qint32 rushyds  ;
    qint32 rectd  ;
    qint32 recyds  ;
    qint32 rec  ;
    qint32 pint  ;
    qint32 fumble ;
    qint32 twopt ;
    qint32 fg;
    qint32 pa;
    qint32 ptsa;
    qint32 deftd;
    qint32 sacks;
    qint32 turnovers;
    qint32 sfty;
    qint32 dtwopt;
};





struct MarketTicItem {
    qint32 size;
    qint32 price;
    OrderFillItem lastFill;
};

struct OrderItem {
    qint32 refnum;
    QString fname;
    QString playerid;
    bool isBuy;
    qint32 size;
    qint32 price;
    qint32 timestamp;
    QList<OrderFillItem> orderFills;
};

struct LimitBookItem {
    qint32 bidsize;
    qint32 bid;
    qint32 ask;
    qint32 asksize;
    QList<OrderItem> bidorders;
    QList<OrderItem> askorders;
};

struct PlayerMarketItem {
    QString playerid;
    QList<LimitBookItem> limitbook;
    QList<MarketTicItem> tics;
};

/*!
 * \brief The LeaderBoardItem struct
 */
struct LeaderBoardItem {
    QString fantasyName;
    QString publicKey;
//    qint64 nameHash;
    qint64 stakeBalance;
    quint64 skillBalance;
};


/*!
 * \brief The PlayerItem struct
 */
struct PlayerItem {
   enum PlayerStatus {ACTIVE, INACTIVE};
   QString firstName;
   QString lastName;
   QString position;
   QString team;
   QString playerStatus;
   QString playerId;
};


/*!
 * \brief The ProjItem struct
 */
struct ProjItem {
   enum PlayerStatus {ACTIVE, INACTIVE};
   QString fantasyName;
   QString playerId;
   qint32 projection;
};

/*!
 * \brief The ProjItem struct
 */
struct AwardItem {
   enum PlayerStatus {ACTIVE, INACTIVE};
   QString fantasyName;
   QString playerId;
   qint32 projection;
   qint32 award;
   qint32 result;
   int week;
   QString gameId;
};



struct GameStatusItem {
    QString gameid;
    qint32  week;
    QString hometeam;
    QString awayteam;
    QString  time;
    QString status;
    qint32 eTime;
};


/*!
 * \brief The ScheduledGamesItem struct contains data for
 *          upcoming scheduled games:
 */
struct ScheduledGamesItem  {
    GameStatusItem gameStatus;
};




/*!
 * \brief The PosItem struct
 */
struct PosItem {
   enum PlayerStatus {ACTIVE, INACTIVE};
   QString fantasyName;
   QString playerId;
   qint32 qty;
   qint32 price;
};

/*!
 * \brief The ProjItem struct
 */
struct PnlItem {
   enum PlayerStatus {ACTIVE, INACTIVE};
   QString fantasyName;
   QString playerId;
   qint32 qty;
   qint32 price;
   qint32 pnl;
   int week;
   QString gameId;
};


struct GamePlayerProjPos {
    QString playerId;
//    GameStatusItem gameStatus;
    QList<ProjItem> fnameProjections;
    QList<PosItem> fnamePositions;
};

struct GamePlayerResultAwards {
    QString playerId;
    float  result;
    Stats   stats;
    QList<AwardItem> fnameProjections;
    QList<PnlItem> fnamePositions;
};


struct TeamProjPos {
    QString gameid;
    QString team;
    qint32 week;
    QList<GamePlayerProjPos> playProjPos;
};

struct TeamGameResults {
    QString gameid;
    QString team;
    qint32 week;
    QList<GamePlayerResultAwards> playResults;
};


/*!
 * \brief The LockedGameItems struct contains data for
 *          games that have started.
 */
struct LockedGameItem {
    QString gameid;
    GameStatusItem gameStatus;
    TeamProjPos homeProj;
    TeamProjPos awayProj;
};

/*!
 * \brief The ClosedGameItems struct contains data for games
 *          that have results and are closed
 */
struct ClosedGameItem {
    QString gameid;
    GameStatusItem gameStatus;
    TeamGameResults homeRes;
    TeamGameResults awayRes;
};

/*!
 * \brief The LockedGameItems struct contains data for
 *          games that have started.
 */
struct PrevWeekItem {
    int week;
    QList<ClosedGameItem> closedGames;
};

struct NextWeekItem {
    int week;
    QList<ScheduledGamesItem> openGames;
};

struct CurrentWeek {
    int week;
    QList<ScheduledGamesItem> openGames;
    QList<LockedGameItem> lockedGames;
    QList<ClosedGameItem> closedGames;
};





/////////////////////////////
/// \brief The TempApi class
///
///
///

class TempApi : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int season READ season)
    Q_PROPERTY(int week READ week)
    Q_PROPERTY(QString globalstate READ globalstate)
    Q_PROPERTY(QString fnamebalrootid READ fnamebalrootid)
    Q_PROPERTY(QString dString READ dString)
    Q_ENUMS(PlayerInfoEnum)

    QML_READONLY_PTR_PROPERTY(PlayerNewModel, pnm)
//    Q_PROPERTY(PlayerNewModel * pnm READ pnm NOTIFY pnmChanged)

public:
    explicit TempApi(QObject *parent = 0);

    enum PlayerInfoEnum{ Name, Pos, Number, };
    static QString fromTime_t_toFantasyString(uint dtt);


//##FIXME add beging and end for the settings
#ifdef Q_OS_WIN32
    Q_INVOKABLE QString lastKnowLoc()
    {
        QSettings settings("HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\{79913283-A35B-4274-927C-1B52D286D939}_is1", QSettings::NativeFormat);
        return settings.value( "InstallLocation" ).toString();

    }
#endif

    //PlayerNewModel *pnm();
    Q_INVOKABLE QString getFantasyIdByName(const QString fName);
    Q_INVOKABLE QString getPlayerNameById(const QString playerId);
    Q_INVOKABLE QString getPlayerPostionById(const QString playerId);
    Q_INVOKABLE QString getTeamsByGameId(const QString gameId);

    int season() const;
    int week() const;
    QString globalstate() const;
    QString fnamebalrootid() const;

    QString dString() const;
    void initState(const std::string &stateid );


    // MODELS
    static NewPlayerModel mPlayerModel;
    static QList<LeaderBoardItem> mLeaderBoard;
    static QList<PlayerItem> mPlayers;
    static QList<ProjItem> mProjections;
    static QList<NextWeekItem> mNextWeeks;
    static QList<PrevWeekItem> mPrevWeeks;
    static QList<GameStatusItem> mGameStatusItem;
    static QList<PosItem>  mPositions;
    static QList<StatsItems> m_statsItem;
    static QList<AwardItem>m_awards;
    static QList<PnlItem>m_pnlItem;
    PlayerNewModel m_PlayerNewModel;
//    static QMap<QString,FantasyPlayer>fname2Id;
    static QMap<QString,PlayerItem> pid2Playeritem;
    static QMap<QString,GameStatusItem> gid2Game;

    static CurrentWeek mCurrentWeek;
    static QList<OrderItem> mOrders;
    static QList<PlayerMarketItem> mPlayerMarkets;
    void setGameItem(const GameStatusMeta &gsm, GameStatusItem &o_gsi);
    QList<LockedGameItem> getGamesList(const std::string &ingameprojmetaroot);
    GameStatusItem getGameItemFromStatusMeta(const std::string &gamestatusmetaid);
    QList<ClosedGameItem> getClosedGamesList(const std::string &gameresultmetaroot);
    QList<ScheduledGamesItem> getOpenGamesList(const std::string &opengamestatusroot);
    TeamProjPos getTeamProj(const std::string &);

    QList<ProjItem> getProjItems(const std::string &projmetaplayerroot);
    QList<GamePlayerProjPos> getGamePlayerProj(const std::string &gameplayerprojmetaroot);
    TeamGameResults getTeamRes(const std::string &teamprojmeta);
    QList<GamePlayerResultAwards> getGamePlayerResult(const std::string &playerresultmetaroot,qint32);
//    QList<AwardItem> getAwardItems(const std::string &awardmetaplayerroot);
    QList<AwardItem> getAwardItems(const std::string &awardmetaplayerroot, const std::string &pid,qint32,qint32,QString);
    QList<PosItem> getPosItems(const std::string &posmetaplayerroot);
    QList<PnlItem> getPnlItems(const std::string &pnlmetaplayerroot, const std::string &pid);
    OrderItem getOrderItem(const OrderMeta &om);
    QList<MarketTicItem> getMarketTics(const std::string &marketticmetaid);
    QList<LimitBookItem> getMarketLimitBook(const std::string &limitbookmetaid);
    QList<OrderItem> getOrders(const MerkleTree &tree);
    OrderFillItem getOrderFillItem(const OrderFillMeta &om);
    OrderFillItem getOrderFillItem(const std::string &ofillsroot);
    QList<OrderFillItem> getOrderFillItems(const QString &ofillsroot);
//    QString getPlayerInfoById(const QString id, TempApi::PlayerEnum playerEnum);


signals:
    void open();



private:
    LdbHashReaderTool& ldb;
    pbstate m_pbstate;
    GlobalStateMeta m_globalstatemeta;
    LeaderBoardMeta m_leaderboardmeta;
    MerkleTree m_projectionstatetree;
    MerkleTree m_positionsstatetree;
    MerkleTree m_marketstatetree;

    MerkleTree m_orderstatetree;

    MerkleTree m_teamstatetree;
    MerkleTree m_weekgamestatusmetatree;


    void initLeaderboard();
    void initPlayers();
    void initProjections();
    void initWeeklyGames();
    void initMarket();
    void initOrders();
    void initPositions();
    void initStats();
    void setAllStats();



};
}

#endif // TEMPAPI_H

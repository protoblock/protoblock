#include "trading.h"
#include "ui_trading.h"
#include "ProtoData.pb.h"
#include "core.h"
#include "Commissioner.h"
#include "RestFullCall.h"
#include "dataservice.h"

using namespace fantasybit;

Trading::Trading(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Trading)
{
    ui->setupUi(this);

    ui->depthView->setModel(&mDepthTableModel);

    ui->playerList->setModel(&mPlayerListModel);
}

void Trading::Init() {

    theLAPIWorker = Core::resolveByName<MainLAPIWorker>("coreapi");

    if (theLAPIWorker!=NULL) {
       ExchangeData *exchangedata = &(theLAPIWorker->ExData());
       QObject::connect(theLAPIWorker,SIGNAL(LiveData(bool)),this,SLOT(OnLive(bool)));
       QObject::connect(theLAPIWorker,SIGNAL(LiveData(bool)),exchangedata,SLOT(OnLive(bool)));

       QObject::connect(exchangedata,SIGNAL(NewMarketTicker(fantasybit::MarketTicker *)),
                         this,SLOT(OnMarketTicker(fantasybit::MarketTicker *)));
       QObject::connect(exchangedata,SIGNAL(NewMarketSnapShot(fantasybit::MarketSnapshot*)),
                         this,SLOT(OnMarketSnapShot(fantasybit::MarketSnapshot*)));

       QObject::connect(exchangedata,SIGNAL(NewDepthDelta(fantasybit::DepthFeedDelta*)),
                         this,SLOT(OnDepthDelta(fantasybit::DepthFeedDelta*)));



    }

    QObject::connect(ui->playerList,SIGNAL(doubleClicked(QModelIndex)),
                     this,SLOT(playerListCliked(QModelIndex)));



/*
    ui->playerid->addItem("1097");
    ui->playerid->addItem("1412");
    ui->playerid->addItem("1822");
    ui->playerid->addItem("1808");
    ui->playerid->addItem("1792");
    ui->playerid->addItem("1772");
    ui->playerid->addItem("1755");
    ui->playerid->addItem("1715");
    ui->playerid->addItem("1");
    ui->playerid->addItem("2");
    ui->playerid->addItem("3");
    ui->playerid->addItem("5");
    ui->playerid->addItem("4");
    */

}

Trading::~Trading()
{
    delete ui;
}

void Trading::playerListCliked(const QModelIndex &index) {
    ViewModel * data = mPlayerListModel.getItemByIndex(index);
    if (data !=NULL) {
        QString playerid = data->propertyValue<QString,PropertyNames::Player_ID>();
        mDepthTableModel.changeSymbol(playerid.toStdString());
        ui->playername->setText(data->propertyValue<QString,PropertyNames::Player_Name>());
        ui->position->setText(data->propertyValue<QString,PropertyNames::Position>());
        ui->teamicon->setTextFormat(Qt::RichText);
        QString team = data->propertyValue<QString,PropertyNames::Team_ID>();
        ui->teamicon->setText("<img src=" + QString::fromStdString(Trading::icons[team.toStdString()]) +">");
        myPlayerid = playerid.toStdString();

    }
}


void Trading::on_buyit_clicked()
{
    NewOrder(true);
}

void Trading::on_sellit_clicked()
{
    NewOrder(false);
}

void Trading::SetCurrentWeekData(int week) {

#ifdef TRACE
    qDebug() << "SetCurrentWeekData level2 ExchangeData OnLive" << week;
#endif

    if ( week == mCurrentWeek ) {
        qWarning() << "already have this week";
        return;
    }
    if ( mCurrentWeek > week ) {
        qCritical() << " bad week" << week << mCurrentWeek;
        return;
    }
    if ( mCurrentWeek > 0 ) {
        qInfo() << " changing week";
        //OnWeekChange();
    }
    mCurrentWeek = week;

    auto myGameRosters = DataService::instance()->GetCurrentWeekGameRosters();

   // QIcon icon(QPixmap(":/NFL/scored.png");)
   // icon.addFile();
    //ui->gamesCombo->insertItem(1,)
    //fill game status
    int i =0;
    int lasttime = 0;
    for(const fantasybit::GameRoster & game  : myGameRosters) {
        QString gameId = game.info.id().data();
        QString text = QString("%1 @ %2");
        QString myt = text.arg(game.info.away().data())
                .arg(game.info.home().data()).leftJustified(9,' ');
        auto ret = myt + QString(" - ")+ fromTime_t_toFantasyString(game.info.time());

        ui->gamesCombo->insertItem(i++,ret,gameId);

        for ( auto hh : { QString("home"), QString("away")} ) {
            for ( auto ho : (hh == "home") ? game.homeroster : game.awayroster ) {
                QString playerId = ho.first.data();
                auto playerDetails = ho.second;
                QString playerName;
                playerName = QString(playerDetails.base.first().data()) + " " + QString(playerDetails.base.last().data());


                mPlayerListModel.updateItemProperty<PropertyNames::Player_ID>(ho.first.data(),ho.first.data());
                mPlayerListModel.updateItemProperty<PropertyNames::Team_ID>(playerId,game.info.home().data());
                mPlayerListModel.updateItemProperty<PropertyNames::Player_Name>(playerId,playerName);
                mPlayerListModel.updateItemProperty<PropertyNames::Position>(playerId,playerDetails.base.position().data());

                mPlayerListModel.updateItemProperty<PropertyNames::Player_Status>(playerId,
                                                  qVariantFromValue<PlayerStatus_Status>(playerDetails.team_status));
            }
        }
    }
        /*
        if ( lasttime != game.info.time()) {
            ui->gameList->addItem(fromTime_t_toFantasyString(game.info.time()));
            lasttime = game.info.time();
        }
        QString gamev = "<img src=" +
                QString::fromStdString(Trading::icons[game.info.away()]) +">"
                + "@" +
                "<img src=" + QString::fromStdString(Trading::icons[game.info.home()]) +">";
        ui->gameList->addItem(gamev);

        ui->gameList->insertItem();
        setItemWidget
*/
        /*
        // add game
        myGameTableModel.updateItemProperty<PropertyNames::Game_ID>(gameId,gameId);
        myGameTableModel.updateItemProperty<PropertyNames::Away>(gameId,game.info.away().data());
        myGameTableModel.updateItemProperty<PropertyNames::Home>(gameId,game.info.home().data());
        myGameTableModel.updateItemProperty<PropertyNames::Game_Time>(gameId,fromTime_t_toFantasyString(game.info.time()));
        myGameTableModel.updateItemProperty<PropertyNames::Game_Status>(gameId,qVariantFromValue<GameStatus_Status>(game.status));
        //add home players
        for(const auto& player : game.homeroster) {
            QString playerId = player.first.data();
            PlayerDetail playerDetails = player.second;
            QString playerName;
            playerName = QString(playerDetails.base.first().data()) + " " + QString(playerDetails.base.last().data());
            myProjectionsModel.updateItemProperty<PropertyNames::Game_ID>(playerId,gameId);
            myProjectionsModel.updateItemProperty<PropertyNames::Player_ID>(playerId,playerId);
            myProjectionsModel.updateItemProperty<PropertyNames::Team_ID>(playerId,game.info.home().data());
            myProjectionsModel.updateItemProperty<PropertyNames::Player_Name>(playerId,playerName);
            myProjectionsModel.updateItemProperty<PropertyNames::Position>(playerId,playerDetails.base.position().data());

            myProjectionsModel.updateItemProperty<PropertyNames::Player_Status>(playerId,
                                              qVariantFromValue<PlayerStatus_Status>(playerDetails.team_status));
            myProjectionsModel.updateItemProperty<PropertyNames::Player_Game_Status>(playerId,
                                              qVariantFromValue<PlayerGameStatus>(playerDetails.game_status));
            myProjectionsModel.updateItemProperty<PropertyNames::Projection>(playerId, 0);
            myProjectionsModel.updateItemProperty<PropertyNames::ProjectionStatus>(playerId,QVariant::fromValue(ScoreState::NonScored));
            myProjectionsModel.updateItemProperty<PropertyNames::KnownProjection>(playerId,-1);

        }

        //add away players
        for(const auto& player : game.awayroster) {
            QString playerId = player.first.data();
            PlayerDetail playerDetails = player.second;
            QString playerName;
            playerName = QString(playerDetails.base.first().data()) + " " + QString(playerDetails.base.last().data());
            myProjectionsModel.updateItemProperty<PropertyNames::Game_ID>(playerId,gameId);
            myProjectionsModel.updateItemProperty<PropertyNames::Player_ID>(playerId,playerId);
            myProjectionsModel.updateItemProperty<PropertyNames::Team_ID>(playerId,game.info.away().data());
            myProjectionsModel.updateItemProperty<PropertyNames::Player_Name>(playerId,playerName);
            myProjectionsModel.updateItemProperty<PropertyNames::Position>(playerId,playerDetails.base.position().data());
            myProjectionsModel.updateItemProperty<PropertyNames::Player_Status>(playerId,
                                              qVariantFromValue<PlayerStatus_Status>(playerDetails.team_status));
            myProjectionsModel.updateItemProperty<PropertyNames::Player_Game_Status>(playerId,
                                                                                     qVariantFromValue<PlayerGameStatus>(playerDetails.game_status));
            myProjectionsModel.updateItemProperty<PropertyNames::Projection>(playerId,0);
            myProjectionsModel.updateItemProperty<PropertyNames::ProjectionStatus>(playerId,QVariant::fromValue(ScoreState::NonScored));
            myProjectionsModel.updateItemProperty<PropertyNames::KnownProjection>(playerId,-1);

        }
        */


    /*
    QString ii = "<img src=" + QString::fromStdString(Trading::icons["NYJ"]) +">";

    ui->gamesCombo->insertItem(i++,ii,"gameId");


    string home ="NYJ";
    string away = "NYG";
    ui->awayicon->setTextFormat(Qt::RichText);
    ui->awayicon->setText("<img src=" + QString::fromStdString(Trading::icons[away]) +">");
    ui->homeicon->setText("<img src=" + QString::fromStdString(Trading::icons[home]) +">");
    */
}

void Trading::NewOrder(bool isbuy) {

    eo.set_playerid(myPlayerid);
    eo.set_type(ExchangeOrder::NEW);

    OrderCore core;
    core.set_buyside(isbuy);
    core.set_size(ui->qty->value());
    core.set_price(ui->price->value());

    eo.mutable_core()->CopyFrom(core);
    //emit SendOrder(eo);

    Transaction trans{};
    trans.set_version(Commissioner::TRANS_VERSION);
    trans.set_type(TransType::EXCHANGE);
    trans.MutableExtension(ExchangeOrder::exchange_order)->CopyFrom(eo);

    SignedTransaction st = Core::resolveByName<MainLAPIWorker>("coreapi")
            ->Agent().makeSigned(trans);

    auto txstr = st.SerializeAsString();
    RestfullClient rest(QUrl(PAPIURL.data()));
    //rest.postRawData("tx","octet-stream",txstr.data(),((size_t)txstr.size()));
    rest.postRawData("trade","octet-stream",txstr.data(),((size_t)txstr.size()));
}

void Trading::OnMarketTicker(fantasybit::MarketTicker* mt) {
#ifdef TRACE
    qDebug() << "level2 OnMarketTicker " << mt->DebugString();
#endif
/*
    if ( mt->symbol() == "" ) return;
    qString playerid = mt->symbol().data();
    mPlayerListModel.updateItemProperty<PropertyNames::Player_ID>(playerid,playerid);
    mPlayerListModel.updateItemProperty<PropertyNames::BIDSIZE>(playerid, mq.bs());
    mPlayerListModel.updateItemProperty<PropertyNames::BID>(playerid,mq.b());
    mPlayerListModel.updateItemProperty<PropertyNames::ASK>(playerid,mq.a());
    mPlayerListModel.updateItemProperty<PropertyNames::ASKSIZE>(playerid,mq.as());
    mPlayerListModel.updateItemProperty<PropertyNames::LAST>(playerid,mq.l());
    mPlayerListModel.updateItemProperty<PropertyNames::LASTSIZE>(playerid,mq.ls());
    */

}

void Trading::OnMarketSnapShot(fantasybit::MarketSnapshot* mt) {
    if ( mt->symbol() == "" )
        return;
#ifdef TRACE
    qDebug() << "level2 OnMarketSnapShot " << mt->DebugString();
#endif

    if ( mt->week() != mCurrentWeek )
        SetCurrentWeekData(mt->week());

    mDepthTableModel.addSnap(mt);
    if ( mt->has_quote()) {
        //ContractOHLC &ohlc = mt->ohlc();
        QString playerid = mt->symbol().data();
        const MarketQuote &mq = mt->quote();
        mPlayerListModel.updateItemProperty<PropertyNames::Player_ID>(playerid,playerid);
        mPlayerListModel.updateItemProperty<PropertyNames::BIDSIZE>(playerid, mq.bs());
        mPlayerListModel.updateItemProperty<PropertyNames::BID>(playerid,mq.b());
        mPlayerListModel.updateItemProperty<PropertyNames::ASK>(playerid,mq.a());
        mPlayerListModel.updateItemProperty<PropertyNames::ASKSIZE>(playerid,mq.as());
        mPlayerListModel.updateItemProperty<PropertyNames::LAST>(playerid,mq.l());
        mPlayerListModel.updateItemProperty<PropertyNames::LASTSIZE>(playerid,mq.ls());
    }
}

void Trading::OnDepthDelta(fantasybit::DepthFeedDelta* df) {
    if ( df->symbol() == "" )
        return;
#ifdef TRACE
    qDebug() << "level2 OnDepthDelta " << df->DebugString();
#endif
    mDepthTableModel.onDelta(df);
}


decltype(Trading::icons) Trading::icons{
    {"SF",":/NFL/png/49ers.png"},
    {"CHI",":/NFL/png/Bears.png"},
    {"CIN",":/NFL/png/Bengels.png"},
    {"BUF",":/NFL/png/Bills.png"},
    {"DEN",":/NFL/png/Broncos.png"},
    {"CLE",":/NFL/png/Browns.png"},
    {"TB",":/NFL/png/Buccaneers.png"},
    {"ARI",":/NFL/png/Cardinals.png"},
    {"SD",":/NFL/png/Chargers.png"},
    {"KC",":/NFL/png/Chiefs.png"},
    {"IND",":/NFL/png/Colts.png"},
    {"DAL",":/NFL/png/Cowboys.png"},
    {"MIA",":/NFL/png/Dolphins.png"},
    {"PHI",":/NFL/png/Eagles.png"},
    {"ATL",":/NFL/png/Falcons.png"},
    {"NYG",":/NFL/png/Giants.png"},
    {"JAC",":/NFL/png/Jaguar.png"},
    {"NYJ",":/NFL/png/Jets.png"},
    {"DET",":/NFL/png/Lions.png"},
    {"GB",":/NFL/png/Packers.png"},
    {"CAR",":/NFL/png/Panthers.png"},
    {"NE",":/NFL/png/Patriots.png"},
    {"OAK",":/NFL/png/Raiders.png"},
    {"STL",":/NFL/png/Rams.png"},
    {"BAL",":/NFL/png/Ravens.png"},
    {"WAS",":/NFL/png/Redskins.png"},
    {"NO",":/NFL/png/Saints.png"},
    {"SEA",":/NFL/png/Seahawks.png"},
    {"PIT",":/NFL/png/Steelers.png"},
    {"HOU",":/NFL/png/Texans.png"},
    {"TEN",":/NFL/png/Titans.png"},
    {"MIN",":/NFL/png/Vikings.png"},
};

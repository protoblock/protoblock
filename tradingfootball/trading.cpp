#include "trading.h"
#include "ui_trading.h"
#include "ProtoData.pb.h"
#include "core.h"
#include "Commissioner.h"
#include "RestFullCall.h"
#include "dataservice.h"
#include "julylightchanges.h"

using namespace fantasybit;

Trading::Trading(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Trading)
{
    ui->setupUi(this);

    ui->depthView->setModel(&mDepthTableModel);

    ui->playerList->setModel(&mPlayerListModel);

    ui->ordersTable->setModel(&mOrderTableModel);
    myFantasyName = "";

    mJLC.push_back(new JulyLightChanges(ui->marketBid));
    mJLC.push_back(new JulyLightChanges(ui->marketAsk));
    //mJLC.push_back(new JulyLightChanges(ui->marketBids));
    //mJLC.push_back(new JulyLightChanges(ui->marketAsks));
    //mJLC.push_back(new JulyLightChanges(ui->marketVolume));
    //mJLC.push_back(new JulyLightChanges(ui->marketHigh));
    //mJLC.push_back(new JulyLightChanges(ui->marketLow));
    mJLC.push_back(new JulyLightChanges(ui->marketLast));
    mJLC.push_back(new JulyLightChanges(ui->marketChng,true));
    mJLC.push_back(new JulyLightChanges(ui->posOpenPnl, true));
    //mJLC.push_back(new JulyLightChanges(ui->posQty));
    //mJLC.push_back(new JulyLightChanges(ui->marketLast));
    mJLC.push_back(new JulyLightChanges(ui->fantasybitStake));
    mJLC.push_back(new JulyLightChanges(ui->fantasybitPnl, true));


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


       QObject::connect(exchangedata,SIGNAL(NewTradeTic(fantasybit::TradeTic*)),
                         this,SLOT(OnTradeTick(fantasybit::TradeTic*)));

       /*
       FantasyNameData *namedata = &(theLAPIWorker->NameData());
       QObject::connect(namedata,SIGNAL(NewFantasyNameOrder(fantasybit::Order&)),
                         this,SLOT(OnMyNewOrder(fantasybit::Order&)));
        */

       QObject::connect(exchangedata,SIGNAL(NewFantasyNameOrder(fantasybit::Order&)),
                         this,SLOT(OnMyNewOrder(fantasybit::Order&)));

    }

    QObject::connect(ui->playerList,SIGNAL(doubleClicked(QModelIndex)),
                     this,SLOT(playerListCliked(QModelIndex)));

    QObject::connect(ui->buyqty, SIGNAL(valueChanged(int)),
                     this, SLOT(UpdateBuys(int)));

    QObject::connect(ui->buyprice, SIGNAL(valueChanged(int)),
                     this, SLOT(UpdateBuys(int)));

    QObject::connect(ui->buyCeil, SIGNAL(valueChanged(int)),
                     this, SLOT(UpdateBuys(int)));

    QObject::connect(ui->buyFloor, SIGNAL(valueChanged(int)),
                     this, SLOT(UpdateBuys(int)));

    QObject::connect(ui->sellqty, SIGNAL(valueChanged(int)),
                     this, SLOT(UpdateSells(int)));

    QObject::connect(ui->sellprice, SIGNAL(valueChanged(int)),
                     this, SLOT(UpdateSells(int)));

    QObject::connect(ui->sellCeil, SIGNAL(valueChanged(int)),
                     this, SLOT(UpdateSells(int)));

    QObject::connect(ui->sellFloor, SIGNAL(valueChanged(int)),
                     this, SLOT(UpdateSells(int)));

}

Trading::~Trading()
{
    delete ui;
}

void Trading::playerListCliked(const QModelIndex &index) {
    ViewModel * data = mPlayerListModel.getItemByIndex(index);
    if (data !=NULL) {
        QString playerid = data->propertyValue<QString,PropertyNames::Player_ID>();
        if ( myPlayerid == playerid.toStdString())
            return;

        mUpdatingS = mUpdatingB = true;
        mDepthTableModel.changeSymbol(playerid.toStdString());
        ui->playername->setText(data->propertyValue<QString,PropertyNames::Player_Name>());
        ui->position->setText(data->propertyValue<QString,PropertyNames::Position>());
        ui->teamicon->setTextFormat(Qt::RichText);
        QString team = data->propertyValue<QString,PropertyNames::Team_ID>();
        ui->teamicon->setText("<img src=" + QString::fromStdString(Trading::icons[team.toStdString()]) +">");

        ui->marketLast->setValue(data->propertyValue<QString,PropertyNames::LAST>().toInt());
        ui->marketBid->setValue(data->propertyValue<QString,PropertyNames::BID>().toInt());
        ui->marketAsk->setValue(data->propertyValue<QString,PropertyNames::ASK>().toInt());
        ui->marketChng->setValue(data->propertyValue<QString,PropertyNames::CHANGE>().toInt());
        ui->marketHigh->setValue(data->propertyValue<QString,PropertyNames::HIGH>().toInt());
        ui->marketLow->setValue(data->propertyValue<QString,PropertyNames::LOW>().toInt());
        ui->marketVolume->setValue(data->propertyValue<QString,PropertyNames::VOLUME>().toInt());
        ui->marketBids->setValue(data->propertyValue<QString,PropertyNames::BIDSIZE>().toInt());
        ui->marketAsks->setValue(data->propertyValue<QString,PropertyNames::ASKSIZE>().toInt());
        ui->buyprice->setValue(1);
        ui->buyqty->setValue(1);
        ui->sellprice->setValue(40);
        ui->sellqty->setValue(1);
        ui->buyCeil->setValue(40);
        ui->sellCeil->setValue(40);
        ui->buyFloor->setValue(0);
        ui->sellFloor->setValue(0);
        mUpdatingS = mUpdatingB = false;
        UpdateSells(0);
        UpdateBuys(0);

        myPlayerid = playerid.toStdString();


    }
}

void Trading::SetFantasyName(std::string name) {
#ifdef TRACE
    qDebug() << "level2 trading SetFantasyName" << name;
#endif

    if ( name != myFantasyName ) {
        ui->fantastname->setText(name.data());
        myFantasyName = name;
        SetMyPositions();
    }
}


void Trading::on_buyit_clicked()
{
#ifdef TRACE
    qDebug() << "level2 on_buyit_clicked";
#endif

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

        ui->playerList->resizeColumnsToContents();
        ui->playerList->resizeRowsToContents();
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
    core.set_size(isbuy ? ui->buyqty->value() : ui->sellqty->value());
    core.set_price(isbuy ? ui->buyprice->value() : ui->sellprice->value());

#ifdef TRACE
    qDebug() << "level2 NewOrder " << core.DebugString();
#endif

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
    if ( mt->symbol() == "" ) return;
    QString playerid = mt->symbol().data();
    mPlayerListModel.updateItemProperty<PropertyNames::Player_ID>(playerid,playerid);
    if ( mt->type() == MarketTicker_Type_BID) {
        mPlayerListModel.updateItemProperty<PropertyNames::BIDSIZE>(playerid, mt->size());
        mPlayerListModel.updateItemProperty<PropertyNames::BID>(playerid,mt->price());
    }
    else if ( mt->type() == MarketTicker_Type_ASK){
        mPlayerListModel.updateItemProperty<PropertyNames::ASK>(playerid,mt->price());
        mPlayerListModel.updateItemProperty<PropertyNames::ASKSIZE>(playerid,mt->size());
    }
}


void Trading::OnTradeTick(fantasybit::TradeTic* tt) {
#ifdef TRACE
    qDebug() << "level2 OnTradeTick " << tt->DebugString();
#endif
    if ( tt->symbol() == "" ) return;
    QString playerid = tt->symbol().data();
    mPlayerListModel.updateItemProperty<PropertyNames::Player_ID>(playerid,playerid);
    mPlayerListModel.updateItemProperty<PropertyNames::LAST>(playerid,tt->price());
    mPlayerListModel.updateItemProperty<PropertyNames::LASTSIZE>(playerid,tt->size());
    mPlayerListModel.updateItemProperty<PropertyNames::CHANGE>(playerid,tt->change());
    ViewModel * item = mPlayerListModel.itemByKey(playerid);
    int volume = tt->size() + item->propertyValue<PropertyNames::VOLUME>().toInt();

    mPlayerListModel.updateItemProperty<PropertyNames::VOLUME>(playerid,volume);
    if ( tt->ishigh())
        mPlayerListModel.updateItemProperty<PropertyNames::HIGH>(playerid, tt->price());

    if ( tt->islow())
        mPlayerListModel.updateItemProperty<PropertyNames::LOW>(playerid, tt->price());
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
    QString playerid = mt->symbol().data();
    mPlayerListModel.updateItemProperty<PropertyNames::Player_ID>(playerid,playerid);

    if ( mt->has_quote()) {
        //ContractOHLC &ohlc = mt->ohlc();
        const MarketQuote &mq = mt->quote();
        if ( mq.has_bs())
            mPlayerListModel.updateItemProperty<PropertyNames::BIDSIZE>(playerid, mq.bs());
        if ( mq.has_b())
            mPlayerListModel.updateItemProperty<PropertyNames::BID>(playerid,mq.b());
        if ( mq.has_a())
            mPlayerListModel.updateItemProperty<PropertyNames::ASK>(playerid,mq.a());
        if ( mq.has_as())
            mPlayerListModel.updateItemProperty<PropertyNames::ASKSIZE>(playerid,mq.as());
        if ( mq.has_l())
            mPlayerListModel.updateItemProperty<PropertyNames::LAST>(playerid,mq.l());
        if ( mq.has_ls())
            mPlayerListModel.updateItemProperty<PropertyNames::LASTSIZE>(playerid,mq.ls());

    }

    if ( mt->has_ohlc()) {
        const ContractOHLC &ohlc = mt->ohlc();

        mPlayerListModel.updateItemProperty<PropertyNames::HIGH>(playerid,ohlc.high());
        mPlayerListModel.updateItemProperty<PropertyNames::LOW>(playerid,ohlc.low());
        mPlayerListModel.updateItemProperty<PropertyNames::VOLUME>(playerid,ohlc.volume());
        mPlayerListModel.updateItemProperty<PropertyNames::CHANGE>(playerid,ohlc.change());
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

void Trading::UpdateBuys(int p) {
    if ( mUpdatingB ) return;
    mUpdatingB = true;
    ui->buyvalue->setValue(ui->buyprice->value() * 100 * ui->buyqty->value());
    ui->buyRisk->setValue((ui->buyFloor->value() - ui->buyprice->value()) * 100 * ui->buyqty->value());
    ui->buyProfit->setValue((ui->buyCeil->value() - ui->buyprice->value()) * 100 * ui->buyqty->value());
    mUpdatingB = false;
}

void Trading::UpdateSells(int p) {
    if ( mUpdatingS ) return;
    mUpdatingS = true;
    ui->sellvalue->setValue(ui->sellprice->value() * -100 * ui->sellqty->value());
    ui->sellProfit->setValue((ui->sellprice->value() - ui->sellFloor->value()) * 100 * ui->sellqty->value());
    ui->sellRisk->setValue(( ui->sellprice->value() - ui->sellCeil->value()) * 100 * ui->sellqty->value());
    mUpdatingS = false;
}

void Trading::on_buyPriceAsMarketAsk_clicked() {
    ui->buyprice->setValue(ui->marketAsk->value());
    ui->buyqty->setValue(ui->marketAsks->value());
}

void Trading::on_buyPriceAsMarketBid_clicked()
{
    ui->buyprice->setValue(ui->marketBid->value());
    ui->buyqty->setValue(ui->marketBids->value());
}

void Trading::on_buyPriceAsMarketLastPrice_clicked()
{
    ui->buyprice->setValue(ui->marketLast->value());
    //ui->buyqty->setValue(ui->marketLast->value());
}

void Trading::on_sellPriceAsMarketLastPrice_clicked()
{
    ui->sellprice->setValue(ui->marketLast->value());
}

void Trading::on_sellPriceAsMarketAsk_clicked()
{
    ui->sellprice->setValue(ui->marketAsk->value());
    ui->sellqty->setValue(ui->marketAsks->value());
}

void Trading::on_sellPriceAsMarketBid_clicked()
{
    ui->sellprice->setValue(ui->marketBid->value());
    ui->sellqty->setValue(ui->marketBids->value());
}

//typedef std::unordered_map<std::string,std::pair<Position,std::vector<Order> > > ordsnap_t;

void Trading::SetMyPositions() {
    myPositionsName = myFantasyName;
    auto myorderpositions = DataService::instance()->GetOrdersPositionsByName(myPositionsName);

#ifdef TRACE
    qDebug() << "level2 Trading SetMyPositions" << myPositionsName << myorderpositions.size();
#endif
    for ( auto p : myorderpositions ) {
      //  qDebug() << "level2 Trading SetMyPositions" << p.first << p.second;
        auto &mypair = p.second;
        auto &myorders = mypair.second;
        for ( auto o : myorders) {
            mOrderTableModel.updateItemProperty<PropertyNames::ORDERID>(o.refnum(),o.refnum());
            QString buyorsell(o.core().buyside() ? "Bid" : "Ask");
            mOrderTableModel.updateItemProperty<PropertyNames::BUYORSELL>(o.refnum(),buyorsell);
            mOrderTableModel.updateItemProperty<PropertyNames::PRICE>(o.refnum(),o.core().price());
            mOrderTableModel.updateItemProperty<PropertyNames::QTY>(o.refnum(),o.core().size());
            mOrderTableModel.updateItemProperty<PropertyNames::ORDERID>(o.refnum(),o.refnum());
            mOrderTableModel.updateItemProperty<PropertyNames::Player_ID>(o.refnum(),p.first.data());
            ViewModel * item = mPlayerListModel.itemByKey(p.first.data());
            auto name = item->propertyValue<PropertyNames::Player_Name>();
            mOrderTableModel.updateItemProperty<PropertyNames::Player_Name>(o.refnum(),name);
        }
    }


}

void Trading::on_pushButton_clicked()
{
}
#ifdef AUTOMMMIKECLAY
#include "playerloader.h"
void Trading::on_pushButton_clicked()
{
    MikeClayLoader mkl;
    auto mproj = mkl.loadProjFromLink(14);


    for ( auto p : mproj ) {
        if ( p.points() < 1) continue;
        if ( !p.has_playerid() || p.playerid() == "")
            continue;

        qDebug() << "Level2" << p.DebugString();
        //continue;
        int bid = 0;
        bool nextask = false;
        do {
            bool isbid = !nextask;
            eo.set_playerid(p.playerid());
            eo.set_type(ExchangeOrder::NEW);

            OrderCore core;
            core.set_buyside(isbid);
            core.set_size(1);
            int delta = floor(p.points() * .50);// * ((isbid) ? -1 : 1);
            if ( isbid ) delta = -delta;
            int price = p.points() + delta;
            if ( isbid ) {
                if ( price <= 0 ) price = 1;
                bid = price;
            }
            else {
                if ( price <= 0 && bid <= 1) continue;

                if ( price <= bid ) price++;

                if ( price >= 30) price = 30;

                if ( price <= bid) price++;

            }
            core.set_price(price);

            eo.mutable_core()->CopyFrom(core);
            //emit SendOrder(eo);
#ifdef TRACE
    qDebug() << "level2 NewOrder " << eo.DebugString();
#endif
            //continue;

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

        } while(nextask = !nextask);
    }
}
#endif

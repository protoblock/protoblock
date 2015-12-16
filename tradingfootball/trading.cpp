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

/*
    int w = ui->currentTeam->width();
    int h = ui->currentTeam->height();
    ui->currentTeam->setPixmap(QPixmap(":/icons/tf_logo_PNG-158x160.png").scaled(w,h,Qt::KeepAspectRatio));
*/
    int i =0;
    ui->posComboFilter->insertItem(i++,"ALL","ALL");
    ui->posComboFilter->insertItem(i++,"QB","QB");
    ui->posComboFilter->insertItem(i++,"RB","RB");
    ui->posComboFilter->insertItem(i++,"WR","WR");
    ui->posComboFilter->insertItem(i++,"TE","TE");
    ui->posComboFilter->insertItem(i++,"K","K");
    ui->posComboFilter->insertItem(i++,"D","D");


    ui->teamComboFilter->insertItem(i++,"ALL","ALL");
    for (int i=0;i<Commissioner::GENESIS_NFL_TEAMS.size();i++) {
        ui->teamComboFilter->insertItem(i+1,Commissioner::GENESIS_NFL_TEAMS[i].data(),
                                        Commissioner::GENESIS_NFL_TEAMS[i].data());
    }

    //ui->ordersTable->setModel(&mOrderTableModel);
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
    ui->depthView->setModel(&mDepthTableModel);


    ui->depthView->horizontalHeader()->setSectionResizeMode(0,QHeaderView::Stretch);
    ui->depthView->horizontalHeader()->setSectionResizeMode(1,QHeaderView::ResizeToContents);
    ui->depthView->horizontalHeader()->setSectionResizeMode(2,QHeaderView::ResizeToContents);
    ui->depthView->horizontalHeader()->setSectionResizeMode(3,QHeaderView::ResizeToContents);
    ui->depthView->horizontalHeader()->setSectionResizeMode(4,QHeaderView::ResizeToContents);
    ui->depthView->horizontalHeader()->setSectionResizeMode(5,QHeaderView::Stretch);

    ui->depthView->verticalHeader()->setResizeContentsPrecision(10);

    //auto w = ui->depthView->fontMetrics().width(" BIS SIZE BID ASK SK SIZE   ");
    //ui->depthView->setMaximumWidth(w*2);

    ui->playerList->setSelectionBehavior(QAbstractItemView::SelectItems);
    ui->playerList->setSelectionMode(QAbstractItemView::SingleSelection);
    myPlayerFilterProxy.reset(new PlayerListViewFilterProxyModel(&myGameTableModel,this->ui));
    myPlayerFilterProxy.data()->setSourceModel(&mPlayerListModel);
    myPlayerFilterProxy.data()->setDynamicSortFilter(true);
    myPlayerFilterProxy.data()->setSortRole(Qt::UserRole);
    ui->playerList->setModel(myPlayerFilterProxy.data());
    playersSelectionModel = new QItemSelectionModel(myPlayerFilterProxy.data());
    //playersSelectionModel.setModel(myPlayerFilterProxy.data());
    ui->playerList->setSelectionModel(playersSelectionModel);

    QObject::connect(playersSelectionModel,
                     SIGNAL(currentChanged(QModelIndex,QModelIndex)),
                     this, SLOT(OnplayerListSelection(QModelIndex,QModelIndex)));

    /*
    QObject::connect(playersSelectionModel,
                     SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
                     this,SLOT(OnPlayerListSelection(QItemSelection,QItemSelection)));
*/

    for ( int i = 0; i < mPlayerListModel.columnCount(); i++)
        if ( i == mPlayerListModel.name1)
            ui->playerList->horizontalHeader()->setSectionResizeMode(i,QHeaderView::Interactive);
        else
            ui->playerList->horizontalHeader()->setSectionResizeMode(i,QHeaderView::ResizeToContents);


    ui->groupBoxPlayerFilterGame->setChecked(false);
    ui->groupBoxPlayerFilterTrading->setChecked(false);
    ui->liquidfilter->setChecked(true);


    //ui->PlayersGroup->children();

    //ui->groupBoxPlayerFilterGame->isChecked()

    //ui->groupBoxPlayerFilterGame->ch



    //ordersModel=new OrdersModel;
    ui->ordersTable->setSelectionBehavior(QAbstractItemView::SelectItems);
    ui->ordersTable->setSelectionMode(QAbstractItemView::SingleSelection);
    myOrdersFilterProxy.reset(new OrdersListViewFilterProxyModel(this->ui));
    myOrdersFilterProxy.data()->setSourceModel(&mOrderTableModel);
    myOrdersFilterProxy.data()->setDynamicSortFilter(true);
    myOrdersFilterProxy.data()->setSortRole(Qt::UserRole);
    ui->ordersTable->setModel(myOrdersFilterProxy.data());
    ordersSelectionModel = new QItemSelectionModel(myOrdersFilterProxy.data());
    ui->ordersTable->setSelectionModel(ordersSelectionModel);

    QObject::connect(ordersSelectionModel,
                     SIGNAL(currentChanged(QModelIndex,QModelIndex)),
                     this, SLOT(OnOrderCancelSelected(QModelIndex,QModelIndex)));
    /*
    ordersSortModel=new QSortFilterProxyModel;
    ordersSortModel->setSortRole(Qt::EditRole);
    ordersSortModel->setFilterRole(Qt::WhatsThisRole);
    ordersSortModel->setDynamicSortFilter(true);
    ordersSortModel->setSourceModel(&mOrderTableModel);
    ui->ordersTable->setModel(ordersSortModel);
    */
    ui->ordersTable->horizontalHeader()->setSectionResizeMode(0,QHeaderView::ResizeToContents);
    ui->ordersTable->horizontalHeader()->setSectionResizeMode(1,QHeaderView::Stretch);
    ui->ordersTable->horizontalHeader()->setSectionResizeMode(2,QHeaderView::ResizeToContents);
    ui->ordersTable->horizontalHeader()->setSectionResizeMode(3,QHeaderView::ResizeToContents);
    ui->ordersTable->horizontalHeader()->setSectionResizeMode(4,QHeaderView::ResizeToContents);
    ui->ordersTable->horizontalHeader()->setSectionResizeMode(5,QHeaderView::ResizeToContents);
    ui->ordersTable->horizontalHeader()->setSectionResizeMode(6,QHeaderView::ResizeToContents);
    ui->ordersTable->horizontalHeader()->setSectionResizeMode(7,QHeaderView::ResizeToContents);

    //ui->ordersTable->setItemDelegateForColumn(7,&myOrdersTableCancelButtonDelgate);

    //ui->ordersTable->setItemDelegateForColumn(7,new OrdersTableCancelButton(ui->ordersTable, this));
    ui->ordersTable->setSelectionModel(ordersSelectionModel);

    ui->ordersTable->setSortingEnabled(true);
    ui->ordersTable->sortByColumn(0);

    /*
    connect(ui->ordersTable->selectionModel(),
            SIGNAL(selectionChanged(const QItemSelection&, const QItemSelection&)),
            this,SLOT(checkValidOrdersButtons()));
    */


    //connect(ordersModel,SIGNAL(ordersIsAvailable()),this,SLOT(ordersIsAvailable()));
    //connect(ordersModel,SIGNAL(cancelOrder(QString,QByteArray)),this,SLOT(cancelOrder(QString,QByteArray)));
    //connect(ordersModel,SIGNAL(volumeAmountChanged(double, double)),this,SLOT(volumeAmountChanged(double, double)));
    //connect(ui.ordersTable->selectionModel(),SIGNAL(selectionChanged(const QItemSelection&, const QItemSelection&)),this,SLOT(checkValidOrdersButtons()));

    /*
    QLayout* layout = ui->logowidget->layout();
    if(layout == NULL)
    {
        layout = new QGridLayout();
        layout->setContentsMargins(0,0,0,0);
        layout->setSpacing(0);
        logowidget->setLayout(layout);
        LogoButton* logoButton = new LogoButton;
        connect(this, SIGNAL(themeChanged()), logoButton, SLOT(themeChanged()));
        layout->addWidget(logoButton);
    }
    */
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

       QObject::connect(theLAPIWorker,SIGNAL(GameStart(string)),this,SLOT(onGameStart(string)));

       QObject::connect(theLAPIWorker,SIGNAL(NameBal(fantasybit::FantasyNameBal)),
                        this,SLOT(OnNameBalance(fantasybit::FantasyNameBal)));


       /*
       FantasyNameData *namedata = &(theLAPIWorker->NameData());
       QObject::connect(namedata,SIGNAL(NewFantasyNameOrder(fantasybit::Order&)),
                         this,SLOT(OnMyNewOrder(fantasybit::Order&)));
        */

       QObject::connect(exchangedata,
                        SIGNAL(NewPos(fantasybit::FullPosition)),
                         this,SLOT(OnNewPos(fantasybit::FullPosition)));

    }

    /*
    QObject::connect(ui->playerList,SIGNAL(doubleClicked(QModelIndex)),
                     this,SLOT(playerListCliked(QModelIndex)));
    */
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

    QObject::connect(ui->lockedrb,SIGNAL(clicked(bool)),this,SLOT(invalidateFilters(bool)));
    QObject::connect(ui->openrb,SIGNAL(clicked(bool)),this,SLOT(invalidateFilters(bool)));
    QObject::connect(ui->groupBoxlocked,SIGNAL(toggled(bool))
            ,this,SLOT(invalidateFilters(bool)));
    QObject::connect(ui->groupBoxPlayerFilterGame,SIGNAL(clicked(bool)),this,SLOT(invalidateFilters(bool)));
    QObject::connect(ui->groupBoxPlayerFilterMarket,SIGNAL(clicked(bool)),this,SLOT(invalidateFilters(bool)));
    QObject::connect(ui->groupBoxPlayerFilterTrading,SIGNAL(clicked(bool)),this,SLOT(invalidateFilters(bool)));
    QObject::connect(ui->teamComboFilter,SIGNAL(currentIndexChanged(int))
            ,this,SLOT(invalidateFilters(int)));
    QObject::connect(ui->posComboFilter,SIGNAL(currentIndexChanged(int))
            ,this,SLOT(invalidateFilters(int)));
    QObject::connect(ui->myorderFilter,SIGNAL(clicked(bool))
            ,this,SLOT(invalidateFilters(bool)));
    QObject::connect(ui->mypositionsFilter,SIGNAL(clicked(bool))
            ,this,SLOT(invalidateFilters(bool)));
    QObject::connect(ui->activefilter,SIGNAL(clicked(bool))
            ,this,SLOT(invalidateFilters(bool)));
    QObject::connect(ui->liquidfilter,SIGNAL(clicked(bool))
            ,this,SLOT(invalidateFilters(bool)));
    QObject::connect(ui->moversfiler,SIGNAL(clicked(bool))
            ,this,SLOT(invalidateFilters(bool)));



}

Trading::~Trading()
{
    delete ui;
}

void Trading::OnOrderCancelSelected(QModelIndex index,QModelIndex index2) {
    index = myOrdersFilterProxy.data()->mapToSource(index);

    if ( index.column() == 7) {
        ViewModel * data = mOrderTableModel.getItemByIndex(index);
        if (data !=NULL) {
            int id = data->propertyValue<QString,PropertyNames::ORDERID>().toInt();
            emit CancelOrder(id);
        }
    }
}

void Trading::OnplayerListSelection(QModelIndex index,QModelIndex) {

    ui->buyit->setEnabled(true);
    ui->sellit->setEnabled(true);
    index = myPlayerFilterProxy.data()->mapToSource(index);

    /*
    ViewModel * data1 = mPlayerListModel.getItemByIndex(i1);
    QString playerid1 = data1->propertyValue<QString,PropertyNames::Player_ID>();
    qDebug() << data1->propertyValue<QString,PropertyNames::Player_Name>();

    QModelIndex i2 = myPlayerFilterProxy.data()->mapToSource(index);
    ViewModel * data2 = mPlayerListModel.getItemByIndex(i2);
    QString playerid2 = data2->propertyValue<QString,PropertyNames::Player_ID>();
    qDebug() << data2->propertyValue<QString,PropertyNames::Player_Name>();

    /*
    QItemSelection filteredSelection = playersSelectionModel->selection();
    QItemSelection sourceSelection1;

    QItemSelection sourceSelection2;

    sourceSelection1 = myPlayerFilterProxy.data()->mapSelectionToSource(filteredSelection);
    QModelIndexList selectionList1 = sourceSelection1.indexes();

    sourceSelection2 = myPlayerFilterProxy.data()->mapSelectionFromSource(filteredSelection);
    QModelIndexList selectionList2 = sourceSelection2.indexes();

    for(int i=0;i<selectionList1.count();i++){
        QModelIndex sourceIndex = selectionList1.at(i);
        ViewModel * data = mPlayerListModel.getItemByIndex(sourceIndex);
        if (data == NULL) return;
        QString playerid = data->propertyValue<QString,PropertyNames::Player_ID>();
        qDebug() << data->propertyValue<QString,PropertyNames::Player_Name>();
    }

    for(int i=0;i<selectionList2.count();i++){
        QModelIndex sourceIndex = selectionList2.at(i);
        ViewModel * data = mPlayerListModel.getItemByIndex(sourceIndex);
        if (data == NULL) return;
        QString playerid = data->propertyValue<QString,PropertyNames::Player_ID>();
        qDebug() << data->propertyValue<QString,PropertyNames::Player_Name>();
    }
*/
    /*
    QItemSelection s1 = playersSelectionModel->selection();


    s1.select(myPlayerFilterProxy->mapSelectionToSource(s1),
              QItemSelectionModel::ClearAndSelect);



    QModelIndex ci = playersSelectionModel->currentIndex();

    QModelIndexList selectionList = s1.indexes();

    ViewModel * data1 = mPlayerListModel.getItemByIndex(ci);

    QString playerid = data1->propertyValue<QString,PropertyNames::Player_ID>();
    qDebug() << " pid1 " << playerid;
*/
    ViewModel * data = mPlayerListModel.getItemByIndex(index);
    if (data !=NULL) {
        QString playerid = data->propertyValue<QString,PropertyNames::Player_ID>();
        qDebug() << " pid2 " << playerid;

        if ( myPlayerid == playerid.toStdString())
            return;

        mUpdatingS = mUpdatingB = true;
        mDepthTableModel.changeSymbol(playerid.toStdString());
        ui->playername->setText(data->propertyValue<QString,PropertyNames::Player_Name>());
        ui->positionTeam->setText(data->propertyValue<QString,PropertyNames::Position>());
        //ui->teamicon->setTextFormat(Qt::RichText);
        QString team = data->propertyValue<QString,PropertyNames::Team_ID>();
        //ui->teamicon->setText("<img src=" + QString::fromStdString(Trading::) +">");
        //ui->positionTeam->setPixmap(QPixmap("icons[team.toStdString()]"));
        int w = ui->currentTeam->width();
        int h = ui->currentTeam->height();

        ui->currentTeam->setPixmap(QPixmap(icons[team.toStdString()].data()).scaled(w,h,Qt::KeepAspectRatio));
        // set a scaled pixmap to a w x h window keeping its aspect ratio
        ui->marketLast->setValue(data->propertyValue<QString,PropertyNames::LAST>().toInt());
        ui->marketBid->setValue(data->propertyValue<QString,PropertyNames::BID>().toInt());
        ui->marketAsk->setValue(data->propertyValue<QString,PropertyNames::ASK>().toInt());
        ui->marketChng->setValue(data->propertyValue<QString,PropertyNames::CHANGE>().toInt());
        ui->marketHigh->setValue(data->propertyValue<QString,PropertyNames::HIGH>().toInt());
        ui->marketLow->setValue(data->propertyValue<QString,PropertyNames::LOW>().toInt());
        ui->marketVolume->setValue(data->propertyValue<QString,PropertyNames::VOLUME>().toInt());
        //ui->marketBids->setValue(data->propertyValue<QString,PropertyNames::BIDSIZE>().toInt());
        //ui->marketAsks->setValue(data->propertyValue<QString,PropertyNames::ASKSIZE>().toInt());
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

        //ui->playername->setText(QString("whta"));
        myPlayerid = playerid.toStdString();

        ui->posAvgPrice->setValue(data->propertyValue<QString,PropertyNames::MYAVG>().toInt());
        ui->posQty->setValue(data->propertyValue<QString,PropertyNames::MYPOS>().toInt());


        /*
        int netqty = ui->posQty->value();
        double avg = 0;
        double pnl = 0;
        if ( neyqty ==0 ) {
            //double pnl = fp.pos.netprice * 100;
        }
        else  {
            ViewModel * item = mPlayerListModel.itemByKey(fp.playerid.data());
            int price = item->propertyValue<PropertyNames::LAST>().toInt();
            pnl = (price * netqty) + fp.pos.netprice;
            avg = fp.pos.netprice / (netqty * -1);
        }
        */

        emit OnPlayerPosTeam(ui->playername->text(),team,ui->positionTeam->text());
        //emit OnPriceChange(ui->marketLast->value());

    }
}

void Trading::SetFantasyName(std::string name,int balance) {
#ifdef TRACE
    qDebug() << "level2 trading SetFantasyName" << name;
#endif

    if ( name != myFantasyName ) {
        ui->fantastname->setText(name.data());
        myFantasyName = name;
        ui->fantasybitSkill->setValue(balance);
        myOrdersFilterProxy->clear();
        SetMyPositions();
        invalidateFilters();
    }
}


void Trading::on_buyit_clicked()
{
    if ( ui->playername->text() == "Player Name" ) return;
    QMessageBox msgBox;
    msgBox.setIcon(QMessageBox::Question);
    msgBox.setWindowTitle(APPLICATION_NAME);

#ifdef TRACE
    qDebug() << "level2 on_buyit_clicked";
#endif
    QString text = "Buy %1 contracts of %2 at price of %3?";
    int result = QMessageBox::Cancel;
    msgBox.setText(text.arg(ui->buyqty->value()).arg(ui->playername->text()).arg(ui->buyprice->value()));
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::Cancel);
    msgBox.setButtonText(QMessageBox::Yes,"Send Buy Order");
    msgBox.setDefaultButton(QMessageBox::Yes);
    //msgBox.setParent(ui->buyit);
    result = msgBox.exec();

    if ( result == QMessageBox::Yes) {
        NewOrder(true);
        QMessageBox msgBox2;
        msgBox2.setText(QString(
                            "Buy Order sent! Will be reflected in Open Orders or \"MyPos\" if accepted or executed"));
        msgBox2.setStandardButtons(QMessageBox::Ok);
        msgBox2.button(QMessageBox::Ok)->animateClick(15000);
        //msgBox2.setParent(ui->ordersTableFrame);
        msgBox2.exec();
    }
}

void Trading::on_sellit_clicked()
{
    if ( ui->playername->text() == "Player Name" ) return;
    QMessageBox msgBox;
    msgBox.setIcon(QMessageBox::Question);
    msgBox.setWindowTitle(APPLICATION_NAME);

#ifdef TRACE
    qDebug() << "level2 on_sellit_clicked";
#endif
    QString text = "Sell %1 contracts of %2 at price of %3?";
    int result = QMessageBox::Cancel;
    msgBox.setText(text.arg(ui->sellqty->value()).arg(ui->playername->text()).arg(ui->sellprice->value()));
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::Cancel);
    msgBox.setButtonText(QMessageBox::Yes,"Send Sell Order");
    msgBox.setDefaultButton(QMessageBox::Yes);
    //msgBox.setParent(ui->sellit);
    result = msgBox.exec();

    if ( result == QMessageBox::Yes) {
        NewOrder(false);
        QMessageBox msgBox2;
        msgBox2.setText(QString("Sell Order sent!  Will be reflected in Open Orders or \"MyPos\" if accepted or executed"));
        msgBox2.setStandardButtons(QMessageBox::Ok);
        msgBox2.button(QMessageBox::Ok)->animateClick(15000);
        //msgBox2.setParent(ui->ordersTableFrame);
        msgBox2.exec();
    }
}


void Trading::onGameStart(string gameId){
    myGameTableModel.updateItemProperty<PropertyNames::Game_Status>
            (gameId.data(),qVariantFromValue<GameStatus_Status>(fantasybit::GameStatus_Status_INGAME));
   invalidateFilters();
   qDebug() << "Trading on Game Start " << gameId;
}

void Trading::invalidateFilters() {
    //myGameModelFilter.invalidate();
    //playersSelectionModel->reset();
    myPlayerFilterProxy.data()->enable();
    myPlayerFilterProxy.data()->invalidate();
    //ui->playerList->horizontalHeader()->setSectionResizeMode(mPlayerListModel.name1,QHeaderView::Interactive);

}

void Trading::invalidateFilters(bool) {
    invalidateFilters();
}

void Trading::invalidateFilters(int) {
    invalidateFilters();
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

        //ui->gamesComboFilter->insertItem(i++,ret,gameId);

        myGameTableModel.updateItemProperty<PropertyNames::Game_ID>(gameId,gameId);
        myGameTableModel.updateItemProperty<PropertyNames::Away>(gameId,game.info.away().data());
        myGameTableModel.updateItemProperty<PropertyNames::Home>(gameId,game.info.home().data());
        myGameTableModel.updateItemProperty<PropertyNames::Game_Time>(gameId,fromTime_t_toFantasyString(game.info.time()));
        myGameTableModel.updateItemProperty<PropertyNames::Game_Status>(gameId,qVariantFromValue<GameStatus_Status>(game.status));

        for ( auto hh : { QString("home"), QString("away")} ) {
            for ( auto ho : (hh == QString("home")) ? game.homeroster : game.awayroster ) {
                QString playerId = ho.first.data();
                auto playerDetails = ho.second;
                QString playerName;
                playerName = QString(playerDetails.base.first().data()) + " " + QString(playerDetails.base.last().data());


                mPlayerListModel.updateItemProperty<PropertyNames::Game_ID>(playerId,gameId);
                mPlayerListModel.updateItemProperty<PropertyNames::Player_ID>(ho.first.data(),ho.first.data());
                mPlayerListModel.updateItemProperty<PropertyNames::Team_ID>(playerId,
                                                      (hh == QString("home")) ? game.info.home().data() : game.info.away().data());
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

void Trading::CancelOrder(int id) {

    eo.set_type(ExchangeOrder::CANCEL);

    //emit SendOrder(eo);
    eo.set_cancel_oref(id);

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

    QMessageBox msgBox2;
    msgBox2.setText(QString("Cancel Order(%1) sent!").arg(id));
    msgBox2.setStandardButtons(QMessageBox::Ok);
    msgBox2.button(QMessageBox::Ok)->animateClick(4000);
    //msgBox2.setParent(ui->ordersTableFrame);
    msgBox2.exec();

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
        if ( mt->symbol() == myPlayerid ) {
            ui->marketBid->setValue(mt->price());
            //ui->marketBids->setValue(mt->size());
        }
    }
    else if ( mt->type() == MarketTicker_Type_ASK){
        mPlayerListModel.updateItemProperty<PropertyNames::ASK>(playerid,mt->price());
        mPlayerListModel.updateItemProperty<PropertyNames::ASKSIZE>(playerid,mt->size());
        if ( mt->symbol() == myPlayerid ) {
            ui->marketAsk->setValue(mt->price());;
            //ui->marketAsks->setValue(mt->size());
        }
    }

}


void Trading::OnTradeTick(fantasybit::TradeTic* tt) {
#ifdef TRACE
    qDebug() << "level2 OnTradeTick " << tt->DebugString();
#endif
    if ( tt->symbol() == "" ) return;
    QString playerid = tt->symbol().data();
    bool updatelive = (tt->symbol() == myPlayerid );

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

    if ( updatelive ) {
        ui->marketLast->setValue(tt->price());
        ui->marketVolume->setValue(volume);
        if ( tt->ishigh() ) ui->marketHigh->setValue(tt->price());
        if ( tt->islow() ) ui->marketLow->setValue(tt->price());
        ui->marketChng->setValue(tt->change());
        //emit OnPriceChange(ui->marketLast->value());
    }

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
    //ui->buyqty->setValue(ui->marketAsks->value());
}

void Trading::on_buyPriceAsMarketBid_clicked()
{
    ui->buyprice->setValue(ui->marketBid->value());
    //ui->buyqty->setValue(ui->marketBids->value());
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
    //ui->sellqty->setValue(ui->marketAsks->value());
}

void Trading::on_sellPriceAsMarketBid_clicked()
{
    ui->sellprice->setValue(ui->marketBid->value());
    //ui->sellqty->setValue(ui->marketBids->value());
}

//typedef std::unordered_map<std::string,std::pair<Position,std::vector<Order> > > ordsnap_t;

void Trading::SetMyPositions() {
    mOrderTableModel.removeAll();
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
            //mOrderTableModel.updateItemProperty<PropertyNames::ORDERID>(o.refnum(),o.refnum());
            mOrderTableModel.updateItemProperty<PropertyNames::Player_ID>(o.refnum(),p.first.data());
            ViewModel * item = mPlayerListModel.itemByKey(p.first.data());
            auto name = item->propertyValue<PropertyNames::Player_Name>();
            mOrderTableModel.updateItemProperty<PropertyNames::Player_Name>(o.refnum(),name);
            mOrderTableModel.updateItemProperty<PropertyNames::ORDERX>(o.refnum(),"");
            //ui->cancelOrdersList->addItem(o.DebugString().data(),o.refnum());
        }


        int netqty = p.second.first.netqty;
        double avg = 0;
        double pnl = 0;
        if ( netqty ==0 ) {
            double pnl = p.second.first.netprice * 100;
        }
        else  {
            ViewModel * item = mPlayerListModel.itemByKey(p.first.data());
            int price = item->propertyValue<PropertyNames::LAST>().toInt();
            pnl = 100 * ((price * netqty) + p.second.first.netprice);
            avg = p.second.first.netprice / (netqty * -1);
        }

        mPlayerListModel.updateItemProperty<PropertyNames::MYPOS>(p.first.data(),netqty);
        mPlayerListModel.updateItemProperty<PropertyNames::MYAVG>(p.first.data(),avg);
        mPlayerListModel.updateItemProperty<PropertyNames::MYPNL>(p.first.data(),pnl);

        if ( p.first == myPlayerid) {
            ui->posQty->setValue(netqty);
            ui->posAvgPrice->setValue(avg);
            ui->posOpenPnl->setValue(pnl);

        }

    }
}


#ifdef AUTOMMMIKECLAY
#include "playerloader.h"
void Trading::on_pushButton_clicked()
{
    MikeClayLoader mkl;
    auto mproj = mkl.loadProjFromLink(15);


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
            core.set_size(5);
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
#else
void Trading::on_pushButton_clicked() {}
#endif


void Trading::cancelOrderByXButton()
{
    QPushButton* buttonCancel=dynamic_cast<QPushButton*>(sender());
    if(!buttonCancel)return;
    QModelIndex & index = (QModelIndex &)buttonCancel->property("OrderId");
    ViewModel * data = mOrderTableModel.getItemByIndex(index);
    if (data !=NULL) {
        QString oid = data->propertyValue<int,PropertyNames::ORDERID>();
        qDebug() << oid;
    }

}

void Trading::OnNameBalance(fantasybit::FantasyNameBal namebal) {
    if ( namebal.name() == myFantasyName ) {
        ui->fantasybitSkill->setValue(namebal.bits());
        ui->fantasybitStake->setValue(namebal.stake());
    }

}

void Trading::on_ordersCancelSelected_clicked()
{
    QModelIndexList selectedIdx=ui->ordersTable->selectionModel()->selectedIndexes();
    ViewModel *data = mOrderTableModel.getItemByIndex(selectedIdx.at(0));
    if (data !=NULL) {
        int oid = data->propertyValue<PropertyNames::ORDERID>().toInt();
        qDebug() << "level2 cancel" << oid;
        emit CancelOrder(oid);
    }


    //    OrderTablesModel *ot = dynamic_cast<OrderTablesModel *>(ui->ordersTable->selectionModel()->model());
 /*   for ( auto index : ot-> selectedRows()) {
        ViewModel * data = mOrderTableModel.getItemByIndex(index);
        if (data !=NULL) {
            int oid = data->propertyValue<int,PropertyNames::ORDERID>();
            qDebug() << "level2 cancel" << oid;
        }
    }
    */
}


void Trading::checkValidOrdersButtons()
{
    ///ui->ordersCancelAllButton->setEnabled(mOrderTableModel.rowCount());
    //ui->ordersCancelSelected->setEnabled(ui->ordersTable->selectionModel()->selectedIndexes().count());
}


void Trading::on_cancelOrderListButton_clicked()
{
    //int oid = ui->cancelOrdersList->currentData().toInt();
    //if ( oid != 0 )
    //    emit CancelOrder(oid);
}

void Trading::OnNewPos(fantasybit::FullPosition fp) {
    qDebug() << "level2 Trading::OnNewPos " << fp.pos.ToString() << fp.playerid << fp.fname;

    if ( fp.fname == myFantasyName ) {
        int netqty = fp.pos.netqty;
        double avg = 0;
        double pnl = 0;
        if ( netqty ==0 ) {
            double pnl = fp.pos.netprice * 100;
        }
        else  {
            ViewModel * item = mPlayerListModel.itemByKey(fp.playerid.data());
            int price = item->propertyValue<PropertyNames::LAST>().toInt();
            pnl = (price * netqty) + fp.pos.netprice;
            avg = fp.pos.netprice / (netqty * -1);
        }

        mPlayerListModel.updateItemProperty<PropertyNames::MYPOS>(fp.playerid.data(),netqty);
        mPlayerListModel.updateItemProperty<PropertyNames::MYPNL>(fp.playerid.data(),pnl);
        mPlayerListModel.updateItemProperty<PropertyNames::MYAVG>(fp.playerid.data(),avg);
        invalidateFilters();

        if ( fp.playerid == myPlayerid ) {
            ui->posQty->setValue(netqty);
            ui->posAvgPrice->setValue(avg);
            ui->posOpenPnl->setValue(pnl);
        }
    }
}

void Trading::onControlMessage(QString msg) {
    if ( msg.contains("[t.2f]") && amlive ) {

        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.setWindowTitle(APPLICATION_NAME);

        msgBox.setText(msg);
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.exec();
    }
}

/*
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
*/

decltype(Trading::icons) Trading::icons{
    {"SF",":/NFL/ico/49ers.ico"},
    {"CHI",":/NFL/ico/Bears.ico"},
    {"CIN",":/NFL/ico/Bengels.ico"},
    {"BUF",":/NFL/ico/Bills.ico"},
    {"DEN",":/NFL/ico/Broncos.ico"},
    {"CLE",":/NFL/ico/Browns.ico"},
    {"TB",":/NFL/ico/Buccaneers.ico"},
    {"ARI",":/NFL/ico/Cardinals.ico"},
    {"SD",":/NFL/ico/Chargers.ico"},
    {"KC",":/NFL/ico/Chiefs.ico"},
    {"IND",":/NFL/ico/Colts.ico"},
    {"DAL",":/NFL/ico/Cowboys.ico"},
    {"MIA",":/NFL/ico/Dolphins.ico"},
    {"PHI",":/NFL/ico/Eagles.ico"},
    {"ATL",":/NFL/ico/Falcons.ico"},
    {"NYG",":/NFL/ico/Giants.ico"},
    {"JAC",":/NFL/ico/Jaguar.ico"},
    {"NYJ",":/NFL/ico/Jets.ico"},
    {"DET",":/NFL/ico/Lions.ico"},
    {"GB",":/NFL/ico/Packers.ico"},
    {"CAR",":/NFL/ico/Panthers.ico"},
    {"NE",":/NFL/ico/Patriots.ico"},
    {"OAK",":/NFL/ico/Raiders.ico"},
    {"STL",":/NFL/ico/Rams.ico"},
    {"BAL",":/NFL/ico/Ravens.ico"},
    {"WAS",":/NFL/ico/Redskins.ico"},
    {"NO",":/NFL/ico/Saints.ico"},
    {"SEA",":/NFL/ico/Seahawks.ico"},
    {"PIT",":/NFL/ico/Steelers.ico"},
    {"HOU",":/NFL/ico/Texans.ico"},
    {"TEN",":/NFL/ico/Titans.ico"},
    {"MIN",":/NFL/ico/Vikings.ico"},
};


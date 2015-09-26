#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "LAPIWorker.h"
#include "core.h"
#include "dataservice.h"
#include "datacache.h"
#include <QCheckBox>
#include <QSettings>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{    
    ui->setupUi(this);
    initDefaultGuiDisplay();
    initialize();
}

void MainWindow::initDefaultGuiDisplay(){
    setWindowTitle(appTitle());
    ui->myFantasyNameStatusLabel->setText("None");
    ui->mySeasonLabel->setText("Unknown Season");
    ui->myWeekLabel->setText("Unknown Week #");
    ui->myPreviousWeek->setEnabled(false);
    ui->myNextWeek->setEnabled(false);
	ui->myClaimFantasyNameButton->setEnabled(false);
    ui->myStackedWidget->setCurrentWidget(ui->myCurrentWeekView);
    myCurrentWeek =-1;    
    ui->myLeaderBaordTableView->setModel(&DataCache::instance()->leaderBoardModel());    
    ui->myLeaderBaordTableView->setContextMenuPolicy(Qt::CustomContextMenu);
}

void MainWindow::initialize() {
    myLAPIWorker = Core::resolveByName<MainLAPIWorker>("coreapi");
    QObject::connect(myLAPIWorker,SIGNAL(LiveGui(fantasybit::GlobalState)),
                     this,SLOT(GoLive(fantasybit::GlobalState)));

    QObject::connect(myLAPIWorker,SIGNAL(GlobalStateChange(fantasybit::GlobalState)),
                     this,SLOT(GlobalStateChange(fantasybit::GlobalState)));

    QObject::connect(myLAPIWorker,SIGNAL(MyNames(vector<fantasybit::MyFantasyName>)),
                     this,SLOT(OnMyFantasyNames(vector<fantasybit::MyFantasyName>)));


    QObject::connect(this,SIGNAL(UseMyFantasyName(QString)),myLAPIWorker,SLOT(OnUseName(QString)));
    //QObject::connect(this,SIGNAL(SubscribeMyNameTx(QString)),myLAPIWorker,SLOT(OnSubName(QString)));
    //QObject::connect(this,SIGNAL(SubscribeMyProjTx(QString)),myLAPIWorker,SLOT(OnSubProj(QString)));
    //QObject::connect(this,SIGNAL(SubscribeAwards(QString)),myLAPIWorker,SLOT(OnSubBits(QString)));

    QObject::connect(myLAPIWorker,SIGNAL(NameStatus(fantasybit::MyFantasyName)),
                     this,SLOT(OnNameStatus(fantasybit::MyFantasyName)));
    QObject::connect(myLAPIWorker,SIGNAL(LiveProj(fantasybit::FantasyBitProj)),
                     this,SLOT(OnProjAck(fantasybit::FantasyBitProj)));
    QObject::connect(myLAPIWorker,SIGNAL(NameBal(fantasybit::FantasyNameBal)),
                     this,SLOT(OnNameBalance(fantasybit::FantasyNameBal)));

    //state
    //QObject::connect(this,SIGNAL(SubscribeGameState()),myLAPIWorker,SLOT(OnSubGame()));

    QObject::connect(myLAPIWorker,SIGNAL(NewWeek(int)),this,SLOT(OnNewWeek(int)));
    QObject::connect(myLAPIWorker,SIGNAL(GameOver(string)),this,SLOT(OnGameOver(string)));
    QObject::connect(myLAPIWorker,SIGNAL(GameStart(string)),this,SLOT(OnGameStart(string)));

     //Player Status Change
     QObject::connect(myLAPIWorker,SIGNAL(PlayerStatusChange(pair<string,fantasybit::PlayerStatus>)),
                      this,SLOT(OnPlayerStatusChange(pair<string,fantasybit::PlayerStatus>)));

    //data
    //QObject::connect(this,SIGNAL(SubscribePlayerData()),myLAPIWorker,SLOT(OnSubPD()));
    //QObject::connect(this,SIGNAL(SubscribeScheduleData()),myLAPIWorker,SLOT(OnSubSS()));

    //QObject::connect(this,SIGNAL(SubscribeTeamRoster()),myLAPIWorker,SLOT(OnSubTeams()));
    //QObject::connect(this,SIGNAL(SubscribePlayerGameStatus()),myLAPIWorker,SLOT(OnSubOut()));

    //    QObject::connect(myCoreInstance,SIGNAL(PlayerData(QString)),this,SLOT(OnPlayerUpdate(QString)));
    //    QObject::connect(myCoreInstance,SIGNAL(TimeChange(QString)),this,SLOT(OnSchedule(QString)));
    //    QObject::connect(myCoreInstance,SIGNAL(TeamActicity(QString)),this,SLOT(OnTeamRoster(QString)));


    //GET
    //    QObject::connect(this,SIGNAL(GetPrevWeekData(int)),myCoreInstance,SLOT(SendWeeklyData(int)));
    //    QObject::connect(this,SIGNAL(RefershLive(int)),myCoreInstance,SLOT(SendLiveSnap(int)));

    //    QObject::connect(this,SIGNAL(GetLiveProjection(QString)),myCoreInstance,SLOT(SendCurrentProj(QString)));
    //    QObject::connect(this,SIGNAL(RefershLive(int)),myCoreInstance,SLOT(SendLiveSnap(int)));

    //PUT.
    //QObject::connect(this,SIGNAL(NewProjection(vector<fantasybit::FantasyBitProj>)),
    //                 myLAPIWorker,SLOT(OnProjTX(vector<fantasybit::FantasyBitProj>)));

    QObject::connect(this,SIGNAL(ClaimFantasyName(QString)),myLAPIWorker,SLOT(OnClaimName(QString)));

    //wait for going live
    myWaitDialog.init(":/icons/footballspin.gif",0,"Syncing...");
    QObject::connect(myLAPIWorker,SIGNAL(Height(int)),&myWaitDialog,SLOT(Height(int)));
    QObject::connect(myLAPIWorker,SIGNAL(BlockNum(int)),&myWaitDialog,SLOT(BlockNum(int)));

    QObject::connect(myLAPIWorker,SIGNAL(LiveGui(fantasybit::GlobalState)),&myWaitDialog,SLOT(stopAndClose()));

    //leader timed refresh
    myLeaderBoardTimer.setInterval(AppSettings::getSetting(AppSettings::LeaderBoardRefreshInterval).toInt()*60000);
    QObject::connect(&myLeaderBoardTimer,SIGNAL(timeout()),
                     this,SLOT(refreshLeaderBoard()));

    QObject::connect(ui->myLeaderBaordTableView,SIGNAL(doubleClicked(QModelIndex)),
                     this,SLOT(leaderboardCliked(QModelIndex)));
    QObject::connect(ui->myLeaderBaordTableView,SIGNAL(customContextMenuRequested(QPoint)),
                     this,SLOT(showLeaderboardContextualMenu(QPoint)));

    QObject::connect(myLAPIWorker,SIGNAL(onControlMessage(QString)),
                     ui->myCurrentWeekWidget,SLOT(onControlMessage(QString)));

    //wake up core thread
    Core::instance()->guiIsAwake();
    myWaitDialog.startExec();
    if (myLAPIWorker == NULL)  {
        qDebug() << "coreapi is not resolved";
        setDisabled(true);
        return;
    }
}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_myNextWeek_clicked()
{
    navigateToWeek(myCurrentWeek+1);
}

void MainWindow::nextWeek(){
    DataCache::instance()->leaderBoardModel().setColumnVisible("Action",false);
    ui->myStackedWidget->setCurrentWidget(ui->myNexWeekView);
    ui->myNextWeekWidget->setWeekData(myCurrentWeek);
}

void MainWindow::previousWeek() {    
    DataCache::instance()->leaderBoardModel().setColumnVisible("Action",false);
    ui->myStackedWidget->setCurrentWidget(ui->myPreviousWeekView);
    ui->myPreviousWeekWidget->setWeekData(myCurrentWeek);
}

void MainWindow::currentWeek() { 
    DataCache::instance()->leaderBoardModel().setColumnVisible("Action",true);
    ui->myStackedWidget->setCurrentWidget(ui->myCurrentWeekView);
}


void MainWindow::on_myPreviousWeek_clicked()
{
    navigateToWeek(myCurrentWeek-1);
}

void MainWindow::GlobalStateChange(fantasybit::GlobalState state){
    if ((myGlobalState.state() == GlobalState_State_OFFSEASON &&
         state.state() == GlobalState_State_INSEASON ) ||
            (myGlobalState.week() == 0 &&
             state.week() > 0 ) )
        GoLive(state);
    else
        if (myGlobalState.week()== state.week()-1 ) {
            DataCache::instance()->refreshLeaderboard();
            ui->myCurrentWeekWidget->setCurrentWeekData(state);
            navigateToWeek(state.week());
                if (!myLeaderBoardTimer.isActive()) myLeaderBoardTimer.start();

            myGlobalState = state;

    }

    qDebug() << "GlobalStateChange : " << "state week :" << state.week()  << "myGlobalState.week():" <<myGlobalState.week() ;
}

void MainWindow::GoLive(fantasybit::GlobalState state){
    myIamLive = true;
    myGlobalState = state;
    QString seasonLabel = "%1";
    QString seasonType;
    switch (state.state() ) {
    case GlobalState_State_INSEASON :
        seasonType ="2015 NFL Season";
        break;
    case GlobalState_State_OFFSEASON :
        seasonType = "Off Season";
        seasonLabel += " %2";
        break;
    default:
        break;
    }

    seasonLabel = seasonLabel.arg(seasonType).arg(myGlobalState.season());
    ui->mySeasonLabel->setText(seasonLabel);

    //load roaster for current week
    if ( myGlobalState.week() > 0) {
        DataCache::instance()->refreshLeaderboard();
        ui->myCurrentWeekWidget->setCurrentWeekData(myGlobalState);
        navigateToWeek(myGlobalState.week());
        if (!myLeaderBoardTimer.isActive()) myLeaderBoardTimer.start();
    }
    qDebug() << "glolive"  ;
}

void MainWindow::refreshLeaderBoard(){
    DataCache::instance()->refreshLeaderboard();
    foreach(QString fantasyName,DataCache::instance()->leaderBoardModel().keys())
      ui->myCurrentWeekWidget->refreshFantasyNamesProjections(fantasyName);
}

void MainWindow::navigateToWeek(int week)
{
    myCurrentWeek = week;
    QString currentWeekNotice = myGlobalState.week()==myCurrentWeek?" (live) ":"";
    ui->myWeekLabel->setText(QString("Week %1").arg(myCurrentWeek)+currentWeekNotice);
    ui->myPreviousWeek->setDisabled(myCurrentWeek==1);
    ui->myNextWeek->setDisabled(myCurrentWeek==17);
    if (myCurrentWeek==myGlobalState.week())
        currentWeek();
    else if (myCurrentWeek < myGlobalState.week())
        previousWeek();
    else
        nextWeek();
}

void MainWindow::OnMyFantasyNames(vector<fantasybit::MyFantasyName> names){
	ui->myClaimFantasyNameButton->setEnabled(true);
    if (names.size()> 0){
		qDebug() <<"received fan. names.";
		MyFantasyName * cachedName = NULL;
        for(int i=0;i<names.size();i++){
            MyFantasyName fName = names.at(i);
            QString stringName = QString(fName.name().data());            
            QVariant itemData= qVariantFromValue(fName);
			qDebug() << "adding a name with status :" << translateNameStatus(fName.status());
            myAddNamesPending = true;
			qDebug() << "caching fan. name :" << stringName;
			cachedName = new MyFantasyName(fName);
			myFantasyNames.insert(stringName, cachedName);
			qDebug() << "adding it to combo";
            ui->myFantasyNamesCombo->insertItem(i,stringName);
            myAddNamesPending = false;
        }        
		if (cachedName != NULL){
			qDebug() << "set last received name as current" << cachedName->name().data();
			setCurrentFantasyName(cachedName,true);
		}
    }
}

void MainWindow::OnNameStatus(MyFantasyName name){
	QString nameString = QString(name.name().data());
	QString statusHint ="none = 1,"
		"notavil = 2,"
		"requested = 5,"
		"transaction_sent = 15,"
		"confirmed = 20";

	qDebug() << "Received OnNameStatus :" << nameString << " status: " << name.status();
	qDebug() << statusHint;
    fantasybit::MyNameStatus newStatus = name.status();
    //HACK : !!!!!!! Name comes a none right after requesting it.
    if (newStatus == fantasybit::none) {
        qDebug() << "Forcing received none status to request :";
        name.set_status(fantasybit::requested);
    }

	qDebug() << "updating cached name status";	
	MyFantasyName * storedName = myFantasyNames.value(nameString);	
	if (storedName != NULL){
		qDebug() << "cached name found and updated.";
		storedName->CopyFrom(name);
	}
	else {
		qDebug() << "cached name not found adding it to the map and combox";
		MyFantasyName * storedName = new MyFantasyName(name);
		myFantasyNames.insert(nameString, storedName);
		myAddNamesPending = true;
		ui->myFantasyNamesCombo->addItem(nameString);
		myAddNamesPending = false;
	}

    bool isCurrent = myCurrentFantasyName.name()== name.name();   

    if (isCurrent){		        
        // update the status label if we received an update on the current name
		qDebug() << "replace myCurrentFantasyName with new status";
        myCurrentFantasyName = name;
		switch (myCurrentFantasyName.status()) {
        case fantasybit::confirmed:
            ui->myFantasyNameStatusLabel->setText("Confirmed");
            break;
        case fantasybit::requested:
            ui->myFantasyNameStatusLabel->setText("Requested");
            break;
        case fantasybit::notavil:
            ui->myFantasyNameStatusLabel->setText("Not Available");
            break;
        case fantasybit::transaction_sent:
            ui->myFantasyNameStatusLabel->setText("Transaction sent");
            break;		
        case fantasybit::none:
            ui->myFantasyNameStatusLabel->setText("none");
            break;
        }
		DataCache::instance()->refreshLeaderboard();
		QString fname = myCurrentFantasyName.name().data();
		QVariant balance;
		DataCache::instance()->leaderBoardModel().itemPropertyValue<PropertyNames::Balance>(fname, balance);
		ui->myBalanceText->setText(QString("%1").arg(balance.toDouble()));
    }
}

void MainWindow::OnProjAck(fantasybit::FantasyBitProj projection){
    qDebug() << "OnProjAck :" << projection.DebugString();
    if (projection.name()==myCurrentFantasyName.name())
        ui->myCurrentWeekWidget->OnProjAck(projection);
}

void MainWindow::OnNameBalance(fantasybit::FantasyNameBal balance) {
    qDebug()<< "received balance for " << balance.name() << " : " << balance.bits();
    if (myCurrentFantasyName.name()==balance.name())
        ui->myBalanceText->setText(QString("%1").arg(balance.bits()));
    DataCache::instance()->refreshLeaderboard();
}

void MainWindow::OnNewWeek(int week){
    qDebug() << "OnNewWeek :" << week;
    DataCache::instance()->refreshLeaderboard();
}

void MainWindow::OnGameOver(string gameId){
    qDebug() << "OnGameOver :" << gameId;    
}

void MainWindow::OnGameStart(string gameId){
    ui->myCurrentWeekWidget->onGameStart(gameId);
}


void MainWindow::on_myFantasyNamesCombo_currentIndexChanged(int index)
{	
	if (myAddNamesPending) {
		qDebug() << "Adding name is pending leave index change event";
		return;
	}	
    QString fantasyNameString = ui->myFantasyNamesCombo->itemText(index);
    MyFantasyName * selectedName = myFantasyNames.value(fantasyNameString);
	qDebug() << "combox box text changed switching to" << fantasyNameString;
    setCurrentFantasyName(selectedName,true);
} 

void MainWindow::on_myClaimFantasyNameButton_clicked()
{
    QString name = ui->myClamNewNameLE->text().trimmed();
    if (name.isEmpty()) return;

	if (myFantasyNames.keys().contains(name)){
		QMessageBox::warning(this, APPLICATION_NAME, QString("You have already claimed the name : %1").arg(name));
		return;
	}

	qDebug() << "Claiming new name " << name;
	ui->myClamNewNameLE->setText("");
    MyFantasyName * newName = new MyFantasyName();
	qDebug() << "creating new MyFantasyName "<< name << " and forcing its status to requested";
    newName->set_status(fantasybit::requested);
    newName->set_name(name.toStdString());
	qDebug() << "Adding new MyFantasyName to the myFantasyNamesCombo map";
    myFantasyNames.insert(name,newName);
    myAddNamesPending = true;
	qDebug() << "Adding new MyFantasyName to combobox";
    ui->myFantasyNamesCombo->addItem(name);
	ui->myFantasyNamesCombo->setCurrentText(name);
	myAddNamesPending = false;
	qDebug() << "emit ClaimFantasyName(" << name << ")";
	emit ClaimFantasyName(name);
    setCurrentFantasyName(newName,false);	
}

void MainWindow::OnPlayerStatusChange(pair<string, fantasybit::PlayerStatus> in){
   ui->myCurrentWeekWidget->OnPlayerStatusChange(in);
}

void MainWindow::onSendFantasyNameProjection(QString fantasyName){
    ui->myCurrentWeekWidget->onSendFantasyNameProjection(fantasyName.toStdString());
}


void MainWindow::leaderboardCliked(const QModelIndex &index){
    LeaderBoardTableModel * leaderboardModel = & DataCache::instance()->leaderBoardModel();
    QString currentFantasyName = QString(myCurrentFantasyName.name().data());
    if (leaderboardModel!=NULL){
        ViewModel * data =leaderboardModel->getItemByIndex(index);
        if (data !=NULL) {
          QString fantasyName = data->propertyValue<QString,PropertyNames::Fantasy_Name>();
          if (fantasyName == currentFantasyName) return;
          ui->myCurrentWeekWidget->toggleFantasyNameColumn(fantasyName);
        }
    }
}

void MainWindow::showLeaderboardContextualMenu(const QPoint & point){
    // we're not in the current week or in offseason
    if (myGlobalState.state()== fantasybit::GlobalState_State_OFFSEASON) return;
    if (myGlobalState.week()!=myCurrentWeek) return;
    if (ui->myCurrentWeekWidget->gamesFilter()==GamesFilter::OpenGames)
        if (ui->myCurrentWeekWidget->gameCount() ==0)
            return;
    QModelIndex index = ui->myLeaderBaordTableView->indexAt(point);
    if (!index.isValid()) return;
    ViewModel * item = DataCache::instance()->leaderBoardModel().getItemByIndex(index);
    if (item==NULL) return;
    QString fantasyName =item->propertyValue<QString,PropertyNames::Fantasy_Name>();
     QString currentFantasyName = QString(myCurrentFantasyName.name().data());
    if (fantasyName == currentFantasyName) return;
    int result = QMessageBox::Cancel;

    QMessageBox msgBox;
    msgBox.setIcon(QMessageBox::NoIcon);
    msgBox.setWindowTitle(APPLICATION_NAME);
    QString text = QString("Copy %1's projections and send them as yours %2 ?").arg(fantasyName).arg(currentFantasyName);
    //QCheckBox * chb = new QCheckBox("Do not show this again");
    //msgBox.setCheckBox(chb);
    msgBox.setText(text);
    QFont font = msgBox.font();
    font.setBold(true);
    //font.setPointSize(font.pointSize()*1.2);
    msgBox.setFont(font);
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::Cancel);
    msgBox.setButtonText(QMessageBox::Yes,"Send");
    msgBox.setDefaultButton(QMessageBox::Yes);
    result = msgBox.exec();

    if (result== QMessageBox::Yes)
        ui->myCurrentWeekWidget->onSendFantasyNameProjection(fantasyName.toStdString());
}

void MainWindow::setCurrentFantasyName(fantasybit::MyFantasyName * fantasyName,bool useName){
	qDebug() << "switch to new fantasy Name :" << 
        QString(fantasyName->name().data()) <<
        " with status :" << translateNameStatus(fantasyName->status());
	
    myCurrentFantasyName.CopyFrom(*fantasyName);

	//update status label
	qDebug() << "updating status label";
	switch (myCurrentFantasyName.status()) {
	case fantasybit::confirmed:
		ui->myFantasyNameStatusLabel->setText("Confirmed");
		break;
	case fantasybit::requested:
		ui->myFantasyNameStatusLabel->setText("Requested");
		break;
	case fantasybit::notavil:
		ui->myFantasyNameStatusLabel->setText("Not Available");
		break;
	case fantasybit::none:
		ui->myFantasyNameStatusLabel->setText("None");
		break;
	default:
		break;
	}

	//update balance
	qDebug() << "updating status balance";
	QVariant balance;
	QString fname = myCurrentFantasyName.name().data();
	DataCache::instance()->refreshLeaderboard();
	DataCache::instance()->leaderBoardModel().itemPropertyValue<PropertyNames::Balance>(fname, balance);
	ui->myBalanceText->setText(QString("%1").arg(balance.toDouble()));
	qDebug() << "updating current week widget according to new name";	
	ui->myCurrentWeekWidget->onUserSwitchFantasyName(myCurrentFantasyName.name());
	qDebug() << "UseMyFantasyName " << QString(myCurrentFantasyName.name().data());
	if (useName)
		emit UseMyFantasyName(QString(myCurrentFantasyName.name().data()));	
}


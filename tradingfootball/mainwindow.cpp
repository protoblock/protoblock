#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "LAPIWorker.h"
#include "core.h"
#include "dataservice.h"
#include "datacache.h"
#include <QCheckBox>
#include <QSettings>
#include "fnametool.h"
#include "trading.h"

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
    ui->myFantasyNamesCombo->setContextMenuPolicy(Qt::CustomContextMenu);

    //ui->tabWidget->setCurrentWidget(ui->level2);

    ui->tradingview->Init();
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
    myWaitDialog.init(":/images/footballspin.gif",3600000,"Syncing...");
    QObject::connect(myLAPIWorker,SIGNAL(Height(int)),&myWaitDialog,SLOT(Height(int)));
    QObject::connect(myLAPIWorker,SIGNAL(BlockNum(int)),&myWaitDialog,SLOT(BlockNum(int)));

    QObject::connect(myLAPIWorker,SIGNAL(LiveGui(fantasybit::GlobalState)),&myWaitDialog,SLOT(stopAndClose()));

    //leader timed refresh
    myLeaderBoardTimer.setInterval(AppSettings::instance()->getSetting(AppSettings::LeaderBoardRefreshInterval).toInt()*60000);
    QObject::connect(&myLeaderBoardTimer,SIGNAL(timeout()),
                     this,SLOT(refreshLeaderBoard()));

    QObject::connect(ui->myLeaderBaordTableView,SIGNAL(doubleClicked(QModelIndex)),
                     this,SLOT(leaderboardCliked(QModelIndex)));
    QObject::connect(ui->myLeaderBaordTableView,SIGNAL(customContextMenuRequested(QPoint)),
                     this,SLOT(showLeaderboardContextualMenu(QPoint)));

    QObject::connect(ui->myFantasyNamesCombo,SIGNAL(customContextMenuRequested(QPoint)),
                     this,SLOT(myFantasyNamesImportExport(QPoint)));

    QObject::connect(myLAPIWorker,SIGNAL(onControlMessage(QString)),
                     ui->myCurrentWeekWidget,SLOT(onControlMessage(QString)));

    /*
    QObject::connect(ui->tradingview,SIGNAL(SendOrder(fantasybit::ExchangeOrder)),
                      myLAPIWorker,SLOT(OnNewOrder(fantasybit::ExchangeOrder)));
    */

    //wake up core thread/
    QObject::connect(ui->tradingview,SIGNAL(OnPlayerPosTeam(QString,QString,QString)),
                     this,SLOT(UpdateTab(QString,QString,QString)));

    Core::instance()->guiIsAwake();
#ifndef DATAAGENTGUI
    myWaitDialog.startExec();
#endif
    if (myLAPIWorker == NULL)  {
        qDebug() << "coreapi is not resolved";
        setDisabled(true);
        return;
    }


    QSize qsize = ui->myCurrentWeekWidget->fixSize();
    //ui->myCurrentWeekWidget->adjustSize();
    QString qs2 = "XXXXXXXXXXXX";
    QString qs = "FantasyName Confirmed FantasyBits ";

    //QSize qsize = ui->leaderboardBox->size();
    int width = 0;
    QLabel *label = ui->myFantasyNameLabel;
    //width += label->fontMetrics().boundingRect(qs).width();
    width = label->fontMetrics().width(qs);
    QSize qsz2;
    qsz2.setHeight(qsize.height());
    qsz2.setWidth(width);
    ui->leaderboardBox->setBaseSize(qsz2);
    qsize.setHeight(qsize.width()+width);
    /*label = ui->myFantasyNameStatusLabel;
    width += label->fontMetrics().boundingRect(label->text()).width();
    label = ui->myFantasyBitsLabel;
    width += label->fontMetrics().boundingRect(label->text()).width();
    qsize.setWidth(width);
    */
    width += 2*label->fontMetrics().width(qs2);
    ui->leaderboardBox->setMaximumWidth(width);
    qsize.setWidth(width);
    //ui->leaderboardBox->SE(qsize);
    //ui->myCurrentWeekView->adjustSize();
    //ui->myStackedWidget->adjustSize();
    //this->adjustSize();

    ui->level1->resize(qsize);
    ui->tradingview->resize(qsize);
    ui->tabWidget->adjustSize();
    ui->tabWidget->setAutoFillBackground(true);
    ui->tabWidget->widget(1)->setAutoFillBackground(true);
    ui->centralwidget->adjustSize();
    this->adjustSize();
    //this->showMaximized();
    width = ui->mySeasonLabel->fontMetrics().width("2015");

    ui->myPreviousWeek->setMinimumHeight(width);
    ui->myPreviousWeek->setMinimumWidth(width);

    ui->myNextWeek->setMinimumHeight(width);
    ui->myNextWeek->setMinimumWidth(width);

    setSlider(sliderright);

}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::UpdateTab(QString player,QString team,QString pos) {
    ui->tabWidget->setTabIcon(1,QIcon(QPixmap(Trading::icons[team.toStdString()].data())));
    ui->tabWidget->setTabText(1,QString("%1 (%2) - trading").arg(player,pos));
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
    if ( ui->myStackedWidget->currentWidget() == ui->myPreviousWeekView)
        emit ui->myStackedWidget->currentChanged(ui->myStackedWidget->currentIndex());
    else
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
    ui->myLeaderBaordTableView->resizeColumnsToContents();
    ui->myLeaderBaordTableView->horizontalHeader()->
            setSectionResizeMode(QHeaderView::Stretch);

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
    ui->myNextWeek->setDisabled(myCurrentWeek==16);
    if (myCurrentWeek==myGlobalState.week())
        currentWeek();
    else if (myCurrentWeek < myGlobalState.week())
        previousWeek();
    else
        nextWeek();

    //this->adjustSize();

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
            ui->myFantasyNamesCombo->setCurrentIndex(names.size()-1);
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
        ui->tradingview->SetFantasyName(myCurrentFantasyName.name(),balance.toInt());

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
    QString text;

    if ( ui->myCurrentWeekWidget->doMerge)
        text = QString("Merge %1's projections with yours %2?").arg(fantasyName).arg(currentFantasyName);
    else
        text = QString("Clone all %1's projections and aend them as yours %2 ?").arg(fantasyName).arg(currentFantasyName);
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

    ui->tradingview->SetFantasyName(fantasyName->name(),balance.toInt());

}


void MainWindow::myFantasyNamesImportExport(const QPoint &pos)
{
    doImportExport();
}

void MainWindow::doImportExport() {
    if ( !myIamLive ) return;

    fnametool fnt(this);
    fnt.setModal(true);
    fnt.initialize();
    fnt.UseName(ui->myFantasyNamesCombo->currentText());
    fnt.exec();
    //fnt.set
    auto newname = fnt.newName();
    if ( newname.status() != MyNameStatus::none ) {
        setCurrentFantasyName(&newname,false);
        int index = ui->myFantasyNamesCombo->findText(newname.name().data());
        if ( index > -1 )
            ui->myFantasyNamesCombo->setCurrentIndex(index);
    }
}

void MainWindow::on_actionFantasyName_Import_Export_triggered()
{   
   doImportExport();

}

void MainWindow::on_toolButton_clicked()
{
    doImportExport();
}

/*
void MainWindow::on_horizontalSlider_sliderPressed()
{
    sliderPressed = true;
    sliderReleased = false;
}
*/

/*
void MainWindow::on_horizontalSlider_sliderMoved(int position) {
    return;
    if ( sliderPressed || !sliderReleased) {
        sliderPressed = false;
        if ( sliderright && position < ui->horizontalSlider->maximum())
            setSlider(ui->horizontalSlider->minimum());
        else if ( !sliderright && position > ui->horizontalSlider->minimum())
            setSlider(ui->horizontalSlider->maximum());
    }
}
*/
void MainWindow::setSlider(bool right)
{
    //return;
    //if ( changingSlider )
   changingSlider = true;
    //sliderReleased = true;
    QPalette onp;
    onp.setColor(QPalette::Window, Qt::darkGreen);
    onp.setColor(QPalette::WindowText, Qt::white);

    QPalette offp;
    offp.setColor(QPalette::Window, Qt::darkGray);
    offp.setColor(QPalette::WindowText, Qt::gray);

    auto onl = ui->clonel;
    auto offl = ui->mergel;

    if ( right ) {
        sliderright = true;
        ui->horizontalSlider->setValue(ui->horizontalSlider->minimum());
        ui->horizontalSlider->setSliderPosition(ui->horizontalSlider->minimum());

        onl = ui->mergel;
        offl = ui->clonel;
        ui->myCurrentWeekWidget->doMerge = true;
    }
    else {
        sliderright = false;
        ui->horizontalSlider->setSliderPosition(ui->horizontalSlider->maximum());
        ui->horizontalSlider->setValue(ui->horizontalSlider->maximum());
        ui->myCurrentWeekWidget->doMerge = false;
    }

    onl->setAutoFillBackground(true);
    offl->setAutoFillBackground(true);
    onl->setPalette(onp);
    offl->setPalette(offp);
    changingSlider = false;
}

void MainWindow::on_horizontalSlider_valueChanged(int value)
{
    if ( !sliderright && value < ui->horizontalSlider->maximum())
        setSlider(true);
    else if ( sliderright && value > ui->horizontalSlider->minimum())
        setSlider(false);

    return;

    qDebug() << " changed value " << value;
    return;
    if ( changingSlider ) return;

    if ( !sliderright && value < ui->horizontalSlider->maximum())
        setSlider(true);
    else if ( sliderright && value > ui->horizontalSlider->minimum())
        setSlider(false);

    return;
    if ( sliderReleased ) {
        if ( sliderright && value == ui->horizontalSlider->maximum()  )
            sliderReleased = false;
        if ( !sliderright && value == ui->horizontalSlider->minimum()  )
            sliderReleased = false;
        return;
    }
    else {
        if ( sliderright && value > ui->horizontalSlider->minimum()  )
            sliderReleased = true;
        if ( !sliderright && value < ui->horizontalSlider->maximum()  )
            sliderReleased = true;

        if ( !sliderright && value < ui->horizontalSlider->maximum())
            setSlider(true);
        else if ( sliderright && value > ui->horizontalSlider->minimum())
            setSlider(false);
    }

//    if ( changingSlider ) return;
//        on_horizontalSlider_sliderMoved(value);
}

/*
void MainWindow::on_horizontalSlider_sliderReleased()
{
    setSlider(!sliderright);
    return;
}
*/
            /*
    if ( !sliderright && value  ui->horizontalSlider->maximum())
        setSlider(true);
    else if ( sliderright && value > ui->horizontalSlider->minimum())
        setSlider(false);

    return;
    sliderReleased = true;
*/
    //return;
    //if ( sliderPressed ) {
    //    sliderPressed = false;

    //if ( changingSlider ) return;
    //if ( ui->horizontalSlider->value() == 1)
        //setSlider(!sliderright);
    //else
    //    setSlider(1);


/*
void MainWindow::on_horizontalSlider_actionTriggered(int action)
{
    return;
    if ( !sliderright && ui->horizontalSlider->sliderPosition()
         < ui->horizontalSlider->maximum())
        setSlider(true);
    else if ( sliderright && ui->horizontalSlider->sliderPosition()
              > ui->horizontalSlider->minimum())
        setSlider(false);
}
*/


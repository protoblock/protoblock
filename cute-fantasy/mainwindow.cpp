#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "LAPIWorker.h"
#include "core.h"
#include "dataservice.h"
#include "datacache.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{    
    ui->setupUi(this);
    initDefaultGuiDisplay();
    initialize();
}

void MainWindow::initDefaultGuiDisplay(){
    ui->myFantasyNameStatusLabel->setText("None");
    ui->myBalanceText->setText("N/A");
    ui->mySeasonLabel->setText("Unknown Season");
    ui->myWeekLabel->setText("Unknown Week #");
    ui->myPreviousWeek->setEnabled(false);
    ui->myNextWeek->setEnabled(false);
    ui->myStackedWidget->setCurrentWidget(ui->myCurrentWeekWidget);
    myCurrentWeek =-1;
    ui->myLeaderBaordTableView->setModel(&DataCache::instance()->leaderBoardModel());
    //ui->myStatusbar->addWidget(); TODO animated label
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
    QObject::connect(myLAPIWorker,SIGNAL(NameBal(fantasybit::FantasyNameBal &)),
                     this,SLOT(OnNameBalance(fantasybit::FantasyNameBal &)));

    //state
    //QObject::connect(this,SIGNAL(SubscribeGameState()),myLAPIWorker,SLOT(OnSubGame()));

    QObject::connect(myLAPIWorker,SIGNAL(NewWeek(int)),this,SLOT(OnNewWeek(int)));
    QObject::connect(myLAPIWorker,SIGNAL(GameOver(string)),this,SLOT(OnGameOver(string)));
    QObject::connect(myLAPIWorker,SIGNAL(GameStart(string)),this,SLOT(OnGameStart(string)));


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

    //PUT
    QObject::connect(this,SIGNAL(NewProjection(fantasybit::FantasyBitProj)),
                     myLAPIWorker,SLOT(OnProjTX(fantasybit::FantasyBitProj)));

    QObject::connect(this,SIGNAL(ClaimFantasyName(QString)),myLAPIWorker,SLOT(OnClaimName(QString)));

    //wait for going live
    myWaitDialog.init(":/icons/waitingprogress.gif",120000,"Syncing...");
    QObject::connect(myLAPIWorker,SIGNAL(LiveGui(fantasybit::GlobalState)),&myWaitDialog,SLOT(stopAndClose()));
    //wake up core thread
    Core::instance()->guiIsAwake();
    //myWaitDialog.startExec();
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
    ui->myStackedWidget->setCurrentWidget(ui->myNexWeekView);
    ui->myNextWeekWidget->setWeekData(myCurrentWeek);
}

void MainWindow::previousWeek() {    
    ui->myStackedWidget->setCurrentWidget(ui->myPreviousWeekView);    
    ui->myPreviousWeekWidget->setWeekData(myCurrentWeek);
}

void MainWindow::currentWeek() { 
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
}

void MainWindow::GoLive(fantasybit::GlobalState state){
   myGlobalState = state;   
   QString seasonLabel = "%1 %2";
   QString seasonType;
   switch (state.state() ) {
     case GlobalState_State_INSEASON :
          seasonType ="in Season";
       break;
   case GlobalState_State_OFFSEASON :
       seasonType = "Off Season";
       break;
   default:
       break;
   }
   seasonLabel = seasonLabel.arg(seasonType).arg(myGlobalState.season());
   ui->mySeasonLabel->setText(seasonLabel);

   //load roaster for current week
   if ( myGlobalState.week() > 0) {
        ui->myCurrentWeekWidget->setCurrentWeekData(myGlobalState);
        navigateToWeek(myGlobalState.week());
   }

   DataCache::instance()->refreshLeaderboard();

}
void MainWindow::navigateToWeek(int week)
{
    myCurrentWeek = week;
    QString currentWeekNotice = myGlobalState.week()==myCurrentWeek?" (Current week) ":"";
    ui->myWeekLabel->setText(QString("Week #%1").arg(myCurrentWeek)+currentWeekNotice);
    ui->myPreviousWeek->setDisabled(myCurrentWeek==1);
    ui->myNextWeek->setDisabled(myCurrentWeek==12);
    if (myCurrentWeek==myGlobalState.week())
        currentWeek();
    else if (myCurrentWeek < myGlobalState.week())
         previousWeek();
    else
        nextWeek();
}

void MainWindow::OnMyFantasyNames(vector<fantasybit::MyFantasyName> names){
   if (names.size()> 0){
    for(int i=0;i<names.size();i++){

       MyFantasyName fName = names.at(i);
       QString stringName = QString(fName.name().data());
       //QVariant itemData = QVariant::fromValue(fName);
       QVariant itemData= qVariantFromValue(fName);
       ui->myFantasyNamesCombo->insertItem(i,stringName,itemData);
    }
    ui->myFantasyNamesCombo->setCurrentIndex(names.size()-1);
   }
}

void MainWindow::OnNameStatus(MyFantasyName name){    

    // get cached FanatasyName from the combox box whicch
    // serves as a model store here using itemData property.
    // this is an exact match case sensitive search
    int nameIndex = ui->myFantasyNamesCombo->findText(QString(name.name().data()));


    //should never happen
    if (nameIndex  == -1){
        qDebug() << "we received a FantasyName status change that doesn't exist in the ui.";
        return;
    }

    fantasybit::MyNameStatus newStatus = name.status();

    MyFantasyName cachedName = ui->myFantasyNamesCombo->itemData(nameIndex).value<MyFantasyName>();
    fantasybit::MyNameStatus oldStatus = cachedName.status();

    //should never happen
    if (newStatus == fantasybit::none ||oldStatus == fantasybit::none){
        qDebug() << "we received an unexpected FantasyName status : " << newStatus;
        return;
    }


    bool isCurrent = myCurrentFantasyName.name()== name.name();

    //always update the cached FantasyName in th combo
    ui->myFantasyNamesCombo->setItemData(nameIndex,QVariant::fromValue(name));

    if (isCurrent){

        // update the status label if we received an update on the current name
        // and update the  class member myCurrentFantasyName
        switch (newStatus) {
        case fantasybit::confirmed:
            ui->myFantasyNameStatusLabel->setText("Confirmed");
            break;
        case fantasybit::requested:
            ui->myFantasyNameStatusLabel->setText("Requested");
            break;
        case fantasybit::notavil:
            ui->myFantasyNameStatusLabel->setText("Not Available");
            break;
        default:
            myCurrentFantasyName = name;
            break;
        }

        if (oldStatus == fantasybit::requested && newStatus == fantasybit::confirmed){}

        //update last player projections
        if (oldStatus == fantasybit::none && newStatus == fantasybit::confirmed){
            ui->myCurrentWeekWidget->onUserSwitchFantasyName(myCurrentFantasyName.name());
        }
    }
}


void MainWindow::OnProjAck(fantasybit::FantasyBitProj){

}

void MainWindow::OnNameBalance(fantasybit::FantasyNameBal &) {

}

void MainWindow::OnNewWeek(int){

}

void MainWindow::OnGameOver(string){

}

void MainWindow::OnGameStart(string){

}


void MainWindow::on_myFantasyNamesCombo_currentIndexChanged(int index)
{
    QVariant vFname = ui->myFantasyNamesCombo->itemData(index);
    MyFantasyName choosenFantasyName = vFname.value<MyFantasyName>();
    if (choosenFantasyName.name() == myCurrentFantasyName.name()) return;
    myCurrentFantasyName = choosenFantasyName;
    //update status label
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
    ui->myCurrentWeekWidget->onUserSwitchFantasyName(myCurrentFantasyName.name());
    emit UseMyFantasyName(QString(myCurrentFantasyName.name().data()));
}

void MainWindow::on_myFantasyNamesCombo_currentTextChanged(const QString &name)
{
    if (myAddNamesPending) return;
}

void MainWindow::on_myFantasyNamesCombo_editTextChanged(const QString &name)
{
    if (myAddNamesPending) return;
    myAddNamesPending = true;
    QVariant cachedName = ui->myFantasyNamesCombo->itemData(ui->myFantasyNamesCombo->currentIndex());
    if (!cachedName.isValid()){
        MyFantasyName newName;
        newName.set_status(fantasybit::none);
        newName.set_name(name.toStdString());
        ui->myFantasyNamesCombo->setItemData(ui->myFantasyNamesCombo->currentIndex(),QVariant::fromValue(newName));
        emit ClaimFantasyName(name);
    }
    myAddNamesPending = false;
}

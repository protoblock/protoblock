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
    initialize();
}

void MainWindow::initialize() {
    myCoreInstance = Core::resolveByName<MainLAPIWorker>("coreapi");
    //wake up core thread
    Core::instance()->guiIsAwake();
    if (myCoreInstance == NULL)  {
        qDebug() << "coreapi is not resolved";
        setDisabled(true);
        return;
    }

    QObject::connect(myCoreInstance,SIGNAL(LiveGui(GlobalState)),
                     this,SLOT(GoLive(GlobalState)));

    QObject::connect(myCoreInstance,SIGNAL(MyNames(vector<MyFantasyName>)),
                     this,SLOT(OnMyFantasyNames(vector<MyFantasyName>)));


    QObject::connect(this,SIGNAL(UseMyFantasyName(QString)),myCoreInstance,SLOT(OnUseName(QString)));
    QObject::connect(this,SIGNAL(SubscribeMyNameTx(QString)),myCoreInstance,SLOT(OnSubName(QString)));
    QObject::connect(this,SIGNAL(SubscribeMyProjTx(QString)),myCoreInstance,SLOT(OnSubProj(QString)));
    QObject::connect(this,SIGNAL(SubscribeAwards(QString)),myCoreInstance,SLOT(OnSubBits(QString)));

    QObject::connect(myCoreInstance,SIGNAL(NameStatus(MyFantasyName)),
                     this,SLOT(OnNameStatus(MyFantasyName)));
    QObject::connect(myCoreInstance,SIGNAL(LiveProj(FantasyBitProj)),
                     this,SLOT(OnProjAck(FantasyBitProj)));
    QObject::connect(myCoreInstance,SIGNAL(NewAward(QVariant)),this,SLOT(OnAward(QVariant)));

    //state
    QObject::connect(this,SIGNAL(SubscribeGameState()),myCoreInstance,SLOT(OnSubGame()));

    QObject::connect(myCoreInstance,SIGNAL(NewWeek(int)),this,SLOT(OnNewWeek(int)));
    QObject::connect(myCoreInstance,SIGNAL(GameOver(string)),this,SLOT(OnGameOver(string)));
    QObject::connect(myCoreInstance,SIGNAL(GameStart(string)),this,SLOT(OnGameStart(string)));


    //data
    QObject::connect(this,SIGNAL(SubscribePlayerData()),myCoreInstance,SLOT(OnSubPD()));
    QObject::connect(this,SIGNAL(SubscribeScheduleData()),myCoreInstance,SLOT(OnSubSS()));

    QObject::connect(this,SIGNAL(SubscribeTeamRoster()),myCoreInstance,SLOT(OnSubTeams()));
    QObject::connect(this,SIGNAL(SubscribePlayerGameStatus()),myCoreInstance,SLOT(OnSubOut()));

//    QObject::connect(myCoreInstance,SIGNAL(PlayerData(QString)),this,SLOT(OnPlayerUpdate(QString)));
//    QObject::connect(myCoreInstance,SIGNAL(TimeChange(QString)),this,SLOT(OnSchedule(QString)));
//    QObject::connect(myCoreInstance,SIGNAL(TeamActicity(QString)),this,SLOT(OnTeamRoster(QString)));


    //GET
//    QObject::connect(this,SIGNAL(GetPrevWeekData(int)),myCoreInstance,SLOT(SendWeeklyData(int)));
//    QObject::connect(this,SIGNAL(RefershLive(int)),myCoreInstance,SLOT(SendLiveSnap(int)));

//    QObject::connect(this,SIGNAL(GetLiveProjection(QString)),myCoreInstance,SLOT(SendCurrentProj(QString)));
//    QObject::connect(this,SIGNAL(RefershLive(int)),myCoreInstance,SLOT(SendLiveSnap(int)));

    //PUT
    QObject::connect(this,SIGNAL(NewProjection(FantasyBitProj)),
                     myCoreInstance,SLOT(OnProjTX(FantasyBitProj)));

    QObject::connect(this,SIGNAL(ClaimFantasyName(QString)),myCoreInstance,SLOT(OnClaimName(QString)));
}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_myNextWeek_clicked()
{
    switch (myCurrentWeekViewMode) {
    case CurrentWeek:
        nextWeek();
        ui->myNextWeek->setDisabled(true);
        break;
    case PreviousWeek:
        ui->myPreviousWeek->setDisabled(false);
        currentWeek();
        break;
    default:
        break;
    }
}

void MainWindow::nextWeek(){
    myCurrentWeekViewMode = NextWeek;
    ui->myStackedWidget->setCurrentWidget(ui->myNexWeekView);
}

void MainWindow::previousWeek() {
    myCurrentWeekViewMode = PreviousWeek;
    ui->myStackedWidget->setCurrentWidget(ui->myPreviousWeekView);
}

void MainWindow::currentWeek() {
  myCurrentWeekViewMode = CurrentWeek;
  ui->myStackedWidget->setCurrentWidget(ui->myCurrentWeekView);
  //TODO
}
void MainWindow::setWeekViewMode(WeekViewMode  viewMode){
    switch (viewMode) {
    case CurrentWeek:
        currentWeek();
        break;
    case PreviousWeek:
        previousWeek();
        break;
    case NextWeek:
        nextWeek();
        break;
    default:
        break;
    }
}

void MainWindow::on_myPreviousWeek_clicked()
{
    switch (myCurrentWeekViewMode) {
    case CurrentWeek:
        previousWeek();
        ui->myPreviousWeek->setDisabled(true);
        break;
    case NextWeek:
        ui->myNextWeek->setDisabled(false);
        currentWeek();
        break;
    default:
        break;
    }
} 


void MainWindow::GoLive(fantasybit::GlobalState state){
   myGlobalState = state;
}

void MainWindow::OnMyFantasyNames(vector<MyFantasyName> names){
   if (names.size()> 0){
    myCurrentFantasyName = names.at(names.size()-1);
    ui->myCurrentWeekWidget->setCurrentWeekData(myGlobalState,myCurrentFantasyName.name());
    emit UseMyFantasyName(myCurrentFantasyName.name().data());
   }
}

void MainWindow::OnNameStatus(MyFantasyName name){
//  myCurrentFantasyName = name;
  //TODO myCurrentFantasyName.status()
}


void MainWindow::OnProjAck(fantasybit::FantasyBitProj){

}

void MainWindow::OnAward(QVariant){

}

void MainWindow::OnNewWeek(int){

}

void MainWindow::OnGameOver(QString){

}

void MainWindow::OnGameStart(QString){

}


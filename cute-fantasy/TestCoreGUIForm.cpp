#include "TestCoreGUIForm.h"
#include "ui_TestCoreGUIForm.h"
#include <QWaitCondition>


TestCoreGUIForm::TestCoreGUIForm(MainLAPIWorker *coreInstance, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TestCoreGUIForm)
{
    qRegisterMetaType<fantasybit::GlobalState>("GlobalState");
    qRegisterMetaType<fantasybit::MyFantasyName>("MyFantasyName");
    qRegisterMetaType<fantasybit::FantasyBitProj>("FantasyBitProj");
    qRegisterMetaType<std::vector<fantasybit::MyFantasyName>>("Vector_MyFantasyName");


    ui->setupUi(this);
    //notify the APIThread that WE the main form are alive
    Core::waitForGUI.wakeAll();
    myCoreInstance = coreInstance;
    //connect the png request to the write slot that handles it
    QObject::connect(this,SIGNAL(requestPong(QVariant)),myCoreInstance,SLOT(processGUIRequest(QVariant)));
    //listen to notifcation from Core
    QObject::connect(myCoreInstance,SIGNAL(sendNotificationWithData(QVariant)),this,SLOT(handleNotificationOrResponse(QVariant)));
    QObject::connect(this,SIGNAL(requestPlayersForWeek(int)),myCoreInstance,SLOT(getPlayers(int)));

    QObject::connect(myCoreInstance,SIGNAL(OnLive()),this,SLOT(GoLive()));
    QObject::connect(myCoreInstance,SIGNAL(OnData(DeltaData)),this,SLOT(NewData(DeltaData)));


    //name
    QObject::connect(this,SIGNAL(GetMyFantasyNames()),myCoreInstance,SLOT(OnGetMyNames()));
    QObject::connect(myCoreInstance,SIGNAL(OnMyNames(std::vector<fantasybit::MyFantasyName> &)),
                     this,SLOT(OnMyFantasyNames(std::vector<fantasybit::MyFantasyName> &)));

    QObject::connect(this,SIGNAL(UseMyFantasyName(QString)),myCoreInstance,SLOT(OnUseName(QString)));
    QObject::connect(this,SIGNAL(SubscribeMyNameTx(QString)),myCoreInstance,SLOT(OnSubName(QString)));
    QObject::connect(this,SIGNAL(SubscribeMyProjTx(QString)),myCoreInstance,SLOT(OnSubProj(QString)));
    QObject::connect(this,SIGNAL(SubscribeAwards(QString)),myCoreInstance,SLOT(OnSubBits(QString)));

    QObject::connect(myCoreInstance,SIGNAL(NameStatus(fantasybit::MyFantasyName)),
                     this,SLOT(OnNameStaus(fantasybit::MyFantasyName)));
    QObject::connect(myCoreInstance,SIGNAL(LiveProj(fantasybit::FantasyBitProj)),
                     this,SLOT(OnProjAck(fantasybit::FantasyBitProj)));
    QObject::connect(myCoreInstance,SIGNAL(NewAward(QVariant)),this,SLOT(OnAward(QVariant)));

    //state
    QObject::connect(this,SIGNAL(SubscribeGameState()),myCoreInstance,SLOT(OnSubGame()));

    QObject::connect(myCoreInstance,SIGNAL(NewWeek(int)),this,SLOT(OnNewWeek(int)));
    QObject::connect(myCoreInstance,SIGNAL(GameOver(QString)),this,SLOT(OnGameOver(QString)));
    QObject::connect(myCoreInstance,SIGNAL(GameStart(QString)),this,SLOT(OnGameStart(QString)));


    //data
    QObject::connect(this,SIGNAL(SubscribePlayerData()),myCoreInstance,SLOT(OnSubPD()));
    QObject::connect(this,SIGNAL(SubscribeScheduleData()),myCoreInstance,SLOT(OnSubSS()));

    QObject::connect(this,SIGNAL(SubscribeTeamRoster()),myCoreInstance,SLOT(OnSubTeams()));
    QObject::connect(this,SIGNAL(SubscribePlayerGameStatus()),myCoreInstance,SLOT(OnSubOut()));

    QObject::connect(myCoreInstance,SIGNAL(PlayerData(QString)),this,SLOT(OnPlayerUpdate(QString)));
    QObject::connect(myCoreInstance,SIGNAL(TimeChange(QString)),this,SLOT(OnSchedule(QString)));
    QObject::connect(myCoreInstance,SIGNAL(TeamActicity(QString)),this,SLOT(OnTeamRoster(QString)));


    //GET
    QObject::connect(this,SIGNAL(GetPrevWeekData(int)),myCoreInstance,SLOT(SendWeeklyData(int)));
    QObject::connect(this,SIGNAL(RefershLive(int)),myCoreInstance,SLOT(SendLiveSnap(int)));

    QObject::connect(this,SIGNAL(GetLiveProjection(QString)),myCoreInstance,SLOT(SendCurrentProj(QString)));
    QObject::connect(this,SIGNAL(RefershLive(int)),myCoreInstance,SLOT(SendLiveSnap(int)));

    //PUT
    QObject::connect(this,SIGNAL(NewProjection(fantasybit::FantasyBitProj)),
                     myCoreInstance,SLOT(OnProjTX(fantasybit::FantasyBitProj)));

    QObject::connect(this,SIGNAL(ClaimFantasyName(QString)),myCoreInstance,SLOT(OnClaimName(QString)));



}

TestCoreGUIForm::~TestCoreGUIForm()
{
    delete ui;
}

void TestCoreGUIForm::on_pushButton_clicked()
{
 emit requestPong(QString("PING"));
// we expect pong later
}

void TestCoreGUIForm::GoLive() {

}

void TestCoreGUIForm::NewData(const fantasybit::DeltaData &) {

}


void TestCoreGUIForm::handleNotificationOrResponse(const QVariant & data){
    switch (data.type()) {
    case  QVariant::String:
        ui->textBrowser->append(QString("we received a notif w/string %1").arg(data.toString()));
        break;
    case  QVariant::Int:
        ui->textBrowser->append(QString("we received a notif w/an integer %1").arg(data.toInt()));
        break;
    case  QVariant::ByteArray:
        ui->textBrowser->append(QString("we received a notif w/an byte array %1").arg(QString(data.toByteArray())));
        break;
    default:
        ui->textBrowser->append(QString("we received a notif w/ data type %1").arg(data.typeName()));
        break;
    }
}

void TestCoreGUIForm::on_pushButton_2_clicked()
{
 emit requestPlayersForWeek(1);
}

void TestCoreGUIForm::OnMyFantasyNames(std::vector<fantasybit::MyFantasyName> &) {
}

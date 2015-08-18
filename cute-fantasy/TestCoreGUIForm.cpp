#include "TestCoreGUIForm.h"
#include "ui_TestCoreGUIForm.h"
#include <QWaitCondition>


TestCoreGUIForm::TestCoreGUIForm(MainLAPIWorker *coreInstance, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TestCoreGUIForm)
{
    qRegisterMetaType<fantasybit::GlobalState>("GlobalState");

    ui->setupUi(this);
    //notify the APIThread that WE the main form are alive
    Core::waitForGUI.wakeAll();
    myCoreInstance = coreInstance;
    //connect the png request to the write slot that handles it
    QObject::connect(this,SIGNAL(requestPong(QVariant)),myCoreInstance,SLOT(processGUIRequest(QVariant)));
    //listen to notifcation from Core
    QObject::connect(myCoreInstance,SIGNAL(sendNotificationWithData(QVariant)),this,SLOT(handleNotificationOrResponse(QVariant)));
    QObject::connect(this,SIGNAL(requestPlayersForWeek(int)),myCoreInstance,SLOT(getPlayers(int)));

    QObject::connect(myCoreInstance,SIGNAL(OnLive(DeltaData)),this,SLOT(GoLive(DeltaData)));
    QObject::connect(myCoreInstance,SIGNAL(OnData(DeltaData)),this,SLOT(NewData(DeltaData)));

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

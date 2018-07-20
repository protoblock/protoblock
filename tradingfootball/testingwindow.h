#ifndef TESTINGWINDOW_H
#define TESTINGWINDOW_H

#include <QMainWindow>
#include "ProtoData.pb.h"
#include <unordered_map>
#include "Data.h"

class PlayerLoaderTR;
namespace Ui {
class TestingWindow;
}
using fantasybit::GlobalState;
using namespace fantasybit;
using namespace std;

class MainLAPIWorker;
class TestingWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit TestingWindow(QWidget *parent = 0);
    ~TestingWindow();

    void initialize();

    QTimer * timer;

    QTimer * tradetimer;

    int32_t mySeq = 0;



signals:
    void UseMyFantasyName(QString);
    void BeOracle();
    void ClaimFantasyName(QString);

public slots:
    void GoLive(fantasybit::GlobalState);
    void OnNameStatus(fantasybit::MyFantasyName);
    //void OnMyFantasyNames(vector<fantasybit::MyFantasyName> );
    void OnNewWeek(int);
    void Timer();
    void TradeTimer();
private slots:
    void on_beoracle_clicked();

    void on_weeks_activated(int index);

    void on_SendBlock_clicked();

    void on_claimname_clicked();

    void on_GetTx_clicked();

    //void on_game_currentIndexChanged(int index);

    void on_game_activated(const QString &arg1);

    //void on_game_activated(const QString &arg1);

    void on_player_activated(const QString &arg1);

    void on_team_activated(const QString &arg1);

    void on_GetGameResult_clicked();

    void on_updatelb_clicked();

    void on_rundataagent_toggled(bool checked);

    void on_StageBlock_clicked();

    void on_nmeonic_clicked();

    void on_MsgButton_clicked();

    void on_stage_player_clicked();

    void on_commit_player_clicked();
/*
    void on_Update_PLayers_2_clicked();
*/
    void on_fix363_clicked();

    void on_FixDef_clicked();

    void on_GetResults4Fix_clicked();

    void on_stage_game_clicked();

    void on_commit_game_clicked();

    void on_cancel_staged_clicked();

private:
    MainLAPIWorker *  myCoreInstance;
    std::unordered_map<string,GameInfo> mGames;
    std::unordered_map<string,GameResult> mGameResult;
    std::unordered_map<string,GameRoster> mGameRoster;
    std::unordered_map<string,PlayerDetail> mPlayerDetail;
    std::vector<PlayerData> myPlayerData, myStagedPlayerData;
    std::vector<GameData> myGameData, myStagedGameData;

    std::unordered_map<string,GameResult> mStagedGameResult;
    int TestingWindow::randomNum(int num) ;
        GameResult TestingWindow::fakeit(GameInfo &g);

    MessageData myMessageData;
    std::unordered_map<string,::google::protobuf::RepeatedPtrField< ::fantasybit::FantasyBitAward>>
    mRewards;

    std::unordered_map<string,PlayerResult>
    mResult;

    std::string mStagedBlock;
    int mStagedBlockNum;
    int realweek() ;

    bool amlive;

    bool sendStageBlock() ;
    bool makeStageBlock(DataTransition &dt);
    PlayerLoaderTR *playerloader;

    bool Cleanit(Block *b);

    void setWeeklySchedule(int week);



    Ui::TestingWindow *ui;
};

#endif // TESTINGWINDOW_H
#include "testingwindow.h"
#include "ui_testingwindow.h"
#include "ProtoData.pb.h"
#include "LAPIWorker.h"
#include "core.h"
#include "dataagent.h"
#include "dataservice.h"
#include "RestFullCall.h"
#include "Commissioner.h"
#include <unordered_map>
#include <leveldb/db.h>
#include <QtSql/QSql>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlDriver>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>

using namespace std;
using namespace fantasybit;

TestingWindow::TestingWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::TestingWindow)
{
    ui->setupUi(this);
    initialize();

}

TestingWindow::~TestingWindow() {
    delete ui;
}

void TestingWindow::initialize() {
    myCoreInstance = Core::resolveByName<MainLAPIWorker>("coreapi");
    //wake up core thread
    Core::instance()->guiIsAwake();
    if (myCoreInstance == NULL)  {
        qDebug() << "coreapi is not resolved";
        setDisabled(true);
        return;
    }
    timer = new QTimer(this);
    tradetimer = new QTimer(this);
    amlive = false;
/*
    qRegisterMetaType<GlobalState>("GlobalState");
    qRegisterMetaType<MyFantasyName>("MyFantasyName");
    qRegisterMetaType<fantasybit::FantasyBitProj>("FantasyBitProj");
    qRegisterMetaType<vector<MyFantasyName>>("vector<MyFantasyName>");
*/
    QObject::connect(timer,SIGNAL(timeout()),this,SLOT(Timer()));
    QObject::connect(tradetimer,SIGNAL(timeout()),this,SLOT(TradeTimer()));


    QObject::connect(myCoreInstance,SIGNAL(GlobalStateChange(fantasybit::GlobalState)),
                     this,SLOT(GoLive(fantasybit::GlobalState)));

    QObject::connect(myCoreInstance,SIGNAL(LiveGui(fantasybit::GlobalState)),
                     this,SLOT(GoLive(fantasybit::GlobalState)));

    QObject::connect(myCoreInstance,SIGNAL(NameStatus(fantasybit::MyFantasyName)),
                     this,SLOT(OnNameStatus(fantasybit::MyFantasyName)));

    QObject::connect(this,SIGNAL(BeOracle()),
                     myCoreInstance,SLOT(BeOracle()));

    QObject::connect(this,SIGNAL(ClaimFantasyName(QString)),myCoreInstance,SLOT(OnClaimName(QString)));

   // QObject::connect(myCoreInstance,SIGNAL(MyNames(vector<fantasybit::MyFantasyName>)),
    //                 this,SLOT(OnMyFantasyNames(vector<fantasybit::MyFantasyName>)));

    QObject::connect(myCoreInstance,SIGNAL(NewWeek(int)),this,SLOT(OnNewWeek(int)));

    //GlobalStateChange(GlobalState);
    for (int i = DataTransition::Type_MIN; i < DataTransition::Type_ARRAYSIZE; i++) {

        if (!DataTransition::Type_IsValid(i)) continue;

        ui->transitions->addItem(
                    QString::fromStdString(DataTransition::Type_Name(DataTransition::Type(i))),
                                         QVariant(i));
    }

    ui->weeks->addItem(" ",QVariant(0));

    for (int i = 1; i <= 16; i++) {
        ui->weeks->addItem("Week " + QString::number(i),
                                         QVariant(i));
    }
}

void TestingWindow::GoLive(GlobalState gs) {
    ui->Live->setText("Live");
    ui->season->setText(QString::number(gs.season()));
    ui->week->setText(QString::number(gs.week()));
    ui->globalstate->setText(QString::fromStdString(GlobalState::State_Name(gs.state())));
    if ( gs.has_week() && gs.week() > 0 ) {
        amlive = true;
        OnNewWeek(gs.week());
        emit BeOracle();
        SqlStuff sql("satoshifantasy","trades");
        mySeq = sql.lastSeq();

        tradetimer->start(5000);

    }
}

void TestingWindow::OnNewWeek(int week) {
    ui->week->setText(QString::number(week));
    ui->gameID->clear();

    auto weekly =  DataService::instance()->GetWeeklySchedule(week);
    //ui->game->clear();

    for ( auto g : weekly.games()) {
        ui->gameID->addItem(QString::fromStdString(g.away() + "@" +g.home()),
                          QString::fromStdString(g.id()));



    }

}

void TestingWindow::OnNameStatus(MyFantasyName name) {
    ui->FantassyNameIn->setText(QString::fromStdString(name.name()));
}

/*
void TestingWindow::OnMyFantasyNames(vector<MyFantasyName> in) {
    qDebug() << in.size();
}
*/

void TestingWindow::on_beoracle_clicked() {
    emit BeOracle();
}

void TestingWindow::setWeeklySchedule(int week) {
    mGames.clear();
    auto weekly =  DataService::instance()->GetWeeklySchedule(week);
    //ui->game->clear();

    qDebug() << weekly.games_size();
    for ( auto g : weekly.games()) {
        mGames[g.id()] = g;
        ui->game->addItem(QString::fromStdString(g.away() + "@" +g.home()),
                          QString::fromStdString(g.id()));

        qDebug() << "weekly" << week << QString::fromStdString(g.away() + "@" +g.home())
                 << QString::fromStdString(g.id());

    }
}

void TestingWindow::on_weeks_activated(int index)
{
   qDebug() <<  index;
   ui->game->clear();
   ui->team->clear();
   if ( index < 1 ) {
       return;
   }

   setWeeklySchedule(index);
   qDebug() << index << realweek();
   if ( index < realweek()) {
       auto gameresults =  DataService::instance()->GetPrevWeekGameResults(index);

       for ( auto g : gameresults ) {
           mGameResult[g.gameid()] = g;
       }
   }
   else if ( index == realweek() ) {
       auto gameroster =  DataService::instance()->GetCurrentWeekGameRosters();

       qDebug() << gameroster.size();
       for ( auto g : gameroster ) {
           mGameRoster[g.info.id()] = g;
           qDebug() << " set roster:" <<  QString::fromStdString(g.info.id());// << " " << g;
       }

   }
}

void TestingWindow::on_game_activated(const QString &arg1)
{
    auto id = ui->game->currentData();
    GameInfo gameinfo = mGames[id.toString().toStdString()];
    qDebug() << id.toString() << " on game" << gameinfo.DebugString();

    ui->team->clear();
    ui->team->addItem(QTD(gameinfo.home()),"home");
    ui->team->addItem(QTD(gameinfo.away()),"away");
}

void TestingWindow::on_team_activated(const QString &arg1)
{
    auto id = ui->game->currentData().toString().toStdString();
    if ( ui->weeks->currentIndex() < realweek())
    {
        qDebug() << "on_team_activated on game" << ui->game->currentData().toString();

        auto it = mGameResult.find(id);
        if ( it == end(mGameResult) )
            return;

        auto result = mGameResult[id];
        bool home = "home" == ui->team->currentData();

        ui->player->clear();
        mResult.clear();
        for ( auto pr : home ? result.home_result() : result.away_result()) {
            auto pbase = DataService::instance()->GetPlayerBase(pr.playerid());
            ui->player->addItem(QString::fromStdString(pbase.last() + ", " + pbase.first() +
                                                       " :" + pbase.position()),
                                QString::fromStdString(pr.playerid()));
            mResult[pr.playerid()] = pr;
            qDebug() << pr.playerid() << pr.DebugString();

        }
    }
    else {

        mResult.clear();

        qDebug() << "live on_team_activated on game" << ui->game->currentData().toString();

        qDebug() << "mGameRoster.find(id)" << id;
        auto it = mGameRoster.find(id);
        if ( it ==  end(mGameRoster) ) {
            qDebug() << "NOT Find " << id;

            return;
        }
        qDebug() << "Find " << id;

        auto result = mGameRoster[id];
        bool home = "home" == ui->team->currentData();
        qDebug() << "home" << home << ui->team->currentData();

        ui->player->clear();
        for ( auto pr : (home ? result.homeroster : result.awayroster)) {
            auto pbase = pr.second.base;
            qDebug() << pr.first << pbase.DebugString();
            ui->player->addItem(QString::fromStdString(pbase.last() + ", " + pbase.first() +
                                                       " :" + pbase.position()),
                                QString::fromStdString(pr.first));
            mPlayerDetail[pr.first] = pr.second;
        }

    }
}

void TestingWindow::on_player_activated(const QString &arg1)
{
    ui->awards->clear();

    if ( ui->weeks->currentIndex() > realweek())
        return;
    else if (ui->weeks->currentIndex() < realweek()) {
        std::string str = ui->player->currentData().toString().toStdString();
        qDebug() << str;
        auto it = mResult.find(str);
        if ( it != mResult.end())
            ui->awards->addItem(QString::fromStdString(it->second.DebugString()));
/*
        for ( auto fba : it->second) {
//              mRewards[ui->player->currentData().toString().toStdString()]) {
            ui->awards->addItem(QString::fromStdString(fba.DebugString()));
        }
        else
            for ( auto kk : mRewards)
                for ( auto jj : kk.second )
                qDebug() << kk.first << jj.DebugString();
                */
    }
    else {
        auto pd = mPlayerDetail[ui->player->currentData().toString().toStdString()];
        //auto pd = pdp.second;
        ui->awards->addItem(QString::fromStdString(
                                pd.base.DebugString() +
                                PlayerStatus::Status_Name(pd.team_status) +
                                PlayerGameStatus_Name(pd.game_status))
                                );

    }
}

void TestingWindow::on_StageBlock_clicked() {

        if ( !ui->StageBlock->isEnabled() )
            return;

        if ( ui->rundataagent->isChecked() ) {
            timer->stop();
            ui->rundataagent->setChecked(false);
            return;
        }
        else {
            ui->rundataagent->setEnabled(false);
            ui->StageBlock->setEnabled(false);
        }

        DataTransition dt{};
        dt.set_type(static_cast<DataTransition_Type>(ui->transitions->currentData().toInt()));
        dt.set_season(2015);
        dt.set_week(ui->weeks->currentIndex());

        if ( dt.type() == DataTransition_Type_GAMESTART) {
            GameData gd{};
            GameStatus gs{};
            gs.set_status(GameStatus::INGAME);
            if ( ui->allgames->isChecked() ) {
                for ( auto i =0; i<ui->gameID->count(); i++) {
                    gd.set_gameid(ui->gameID->itemData(i).toString().toStdString());
                    gd.mutable_status()->CopyFrom(gs);
                    dt.add_gamedata()->CopyFrom(gd);
                }
            }
            else {
                gd.set_gameid(ui->gameID->currentData().toString().toStdString());
                gd.mutable_status()->CopyFrom(gs);
                dt.add_gamedata()->CopyFrom(gd);
                string newmsg{};
                if ( myMessageData.has_msg())
                    newmsg += myMessageData.msg() + " | ";
                newmsg += ui->gameID->currentText().toStdString()
                        + " kickoff (locked)";

                myMessageData.set_msg(newmsg);
            }

        }
        Data d{};
        d.set_type(Data_Type_RESULT);
        for ( auto gr : mStagedGameResult ) {
            ResultData rd{};
            rd.mutable_game_result()->CopyFrom(gr.second);
            d.MutableExtension(ResultData::result_data)->CopyFrom(rd);
            dt.add_data()->CopyFrom(d);
            qDebug() << rd.DebugString();
        }
        mStagedGameResult.clear();



        d.set_type(Data::PLAYER);
        //PlayerData pd{};
        for ( auto pd : myPlayerData ) {
            d.MutableExtension(PlayerData::player_data)->CopyFrom(pd);
            dt.add_data()->CopyFrom(d);
            qDebug() << pd.DebugString();
        }
        myPlayerData.clear();


        d.set_type(Data::GAME);
        for ( auto gd : myGameData ) {
            d.MutableExtension(GameData::game_data)->CopyFrom(gd);
            dt.add_data()->CopyFrom(d);
            qDebug() << gd.DebugString();
        }
        myGameData.clear();

        if ( !makeStageBlock(dt)) {
            ui->out->setText("error making block");
            ui->StageBlock->setEnabled(true);
        }
        else
            ui->SendBlock->setEnabled(true);


        //leveldb::Slice snum((char*)&mStagedBlockNum, sizeof(int));
        //Node::blockchain->Put(leveldb::WriteOptions(), snum, mStagedBlock);



        /*
        auto prev = Node::getlastLocalBlock().signedhead().head();

        BlockHeader bh{};
        bh.set_version(Commissioner::BLOCK_VERSION);
        bh.set_num(prev.num() + 1);
        bh.set_prev_id(fc::sha256::hash(prev.SerializeAsString()).str());
        bh.set_timestamp( std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count());
        //todo: t
        bh.set_generator_pk(Commissioner::pk2str(Commissioner::GENESIS_PUB_KEY));
        bh.set_generating_sig(fc::sha256::hash(prev.generating_sig() + bh.generator_pk()).str());
        bh.set_basetarget(0); //todo
        bh.set_blocktype(BlockHeader_Type_DATA);
        bh.set_transaction_id(""); //todo: merkle root of tx

        SignedBlockHeader sbh{};
        sbh.mutable_head()->CopyFrom(bh);

        auto p = getIdSig(sbh.head().SerializeAsString(),*m_oracle);
        sbh.set_sig(p.second);
        //todo: store block hash from p.first

        Block b{};
        b.mutable_signedhead()->CopyFrom(sbh);

        SignedTransaction st{};
        SignedTransaction* st2 = b.add_signed_transactions();
        st2->CopyFrom(dt);


        leveldb::ReadOptions options;
        options.snapshot = Node::txpool->GetSnapshot();
        leveldb::Iterator *iter = Node::txpool->NewIterator(options);

        for (iter->SeekToFirst(); iter->Valid(); iter->Next()) {

            st.ParseFromString(iter->value().ToString());

            SignedTransaction* st2 = b.add_signed_transactions();
            st2->CopyFrom(st);
        }


        if (pendingTrans.size() == 0)
            return b;

        for (auto &pt : pendingTrans)
        {
            SignedTransaction* st2 = b.add_signed_transactions();
            st2->CopyFrom(pt);
        }

        pendingTrans.clear();


        delete iter;
        Node::txpool->ReleaseSnapshot(options.snapshot);
        */
}

void TestingWindow::on_claimname_clicked()
{
    emit ClaimFantasyName(ui->FantassyNameIn->text());
}

void TestingWindow::TradeTimer() {
    SignedTransaction st{};
    StampedTrans stt{};

    while(true) {
        auto txstr = RestfullService::myGetTr();
        if ( !st.ParseFromString(txstr) ) {
            qDebug() << "no more trades - break" << txstr;
            break;
        }

        if ( !st.has_trans() ) {
            qDebug() << "bad tx - break" << txstr;
            break;
        }

        stt.Clear();
        stt.mutable_signed_orig()->CopyFrom(st);
        stt.set_timestamp(std::chrono::duration_cast<std::chrono::milliseconds>
                          (std::chrono::system_clock::now().time_since_epoch()).count());
        stt.set_seqnum(++mySeq);

        Transaction trans{};
        trans.set_version(Commissioner::TRANS_VERSION);
        trans.set_type(TransType::STAMPED);
        trans.MutableExtension(StampedTrans::stamped_trans)->CopyFrom(stt);

        st.Clear();
        st = Core::resolveByName<MainLAPIWorker>("coreapi")
                ->Agent().makeSigned(trans);

        txstr = st.SerializeAsString();
        {
        RestfullClient rest(QUrl(PAPIURL.data()));
        rest.postRawData("tx","octet-stream",txstr.data(),((size_t)txstr.size()));
        }
    }
}

void TestingWindow::Timer() {

    int count = 0;
    while(true) {
        auto txstr = RestfullService::myGetTx();

        //ToDo: verify
        SignedTransaction st{};
        if ( !st.ParseFromString(txstr) ) {
            qDebug() << "no more tx - break" << txstr;
            break;
        }

        if ( !st.has_trans() ) {
            qDebug() << "bad tx - break" << txstr;
            break;
        }

        qDebug() << "processed " << st.DebugString();


        count++;

        ui->staging_tx->addItem(QString::fromStdString(st.DebugString()));

        //st.id();
        Node::addTxPool(st.id(), txstr);
        if ( count >= 500)
            break;
    }

    if (count < 1) return;

    qDebug() << "making block " << count;

    DataTransition dt{};
    dt.set_type(DataTransition_Type_HEARTBEAT);
    dt.set_season(2015);
    dt.set_week(realweek());

    if ( !makeStageBlock(dt) )
        qDebug() << "error mamakeStageBlock ";
    else if ( !sendStageBlock() )
        qDebug() << "error sendStageBlock ";
    else {
        qInfo() << "sending block of tx";
        ui->staging_tx->clear();
    }
}

void TestingWindow::on_GetTx_clicked()
{
    auto txstr = RestfullService::getTx(LAPIURL.data());


    SignedTransaction st{};
    if ( !st.ParseFromString(txstr) )
        return;

    ui->staging_tx->addItem(QString::fromStdString(st.DebugString()));

    st.id();

    Node::addTxPool(st.id(), txstr);
}

int TestingWindow::realweek() {
    return ui->week->text().toInt();
}


GameResult TestingWindow::fakeit(GameInfo &g) {
    GameRoster gamer = mGameRoster[g.id()];

    GameResult gr{};
    gr.set_gameid(g.id());
    gr.set_kickofftime(gamer.info.time());

    for (auto p : gamer.awayroster) {
        PlayerResult pr;
        pr.set_playerid(p.first);
        Stats s{};
        if ( p.second.base.position() == "QB") {
            Ostats o;
            o.set_passtd(randomNum(10) / 15);
            o.set_passyds(randomNum(25) * (1+o.passtd()));
            o.set_pint(randomNum(10) / 30);
            o.set_fumble(randomNum(50) / 25 );
            o.set_rushtd( randomNum(90) / 5);
            o.set_rushyds( randomNum(0) / 6);
            o.set_twopt( randomNum(94) / 6 );
            s.mutable_ostats()->CopyFrom(o);
        }
        else if ( p.second.base.position() == "RB" ) {
            Ostats o;
            o.set_twopt( randomNum(94) / 6 );
            o.set_rushtd( randomNum(80) / 5);
            o.set_rushyds( randomNum(10) + o.rushtd() * 10);
            o.set_rec(randomNum(50) / 7);
            o.set_rectd(randomNum(90) / 5);
            o.set_recyds(o.rec() * 10);
            s.mutable_ostats()->CopyFrom(o);
        }
        else if ( p.second.base.position() == "WR" ) {
            Ostats o;
            o.set_twopt( randomNum(94) / 6 );
            o.set_rushtd( randomNum(88) / 10);
            o.set_rushyds( randomNum(92) + o.rushtd() * 20);
            o.set_rectd(randomNum(80) / 5);
            o.set_rec(randomNum(10) / 10 );
            o.set_recyds(o.rec() * 10 + o.rectd() * 20);
            s.mutable_ostats()->CopyFrom(o);
        }
        else if ( p.second.base.position() == "TE" ) {
            Ostats o;
            o.set_rectd(randomNum(80) / 10);
            o.set_rec(randomNum(10) / 22);
            o.set_recyds(o.rec() * 10 + o.rectd() * 20);
            s.mutable_ostats()->CopyFrom(o);
        }
        else if ( p.second.base.position() == "K" ) {
            Kstats k;
            k.set_pa(randomNum(10) / 15);
            for (int i=0;i<5;i++) {
                int hit = (randomNum(50) / 4) * 5;
                if ( hit == 0) break;

                k.add_fg(hit);
            }
            s.mutable_kstats()->CopyFrom(k);
        }
        else if ( p.second.base.position() == "DEF") {
            Dstats d{};
            d.set_deftd(randomNum(90)/5);
            d.set_ptsa(randomNum(13) / 2);
            d.set_sacks(randomNum(50) / 4);
            d.set_turnovers(randomNum(80)/5);
            s.mutable_dstats()->CopyFrom(d);
        }

        pr.mutable_stats()->CopyFrom(s);
        pr.set_result(BlockProcessor::CalcResults(s));
        gr.add_away_result()->CopyFrom(pr);
    }

    for (auto p : gamer.homeroster) {
        PlayerResult pr;
        pr.set_playerid(p.first);
        Stats s{};
        if ( p.second.base.position() == "QB") {
            Ostats o;
            o.set_passtd(randomNum(10) / 15);
            o.set_passyds(randomNum(25) * (1+o.passtd()));
            o.set_pint(randomNum(10) / 30);
            o.set_fumble(randomNum(50) / 25 );
            o.set_rushtd( randomNum(90) / 5);
            o.set_rushyds( randomNum(0) / 6);
            o.set_twopt( randomNum(94) / 6 );
            s.mutable_ostats()->CopyFrom(o);
        }
        else if ( p.second.base.position() == "RB" ) {
            Ostats o;
            o.set_twopt( randomNum(94) / 6 );
            o.set_rushtd( randomNum(80) / 5);
            o.set_rushyds( randomNum(10) + o.rushtd() * 10);
            o.set_rec(randomNum(50) / 7);
            o.set_rectd(randomNum(90) / 5);
            o.set_recyds(o.rec() * 10);
            s.mutable_ostats()->CopyFrom(o);
        }
        else if ( p.second.base.position() == "WR" ) {
            Ostats o;
            o.set_twopt( randomNum(94) / 6 );
            o.set_rushtd( randomNum(88) / 10);
            o.set_rushyds( randomNum(92) + o.rushtd() * 20);
            o.set_rectd(randomNum(80) / 5);
            o.set_rec(randomNum(10) / 10 );
            o.set_recyds(o.rec() * 10 + o.rectd() * 20);
            s.mutable_ostats()->CopyFrom(o);
        }
        else if ( p.second.base.position() == "TE" ) {
            Ostats o;
            o.set_rectd(randomNum(80) / 10);
            o.set_rec(randomNum(10) / 22);
            o.set_recyds(o.rec() * 10 + o.rectd() * 20);
            s.mutable_ostats()->CopyFrom(o);
        }
        else if ( p.second.base.position() == "K" ) {
            Kstats k;
            k.set_pa(randomNum(10) / 15);
            for (int i=0;i<5;i++) {
                int hit = (randomNum(50) / 4) * 5;
                if ( hit == 0) break;

                k.add_fg(hit);
            }
            s.mutable_kstats()->CopyFrom(k);
        }
        else if ( p.second.base.position() == "DEF") {
            Dstats d{};
            d.set_deftd(randomNum(90)/5);
            d.set_ptsa(randomNum(93) / 2);
            d.set_sacks(randomNum(50) / 4);
            d.set_turnovers(randomNum(80)/5);
            s.mutable_dstats()->CopyFrom(d);
        }

        pr.mutable_stats()->CopyFrom(s);
        pr.set_result(BlockProcessor::CalcResults(s));
        gr.add_home_result()->CopyFrom(pr);
    }

    return gr;
}

int TestingWindow::randomNum(int num) {
    int ret = (qrand() % 100) - num;
    if ( ret < 0 ) ret = 0;

    return ret;;

}

void TestingWindow::on_GetGameResult_clicked()
{
    string gid = ui->gameID->currentData().toString().toStdString();

    ui->gameID->removeItem(ui->gameID->currentIndex());
    ui->staging_data->clear();

    auto itg = mGames.find(gid);
    if ( itg == end(mGames)) {
        qDebug() << "cant find" << gid;
        return;
    }

    GameResult result;
    auto rsp =  mStagedGameResult.find(gid);
    if ( rsp != mStagedGameResult.end() )
        result = rsp->second;
    else {
        result = dataagent::instance()->getGameResult(realweek(),itg->second);
        //result = fakeit(itg->second);
        mStagedGameResult[gid] = result;
    }

    for (auto pr : result.home_result()) {
        auto itt = mPlayerDetail.find(pr.playerid());

        if ( itt != end(mPlayerDetail)) {
            PlayerDetail *it = &(itt->second);
            std::string st = itg->second.home() + " " + it->base.last()
            + ", "  + it->base.first() + " : " + it->base.position() +
            pr.stats().ostats().DebugString();

            ui->staging_data->addItem(QString::fromStdString(st));
         }
    }

    for (auto pr : result.away_result()) {
        auto itt = mPlayerDetail.find(pr.playerid());
        if ( itt != end(mPlayerDetail)) {
            PlayerDetail *it = &(itt->second);

            std::string st = itg->second.away() + " " + it->base.last()
            + ", "  + it->base.first() + " : " + it->base.position() +
            pr.stats().ostats().DebugString();

            ui->staging_data->addItem(QString::fromStdString(st));
         }
    }

}

void TestingWindow::on_updatelb_clicked()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");

    db.setHostName("192.96.159.216");
    db.setPort(3306);
    db.setDatabaseName("dev_satoshifantasy");
    db.setUserName("root");
    db.setPassword("fantasyf00tball!");
    bool success = true;
    if (!db.open()) {
         qDebug() << "Database error occurred :" << db.lastError().databaseText();
         //LogIt(db.lastError().databaseText().toStdString());
        return;
    }

    auto leaderBoardData = DataService::instance()->GetLeaderBoard();


    for (auto pn : leaderBoardData) {
        QSqlQuery insertQuery(db);
        insertQuery.prepare("INSERT INTO fantasyteam(fantasyteam, fantasybits, stake) VALUES(:player_name,:bits,:bits)");
        insertQuery.bindValue(":player_name",QString::fromStdString(pn->alias()));
        insertQuery.bindValue(":bits",pn->getBalance());

        if ( !insertQuery.exec() )
        {
            qDebug() << " exec ret " << insertQuery.lastError().databaseText();

            QSqlQuery updateQuery(db);
            updateQuery.prepare("UPDATE fantasyteam set fantasybits= :bits, stake= :bits where fantasyteam= :player_name");
            updateQuery.bindValue(":player_name",QString::fromStdString(pn->alias()));
            updateQuery.bindValue(":bits",pn->getBalance());
            if (!updateQuery.exec()) {
                qDebug() << " exec ret " << updateQuery.lastError().databaseText();
                success= false;
            }
        }
    }
    db.close();

}

void TestingWindow::on_rundataagent_toggled(bool checked)
{
    if ( !checked )
        timer->stop();
    else
        timer->start(5000);

}

void TestingWindow::on_SendBlock_clicked() {
    if (!ui->SendBlock->isEnabled())
        return;
    if (ui->rundataagent->isEnabled())
        return;

    if ( ui->rundataagent->isChecked())
        return;

    if ( timer->isActive() )
        return;

    ui->SendBlock->setEnabled(false);

    if ( !sendStageBlock() )
        qDebug() << "error sendStageBlock ";
    else {
        ui->rundataagent->setEnabled(true);
        ui->StageBlock->setEnabled(true);
    }
}

#include "DataPersist.h"
bool TestingWindow::sendStageBlock() {
    Block b{};
    b.ParseFromString(mStagedBlock);
    qDebug() << mStagedBlock.size() << "Sending" << b.DebugString() << b.SerializeAsString().size();
    RestfullClient rest(QUrl(LAPIURL.data()));
    rest.postRawData("block/"+QString::number(mStagedBlockNum),"xxx",mStagedBlock.data(),mStagedBlock.size());

    Node::ClearTx(b);
    //write(b);
    //TODO HANDLE ERROR
    qDebug() << "sent block" << b.DebugString();
    return true;
}

bool TestingWindow::makeStageBlock(DataTransition &dt) {
    string number = to_string(mStagedBlockNum);
    Data d{};
    d.set_type(Data_Type_MESSAGE);
    if ( !myMessageData.has_msg()  || myMessageData.msg() == "" )
        myMessageData.set_msg(number);
    myMessageData.set_gt(1029);
    d.MutableExtension(MessageData::message_data)->CopyFrom(myMessageData);
    dt.add_data()->CopyFrom(d);
    myMessageData.Clear();

    myMessageData.set_msg(number +
        " <a href=\"http://trading.football:8080/tradingfootball-setup.exe\">New Version 2.1 - Trading. (win64)[t.2f][t.f]</a>");
    myMessageData.set_lt(2101);
    d.MutableExtension(MessageData::message_data)->CopyFrom(myMessageData);
    dt.add_data()->CopyFrom(d);
    qDebug() << myMessageData.msg();
    myMessageData.Clear();

    myMessageData.set_msg(number +
        " <a href=\"http://trading.football:8080/tradingfootball.dmg\">New Version 2.1 - Trading. (osx64)[t.2f][t.f]</a>");
    myMessageData.set_lt(2100);
    d.MutableExtension(MessageData::message_data)->CopyFrom(myMessageData);
    dt.add_data()->CopyFrom(d);
    qDebug() << myMessageData.msg();
    myMessageData.Clear();

    Transaction trans{};
    trans.set_version(Commissioner::TRANS_VERSION);
    trans.set_type(TransType::DATA);
    trans.MutableExtension(DataTransition::data_trans)->CopyFrom(dt);
    auto b = dataagent::instance()->makeNewBlockAsDataAgent(trans);

    //RestfullClient rest(QUrl(PAPIURL.data()));
    //rest.postRawData("tx","shit",trans.data(),((size_t)trans.size()));

    ui->out->clear();
    //todo: verify block
    if ( b )  {
        mStagedBlockNum = (*b).signedhead().head().num();
        mStagedBlock = (*b).SerializeAsString();
        return true;
    }
    else return false;
}

void TestingWindow::on_nmeonic_clicked() {
    auto mfn = dataagent::instance()->importMnemonic(ui->FantassyNameIn->text().toStdString());
    ui->out->setText(QString::fromStdString(MyNameStatus_Name(mfn.status())));
    if ( mfn.status() == MyNameStatus::confirmed)
        ui->FantassyNameIn->setText(QString::fromStdString(mfn.name()));

}

void TestingWindow::on_MsgButton_clicked() {

    myMessageData.set_msg(ui->out->text().toStdString());
}

void TestingWindow::on_stage_player_clicked() {
    playerloader = new PlayerLoaderTR();
    myStagedPlayerData = playerloader->loadPlayersFromTradeRadar(realweek(),false);
    for ( auto pd : myStagedPlayerData) {
        ui->staging_data->addItem(QString::fromStdString(pd.DebugString()));
    }

}

void TestingWindow::on_commit_player_clicked() {
    playerloader->dump();
    delete playerloader;

    myPlayerData = myStagedPlayerData;
    myStagedPlayerData.clear();

    ui->staging_data->clear();

}

void TestingWindow::on_stage_game_clicked() {
    ScheduleLoader mScheduleLoader;
    std::vector<fantasybit::ScheduleData> vsd =
            mScheduleLoader.loadScheduleMovingFwdFromTR(realweek());

    for ( auto sd : vsd ) {
        auto myweekly =  DataService::instance()->GetWeeklySchedule(sd.week());

        map<string,GameInfo> old;
        for (auto gi : myweekly.games()) {
            old[gi.id()] = gi;
        }

        for ( auto gi : sd.weekly().games()) {
            auto it = old.find(gi.id());
            if ( it == end(old)) {
                qCritical() << "cant find game" << gi.id();
                continue;
            }
            if ( it->second.time() != gi.time()) {
                GameData gd{};
                GameStatus gs = DataService::instance()->GetGameStatus(gi.id());
                if ( !gs.has_status() || gs.status() == GameStatus_Status_SCHEDULED ) {
                    if ( !gs.has_datetime() || gs.datetime() != gi.time() ) {
                        gs.set_datetime(gi.time());
                        gd.set_gameid(gi.id());
                        gd.mutable_status()->CopyFrom(gs);
                        myStagedGameData.push_back(gd);
                    }
                }
            }
        }
    }

    for ( auto sgd : myStagedGameData) {
        ui->staging_data->addItem(QString::fromStdString(sgd.DebugString()));
    }
}

void TestingWindow::on_commit_game_clicked() {
    myGameData = myStagedGameData;
    myStagedGameData.clear();

    ui->staging_data->clear();
}

void TestingWindow::on_cancel_staged_clicked() {

    if ( playerloader != nullptr )
        delete playerloader;

    myStagedPlayerData.clear();
    myStagedGameData.clear();

    ui->staging_data->clear();

}

/*
void TestingWindow::on_Update_PLayers_2_clicked()
{
    int count = 0;
    bool done = false;
    while(!done) {
        auto txstr = RestfullService::myGetTx();

        //ToDo: verify
        SignedTransaction st{};
        if ( !st.ParseFromString(txstr) )
            continue;

        if ( st.trans().type() == TransType::NAME) {
            auto nt = st.trans().GetExtension(NameTrans::name_trans);
            if ( nt.fantasy_name() == "ffwc")
                done = true;
        }

        count++;

        ui->staging_tx->addItem(QString::fromStdString(st.DebugString()));

        st.id();
        Node::addTxPool(st.id(), txstr);
        if ( count >= 30 || done) {
            count = 0;
            DataTransition dt{};
            dt.set_type(DataTransition_Type_HEARTBEAT);
            dt.set_season(2015);
            dt.set_week(realweek());

            if ( !makeStageBlock(dt) )
                qDebug() << "error mamakeStageBlock ";
            else if ( !sendStageBlock() )
                qDebug() << "error sendStageBlock ";
            else {
                qInfo() << "sending block of tx";
                ui->staging_tx->clear();
            }
        }
    }
}
*/

void TestingWindow::on_fix363_clicked()
{

    auto txstr = RestfullService::myGetTx();

    //ToDo: verify
    SignedTransaction st{};
    st.ParseFromString(txstr);

    qDebug() << st.DebugString();
    return;

    SqlStuff sql("satoshifantasy","on_fix363_clicked");
   for ( int i =6237; i<6246; i++) {
       sql.dumpTx(i);
   }
/*
   auto b = Node::getLocalBlock(i,true);

   for ( auto tr : (*b).signed_transactions()) {
       qDebug() << i << tr.DebugString();
   }
   }
   */
   return;

   auto b = Node::getLocalBlock(363,true);

   auto id = ui->game->currentData();
   GameInfo gameinfo = mGames[id.toString().toStdString()];

   GameResult result = dataagent::instance()->getGameResult(2,gameinfo);

   Data d{};
   d.set_type(Data_Type_RESULT);
   ResultData rd{};
   rd.mutable_game_result()->CopyFrom(result);
   d.MutableExtension(ResultData::result_data)->CopyFrom(rd);

   DataTransition *dt = (*b).mutable_signed_transactions(0)->mutable_trans()->MutableExtension(DataTransition::data_trans);
   dt->add_data()->CopyFrom(d);
   qDebug() << (*b).DebugString();

   string bdata = (*b).SerializeAsString();
   RestfullClient rest(QUrl(LAPIURL.data()));
   rest.postRawData("block/363","xxx",bdata.data(),bdata.size());

}

void TestingWindow::on_FixDef_clicked()
{
    Block b{};
    std::vector<Block> replace{};
    auto *it = Node::blockchain->NewIterator(leveldb::ReadOptions());
    for (it->SeekToFirst() ;it->Valid();it->Next() ) {
        b.ParseFromString(it->value().ToString());
        if ( Cleanit(&b) )
            replace.push_back(b);
    }
    delete it;

    //return;

    for ( auto b : replace) {
        int32_t bnum = b.signedhead().head().num();
        leveldb::Slice snum((char*)&bnum, sizeof(int32_t));

        Node::blockchain->Put(leveldb::WriteOptions(), snum, b.SerializeAsString());

        string bdata = b.SerializeAsString();
        RestfullClient rest(QUrl(LAPIURL.data()));
        rest.postRawData("block/"+QString::number(bnum),"xxx",bdata.data(),bdata.size());

    }

}

void TestingWindow::on_GetResults4Fix_clicked()
{
    GameResult result;
    for ( auto gp : mGames ) {
        result = dataagent::instance()->getGameResult(ui->weeks->currentIndex(),gp.second);
        mStagedGameResult[gp.first] = result;
        QThread::currentThread()->msleep(1000);
    }

}


bool TestingWindow::Cleanit(Block *b) {
    auto dt = //b.signed_transactions(0).trans().GetExtension(DataTransition::data_trans);
    b->mutable_signed_transactions(0)->mutable_trans()->MutableExtension(DataTransition::data_trans);
    bool replaceit = false;
    for ( int i=0;i<dt->data_size(); i++) {
        if ( dt->data(i).type() != Data::RESULT) continue;
        replaceit = true;
        qDebug() << "replace block" << b->signedhead().head().num();
        //continue;
        Data *d = dt->mutable_data(i);
        ResultData *prd = d->MutableExtension(ResultData::result_data);
        auto it = mStagedGameResult.find(prd->game_result().gameid());
        prd->mutable_game_result()->CopyFrom(it->second);
    }
    return replaceit;

}



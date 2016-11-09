#include "server.h"

//fantasybit::GetAllNamesRep mAllNamesRep{};
fantasybit::ExchangeData Server::TheExchange{};
//fantasybit::GetROWMarketRep Server::ROWMarketRep{};
//fantasybit::NFLStateData Server::NFLData;

//decltype(Server::Pk2Bal) Server::Pk2Bal{};

Server *Server::instance() {
    if (myInstance == NULL) {
        myInstance = new Server();
    }
    return myInstance;
}

void Server::setupConnection(pb::IPBGateway *ingateway) {

    QObject* that = dynamic_cast<QObject*>(ingateway);
    connect( that, SIGNAL   ( LiveGui(fantasybit::GlobalState)     ),
            this,      SLOT     (  LiveGui(fantasybit::GlobalState)     ));

    connect(that, SIGNAL(NameStatus(fantasybit::MyFantasyName)),
            this, SLOT(NameStatus(fantasybit::MyFantasyName)));

    connect( that, SIGNAL(LiveProj(fantasybit::FantasyBitProj)),
            this, SLOT(LiveProj(fantasybit::FantasyBitProj )));

    connect( that, SIGNAL(MyNames(vector<fantasybit::MyFantasyName>)),
            this, SLOT(MyNames(vector<fantasybit::MyFantasyName> )));
    connect( that, SIGNAL(NameBal(fantasybit::FantasyNameBal)),
            this, SLOT(NameBal(fantasybit::FantasyNameBal )));
    connect( that, SIGNAL( PlayerStatusChange(pair<string,fantasybit::PlayerStatus>) ),
            this, SLOT(PlayerStatusChange(pair<string,fantasybit::PlayerStatus> )));


    connect( that, SIGNAL(  GlobalStateChange(fantasybit::GlobalState)  ),
            this,      SLOT(    GlobalStateChange(fantasybit::GlobalState)        ));


    connect( that, SIGNAL   (  NewWeek(int)    ),
            this,      SLOT     (  NewWeek(int)     ));
    connect( that, SIGNAL   (  GameStart(string)    ),
            this,      SIGNAL     (  GameStart(string)     ));

    connect( that, SIGNAL   (  GameStart(string)    ),
            this,      SLOT     (  ResetData(string)     ));

    connect( that, SIGNAL   (  GameOver(string)    ),
            this,      SLOT     (  GameOver(string)     ));
    connect( that, SIGNAL   (  onControlMessage(QString)    ),
            this,      SLOT     (  onControlMessage(QString)     ));


    connect( that, SIGNAL   (  NewFantasyName(fantasybit::FantasyNameBal)    ),
            this,  SLOT     (  OnNewFantasyName(fantasybit::FantasyNameBal)      ));

    connect( that, SIGNAL   (  AnyFantasyNameBalance(fantasybit::FantasyNameBal)    ),
            this,  SLOT     (  OnAnyFantasyNameBalance(fantasybit::FantasyNameBal)      ));

    connect( that, SIGNAL(Height(int)),
             this, SLOT(Height(int)));

    connect( that, SIGNAL(BlockNum(int)),
             this, SLOT(BlockNum(int)));

//    QObject::connect(this,SIGNAL(OnClaimName(QString)),that,SLOT(OnClaimName(QString)));

//    connect( this, SIGNAL(OnUseName(QString)),
//             that, SLOT(UseName(QString)));

//    connect( this, SIGNAL(doNameCheck(QString)),
//             that, SLOT(nameCheck(QString)));

//    connect( that, SIGNAL(nameAvail(QString &,bool)),
//             this, SLOT(nameAvail(QString &,bool)));


//    return that;
}

void Server::LiveGui(GlobalState gs) {

    qDebug() << "Server received Livegui " << gs.DebugString().data();
    if ( !amLive ) {
        amLive = true;
//        m_season = gs.season();
//        settheWeek(gs.week());
//        setliveSync("Live");
//        setseasonString(gs.state() == GlobalState_State_OFFSEASON ? "Off Season" : "NFL Season");
        GlobalStateRep.mutable_globalstate()->CopyFrom(gs);
        initData();

        #ifdef TESTING_ONLY
            testGameStart.setInterval(5000);
            connect(&testGameStart, SIGNAL(timeout()),
                    this, SLOT(setTestGameStart()));
            testGameStart.start();
        #endif

    }

    qDebug() << "Server GlobalStateRep " << GlobalStateRep.DebugString().data();

    emit GoLive();

}

void Server::initData() {

    if ( mGateway == nullptr ) {
        qDebug() << " mGateway null ";
        return;
    }
    if ( mGateway->dataService == nullptr) {
        qDebug() << " mGateway->dataService null ";
        return;
    }
//        updateLiveLeaders();

    mAllNamesRep.Clear();
    mPk2Index.clear();
    mPk2Bal.clear();
    auto lb = mGateway->dataService->GetLeaderBoard();
    qDebug() << " mGateway->dataService->GetLeaderBoard() " << lb.size();
    int i = 0;
    for(auto fPlayer  : lb ) {
        AddNames(FantasyName::toFantasyNameBal(*fPlayer));
    }

    qDebug() << " all " << mAllNamesRep.DebugString().data();

    avgProjByName = ProjByName::default_instance().New();
    resetRosterProjScheduleData();
}


        //        ScheduleRep.mutable_scheduledata()->mutable_weekly()->CopyFrom(
        //            mGateway->dataService->GetWeeklySchedule(GlobalStateRep.globalstate().week()));

void Server::resetRosterProjScheduleData() {
    mGetCurrRostersReply.Clear();
    zeroProjByName.Clear();
    avgProjByName->Clear();
    mPlayerId2Index.clear();
    ScheduleRep.Clear();

    mGetCurrRostersReply.set_ctype(GETGAMEROSTER);
    mGetCurrRostersRep = mGetCurrRostersReply.MutableExtension(GetCurrRostersRep::rep);

    for (GameRoster &gr : mGateway->dataService->GetCurrentWeekGameRosters() ) {

        if ( gr.status >= GameStatus_Status_INGAME )
            continue;

        #ifdef TESTING_ONLY
        GameData tgd = holdGameData[gr.info.id()];
        if ( tgd.status().status() >= GameStatus_Status_INGAME) {
            qDebug() << " not using game" << tgd.status().DebugString().data();

            continue;
        }

        qDebug() << " using game" << tgd.status().DebugString().data();
        #endif

        ScheduleRep.mutable_scheduledata()->mutable_weekly()->add_games()->CopyFrom(gr.info);
        GameDataRoster *gdr = mGetCurrRostersRep->add_gamerosters();
        GameData *gd = gdr->mutable_game_data();
        gd->set_gameid(gr.info.id());
        gd->mutable_status()->set_status(gr.status);

        #ifdef TESTING_ONLY
            holdGameData[gr.info.id()]= *gd;
        #endif

        TeamRoster *homet = gdr->mutable_homeroster();
        homet->set_teamid(gr.info.home());
        for ( auto &h : gr.homeroster) {
            PlayerData *pd = homet->add_players();
            pd->mutable_player_base()->CopyFrom(h.second.base);
            pd->set_playerid(h.first);
            pd->mutable_player_status()->set_status(h.second.team_status);
            PlayerProj *pp = zeroProjByName.add_playerproj();
            pp->set_playerid(pd->playerid());
//                pp->set_proj(mGateway->dataService->GetAvgProjection(pp->playerid()));
            mPlayerId2Index.insert({pp->playerid(),zeroProjByName.playerproj_size()-1});
        }

        TeamRoster *awayt = gdr->mutable_awayroster();
        awayt->set_teamid(gr.info.away());
        for ( auto &h : gr.awayroster) {
            PlayerData *pd = awayt->add_players();
            pd->mutable_player_base()->CopyFrom(h.second.base);
            pd->set_playerid(h.first);
            pd->mutable_player_status()->set_status(h.second.team_status);
            PlayerProj *pp = zeroProjByName.add_playerproj();
            pp->set_playerid(pd->playerid());
//                pp->set_proj(mGateway->dataService->GetAvgProjection(pp->playerid()));
            mPlayerId2Index.insert({pp->playerid(),zeroProjByName.playerproj_size()-1});
        }
    }
    mGetCurrRostersRep->set_week(GlobalStateRep.globalstate().week());
    ScheduleRep.mutable_scheduledata()->set_week(GlobalStateRep.globalstate().week());


    mGetCurrRostersReply.SerializeToString(&mGetCurrRostersRepStrWSreply);

//    qDebug() << "mGetCurrRostersRep->gamerosters_size()" << mGetCurrRostersRep->gamerosters_size();
//    qDebug() << "mGetCurrRostersRep.week" << mGetCurrRostersRep->week();
//    qDebug() << "mGetCurrRostersRepStrWSreply.size()" << mGetCurrRostersRepStrWSreply.size();
//    qDebug() << " zeroProjByName " << zeroProjByName.DebugString().data();
//    qDebug() << " zeroProjByName size" << zeroProjByName.playerproj_size();

    avgProjByName->mutable_playerproj()->CopyFrom(zeroProjByName.playerproj());

    for ( auto &pp : *avgProjByName->mutable_playerproj()) {
        pp.set_proj(mGateway->dataService->GetAvgProjection(pp.playerid()));
    }
//    qDebug() << " avgProjByName " << avgProjByName->DebugString().data();
//    qDebug() << " avgProjByName size" << avgProjByName->playerproj_size();


    for(auto mpm : mProjByNames ) {
        ProjByName *ppn = mpm.second;//mProjByNames[fPlayer.name()];
        if ( ppn == nullptr )
            ppn = ProjByName::default_instance().New();
        ppn->mutable_playerproj()->CopyFrom(zeroProjByName.playerproj());
    }

    for ( auto ii : mPlayerId2Index) {
        auto projmap = mGateway->dataService->GetProjById(ii.first);
        for ( auto proj : projmap ) {
            ProjByName *ppm = mProjByNames[proj.first];
            ppm->mutable_playerproj(ii.second)->set_proj(proj.second);
        }
    }
}

void Server::LiveProj(FantasyBitProj proj) {
    qDebug() << " LiveProj " << proj.DebugString().data();
    int index;
    PlayerProj *avgpp;
    auto it = mPlayerId2Index.find(proj.playerid());
    if ( it == end(mPlayerId2Index)) {
        PlayerProj *pp = zeroProjByName.add_playerproj();
        pp->set_playerid(proj.playerid());
        index = zeroProjByName.playerproj_size()-1;
        mPlayerId2Index.insert({pp->playerid(),index});
        qWarning() << " Server::LiveProj not found - added " << proj.DebugString().data();
        avgpp = avgProjByName->add_playerproj();
        avgpp->CopyFrom(*pp);
    }
    else {
        index = it->second;
        avgpp = avgProjByName->mutable_playerproj(index);
    }
    avgpp->set_proj(mGateway->dataService->GetAvgProjection(avgpp->playerid()));

    qDebug() << " avgpp " << avgpp->DebugString().data();

    ProjByName *ppn = mProjByNames[proj.name()];
    if ( ppn == nullptr )
        ppn = ProjByName::default_instance().New();

    if ( ppn->playerproj_size() == 0 ) {
        ppn->mutable_playerproj()->CopyFrom(zeroProjByName.playerproj());
    }
    else if ( index > ppn->playerproj_size()-1) {
        for ( int i = ppn->playerproj_size()-1; i <= index; i++ )
            ppn->add_playerproj()->CopyFrom(zeroProjByName.playerproj(i));
    }

//    ppn->set_block(proj.block());
//    ppn->set_count(proj.count());
    ppn->mutable_playerproj(index)->set_proj(proj.proj());

    qDebug() << " LiveProj ProjByName playerproj_size" << ppn->playerproj_size();
    qDebug() << " LiveProj avgProjByName playerproj_size" << avgProjByName->playerproj_size();
    SaveProj(proj);


}


Server *Server::myInstance;

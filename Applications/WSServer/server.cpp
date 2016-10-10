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
            this,      SLOT     (  GameStart(string)     ));
    connect( that, SIGNAL   (  GameOver(string)    ),
            this,      SLOT     (  GameOver(string)     ));
    connect( that, SIGNAL   (  onControlMessage(QString)    ),
            this,      SLOT     (  onControlMessage(QString)     ));


    connect( that, SIGNAL   (  NewFantasyName(fantasybit::FantasyNameBal)    ),
            this,  SLOT     (  OnNewFantasyName(fantasybit::FantasyNameBal)      ));

    connect( that, SIGNAL   (  AnyFantasyNameBalance(fantasybit::FantasyNameBal)    ),
            this,  SLOT     (  OnAnyFantasyNameBalance(fantasybit::FantasyNameBal)      ));


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
    }

    qDebug() << "Server GlobalStateRep " << GlobalStateRep.DebugString().data();

}

void Server::initData() {
    if ( mGateway == nullptr ) {
        qDebug() << " mGateway null ";
    }
    else if ( mGateway->dataService == nullptr)
        qDebug() << " mGateway->dataService null ";
    else {
//        updateLiveLeaders();

        auto lb = mGateway->dataService->GetLeaderBoard();
        qDebug() << " mGateway->dataService->GetLeaderBoard() " << lb.size();
        int i = 0;
        for(auto fPlayer  : lb ) {
            AddNames(FantasyName::toFantasyNameBal(*fPlayer));
//            if ( i++ > 250 )
//                break;
        }

        qDebug() << " all " << mAllNamesRep.DebugString().data();


        ScheduleRep.mutable_scheduledata()->set_week(GlobalStateRep.globalstate().week());

        ScheduleRep.mutable_scheduledata()->mutable_weekly()->CopyFrom(
            mGateway->dataService->GetWeeklySchedule(GlobalStateRep.globalstate().week()));


        mGetCurrRostersReply.set_ctype(GETGAMEROSTER);
        mGetCurrRostersRep = mGetCurrRostersReply.MutableExtension(GetCurrRostersRep::rep);
        for (GameRoster &gr : mGateway->dataService->GetCurrentWeekGameRosters() ) {
            GameDataRoster *gdr = mGetCurrRostersRep->add_gamerosters();
            GameData *gd = gdr->mutable_game_data();
            gd->set_gameid(gr.info.id());
            gd->mutable_status()->set_status(gr.status);

            TeamRoster *homet = gdr->mutable_homeroster();
            homet->set_teamid(gr.info.home());
            for ( auto &h : gr.homeroster) {
                PlayerData *pd = homet->add_players();
                pd->mutable_player_base()->CopyFrom(&h.second.base);
                pd->set_playerid(h.first);
                pd->mutable_player_status()->set_status(h.second.team_status);
            }

            TeamRoster *awayt = gdr->mutable_awayroster();
            awayt->set_teamid(gr.info.away());
            for ( auto &h : gr.awayroster()) {
                PlayerData *pd = awayt->add_players();
                pd->mutable_player_base()->CopyFrom(&h.second.base);
                pd->set_playerid(h.first);
                pd->mutable_player_status()->set_status(h.second.team_status);
            }
        }
        mGetCurrRostersRep->set_week(GlobalStateRep.globalstate().week());
        mGetCurrRostersReply.SerializeToArray(mGetCurrRostersRepStrBytes.data(),
                                            mGetCurrRostersReply.ByteSize());




//        if ( m_theWeek > 0  && m_theWeek < 17) {
//            //setCurrentWeekData();

//            m_pWeeklyScheduleModel->updateWeeklySchedule(m_theWeek,
//                          mGateway->dataService->GetWeeklySchedule(m_theWeek));
//            const auto &vgr = mGateway->dataService->GetCurrentWeekGameRosters();
//            mPlayerProjModel.updateRosters(vgr,mGateway->dataService);

//            for ( auto gr : vgr ) {
//                m_pWeeklyScheduleModel->UpdateStatus(gr.info.id(),gr.status);
//            }
//            if (myFantasyName != "" )
//                updateCurrentFantasyPlayerProjections();

//        }
    }
}


Server *Server::myInstance;

#include "server.h"

//fantasybit::GetAllNamesRep mAllNamesRep{};
//fantasybit::ExchangeData Server::TheExchange{};
//fantasybit::GetROWMarketRep Server::ROWMarketRep{};
//fantasybit::NFLStateData Server::NFLData;

//decltype(Server::Pk2Bal) Server::Pk2Bal{};
//decltype(Server::myNewNames) Server::myNewNames{};
using namespace fantasybit;

Server *Server::instance() {
    if (myInstance == NULL) {
        myInstance = new Server();
    }
    return myInstance;
}

void Server::setupConnection(pb::IPBGateway *ingateway) {

    QObject* that = dynamic_cast<QObject*>(ingateway);
    mOGateway = that;
    connect( that, SIGNAL   ( LiveGui(fantasybit::GlobalState)     ),
            this,      SLOT     (  LiveGui(fantasybit::GlobalState)     ));

//    connect(that, SIGNAL(NameStatus(fantasybit::MyFantasyName)),
//            this, SLOT(NameStatus(fantasybit::MyFantasyName)));

    connect( that, SIGNAL(LiveProj(fantasybit::FantasyBitProj)),
            this, SLOT(LiveProj(fantasybit::FantasyBitProj )));

//    connect( that, SIGNAL(MyNames(vector<fantasybit::MyFantasyName>)),
//            this, SLOT(MyNames(vector<fantasybit::MyFantasyName> )));
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

    connect( that, SIGNAL   (  NameBal(fantasybit::FantasyNameBal)    ),
            this,  SLOT     (  OnAnyFantasyNameBalance(fantasybit::FantasyNameBal)      ));

    connect( that, SIGNAL(Height(int)),
             this, SLOT(Height(int)));

    connect( that, SIGNAL(BlockNum(int)),
             this, SLOT(BlockNum(int)));
}

void Server::init() {
    connect( mOGateway, SIGNAL   ( LiveGui(fantasybit::GlobalState)     ),
            this,      SLOT     (  LiveGui(fantasybit::GlobalState)     ));

#ifdef WSSERVER_WRITE_TWEET
    sock.bind("tcp://127.0.0.1:5088");
    string strtweet("test tweet");
    sock.send(strtweet.data(), strtweet.size(),0);
#endif

    doReset = false;
    Core::instance()->guiIsAwake();
    ready();
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

//    auto gss = mGateway->dataService->GetAllSymbols();

    const auto &vms = mGateway->dataService->GetCurrentMarketSnaps();
    qDebug() << "  vms " << vms.size();
    for ( auto ms : vms ) {
//        string syb;
//        string symbol = ms.symbol();
//        auto iit = gss.lower_bound(symbol);
//        if ( iit != begin(gss)) {
//            syb = (--iit)->first;
//            int ret = syb.compare(0,syb.size(),symbol.data(),syb.size());
//            if ( ret == 0 &&
//                 (syb.size() != 4 || symbol.at(4) == '1') ) {
//                 qDebug() << "stripped raw symbol" << symbol.data() << syb.data() << iit->second.data();
//                 if ( it == nullptr ) {
//                    it = new PlayerQuoteSliceModelItem(symbol.data());
//                    mPlayerQuoteSliceModel.append(it);
//                 }
//            }
//                else
//                    qWarning() << " cant find " << syb.data() << " | " << symbol.data();
//            }
//        }
//        else {
//            syb = it->get_pPlayerSymbolsModelItem()->get_symbol().toStdString();
//        }

        ProcessMarketSnapShot(&ms);
    }
}



void Server::ProcessMarketSnapShot(fantasybit::MarketSnapshot* mt) {
    if ( mt->symbol() == "" )
        return;

    qDebug() << "server level2  " << mt->DebugString().data();
#ifdef TRACE2
    qDebug() << "level2 OnMarketSnapShot " << mt->DebugString().data();
#endif

    ROWMarket *pROWMarket = getRowmarket(mt->symbol());

    if ( mt->has_quote())
        pROWMarket->set_allocated_quote(mt->release_quote());

    if ( mt->has_ohlc())
        pROWMarket->set_allocated_ohlc(mt->release_ohlc());

//#ifdef TRACE2
//    qDebug() << "LiteServer::OnMarketSnapShot " << pROWMarket->DebugString().data();
//#endif

    if ( mt->depth_size() > 0 ) {
        GetDepthRep *depths = getDepthRep(mt->symbol());
        if ( depths == nullptr) {
            qDebug() << " error bad depth LiteServer::OnMarketSnapShot" << mt->DebugString().data();
            return;
        }

        depths->mutable_depthitems()->Swap(mt->mutable_depth());

#ifdef TRACE2
        qDebug() << "LiteServer::OnMarketSnapShot depths" << depths->DebugString().data();
#endif

//        Reserve(mt->depth_size());
//        for ( int i = mt->depth_size(); i >0 ; i--) {
//            mt->mutable_depth()->ReleaseLast();
//            depths->mutable_depthitems(i) add_depthitems(i) )
//        }
//        for ( auto *d : mt->
//        depths->Adepthitems().AddAllocated();
    }
}


        //        ScheduleRep.mutable_scheduledata()->mutable_weekly()->CopyFrom(
        //            mGateway->dataService->GetWeeklySchedule(GlobalStateRep.globalstate().week()));

void Server::resetRosterProjScheduleData() {
    qDebug() << " resetRosterProjScheduleData ";
    mGetCurrRostersReply.Clear();
    zeroProjByName.Clear();
    avgProjByName->Clear();
    mPlayerId2Index.clear();
    ScheduleRep.Clear();

    mGetCurrRostersReply.set_ctype(GETGAMEROSTER);
    mGetCurrRostersRep = mGetCurrRostersReply.MutableExtension(GetCurrRostersRep::rep);

    for (GameRoster &gr : mGateway->dataService->GetCurrentWeekGameRosters() ) {

        if ( gr.status != GameStatus_Status_SCHEDULED )
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

ROWMarket * Server::getRowmarket(const std::string &symbol) {

    ROWMarket *pROWMarket;
    std::unordered_map<std::string, ROWMarket *>::iterator it;

    if ( (it = mPidROWMarket.find(symbol)) == end(mPidROWMarket) ) {
        pROWMarket = mROWMarketRep.add_rowmarket();//new ROWMarket;
        mPidROWMarket.insert({symbol,pROWMarket});
        pROWMarket->set_pid(symbol);
        if ( symbol.size() < 5 )
            return nullptr;

        int ix =  (symbol.at(4) > '9') ?  5 : 4;
        string playerid = mGateway->dataService->GetPidfromSymb(symbol.substr(0,ix));
//        auto ppd = mGateway->dataService->GetPlayerDetail(playerid);
        PlayerData *pd = new PlayerData;
        pd->set_playerid(playerid);
        pd->mutable_player_base()->CopyFrom(mGateway->dataService->GetPlayerBase(playerid)); //TODO
        pd->mutable_player_status()->CopyFrom(mGateway->dataService->GetPlayerStatus(playerid));
        pROWMarket->set_allocated_playerdata(pd);
    }
    else
        pROWMarket = it->second;

    return pROWMarket;
}

GetDepthRep * Server::getDepthRep(const std::string &playerid) {

    GetDepthRep *pGetDepthRep;
    std::unordered_map<std::string, GetDepthRep *>::iterator it;

    if ( (it = mPidGetDepthRep.find(playerid)) == end(mPidGetDepthRep) ) {
//        if ( !Server::goodPid(playerid) )
//            return nullptr;
        pGetDepthRep = GetDepthRep::default_instance().New();
        mPidGetDepthRep.insert({playerid,pGetDepthRep});
        pGetDepthRep->set_pid(playerid);
    }
    else
        pGetDepthRep = it->second;

    return pGetDepthRep;
}

void Server::OnMarketTicker(fantasybit::MarketTicker mtv,int32_t blocknum) {
    fantasybit::MarketTicker *mt = &mtv;
#ifdef TRACE
    qDebug() << "level2 OnMarketTicker " << mt->DebugString().data();
#endif

    if ( mt->symbol() == "" ) return;
    saveMarket(mt->symbol());

    ROWMarket *pROWMarket = getRowmarket(mt->symbol());

    if ( mt->type() == MarketTicker_Type_BID) {
        pROWMarket->mutable_quote()->set_b(mt->price());
        pROWMarket->mutable_quote()->set_bs(mt->size());
    }
    else if ( mt->type() == MarketTicker_Type_ASK){
        pROWMarket->mutable_quote()->set_a(mt->price());
        pROWMarket->mutable_quote()->set_as(mt->size());
    }
    else if ( mt->type() == MarketTicker_Type_LAST){
        if ( pROWMarket->quote().l() > 0  )
            pROWMarket->mutable_quote()->set_udn( mt->price() -pROWMarket->quote().l() );
        pROWMarket->mutable_quote()->set_l(mt->price());
        pROWMarket->mutable_quote()->set_ls(mt->size());
    }
}

void Server::OnTradeTick(fantasybit::TradeTic* tt) {
#ifdef TRACE
    qDebug() << "level2 OnTradeTick " << tt->DebugString().data();
#endif

    if ( tt->symbol() == "" ) return;
    saveMarket(tt->symbol());
    ROWMarket *pROWMarket = getRowmarket(tt->symbol());

    pROWMarket->mutable_quote()->set_l(tt->price());
    pROWMarket->mutable_quote()->set_ls(tt->size());
    pROWMarket->mutable_quote()->set_udn(tt->tic());
    pROWMarket->mutable_ohlc()->set_change(tt->change());
    auto vol = pROWMarket->ohlc().volume() + tt->size();
    pROWMarket->mutable_ohlc()->set_volume(vol);
    if ( tt->ishigh() )
       pROWMarket->mutable_ohlc()->set_high(tt->price());

    if ( tt->islow() )
       pROWMarket->mutable_ohlc()->set_low(tt->price());

#ifdef WSSERVER_WRITE_TWEET
    TweetIt(tt);
#endif

}

void Server::OnDepthDelta(fantasybit::DepthFeedDelta* dfd) {
    if ( dfd->symbol() == "" )
        return;
#ifdef TRACE
    qDebug() << "Server OnDepthDelta " << dfd->DebugString().data();
#endif
    saveDepth(dfd->symbol());

    GetDepthRep *depths = getDepthRep(dfd->symbol());

    qDebug() << " before " << depths->DebugString().data();

    auto book = depths->mutable_depthitems()->begin();
    auto bsize = depths->depthitems_size();
//    depths->depthitems().iterator
    if ( dfd->isbid() )
    {
        bool nopush = false;
        if ( bsize > 0 ) {
            if ( book[bsize-1].b() == 0)
                nopush = true;;
        }

        for ( int i =0; i<bsize; i++) {
            if ( book[i].b() == 0 ) {
                if ( dfd->size() > 0 ) {
                    book[i].set_b(dfd->price());
                    book[i].set_bs(dfd->size());
                }
                break;
            }
            else if ( dfd->price() < book[i].b())  {
                if ( i < bsize-1)
                    continue;

                if ( dfd->size() > 0) {
                    DepthItem &bi = *(depths->mutable_depthitems()->Add());
                    book = depths->mutable_depthitems()->begin();
                    bsize = depths->depthitems_size();
                    bi.set_a(0);
                    bi.set_as(0);
                    bi.set_b(dfd->price());
                    bi.set_bs(dfd->size());
                }
                break;
            }
            else if ( dfd->price() > book[i].b() ) {
                if ( dfd->size() > 0) {
                    int end = bsize-1;
                    if ( nopush )
                        ;//end = bsize-1;
                    else {
                        //end = bsize-2;
                        DepthItem *bi = depths->mutable_depthitems()->Add();
                        book = depths->mutable_depthitems()->begin();
                        bsize = depths->depthitems_size();
                        bi->set_a(0);
                        bi->set_as(0);
                        bi->set_b(book[bsize-2].b());
                        bi->set_bs(book[bsize-2].bs());
                    }
                    for (int j=end;j > i;--j) {
                        if ( nopush && book[j-1].b() != 0 )
                            nopush = false;

                        if ( !nopush ) {
                            book[j].set_b(book[j-1].b());
                            book[j].set_bs(book[j-1].bs());
                        }
                    }

                    book[i].set_b(dfd->price());
                    book[i].set_bs(dfd->size());
                }

                break;
            }
            else {
                if ( dfd->size() > 0 )
                    book[i].set_bs(dfd->size());
                else {
                    int j=i;
                    for (;
                          j<bsize-1 && book[j].b() > 0;
                          ++j) {
                        book[j].set_b(book[j+1].b());
                        book[j].set_bs(book[j+1].bs());
                    }
                    book[j].set_b(0);
                    book[j].set_bs(0);
                }

                break;
            }
        }
    }
    else //!isbid
    {
        bool nopush = false;
        if ( bsize > 0 ) {
            if ( book[bsize-1].a() == 0)
                nopush = true;
        }

        for ( int i =0; i<bsize; i++) {
            if ( book[i].a()== 0 ) {
                if ( dfd->size() > 0 ) {
                    book[i].set_a(dfd->price());
                    book[i].set_as(dfd->size());
                }
                break;
            }
            else if ( dfd->price() > book[i].a())  {
                if ( i < bsize-1)
                    continue;

                if ( dfd->size() > 0) {
                    DepthItem &bi = *(depths->mutable_depthitems()->Add());
                    book = depths->mutable_depthitems()->begin();
                    bsize = depths->depthitems_size();
                    bi.set_b(0);
                    bi.set_bs(0);
                    bi.set_a(dfd->price());
                    bi.set_as(dfd->size());
                }
                break;
            }
            else if ( dfd->price() < book[i].a()) {
                if ( dfd->size() > 0) {
                    int end = bsize-1;
                    if ( nopush )
                        ;//end = dfd->size()-1;
                    else {
                        //end = dfd->size()-2;
                        DepthItem &bi = *(depths->mutable_depthitems()->Add());
                        book = depths->mutable_depthitems()->begin();
                        bsize = depths->depthitems_size();
                        bi.set_b(0);
                        bi.set_bs(0);
                        bi.set_a(book[bsize-2].a());
                        bi.set_as(book[bsize-2].as());
                    }
                    for (int j=end;j > i;--j) {
                        if ( nopush && book[j-1].a()!= 0 )
                            nopush = false;

                        if ( !nopush ) {
                            book[j].set_a(book[j-1].a());
                            book[j].set_as(book[j-1].as());
                        }
                    }

                    book[i].set_a(dfd->price());
                    book[i].set_as(dfd->size());
                }
                break;
            }
            else {
                if ( dfd->size() > 0 )
                    book[i].set_as(dfd->size());
                else {
                    int j=i;
                    for (;
                          j<bsize-1 && book[j].a()> 0;
                          ++j) {
                       book[j].set_a(book[j+1].a());
                       book[j].set_as(book[j+1].as());
                    }
                    book[j].set_a(0);
                    book[j].set_as(0);
                }
                break;
            }
        }
    }

    if (bsize == 0 && dfd->size() >0 && dfd->price() > 0) {
        DepthItem &bi = *(depths->mutable_depthitems()->Add());
        book = depths->mutable_depthitems()->begin();
        bsize = depths->depthitems_size();
        if ( dfd->isbid() ) {
            bi.set_a(0);
            bi.set_as(0);
            bi.set_b(dfd->price());
            bi.set_bs(dfd->size());
        }
        else {
            bi.set_b(0);
            bi.set_bs(0);
            bi.set_a(dfd->price());
            bi.set_as(dfd->size());
        }
    }
}

void Server::OnNewPos(fantasybit::FullPosition fp) {
    qDebug() << "level2 Trading::OnNewPos " << fp.pos.ToString().data() << fp.symbol.data() << fp.fname.data();

    savePos(fp.fname);
    fnameptrs &fptr = getfnameptrs(fp.fname);
    AllOdersSymbol *allords = getAllOdersSymbol(fptr,fp.symbol);
    if ( allords == nullptr ) return;

    allords->set_netprice(fp.pos.netprice);
    allords->set_avg(0);
    if ( fp.pos.netqty != 0 ) {
        allords->set_netqty(fp.pos.netqty);
        if ( fp.pos.netprice != 0  )
           allords->set_avg(fp.pos.netprice / (allords->netqty() * -1));
    }
    else {
        allords->set_pnl(fp.pos.netprice);
        allords->set_netqty(0);
    }
}

void Server::OnNewOO(fantasybit::FullOrderDelta fo) {
    qDebug() << "level2 Trading::OnNewOO " << fo.fname.data() << fo.openorder.DebugString().data();

    savePos(fo.fname);
    auto &o = fo.openorder;

    auto &fptr = getfnameptrs(fo.fname);
    auto it = fptr.mSeqOrderMap.find(o.refnum());
    bool found =  ( it != end(fptr.mSeqOrderMap));

    if ( o.core().size() <= 0) {
        if ( !found )
            return;
        else {
            Order *hold = it->second;
            hold->set_refnum(0);
            fptr.mSeqOrderMap.erase(it);
            auto it2 = fptr.fnamesymbolAllOrders.find(fo.symbol);
            if ( it2 != end(fptr.fnamesymbolAllOrders)) {
/*                if ( *hold == it2->second->mutable_orders()->rbegin() )
                    it2->second->orders().RemoveLast();
                else */
                {
                    auto &s = fptr.openOrderSlots[it2->second];
                    s.push(hold);
                    if ( s.size() == it2->second->orders_size() ) {
                       AllOdersSymbol *hold2 = it2->second;
                       hold2->clear_orders();
                       if ( hold2->netprice() == 0 && hold2->netqty() == 0 && hold2->pnl() == 0) {
                           hold2->clear_symbol();

                           fptr.fnamesymbolAllOrders.erase(it2);
                           if ( fptr.fnameAllOdersFname != nullptr) {
                                if ( fptr.openOrderSymbolSlot.size() +1 == fptr.fnameAllOdersFname->pidorders_size() )
                                    cleanIt(fptr);
                                else
                                    fptr.openOrderSymbolSlot.push(hold2);
                           }
                       }
                    }
                }
            }
        }
    }
    else if ( found ) {
        it->second->mutable_core()->set_size(fo.openorder.core().size());
    }
    else {
        AllOdersSymbol *allords = getAllOdersSymbol(fptr,fo.symbol);
        Order *po = addOrder(fptr,allords,fo.openorder);
        fptr.mSeqOrderMap[po->refnum()] = po;
    }

    //                            AllOdersFname *hold3 = it3->second;
    //                            openOrderSymbolSlot.erase(it3->second);
    //                            fname2sAllOdersFname.erase(it3);
    //                            hold3->Clear();
    //                            delete hold3;

}

Order * Server::addOrder(fnameptrs &fptr,AllOdersSymbol *allords,const Order &orderin) {
    auto &s = fptr.openOrderSlots[allords];
    if ( !s.empty() ) {
        Order *op = s.top();
        s.pop();
        op->CopyFrom(orderin);
//        delete orderin;
        return op;
    }
    else {
        auto ret = allords->add_orders();
        ret->CopyFrom(orderin);
        return ret;
    }
}

AllOdersSymbol * Server::getAllOdersSymbol(fnameptrs &fptr,const std::string &symbol) {
    auto it = fptr.fnamesymbolAllOrders.find(symbol);
    if ( it == end(fptr.fnamesymbolAllOrders)) {
        AllOdersSymbol *allords;
        auto &s = fptr.openOrderSymbolSlot;
        if ( !s.empty() ) {
            allords = s.top();
            s.pop();
        }
        else {
            allords = fptr.fnameAllOdersFname->mutable_pidorders()->Add();
//            allords = AllOdersSymbol::default_instance().New();
        }

        allords->set_symbol(symbol);
        fptr.fnamesymbolAllOrders.insert({symbol,allords});
        return allords;
    }
    else
        return it->second;
}

fnameptrs & Server::getfnameptrs(const std::string &fname, bool clean) {
    auto it = fnameptrsmap.find(fname);
    if ( it == end(fnameptrsmap)) {
        auto *aof = AllOdersFname::default_instance().New();
        aof->set_fname(fname);
        auto it2 = fnameptrsmap.insert({fname,aof});
#ifdef TRACE
        qDebug() << " getfnameptrs " << fname.data() << " new " << it2.first->second.fnameAllOdersFname->DebugString().data();
#endif
        return it2.first->second;
    }
    else {
        if ( clean )
            cleanIt(it->second);
        return it->second;
    }
}

void Server::getFnameSnap(const std::string &fname) {
    auto myorderpositions = mGateway->dataService->GetOrdersPositionsByName(fname);

#ifdef TRACE
    qDebug() << "lite getFnameSnap 1" << fname.data() << myorderpositions.size();
#endif
//    double totpnl = 0.0;

    fnameptrs &fptr = getfnameptrs(fname,true);
    for ( auto &p : myorderpositions ) {

        qDebug() << "lite getFnameSnap 2" << p.first << p.second.first.ToString().data();
        auto &mypair = p.second;
        auto &myorders = mypair.second;
//        {
//            auto &stake = openOrderSymbolSlot[allof];
//            stack.push(allords);
//        }
//        else {
        if ( !myorders.empty() || p.second.first.netprice != 0 || p.second.first.netqty != 0) {
            AllOdersSymbol *allords = getAllOdersSymbol(fptr,p.first);
            if ( allords == nullptr ) return;

            for ( auto o : myorders) {
                Order *po = addOrder(fptr,allords,o);
                fptr.mSeqOrderMap[o.refnum()] = po;
            }

            allords->set_netprice(p.second.first.netprice);
            allords->set_avg(0);
            if ( p.second.first.netqty != 0 ) {
                allords->set_netqty(p.second.first.netqty);
                if (  p.second.first.netprice != 0 ) {
                    double avg = (double)p.second.first.netprice / (double)p.second.first.netqty ;
                    avg  = avg * -1.0;
                    allords->set_avg(avg);
                }
            }
            else {
                allords->set_pnl(p.second.first.netprice);
                allords->set_netqty(0);
            }
        }

//        int netqty = p.second.first.netqty;
//        double avg = 0;
//        double pnl = 0;
//        if ( netqty ==0 ) {
//            pnl = p.second.first.netprice * 100;
//        }
//        else  {
//            ViewModel * item = mPlayerListModel.itemByKey(p.first.data());
//            int bid = item->propertyValue<PropertyNames::BID>().toInt();
//            int ask = item->propertyValue<PropertyNames::ASK>().toInt();
//            int price = (netqty > 0) ? bid :  ask;

//            if ( bid == 0 && ask == 0 )
//                pnl = 0;
//            else
//                pnl = 100 * ((price * netqty) + p.second.first.netprice);

//        }

//        mPlayerListModel.updateItemProperty<PropertyNames::MYPOS>(p.first.data(),netqty);
//        mPlayerListModel.updateItemProperty<PropertyNames::MYAVG>(p.first.data(),avg);
//        mPlayerListModel.updateItemProperty<PropertyNames::MYPNL>(p.first.data(),pnl);

//        if ( p.first == myPlayerid) {
//            ui->posQty->setValue(netqty);
//            ui->posAvgPrice->setValue(avg);
//            ui->posOpenPnl->setValue(pnl);
//        }

//        totpnl += pnl;

//    }

//    ui->fantasybitPnl->setValue(ui->fantasybitPnl->value()+totpnl);

    }
}

#ifdef WSSERVER_WRITE_TWEET
void Server::TweetIt(fantasybit::TradeTic *tt) {
    qDebug() << " TweetIt " << tt->DebugString().data();

    if ( !amLive )
        return;

    std::string strtweet;
    auto it = getRowmarket(tt->symbol());

    strtweet = it->playerdata().player_base().first() + " " +
            it->playerdata().player_base().last();
    if ( strtweet.length() > 31)
        strtweet = it->playerdata().player_base().last();

    strtweet += " (";
    std::string teamid = it->playerdata().player_status().teamid();
    if ( teamid == "") teamid = "FA";
    strtweet += teamid +", " + it->playerdata().player_base().position();

#ifndef SUPERBOWL52LIVE
    strtweet +=  !fantasybit::isWeekly(tt->symbol())
            ? (") " + to_string(GlobalStateRep.globalstate().season()) + " Season")
            : (") Week " + to_string(GlobalStateRep.globalstate().week()));

    std::string price = to_string(tt->price()) + (tt->tic() < 0 ? " ↓" : tt->tic() > 0 ? " ↑" : "");
    strtweet += " trading at: " + price;
    strtweet += "\n«" + tt->symbol() + "» @protoblock";
    strtweet += "\nhttp://protoblock.com/ticks.html?symbol=" + tt->symbol();
    strtweet += "\n#fantasyfootball #fantasybits $ƑɃ";
    qDebug() << " sending tweet " << strtweet.data();
    sock.send(strtweet.data(), strtweet.size(),0);
    qDebug() << " sent tweet " << strtweet.data();

#else
    std::string price = to_string(tt->price()) + (tt->tic() < 0 ? " ↓" : tt->tic() > 0 ? " ↑" : "");
    strtweet += ") trading at: " + price + " fantasy points for SuperBowl LII";
    strtweet += "\n«" + tt->symbol() + "» @protoblock";
    strtweet += "\nhttp://protoblock.com/ticks.html?symbol=" + tt->symbol();
    strtweet += "\n#fantasyfootball #fantasybits $ƑɃ #sb52 #blockchain";
    qDebug() << " sending tweet " << strtweet.data();
    sock.send(strtweet.data(), strtweet.size(),0);
    qDebug() << " sent tweet " << strtweet.data();

#endif

     /*
    "name"
    " (WR TEN) - 2017 Totals - last trade @ 100 "
    "http://protoblock.com/ticks.html?playerid=$RBRBa17s"
    "$RBRBa17s "
    "@protoblock #fantasyfootball"
    "12/21/17 12:12:12.12"

    "01234567890123456789012345678901 (WR TEN) - 2017 Totals - trading at 100 @protoblock "
    "http://protoblock.com/ticks.html?playerid=RBRBa17s"
    "$RBRBa17s  #fantasybits"
     */

    /*
    QString tweet = "%1 (%3, %2) trading at %4";
    tweet = tweet.arg(pname,pos,t,to_string(tt->price()).data());
    QString end = "\n%1\nWeek %2 %3\n@protoblock #fantasyfootball %4";
    end = end.arg(getLink(tt->symbol()), to_string(m_theWeek).data(),
                  TimetoTweetString(),m_pWeeklyScheduleModel->getTwitterGame(t) );
    string tosend = tweet.toStdString() + end.toStdString() ;
    qDebug() << " sending tweet " << tosend.data();
    sock.send(tosend.data(), tosend.size(),0);
    qDebug() << " sent tweet " << tosend.data();
    */

    /*
    bool doit = false;
    QString type(" ");
    if ( tt->ishigh() || tt->islow()) {
        int price = 0;
        auto &lasttime = mLastTweet[tt->symbol()];
        auto sincelast = (std::chrono::duration_cast<std::chrono::minutes>
                            (std::chrono::system_clock::now()-lasttime.second).count());

        if ( tt->islow() && (sincelast > 30 || lasttime.first)) {
            if ( !tt->ishigh() )
                type = " at new Low! ";
            lasttime.first = false;
            lasttime.second = std::chrono::system_clock::now();
            doit = true;;
        }
        else if ( tt->ishigh() && (sincelast > 30 || !lasttime.first)) {
            type = " at new High! ";
            lasttime.first = true;
            lasttime.second = std::chrono::system_clock::now();
            doit = true;
        }
    }

    if ( !doit ) {
         auto sincelast = (std::chrono::duration_cast<std::chrono::minutes>
                             (std::chrono::system_clock::now()-last_tweet).count());
         if ( sincelast > 30)
            doit = true;

         type = " ";
    }

    if ( !doit ) return;

    last_tweet = std::chrono::system_clock::now();

    */
}

#endif

Server *Server::myInstance;

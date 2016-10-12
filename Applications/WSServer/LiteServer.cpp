#include "liteserver.h"
#include "QtWebSockets/qwebsocketserver.h"
#include "QtWebSockets/qwebsocket.h"
#include <QtCore/QDebug>
//#include "tempapi.h"
#include "StateData.pb.h"
//#include "ldbhashreadertool.h"
//#include "pbjson.hpp"
//#include "src/protoblockapi.h"

#include "Commissioner.h"
//#include "txpool.h"
#include "server.h"

QT_USE_NAMESPACE


LiteServer::LiteServer(quint16 port, bool debug, QObject *parent) :
    QObject(parent),
    m_pWebSocketServer(new QWebSocketServer(QStringLiteral("WS LiteServer"),
                                            QWebSocketServer::NonSecureMode, this)),
    m_clients(),
    m_debug(debug)
{

    if (m_pWebSocketServer->listen(QHostAddress::Any, port)) {

        QHostAddress hInf = m_pWebSocketServer->serverAddress();

        qDebug() << "WS LiteServer " << hInf.toString() << " listening on port" << port << m_pWebSocketServer->serverName() << " ";
        connect(m_pWebSocketServer, &QWebSocketServer::newConnection,
                this, &LiteServer::onNewConnection);

        connect(m_pWebSocketServer, &QWebSocketServer::closed, this, &LiteServer::closed);
    }

    Server *server = Server::instance();

    connect (this,SIGNAL(error(QString)),this,SLOT(handleError(QString)));
    QObject::connect(&Server::TheExchange,SIGNAL(NewMarketSnapShot(fantasybit::MarketSnapshot*)),
                      this,SLOT(OnMarketSnapShot(fantasybit::MarketSnapshot*)));

    QObject::connect(&Server::TheExchange,SIGNAL(NewMarketTicker(fantasybit::MarketTicker *)),
                      this,SLOT(OnMarketTicker(fantasybit::MarketTicker *)));

    QObject::connect(&Server::TheExchange,SIGNAL(NewTradeTic(fantasybit::TradeTic*)),
                      this,SLOT(OnTradeTick(fantasybit::TradeTic*)));

    QObject::connect(&Server::TheExchange,SIGNAL(NewDepthDelta(fantasybit::DepthFeedDelta*)),
                      this,SLOT(OnDepthDelta(fantasybit::DepthFeedDelta*)));

    QObject::connect(&Server::TheExchange,
                     SIGNAL(NewOO(fantasybit::FullOrderDelta)),
                      this,SLOT(OnNewOO(fantasybit::FullOrderDelta)));

    QObject::connect(&Server::TheExchange,
                     SIGNAL(NewPos(fantasybit::FullPosition)),
                      this,SLOT(OnNewPos(fantasybit::FullPosition)));

}

LiteServer::~LiteServer()
{
    m_pWebSocketServer->close();
    qDeleteAll(m_clients.begin(), m_clients.end());
}

void LiteServer::OnMarketSnapShot(fantasybit::MarketSnapshot* mt) {
    if ( mt->symbol() == "" )
        return;
#ifdef TRACE2
    qDebug() << "level2 OnMarketSnapShot " << mt->DebugString().data();
#endif

    ROWMarket *pROWMarket = getRowmarket(mt->symbol());

    if ( mt->has_quote())
        pROWMarket->set_allocated_quote(mt->release_quote());

    if ( mt->has_ohlc())
        pROWMarket->set_allocated_ohlc(mt->release_ohlc());

#ifdef TRACE2
    qDebug() << "LiteServer::OnMarketSnapShot " << pROWMarket->DebugString().data();
#endif

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

void LiteServer::OnMarketTicker(fantasybit::MarketTicker* mt) {
#ifdef TRACE
    qDebug() << "level2 OnMarketTicker " << mt->DebugString().data();
#endif
    if ( mt->symbol() == "" ) return;

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

void LiteServer::OnTradeTick(fantasybit::TradeTic* tt) {
#ifdef TRACE
    qDebug() << "level2 OnTradeTick " << tt->DebugString().data();
#endif

    if ( tt->symbol() == "" ) return;
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

#ifdef TIMEAGENTWRITETWEETS
    TweetIt(tt);
#endif

}

ROWMarket * LiteServer::getRowmarket(const std::string &playerid) {

    ROWMarket *pROWMarket;
    std::unordered_map<std::string, ROWMarket *>::iterator it;

    if ( (it = mPidROWMarket.find(playerid)) == end(mPidROWMarket) ) {
        pROWMarket = mROWMarketRep.add_rowmarket();//new ROWMarket;
        mPidROWMarket.insert({playerid,pROWMarket});
        pROWMarket->set_pid(playerid);
        PlayerData *pd = new PlayerData;
        pd->set_playerid(playerid);
        pd->mutable_player_base()->CopyFrom(DataService::instance()->GetPlayerBase(playerid));
        pd->mutable_player_status()->CopyFrom(DataService::instance()->GetPlayerStatus(playerid));
        pROWMarket->set_allocated_playerdata(pd);
    }
    else
        pROWMarket = it->second;

    return pROWMarket;
}

GetDepthRep * LiteServer::getDepthRep(const std::string &playerid) {

    GetDepthRep *pGetDepthRep;
    std::unordered_map<std::string, GetDepthRep *>::iterator it;

    if ( (it = mPidGetDepthRep.find(playerid)) == end(mPidGetDepthRep) ) {
        if ( !Server::goodPid(playerid) )
            return nullptr;
        pGetDepthRep = GetDepthRep::default_instance().New();
        mPidGetDepthRep.insert({playerid,pGetDepthRep});
        pGetDepthRep->set_pid(playerid);
    }
    else
        pGetDepthRep = it->second;

    return pGetDepthRep;
}

void LiteServer::OnDepthDelta(fantasybit::DepthFeedDelta* dfd) {
    if ( dfd->symbol() == "" )
        return;
#ifdef TRACE
    qDebug() << "level2 OnDepthDelta " << dfd->DebugString().data();
#endif
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


void LiteServer::onNewConnection() {
    QWebSocket *pSocket = m_pWebSocketServer->nextPendingConnection();

    connect(pSocket, &QWebSocket::binaryMessageReceived, this, &LiteServer::processBinaryMessage);
    connect(pSocket, &QWebSocket::disconnected, this, &LiteServer::socketDisconnected);
    m_clients << pSocket;
}


//enum CType {
//CHECKNAME = 1;
//NEWTX = 2;
//PK2FNAME = 3;
//GETSTATUS = 4;
//GETALLNAMES = 5;
//GETROWMARKET = 6;
//GETDEPTH = 7;
//GETORDERS = 8;
//GETPOSITIONS = 9;
//SUBSCRIBEFNAME = 10;
//GETGLOBALSTATE = 11;
//GETSCHEDULE = 12;
//GETGAMEROSTER = 13;
//GETAVGPROJECTION = 14;
//GETPROJECTIONS = 15;
//}
void LiteServer::processBinaryMessage(const QByteArray &message) {
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());

    fantasybit::WsReq req;
    req.ParseFromString(message.toStdString());
    fantasybit::WSReply rep;

    qDebug() << " LITESERVER processBinaryMessage " << req.DebugString().data();
int i =0;
    switch ( req.ctype() ) {
        case SUBSCRIBEFNAME: {
            auto fname = req.GetExtension(SubscribeReq::req).name();
            auto &vec = mSocketSubscribed[pClient];
            vec.push_back(fname);
            auto &set = mFnameSubscribed[fname];
            if ( set.size() == 0 ) {
                getFnameSnap(fname);
                Server::TheExchange.Subscribe(fname);
                //todo get snap
            }
            set.insert(pClient);
            return;
        }
        case PK2FNAME: //return Fantasy Name given public Key
        {
            Pk2FnameRep pkr;
            pkr.mutable_req()->CopyFrom(req.GetExtension(Pk2FnameReq::req));
            rep.set_ctype(PK2FNAME);
            auto fname = Commissioner::getName(Commissioner::str2pk(pkr.req().pk()));
            if ( fname == nullptr)
                pkr.set_fname("");
            else {
                pkr.set_fname(fname->alias());
                auto it = Server::instance()->mPk2Bal.find(pkr.req().pk());
                if ( it != end(Server::instance()->mPk2Bal))
                    pkr.set_allocated_fnb(it->second);
            }

            rep.MutableExtension(Pk2FnameRep::rep)->CopyFrom(pkr);
            rep.SerializeToString(&mRepstr);
            if ( pkr.has_fnb() )
                pkr.release_fnb();

            break;
        }
        case CHECKNAME: //is name available
        {
            rep.set_ctype(CHECKNAME);
            CheckNameRep cr;
            cr.mutable_req()->CopyFrom(req.GetExtension(CheckNameReq::req));
            if ( fantasybit::Commissioner::isAliasAvailable(cr.req().fantasy_name()))
                cr.set_isavail("true");
            else
                cr.set_isavail("false");

            qDebug() << " liteserver " << cr.req().fantasy_name().data() << cr.isavail().data();

            cr.set_isavail("false");
            rep.MutableExtension(CheckNameRep::rep)->CopyFrom(cr);
            rep.SerializeToString(&mRepstr);
            break;
        }
        case GETALLNAMES: { //get all names
                            // todo: replace with leaderboard
            qDebug() << " GETALLNAMESGETALLNAMES ";
            rep.set_ctype(GETALLNAMES);
//            rep.SerializeToString(&mRepstr);
//            break;
    //            rep.MutableExtension(GetAllNamesRep::rep)->CopyFrom(*AllNamesRepPtr);
            GetAllNamesRep &ganp = Server::instance()->mAllNamesRep;
            rep.SetAllocatedExtension(GetAllNamesRep::rep,&(Server::instance()->mAllNamesRep));
//            qDebug() << "GETALLNAMES liteserver " << rep.DebugString().data();
            qDebug() << "GETALLNAMES liteserver1 ";
            qDebug() << " ganp.names_size() " << ganp.names_size();

//            for ( auto it : ganp.fnb())
//                qDebug() << it.DebugString().data();

//            qDebug() << "GETALLNAMES liteserver2 ";
//            qDebug() << " rep.names_size() " << rep.GetExtension(GetAllNamesRep::rep).names_size();
//            for ( auto it : rep.GetExtension(GetAllNamesRep::rep).fnb())
//                qDebug() << it.DebugString().data();

//            qDebug() << rep.DebugString().data();
            qDebug() << "GETALLNAMES liteserver3 ";
            qDebug() << "names-size " << Server::instance()->mAllNamesRep.names_size();
//            for ( auto it : Server::instance()->mAllNamesRep.fnb())
//                qDebug() << it.DebugString().data();

//            qDebug() << Server::instance()->mAllNamesRep.DebugString().data();
                // << Server::instance()->AllNamesRep.DebugString().data();
            rep.SerializeToString(&mRepstr);
            WSReply rep2;
            rep2.ParseFromString(mRepstr);
            qDebug() << "GETALLNAMES liteserver4 ";
            qDebug() << " rep2.names_size() " << rep2.GetExtension(GetAllNamesRep::rep).names_size();

//            for ( auto it : rep2.GetExtension(GetAllNamesRep::rep).fnb())
//                qDebug() << it.DebugString().data();


            rep.ReleaseExtension(GetAllNamesRep::rep);
            break;
        }

#ifdef PROD_SEASON_TRADING
        case GETDEPTH: {
            rep.set_ctype(GETDEPTH);
            auto &pid = req.GetExtension(GetDepthReq::req).pid();
            GetDepthRep *depths = getDepthRep(pid);
            if ( depths == nullptr ) {
                qDebug() << " bad market request " << pid;
                return;
            }
//            if ( depths == nullptr ) {
//                qDebug() << "depths == nullptr";
//                return;
//            }
            depths->set_allocated_rowmarket(getRowmarket(pid));
            rep.SetAllocatedExtension(GetDepthRep::rep,depths);
            rep.SerializeToString(&mRepstr);
#ifdef TRACE2
            qDebug() << rep.DebugString().data();
#endif
            rep.ReleaseExtension(GetDepthRep::rep);
            depths->release_rowmarket();
            break;
        }
        case GETROWMARKET:
//            rep.set_ctype(GETROWMARKET);
//    //            rep.SetAllocatedExtension( MutableExtension(GetAllNamesRep::rep)->CopyFrom(*AllNamesRepPtr);
//            rep.SerializeToString(&mRepstr);
//            break;

            rep.set_ctype(GETROWMARKET);
            rep.SetAllocatedExtension(GetROWMarketRep::rep,&mROWMarketRep);
            rep.SerializeToString(&mRepstr);
            rep.ReleaseExtension(GetROWMarketRep::rep);
            break;
#endif
        case GETORDERS: {
            rep.set_ctype(GETORDERS);
                GetOrdersRep gor;
            auto &fp = getfnameptrs(req.GetExtension(GetOrdersReq::req).fname());
            gor.set_allocated_oorders(fp.fnameAllOdersFname);
            rep.SetAllocatedExtension(GetOrdersRep::rep,&gor);
            rep.SerializeToString(&mRepstr);
            gor.release_oorders();
            rep.ReleaseExtension(GetOrdersRep::rep);
        }
        break;

        case GETGLOBALSTATE:
            qDebug() << "LiteServer server GlobalStateRep " << Server::instance()->GlobalStateRep.DebugString().data();

            rep.set_ctype(GETGLOBALSTATE);
            rep.SetAllocatedExtension(GetGlobalStateRep::rep,&Server::instance()->GlobalStateRep);
            rep.SerializeToString(&mRepstr);
            qDebug() << "LiteServer GlobalStateRep " << rep.DebugString().data();
            rep.ReleaseExtension(GetGlobalStateRep::rep);
            break;
        case GETSCHEDULE:
            rep.set_ctype(GETSCHEDULE);
            rep.SetAllocatedExtension(GetScheduleRep::rep,&Server::instance()->ScheduleRep);
            rep.SerializeToString(&mRepstr);
            rep.ReleaseExtension(GetScheduleRep::rep);
            break;
        case GETGAMEROSTER: {
            QByteArray qb(Server::instance()->mGetCurrRostersRepStrWSreply.data(),
                          (size_t)Server::instance()->mGetCurrRostersRepStrWSreply.size());
            pClient->sendBinaryMessage(qb);
//            rep.set_ctype(GETGAMEROSTER);
//            rep.SetAllocatedExtension(GetScheduleRep::rep,&Server::instance()->ScheduleRep);
//            rep.SerializeToString(&mRepstr);
//            rep.ReleaseExtension(GetScheduleRep::rep);
            return;
            break;
        }

        case GETPROJECTIONS: {

            qDebug() << " GETPROJECTIONS " << i++;
            rep.set_ctype(GETPROJECTIONS);
            qDebug() << " GETPROJECTIONS " << i++;

            ProjByName &ppn = Server::instance()->mProjByNames[req.GetExtension(GetProjectionReq::req).fname()];
            if ( ppn.name() != "" )
//                mGetProjectionRep.set_allocated_projs(&ppn);
                mGetProjectionRep.mutable_projs()->CopyFrom(ppn);
            qDebug() << " GETPROJECTIONS " << i++;

            mGetProjectionRep.mutable_avg()->CopyFrom(Server::instance()->avgProjByName);
//                        set_allocated_avg(&Server::instance()->avgProjByName);
            rep.SetAllocatedExtension(GetProjectionRep::rep,&mGetProjectionRep);
            qDebug() << " GETPROJECTIONS " << i++;

            rep.SerializeToString(&mRepstr);
            qDebug() << " GETPROJECTIONS " << i++;

            rep.ReleaseExtension(GetProjectionRep::rep);
//            mGetProjectionRep.release_avg();
            qDebug() << " GETPROJECTIONS ppn" << i++ << ppn.name().data();

            if ( ppn.name() != "" ) {
                qDebug() << " GETPROJECTIONS ppn try" << i++ << ppn.name().data();

//                mGetProjectionRep.release_projs();
            }
            qDebug() << " GETPROJECTIONS " << i++;

            break;
        }
        default:
            return;
    }

//        rep.mutable_req()->CopyFrom(req);

    QByteArray qb(mRepstr.data(),(size_t)mRepstr.size());
    qDebug() << "qb GETPROJECTIONS " << i++;

    pClient->sendBinaryMessage(qb);
    qDebug() << "sent qb GETPROJECTIONS " << i++;

    if ( rep.ctype() == GETALLNAMES ) {
        qDebug() << rep.ctype() <<" size " << Server::instance()->mAllNamesRep.names_size();
        WSReply rep2;
        rep2.ParseFromString(mRepstr);
        qDebug() << "GETALLNAMES liteserver5 ";
        qDebug() << " rep2.names_size() " << rep2.GetExtension(GetAllNamesRep::rep).names_size();

//        for ( auto it : rep2.GetExtension(GetAllNamesRep::rep).fnb())
//            qDebug() << it.DebugString().data();

    }
    else if ( rep.ctype() == GETROWMARKET)
        qDebug() << mROWMarketRep.DebugString().data();
    else if ( rep.ctype() == GETGLOBALSTATE) {
        WSReply gsr;
        gsr.ParseFromString(mRepstr);
        qDebug() << " LITESERVER out" << gsr.DebugString().data();
    }

    qDebug() << "clear GETPROJECTIONS " << i++;

    mRepstr.clear();
    return;
}

void LiteServer::socketDisconnected()
{
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
    qDebug() << "socketDisconnected:" << pClient  << " Reason: " << pClient->closeReason ();

    auto it = mSocketSubscribed.find(pClient);
    if ( it != end(mSocketSubscribed)) {
        for ( auto &fn : it->second) {
            auto iit = mFnameSubscribed.find(fn);
            if ( iit == end(mFnameSubscribed))
                continue;

            iit->second.erase(pClient);
            if ( iit->second.empty() ) {
                Server::TheExchange.UnSubscribe(fn);
                cleanIt(fn);
            }
        }
        mSocketSubscribed.erase(it);
    }

    if (pClient) {
        m_clients.removeAll(pClient);
        pClient->deleteLater();
    }
}

void LiteServer::handleError(const QString err)
{
    qDebug() << "LiteServer ProRoto Error " << err ;
}

void LiteServer::getFnameSnap(const std::string &fname) {
    auto myorderpositions = Server::TheExchange.GetOrdersPositionsByName(fname);

#ifdef TRACE
    qDebug() << "lite getFnameSnap 1" << fname.data() << myorderpositions.size();
#endif
//    double totpnl = 0.0;

    fnameptrs &fptr = getfnameptrs(fname,true);
    for ( auto &p : myorderpositions ) {

        qDebug() << "lite getFnameSnap 2" << p.first << p.second.first.ToString();
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
//            avg = p.second.first.netprice / (netqty * -1);
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
//    invalidateOrderFilters();

//}

void LiteServer::OnNewOO(const fantasybit::FullOrderDelta &fo) {
    qDebug() << "level2 Trading::OnNewOO " << fo.fname.data() << fo.openorder.DebugString().data();

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
            auto it2 = fptr.fnamesymbolAllOrders.find(fo.playerid);
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
        AllOdersSymbol *allords = getAllOdersSymbol(fptr,fo.playerid);
        Order *po = addOrder(fptr,allords,fo.openorder);
        fptr.mSeqOrderMap[po->refnum()] = po;
    }

    //                            AllOdersFname *hold3 = it3->second;
    //                            openOrderSymbolSlot.erase(it3->second);
    //                            fname2sAllOdersFname.erase(it3);
    //                            hold3->Clear();
    //                            delete hold3;

}

LiteServer::fnameptrs & LiteServer::getfnameptrs(const std::string &fname, bool clean) {
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

AllOdersSymbol * LiteServer::getAllOdersSymbol(fnameptrs &fptr,const std::string &symbol) {
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

void LiteServer::cleanIt(fnameptrs &fptr) {
//    for ( auto &all : fptr.fnamesymbolAllOrders ) {
//        all.second->orders.clear();
//    }
    fptr.fnameAllOdersFname->clear_pidorders();
    fptr.mSeqOrderMap.clear();
    fptr.fnamesymbolAllOrders.clear();
    fptr.openOrderSlots.clear();
    fptr.openOrderSymbolSlot = std::stack<AllOdersSymbol *> {};
}

void LiteServer::cleanIt(const std::string &fname) {
    auto it = fnameptrsmap.find(fname);
    if ( it != end(fnameptrsmap))
        cleanIt(it->second);
}


Order * LiteServer::addOrder(fnameptrs &fptr,AllOdersSymbol *allords,const Order &orderin) {
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

void LiteServer::OnNewPos(const fantasybit::FullPosition &fp) {
    qDebug() << "level2 Trading::OnNewPos " << fp.pos.ToString().data() << fp.playerid.data() << fp.fname.data();

    fnameptrs &fptr = getfnameptrs(fp.fname);
    AllOdersSymbol *allords = getAllOdersSymbol(fptr,fp.playerid);
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
//        int netqty = fp.pos.netqty;
//        double avg = 0;
//        double pnl = 0;
//        if ( netqty ==0 ) {
//            double pnl = fp.pos.netprice * 100;
//        }
//        else  {
//            ViewModel * item = mPlayerListModel.itemByKey(fp.playerid.data());
//            int price = item->propertyValue<PropertyNames::LAST>().toInt();
//            pnl = (price * netqty) + fp.pos.netprice;
//            avg = fp.pos.netprice / (netqty * -1);
//        }

//        mPlayerListModel.updateItemProperty<PropertyNames::MYPOS>(fp.playerid.data(),netqty);
//        mPlayerListModel.updateItemProperty<PropertyNames::MYPNL>(fp.playerid.data(),pnl);
//        mPlayerListModel.updateItemProperty<PropertyNames::MYAVG>(fp.playerid.data(),avg);
//        invalidateFilters();

//        if ( fp.playerid == myPlayerid ) {
//            ui->posQty->setValue(netqty);
//            ui->posAvgPrice->setValue(avg);
//            ui->posOpenPnl->setValue(pnl);
//        }
//    }
//}

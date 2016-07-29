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
#include "txpool.h"
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


    connect (this,SIGNAL(error(QString)),this,SLOT(handleError(QString)));
    QObject::connect(&Server::TheExchange,SIGNAL(NewMarketSnapShot(fantasybit::MarketSnapshot*)),
                      this,SLOT(OnMarketSnapShot(fantasybit::MarketSnapshot*)));

    QObject::connect(&Server::TheExchange,SIGNAL(NewMarketTicker(fantasybit::MarketTicker *)),
                      this,SLOT(OnMarketTicker(fantasybit::MarketTicker *)));

    QObject::connect(&Server::TheExchange,SIGNAL(NewTradeTic(fantasybit::TradeTic*)),
                      this,SLOT(OnTradeTick(fantasybit::TradeTic*)));

    QObject::connect(&Server::TheExchange,SIGNAL(NewDepthDelta(fantasybit::DepthFeedDelta*)),
                      this,SLOT(OnDepthDelta(fantasybit::DepthFeedDelta*)));

}

LiteServer::~LiteServer()
{
    m_pWebSocketServer->close();
    qDeleteAll(m_clients.begin(), m_clients.end());
}

void LiteServer::OnMarketSnapShot(fantasybit::MarketSnapshot* mt) {
    if ( mt->symbol() == "" )
        return;
#ifdef TRACE
    qDebug() << "level2 OnMarketSnapShot " << mt->DebugString().data();
#endif

#ifndef SEASON_TRADING
/*    if ( mt->week() != mCurrentWeek )
        SetCurrentWeekData(mt->wee*/k());
#endif

    ROWMarket *pROWMarket = getRowmarket(mt->symbol());

    if ( mt->has_quote())
        pROWMarket->set_allocated_quote(mt->release_quote());

    if ( mt->has_ohlc())
        pROWMarket->set_allocated_ohlc(mt->release_ohlc());

#ifdef TRACE
    qDebug() << "LiteServer::OnMarketSnapShot " << pROWMarket->DebugString().data();
#endif

    if ( mt->depth_size() > 0 ) {
        GetDepthRep *depths = getDepthRep(mt->symbol());
        if ( depths == nullptr) {
            qDebug() << " error bad depth LiteServer::OnMarketSnapShot" << mt->DebugString().data();
            return;
        }

        depths->mutable_depthitems()->Swap(mt->mutable_depth());

#ifdef TRACE
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
        pd->mutable_player_base()->CopyFrom(Server::NFLData.GetPlayerBase(playerid));
        pd->mutable_player_status()->CopyFrom(Server::NFLData.GetPlayerStatus(playerid));
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
                    DepthItem &bi = *(depths->add_depthitems());
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
                        DepthItem &bi = *(depths->add_depthitems());
                        book = depths->mutable_depthitems()->begin();
                        bsize = depths->depthitems_size();
                        bi.set_a(0);
                        bi.set_as(0);
                        bi.set_b(book[bsize-1].b());
                        bi.set_bs(book[bsize-1].bs());
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
                    DepthItem &bi = *(depths->add_depthitems());
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
                        DepthItem &bi = *(depths->add_depthitems());
                        book = depths->mutable_depthitems()->begin();
                        bsize = depths->depthitems_size();
                        bi.set_b(0);
                        bi.set_bs(0);
                        bi.set_a(book[bsize-1].a());
                        bi.set_as(book[bsize-1].as());
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
        DepthItem &bi = *(depths->add_depthitems());
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

    qDebug() << " after " << depths->DebugString().data();
}


void LiteServer::onNewConnection() {
    QWebSocket *pSocket = m_pWebSocketServer->nextPendingConnection();

    connect(pSocket, &QWebSocket::binaryMessageReceived, this, &LiteServer::processBinaryMessage);
    connect(pSocket, &QWebSocket::disconnected, this, &LiteServer::socketDisconnected);
    m_clients << pSocket;
}


//enum CType {
//    CHECKNAME = 1;
//    NEWTX = 2;
//    PK2FNAME = 3;
//    GETSTATUS = 4;
//    GETALLNAMES = 5;
//    GETROWMARKET = 6;
//    GETDEPTH = 7;
//}
void LiteServer::processBinaryMessage(const QByteArray &message) {
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());

    fantasybit::WsReq req;
    req.ParseFromString(message.toStdString());
    fantasybit::WSReply rep;

    qDebug() << " processBinaryMessage " << req.DebugString().data();

    switch ( req.ctype() ) {
        case SUBSCRIBEFNAME: {
            auto fname = req.GetExtension(SubscribeReq::req).name();
            auto &vec = mSocketSubscribed[pClient];
            vec.push_back(fname);
            auto &set = mFnameSubscribed[fname];
            if ( set.size() == 0 ) {
                auto bigcrap = Server::TheExchange.GetOrdersPositionsByName(fname);
                //Server::TheExchange.Subscribe(pname);
                //todo get snap
            }
            set.insert(pClient);
            return;
        }
        case PK2FNAME:
        {
            Pk2FnameRep pkr;
            pkr.mutable_req()->CopyFrom(req.GetExtension(Pk2FnameReq::req));
            rep.set_ctype(PK2FNAME);
            auto fname = Commissioner::getName(Commissioner::str2pk(pkr.req().pk()));
            if ( fname == nullptr)
                pkr.set_fname("");
            else {
                pkr.set_fname(fname->alias());
                auto it = Server::Pk2Bal.find(pkr.req().pk());
                if ( it != end(Server::Pk2Bal))
                    pkr.set_allocated_fnb(it->second);
            }

            rep.MutableExtension(Pk2FnameRep::rep)->CopyFrom(pkr);
            rep.SerializeToString(&mRepstr);
            if ( pkr.has_fnb() )
                pkr.release_fnb();

            break;
        }
        case CHECKNAME:
        {
            rep.set_ctype(CHECKNAME);
            CheckNameRep cr;
            cr.mutable_req()->CopyFrom(req.GetExtension(CheckNameReq::req));
            if ( fantasybit::Commissioner::isAliasAvailable(cr.req().fantasy_name()))
                cr.set_isavail("true");
            else
                cr.set_isavail("false");

            rep.MutableExtension(CheckNameRep::rep)->CopyFrom(cr);
            rep.SerializeToString(&mRepstr);
            break;
        }
        case GETALLNAMES: {
            rep.set_ctype(GETALLNAMES);
    //            rep.MutableExtension(GetAllNamesRep::rep)->CopyFrom(*AllNamesRepPtr);
            rep.SetAllocatedExtension(GetAllNamesRep::rep,&Server::AllNamesRep);
            rep.SerializeToString(&mRepstr);
            rep.ReleaseExtension(GetAllNamesRep::rep);
            break;
        }

#ifdef PROD_SEASON_TRADING
        case GETDEPTH: {
            rep.set_ctype(GETDEPTH);
            GetDepthRep *depths = getDepthRep(req.GetExtension(GetDepthReq::req).pid());
//            if ( depths == nullptr ) {
//                qDebug() << "depths == nullptr";
//                return;
//            }
            rep.SetAllocatedExtension(GetDepthRep::rep,depths);
            rep.SerializeToString(&mRepstr);
            qDebug() << rep.DebugString().data();
            rep.ReleaseExtension(GetDepthRep::rep);
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
        default:
            return;
    }

//        rep.mutable_req()->CopyFrom(req);

    QByteArray qb(mRepstr.data(),(size_t)mRepstr.size());
    pClient->sendBinaryMessage(qb);
    if ( rep.ctype() == GETALLNAMES )
        qDebug() << rep.ctype() <<" size " << Server::AllNamesRep.names_size();
    else if ( rep.ctype() == GETROWMARKET)
        qDebug() << mROWMarketRep.DebugString().data();
    else
        qDebug() << rep.DebugString().data();
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
                //todo cleanup
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

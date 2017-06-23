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
#include "server.h"

QT_USE_NAMESPACE


LiteServer::LiteServer(quint16 port, bool debug, QObject *parent) :
    mPort(port),
    QObject(parent),
    m_pWebSocketServer(new QWebSocketServer(QStringLiteral("WS LiteServer"),
                                    QWebSocketServer::NonSecureMode, this)),
    m_clients() {

    connect (this,SIGNAL(error(QString)),this,SLOT(handleError(QString)));

    gameStart.set_ctype(GETGAMESTART);
    mWSReplyGetProjectionRep.set_ctype(GETPROJECTIONS);
    mGetProjectionRep = mWSReplyGetProjectionRep.MutableExtension(GetProjectionRep::rep);

    mServer = Server::instance();

    connect(mServer,&Server::onNewProj, this, &LiteServer::onNewProj);
    connect(mServer,&Server::GameStart, this, &LiteServer::GameStart);
    connect(mServer,&Server::GoLive,this, &LiteServer::OnLive);
}

LiteServer::~LiteServer() {
    m_pWebSocketServer->close();
    qDeleteAll(m_clients.begin(), m_clients.end());
}

void LiteServer::OnLive() {
    if (m_pWebSocketServer->listen(QHostAddress::Any, mPort)) {

        QHostAddress hInf = m_pWebSocketServer->serverAddress();

        qDebug() << "WS LiteServer " << hInf.toString() << " listening on port" << mPort << m_pWebSocketServer->serverName() << " ";
        connect(m_pWebSocketServer, &QWebSocketServer::newConnection,
                this, &LiteServer::onNewConnection);

        connect(m_pWebSocketServer, &QWebSocketServer::closed, this, &LiteServer::closed);
    }
}

void LiteServer::GameStart(string gameid) {
    gameStart.set_data(gameid);
    QByteArray qbuf;
    qbuf.resize(gameStart.ByteSize());
    gameStart.SerializeToArray(qbuf.data(), qbuf.size());
    for (auto it : mSocketSubscribed )
        it.first->sendBinaryMessage(qbuf);
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
        case PK2FNAME: {
            Pk2FnameRep pkr;
            pkr.mutable_req()->CopyFrom(req.GetExtension(Pk2FnameReq::req));
            rep.set_ctype(PK2FNAME);
            auto fname = Commissioner::getName(Commissioner::str2pk(pkr.req().pk()));
            if ( fname == nullptr)
                pkr.set_fname("");
            else {
                pkr.set_fname(fname->alias());
                auto it = mServer->mPk2Bal.find(pkr.req().pk());
                if ( it != end(mServer->mPk2Bal))
                    pkr.set_allocated_fnb(it->second);
            }

            rep.MutableExtension(Pk2FnameRep::rep)->CopyFrom(pkr);
            rep.SerializeToString(&mRepstr);
            if ( pkr.has_fnb() )
                pkr.release_fnb();

            break;
        }

        case GETALLNAMES: {
            rep.set_ctype(GETALLNAMES);
            GetAllNamesRep &ganp = mServer->mAllNamesRep;
            rep.SetAllocatedExtension(GetAllNamesRep::rep,&(mServer->mAllNamesRep));
            rep.SerializeToString(&mRepstr);
            rep.ReleaseExtension(GetAllNamesRep::rep);
            break;
        }

        case GETGLOBALSTATE:
            qDebug() << "LiteServer server GlobalStateRep " << mServer->GlobalStateRep.DebugString().data();

            rep.set_ctype(GETGLOBALSTATE);
            rep.SetAllocatedExtension(GetGlobalStateRep::rep,&mServer->GlobalStateRep);
            rep.SerializeToString(&mRepstr);
            qDebug() << "LiteServer GlobalStateRep " << rep.DebugString().data();
            rep.ReleaseExtension(GetGlobalStateRep::rep);
            break;

        case GETSCHEDULE:
            rep.set_ctype(GETSCHEDULE);
            rep.SetAllocatedExtension(GetScheduleRep::rep,&mServer->ScheduleRep);
            rep.SerializeToString(&mRepstr);
            rep.ReleaseExtension(GetScheduleRep::rep);
            break;

        case GETGAMEROSTER: {
            QByteArray qb(mServer->mGetCurrRostersRepStrWSreply.data(),
                          (size_t)mServer->mGetCurrRostersRepStrWSreply.size());
            pClient->sendBinaryMessage(qb);
    //            rep.set_ctype(GETGAMEROSTER);
    //            rep.SetAllocatedExtension(GetScheduleRep::rep,&mServer->ScheduleRep);
    //            rep.SerializeToString(&mRepstr);
    //            rep.ReleaseExtension(GetScheduleRep::rep);
            return;
            break;
        }

        case GETPROJECTIONS: {
            auto &name = req.GetExtension(GetProjectionReq::req).fname();
            if ( name != "") {
                mFnameSubscribed[name].insert(pClient);
                mSocketSubscribed[pClient].push_back(name);
            }
            doSendProjections(pClient,name);
            return;
            break;
        }

        case GETDEPTH: {
            rep.set_ctype(GETDEPTH);
            auto &pid = req.GetExtension(GetDepthReq::req).pid();
            GetDepthRep *depths = mServer->getDepthRep(pid);
            if ( depths == nullptr ) {
                qDebug() << " bad market request " << pid;
                return;
            }
//            if ( depths == nullptr ) {
//                qDebug() << "depths == nullptr";
//                return;
//            }
            depths->set_allocated_rowmarket(mServer->getRowmarket(pid));
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
            rep.SetAllocatedExtension(GetROWMarketRep::rep,&mServer->mROWMarketRep);
            rep.SerializeToString(&mRepstr);
            rep.ReleaseExtension(GetROWMarketRep::rep);
            break;

        case GETORDERS: {
            rep.set_ctype(GETORDERS);
            GetOrdersRep gor;
            auto &fp = mServer->getfnameptrs(req.GetExtension(GetOrdersReq::req).fname());
            gor.set_allocated_oorders(fp.fnameAllOdersFname);
            rep.SetAllocatedExtension(GetOrdersRep::rep,&gor);
            rep.SerializeToString(&mRepstr);
            gor.release_oorders();
            rep.ReleaseExtension(GetOrdersRep::rep);
            break;
        }

        default:
            break;
            return;
    }

//        rep.mutable_req()->CopyFrom(req);

    QByteArray qb(mRepstr.data(),(size_t)mRepstr.size());
    pClient->sendBinaryMessage(qb);

    if ( rep.ctype() == GETALLNAMES )
        qDebug() << rep.ctype() <<" size " << mServer->mAllNamesRep.names_size();
    else if ( rep.ctype() == GETROWMARKET)
        qDebug() << mServer->mROWMarketRep.DebugString().data();

    mRepstr.clear();
    return;
}

void LiteServer::doSendProjections(QWebSocket *pClient,const std::string & fname ) {
    ProjByName *ppn = nullptr;
    if ( fname != "" ) {
        ppn = mServer->mProjByNames[fname];
        mGetProjectionRep->set_allocated_projs(ppn);
    }

    mGetProjectionRep->set_allocated_avg(mServer->avgProjByName);
    mWSReplybyteArray.resize(mWSReplyGetProjectionRep.ByteSize());
    mWSReplyGetProjectionRep.SerializeToArray(mWSReplybyteArray.data(), mWSReplybyteArray.size());
    mServer->avgProjByName = mGetProjectionRep->release_avg();
    if ( ppn != nullptr )
        ppn = mGetProjectionRep->release_projs();
    pClient->sendBinaryMessage(mWSReplybyteArray);
}


void LiteServer::socketDisconnected() {
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
               mServer->UnSubscribe(fn);
               mServer->cleanIt(fn);
            }
        }
        mSocketSubscribed.erase(it);
    }

    if (pClient) {
        m_clients.removeAll(pClient);
        pClient->deleteLater();
    }
}

/*
void LiteServer::socketDisconnected() {
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
    qDebug() << "socketDisconnected:" << pClient  << " Reason: " << pClient->closeReason ();

    mFnameSubscribed.erase(mSocketSubscribed[pClient]);
    mSocketSubscribed.erase(pClient);

    pClient->deleteLater();
}
*/

void LiteServer::handleError(const QString err) {
    qDebug() << "LiteServer ProRoto Error " << err ;
}


/*
void LiteServer::doSendLeaders(QWebSocket *pClient) {
    fantasybit::WSReply rep;
    rep.set_ctype(GETALLNAMES);
    GetAllNamesRep &ganp = mServer->mAllNamesRep;
    rep.SetAllocatedExtension(GetAllNamesRep::rep,&(mServer->mAllNamesRep));
    rep.SerializeToString(&mRepstr);
    rep.ReleaseExtension(GetAllNamesRep::rep);
    QByteArray qb(mRepstr.data(),(size_t)mRepstr.size());
    pClient->sendBinaryMessage(qb);
}
*/
/*
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


void LiteServer::OnMarketSnapShot(fantasybit::MarketSnapshot* mt) {
    if ( mt->symbol() == "" )
        return;
#ifdef TRACE2
    qDebug() << "level2 OnMarketSnapShot " << mt->DebugString().data();
#endif

#ifndef SEASON_TRADING
/*    if ( mt->week() != mCurrentWeek )
        SetCurrentWeekData(mt->week());
//#endif

//    ROWMarket *pROWMarket = getRowmarket(mt->symbol());

//    if ( mt->has_quote())
//        pROWMarket->set_allocated_quote(mt->release_quote());

//    if ( mt->has_ohlc())
//        pROWMarket->set_allocated_ohlc(mt->release_ohlc());

////#ifdef TRACE2
////    qDebug() << "LiteServer::OnMarketSnapShot " << pROWMarket->DebugString().data();
////#endif

//    if ( mt->depth_size() > 0 ) {
//        GetDepthRep *depths = getDepthRep(mt->symbol());
//        if ( depths == nullptr) {
//            qDebug() << " error bad depth LiteServer::OnMarketSnapShot" << mt->DebugString().data();
//            return;
//        }

//        depths->mutable_depthitems()->Swap(mt->mutable_depth());

//#ifdef TRACE2
//        qDebug() << "LiteServer::OnMarketSnapShot depths" << depths->DebugString().data();
//#endif

////        Reserve(mt->depth_size());
////        for ( int i = mt->depth_size(); i >0 ; i--) {
////            mt->mutable_depth()->ReleaseLast();
////            depths->mutable_depthitems(i) add_depthitems(i) )
////        }
////        for ( auto *d : mt->
////        depths->Adepthitems().AddAllocated();
//    }
//}

/*

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







*/


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

/*
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

*/

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
//    m_clients(),
    m_debug(debug)
{

    gameStart.set_ctype(GETGAMESTART);
    mWSReplyGetProjectionRep.set_ctype(GETPROJECTIONS);
    mGetProjectionRep = mWSReplyGetProjectionRep.MutableExtension(GetProjectionRep::rep);

    if (m_pWebSocketServer->listen(QHostAddress::Any, port)) {

        QHostAddress hInf = m_pWebSocketServer->serverAddress();

        qDebug() << "WS LiteServer " << hInf.toString() << " listening on port" << port << m_pWebSocketServer->serverName() << " ";
        connect(m_pWebSocketServer, &QWebSocketServer::newConnection,
                this, &LiteServer::onNewConnection);

        connect(m_pWebSocketServer, &QWebSocketServer::closed, this, &LiteServer::closed);
    }

    Server *server = Server::instance();

    connect(server,&Server::onNewProj, this, &LiteServer::onNewProj);
    connect(server,&Server::GameStart, this, &LiteServer::GameStart);

    connect (this,SIGNAL(error(QString)),this,SLOT(handleError(QString)));
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

    mSocketSubscribed[pSocket] = "";
    connect(pSocket, &QWebSocket::binaryMessageReceived, this, &LiteServer::processBinaryMessage);
    connect(pSocket, &QWebSocket::disconnected, this, &LiteServer::socketDisconnected);

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
//    req.ParseFromString(message.toStdString());
    req.ParseFromArray(message.data(),message.size());
    fantasybit::WSReply rep;

    qDebug() << " LITESERVER processBinaryMessage " << req.DebugString().data();
int i =0;
    switch ( req.ctype() ) {
//        case SUBSCRIBEFNAME: {
//            auto fname = req.GetExtension(SubscribeReq::req).name();
//            auto &vec = mSocketSubscribed[pClient];
//            vec.push_back(fname);
//            auto &set = mFnameSubscribed[fname];
//            if ( set.size() == 0 ) {
//                getFnameSnap(fname);
//                Server::TheExchange.Subscribe(fname);
//                //todo get snap
//            }
//            set.insert(pClient);
//            return;
//        }
//        case CHECKNAME: //is name available
//        {
//            rep.set_ctype(CHECKNAME);
//            CheckNameRep cr;
//            cr.mutable_req()->CopyFrom(req.GetExtension(CheckNameReq::req));
//            if ( fantasybit::Commissioner::isAliasAvailable(cr.req().fantasy_name()))
//                cr.set_isavail("true");
//            else
//                cr.set_isavail("false");

//            qDebug() << " liteserver " << cr.req().fantasy_name().data() << cr.isavail().data();

//            cr.set_isavail("false");
//            rep.MutableExtension(CheckNameRep::rep)->CopyFrom(cr);
//            rep.SerializeToString(&mRepstr);
//            break;
//        }
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

            auto &name = req.GetExtension(GetProjectionReq::req).fname();
            if ( name != "") {
                mFnameSubscribed[name] = pClient;
                mSocketSubscribed[pClient] = name;
            }
            doSendProjections(pClient,name);
            return;
            break;
        }



//            qDebug() << " GETPROJECTIONS " << i++;
//            qDebug() << " GETPROJECTIONS " << i++;

////                mGetProjectionRep.set_allocated_projs(&ppn);
//            qDebug() << " GETPROJECTIONS " << i++;

////                        set_allocated_avg(&Server::instance()->avgProjByName);
//            qDebug() << " GETPROJECTIONS " << i++;


//            qDebug() << " GETPROJECTIONS " << i++;


////            mGetProjectionRep.release_avg();
//            qDebug() << " GETPROJECTIONS ppn" << i++ << ppn.name().data();

//            if ( ppn.name() != "" ) {
//                qDebug() << " GETPROJECTIONS ppn try" << i++ << ppn.name().data();

////                mGetProjectionRep.release_projs();
//            }
//            qDebug() << " GETPROJECTIONS " << i++;

//            break;
//        }
        default:
            return;
    }

//        rep.mutable_req()->CopyFrom(req);

    QByteArray qb(mRepstr.data(),(size_t)mRepstr.size());

    pClient->sendBinaryMessage(qb);


    if ( rep.ctype() == GETALLNAMES ) {
        qDebug() << rep.ctype() <<" size " << Server::instance()->mAllNamesRep.names_size();
        WSReply rep2;
        rep2.ParseFromString(mRepstr);
        qDebug() << "GETALLNAMES liteserver5 ";
        qDebug() << " rep2.names_size() " << rep2.GetExtension(GetAllNamesRep::rep).names_size();

//        for ( auto it : rep2.GetExtension(GetAllNamesRep::rep).fnb())
//            qDebug() << it.DebugString().data();

    }
//    else if ( rep.ctype() == GETROWMARKET)
//        qDebug() << mROWMarketRep.DebugString().data();
//    else if ( rep.ctype() == GETGLOBALSTATE) {
//        WSReply gsr;
//        gsr.ParseFromString(mRepstr);
//        qDebug() << " LITESERVER out" << gsr.DebugString().data();
//    }


    mRepstr.clear();
    return;
}


void LiteServer::doSendProjections(QWebSocket *pClient,const std::string & fname ) {
    ProjByName *ppn = nullptr;
    if ( fname != "" ) {
        ppn = Server::instance()->mProjByNames[fname];
        mGetProjectionRep->set_allocated_projs(ppn);
    }

    mGetProjectionRep->set_allocated_avg(Server::instance()->avgProjByName);
    mWSReplybyteArray.resize(mWSReplyGetProjectionRep.ByteSize());
    mWSReplyGetProjectionRep.SerializeToArray(mWSReplybyteArray.data(), mWSReplybyteArray.size());
    Server::instance()->avgProjByName = mGetProjectionRep->release_avg();
    if ( ppn != nullptr )
        ppn = mGetProjectionRep->release_projs();
    pClient->sendBinaryMessage(mWSReplybyteArray);
}

void LiteServer::socketDisconnected()
{
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
    qDebug() << "socketDisconnected:" << pClient  << " Reason: " << pClient->closeReason ();

    mFnameSubscribed.erase(mSocketSubscribed[pClient]);
    mSocketSubscribed.erase(pClient);
//    auto it = mSocketSubscribed.find(pClient);

    pClient->deleteLater();
}

void LiteServer::handleError(const QString err)
{
    qDebug() << "LiteServer ProRoto Error " << err ;
}

//void LiteServer::getFnameSnap(const std::string &fname) {
//    auto myorderpositions = Server::TheExchange.GetOrdersPositionsByName(fname);

//#ifdef TRACE
//    qDebug() << "lite getFnameSnap 1" << fname.data() << myorderpositions.size();
//#endif
////    double totpnl = 0.0;

//    fnameptrs &fptr = getfnameptrs(fname,true);
//    for ( auto &p : myorderpositions ) {

//        qDebug() << "lite getFnameSnap 2" << p.first << p.second.first.ToString();
//        auto &mypair = p.second;
//        auto &myorders = mypair.second;
////        {
////            auto &stake = openOrderSymbolSlot[allof];
////            stack.push(allords);
////        }
////        else {
//        if ( !myorders.empty() || p.second.first.netprice != 0 || p.second.first.netqty != 0) {
//            AllOdersSymbol *allords = getAllOdersSymbol(fptr,p.first);
//            if ( allords == nullptr ) return;

//            for ( auto o : myorders) {
//                Order *po = addOrder(fptr,allords,o);
//                fptr.mSeqOrderMap[o.refnum()] = po;
//            }

//            allords->set_netprice(p.second.first.netprice);
//            allords->set_avg(0);
//            if ( p.second.first.netqty != 0 ) {
//                allords->set_netqty(p.second.first.netqty);
//                if (  p.second.first.netprice != 0 ) {
//                    double avg = (double)p.second.first.netprice / (double)p.second.first.netqty ;
//                    avg  = avg * -1.0;
//                    allords->set_avg(avg);
//                }
//            }
//            else {
//                allords->set_pnl(p.second.first.netprice);
//                allords->set_netqty(0);
//            }
//        }

////        int netqty = p.second.first.netqty;
////        double avg = 0;
////        double pnl = 0;
////        if ( netqty ==0 ) {
////            pnl = p.second.first.netprice * 100;
////        }
////        else  {
////            ViewModel * item = mPlayerListModel.itemByKey(p.first.data());
////            int bid = item->propertyValue<PropertyNames::BID>().toInt();
////            int ask = item->propertyValue<PropertyNames::ASK>().toInt();
////            int price = (netqty > 0) ? bid :  ask;

////            if ( bid == 0 && ask == 0 )
////                pnl = 0;
////            else
////                pnl = 100 * ((price * netqty) + p.second.first.netprice);

////        }

////        mPlayerListModel.updateItemProperty<PropertyNames::MYPOS>(p.first.data(),netqty);
////        mPlayerListModel.updateItemProperty<PropertyNames::MYAVG>(p.first.data(),avg);
////        mPlayerListModel.updateItemProperty<PropertyNames::MYPNL>(p.first.data(),pnl);

////        if ( p.first == myPlayerid) {
////            ui->posQty->setValue(netqty);
////            ui->posAvgPrice->setValue(avg);
////            ui->posOpenPnl->setValue(pnl);
////        }

////        totpnl += pnl;

////    }

////    ui->fantasybitPnl->setValue(ui->fantasybitPnl->value()+totpnl);

//    }
//}

////        int netqty = p.second.first.netqty;
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

//void LiteServer::OnNewOO(const fantasybit::FullOrderDelta &fo) {
//    qDebug() << "level2 Trading::OnNewOO " << fo.fname.data() << fo.openorder.DebugString().data();

//    auto &o = fo.openorder;

//    auto &fptr = getfnameptrs(fo.fname);
//    auto it = fptr.mSeqOrderMap.find(o.refnum());
//    bool found =  ( it != end(fptr.mSeqOrderMap));

//    if ( o.core().size() <= 0) {
//        if ( !found )
//            return;
//        else {
//            Order *hold = it->second;
//            hold->set_refnum(0);
//            fptr.mSeqOrderMap.erase(it);
//            auto it2 = fptr.fnamesymbolAllOrders.find(fo.playerid);
//            if ( it2 != end(fptr.fnamesymbolAllOrders)) {
///*                if ( *hold == it2->second->mutable_orders()->rbegin() )
//                    it2->second->orders().RemoveLast();
//                else */
//                {
//                    auto &s = fptr.openOrderSlots[it2->second];
//                    s.push(hold);
//                    if ( s.size() == it2->second->orders_size() ) {
//                       AllOdersSymbol *hold2 = it2->second;
//                       hold2->clear_orders();
//                       if ( hold2->netprice() == 0 && hold2->netqty() == 0 && hold2->pnl() == 0) {
//                           hold2->clear_symbol();

//                           fptr.fnamesymbolAllOrders.erase(it2);
//                           if ( fptr.fnameAllOdersFname != nullptr) {
//                                if ( fptr.openOrderSymbolSlot.size() +1 == fptr.fnameAllOdersFname->pidorders_size() )
//                                    cleanIt(fptr);
//                                else
//                                    fptr.openOrderSymbolSlot.push(hold2);
//                           }
//                       }
//                    }
//                }
//            }
//        }
//    }
//    else if ( found ) {
//        it->second->mutable_core()->set_size(fo.openorder.core().size());
//    }
//    else {
//        AllOdersSymbol *allords = getAllOdersSymbol(fptr,fo.playerid);
//        Order *po = addOrder(fptr,allords,fo.openorder);
//        fptr.mSeqOrderMap[po->refnum()] = po;
//    }

//    //                            AllOdersFname *hold3 = it3->second;
//    //                            openOrderSymbolSlot.erase(it3->second);
//    //                            fname2sAllOdersFname.erase(it3);
//    //                            hold3->Clear();
//    //                            delete hold3;

//}

//LiteServer::fnameptrs & LiteServer::getfnameptrs(const std::string &fname, bool clean) {
//    auto it = fnameptrsmap.find(fname);
//    if ( it == end(fnameptrsmap)) {
//        auto *aof = AllOdersFname::default_instance().New();
//        aof->set_fname(fname);
//        auto it2 = fnameptrsmap.insert({fname,aof});
//#ifdef TRACE
//        qDebug() << " getfnameptrs " << fname.data() << " new " << it2.first->second.fnameAllOdersFname->DebugString().data();
//#endif
//        return it2.first->second;
//    }
//    else {
//        if ( clean )
//            cleanIt(it->second);
//        return it->second;
//    }
//}

//AllOdersSymbol * LiteServer::getAllOdersSymbol(fnameptrs &fptr,const std::string &symbol) {
//    auto it = fptr.fnamesymbolAllOrders.find(symbol);
//    if ( it == end(fptr.fnamesymbolAllOrders)) {
//        AllOdersSymbol *allords;
//        auto &s = fptr.openOrderSymbolSlot;
//        if ( !s.empty() ) {
//            allords = s.top();
//            s.pop();
//        }
//        else {
//            allords = fptr.fnameAllOdersFname->mutable_pidorders()->Add();
////            allords = AllOdersSymbol::default_instance().New();
//        }

//        allords->set_symbol(symbol);
//        fptr.fnamesymbolAllOrders.insert({symbol,allords});
//        return allords;
//    }
//    else
//        return it->second;
//}

//void LiteServer::cleanIt(fnameptrs &fptr) {
////    for ( auto &all : fptr.fnamesymbolAllOrders ) {
////        all.second->orders.clear();
////    }
//    fptr.fnameAllOdersFname->clear_pidorders();
//    fptr.mSeqOrderMap.clear();
//    fptr.fnamesymbolAllOrders.clear();
//    fptr.openOrderSlots.clear();
//    fptr.openOrderSymbolSlot = std::stack<AllOdersSymbol *> {};
//}

//void LiteServer::cleanIt(const std::string &fname) {
//    auto it = fnameptrsmap.find(fname);
//    if ( it != end(fnameptrsmap))
//        cleanIt(it->second);
//}


//Order * LiteServer::addOrder(fnameptrs &fptr,AllOdersSymbol *allords,const Order &orderin) {
//    auto &s = fptr.openOrderSlots[allords];
//    if ( !s.empty() ) {
//        Order *op = s.top();
//        s.pop();
//        op->CopyFrom(orderin);
////        delete orderin;
//        return op;
//    }
//    else {
//        auto ret = allords->add_orders();
//        ret->CopyFrom(orderin);
//        return ret;
//    }
//}

//void LiteServer::OnNewPos(const fantasybit::FullPosition &fp) {
//    qDebug() << "level2 Trading::OnNewPos " <f!L< fp.pos.ToString().data() << fp.playerid.data() << fp.fname.data();

//    fnameptrs &fptr = getfnameptrs(fp.fname);
//    AllOdersSymbol *allords = getAllOdersSymbol(fptr,fp.playerid);
//    if ( allords == nullptr ) return;

//    allords->set_netprice(fp.pos.netprice);
//    allords->set_avg(0);
//    if ( fp.pos.netqty != 0 ) {
//        allords->set_netqty(fp.pos.netqty);
//        if ( fp.pos.netprice != 0  )
//           allords->set_avg(fp.pos.netprice / (allords->netqty() * -1));
//    }
//    else {
//        allords->set_pnl(fp.pos.netprice);
//        allords->set_netqty(0);
//    }
//}
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

/*
//  mesh.h
//
//  Protoblock
//
//  Created by Jay Berg on 5/7/2018
//
*/
#ifndef MESH_H
#define MESH_H

#include <QDebug>
#include <string>
#include <map>
#include <set>

#include <nng/nng.h>
#include <nng/supplemental/util/platform.h>
#include <nng/protocol/bus0/bus.h>


#include <QHostAddress>

#include <qendian.h>

namespace pb {

class mesh;
struct work {
    enum { INIT, RECV, WAIT, SEND } state;
    nng_aio *aio;
    nng_msg *msg;
    nng_ctx  ctx;
    nng_dialer dialer;
    std::string peer;
    mesh *mesh;
};

class mesh : public QObject
{
    Q_OBJECT

    nng_socket   _socket;
    nng_listener listener;
    std::set<std::string> connected;
    std::set<std::string> known;
    std::map<std::string,nng_dialer> dialers;
    std::map<std::string,nng_pipe> listen_pipes;

    int numKnownConnected;
public:
    mesh();

    nng_aio      *receiveAio;

    void sendForProcessing(const std::string &msg) {

    }

    void init() {
        nng_bus_open(&_socket);
        nng_pipe_notify(_socket, &mesh::onNotify, this);
        int rv = nng_listen(_socket, "tcp://[]:10130", &listener, 0);

        if ((rv = nng_aio_alloc(&receiveAio, &mesh::callBack, this)) != 0) {
        }
    }

    void recvAio() {
        nng_recv_aio(_socket,receiveAio);
    }

    void connectTO(std::string peer) {
        qDebug() << "mesh::connectto" << peer.data();
        onNewSynced(peer);
    }

    void onNewSynced(const std::string &peer) {
        auto it = known.find(peer);
        bool newknown = ( it == end(known) );
        if ( newknown )
            known.insert(it,peer);

        doDial(peer);
        return;
        if ( numKnownConnected < 8) {
            auto mit = connected.find(peer);
            if ( mit == end(connected))
                doDial(peer);
            else if ( newknown )
                numKnownConnected++;
        }
    }

    void onConnected(const std::string &peer, const nng_pipe &p, int dialid = -1) {
        bool newdial = (dialid != -1);

        auto itd = dialers.find(peer);
        bool olddial = ( itd != end(dialers) );

        bool newlist = !newdial;
        bool oldlist = false;
        auto lit = listen_pipes.find(peer);
        oldlist = ( lit != end(listen_pipes));

        bool alreadyconnected = false;
        auto itc = connected.find(peer);
        alreadyconnected = ( itc != end(connected));

        auto kit = known.find(peer);
        bool isknown = ( kit != end(known) );

        bool gooddial = newdial && olddial && (dialid == nng_dialer_id(itd->second));
        bool goodlist = newlist && !oldlist;

        bool goodconnection = (goodlist && !olddial) || (gooddial && !oldlist);

        qDebug() << "mesh::onConnected" << peer.data()
                 << "gooddial" << gooddial
                 << "goodlist" << goodlist
                 << "goodconnection" << goodconnection
                 << "olddial" << olddial
                 << "oldlist" << oldlist
                 << "isknown" << isknown
                 << "alreadyconnected" << alreadyconnected;



        if ( goodconnection ) {
            if ( alreadyconnected )
                qDebug() << "mesh::onConnected" << "but already connected?!?" << peer.data() ;
            else {
                connected.insert(itc,peer);

                if ( isknown )
                    numKnownConnected++;
                else if ( gooddial )
                    qDebug() << "mesh::onConnected" << "newdial && !known error" << peer.data() ;
            }

            if ( goodlist )
                listen_pipes.insert(lit,{peer,p});
        }

        else if ( alreadyconnected ) {
            qDebug() << "mesh::onConnected" << "dup connection?" << peer.data();

            if ( newdial && oldlist) {
                qDebug() << "mesh::onConnected already have list_pipe - now new connection dial - try to close it";
                nng_dialer_close(nng_pipe_dialer(p));
                if ( olddial ) {
                    qDebug() << "mesh::onConnected" << " should never happen" << peer.data() ;
                    nng_dialer_close(itd->second);
                }
            }

            else if ( newlist && olddial) {
                qDebug() << "mesh::onConnected already have dial - now new connection list - try to close it";
                if ( nng_dialer_close(itd->second) != 0 )
                    qDebug() << "mesh::onConnected error on olddial close" << peer.data();

                if ( oldlist )
                    nng_pipe_close(lit->second);
            }

            else if ( newlist && oldlist) {
                qDebug() << "mesh::onConnected already have list - now new connection list - try to close it" << peer.data();
                if ( nng_pipe_close(lit->second) != 0 )
                    qDebug() << "mesh::onConnected error on piper close" << peer.data();
            }

            else if (newdial != olddial && (dialid != nng_dialer_id(itd->second)) ) {
                qDebug() << "mesh::onConnected" << "dialer_is mismatch!" << peer.data() << dialid << nng_dialer_id(itd->second);
                nng_dialer_close(itd->second);
            }
        }
        else {
            qDebug() << "mesh::onConnected" << "dup and !goodconnection" << peer.data() ;
            connected.insert(itc,peer);
            if ( isknown )
                numKnownConnected++;

            if ( newdial && oldlist) {
                qDebug() << "mesh::onConnected !goodconnection !connected newdial oldlist - try to close it" << peer.data();
                nng_pipe_close(lit->second);
                if ( olddial ) {
                    qDebug() << "mesh::onConnected" << " should never happen" << peer.data() ;
                    nng_dialer_close(itd->second);
                }
            }

            else if ( newlist && olddial) {
                qDebug() << "mesh::onConnected !goodconnection !connected already have dial - now new connection list - try to close it";
                if ( nng_dialer_close(itd->second) != 0 )
                    qDebug() << "mesh::onConnected error on olddial close" << peer.data();

                if ( oldlist )
                    nng_pipe_close(lit->second);
            }

            else if ( newlist && oldlist) {
                qDebug() << "mesh::onConnected !goodconnection !connected already have list - now new connection list - try to close it" << peer.data();
                if ( nng_pipe_close(lit->second) != 0 )
                    qDebug() << "mesh::onConnected error on piper close" << peer.data();
            }

            else if (newdial != olddial && (dialid != nng_dialer_id(itd->second)) ) {
                qDebug() << "mesh::onConnected" << "dialer_is mismatch!" << peer.data() << dialid << nng_dialer_id(itd->second);
                nng_dialer_close(itd->second);
            }
        }

        if ( newlist ) {
            if ( oldlist )
                lit->second = p;
            else
                listen_pipes.insert(lit,{peer,p});
        }
    }

    void onDisconnected(const std::string &peer, const nng_pipe &p, int dialid = -1) {
        bool alreadyconnected = false;
        auto itc = connected.find(peer);
        alreadyconnected = ( itc != end(connected));

        auto kit = known.find(peer);
        bool isknown = ( kit == end(known) );

        bool isdial = (dialid != -1);
        auto itd = dialers.find(peer);
        bool havedial = ( itd != end(dialers) );

        auto lit = listen_pipes.find(peer);
        bool havelist = ( lit != end(listen_pipes));

        if ( isdial ) {
            if ( havedial ) {
                int oldid = nng_dialer_id(itd->second);
                if ( (dialid == oldid) || oldid == -1 ) {
                    dialers.erase(itd);
                    if ( alreadyconnected && !havelist ) {
                         connected.erase(itc);
                         if ( isknown )
                             numKnownConnected--;
                    }
                    else
                        qDebug() << "error mesh::onDisconnected isdial sameid but !connected" << peer.data();
                }
                else qDebug() << "error mesh::onDisconnected isdial diff id" << peer.data() << dialid << oldid;
            }
            else qDebug() << "error mesh::onDisconnected isdial nooldid" << peer.data() << dialid;

            nng_dialer_close(nng_pipe_dialer(p));
        }
        else {
            int newid = nng_pipe_id(p);
            if ( havelist ) {
                int oldid = nng_pipe_id(lit->second);
                if ( (newid == oldid) || oldid == -1 ) {
                    listen_pipes.erase(lit);
                    if ( alreadyconnected && !havedial ) {
                         connected.erase(itc);
                         if ( isknown )
                             numKnownConnected--;
                    }
                    else
                        qDebug() << "error mesh::onDisconnected !isdial sameid but !connected" << peer.data();
                }
                else qDebug() << "error mesh::onDisconnected !isdial diff id" << peer.data() << newid << oldid;
            }
            else qDebug() << "error mesh::onDisconnected !isdial !havelist" << peer.data() << newid;

            nng_pipe_close(p);
        }
    }

    void doDial(const std::string &peer) {
        qDebug() << " try dial peer " << peer.data();
        auto it = dialers.find(peer);
        if ( it == end(dialers)) {
            nng_dialer myd;
            auto iit = dialers.insert(it,{peer,myd});
            int rv = nng_dial(_socket, mesh::makeAddress(peer).c_str(), &iit->second, 0);
            if ( rv != 0 )
                qDebug() << mesh::makeAddress(peer).data() << nng_strerror(rv);
            else
                qDebug() << " dialled " << peer.data() << rv;
        }
//        struct work *w = makeWork(peer);
//        int rv = nng_pipe_notify(_socket, &mesh::onNotify, w);
    }

    work * makeWork(const std::string &peer) {
        struct work *w;
        int          rv;

        w = new work();

        w->peer = peer;
        w->mesh = this;
//        if ((rv = nng_aio_alloc(&w->aio, &mesh::callBackDial, w)) != 0) {
//        }

        return (w);
    }

    static std::string makeAddress(const std::string &in) {
        std::string pre = "tcp://[";
        std::string po = "]:10130";
        return pre + in + po;
    }

//    static void callBackDial(void *) {

//    }

    void putto() {
       putPipe("158.222.102.175");
    }

    void putPipe(const std::string  &peer) {
        qDebug() << "put pipe" << peer.data();
        nng_msg *msg;

        int rv = nng_msg_alloc(&msg,5);
        rv = nng_msg_append(msg,"msgit",5);

        auto it = listen_pipes.find(peer);
        if ( it != end(listen_pipes)) {
            nng_msg_set_pipe(msg,it->second);
//            nni_msgq_tryput(it->second.sendq,msg);
            nng_sendmsg(_socket,msg,0);
        }

//        nng_msg_set_pipe(nng_msg *msg, nng_pipe p);
        }

    static void callBack(void *m) {

        qDebug() << "mesh::calback";
        mesh *mm = (mesh *)m;
        nng_msg *msg = nng_aio_get_msg(mm->receiveAio);
        int sz = nng_msg_len(msg);
        char* buf = (char *)nng_msg_body(msg);
        std::string rec;
        rec.assign(buf,sz);
        qDebug() << rec.data();

    }

    static void onNotify(nng_pipe p, nng_pipe_action act, void *arg) {

        qDebug() << "mesh::onNotify";

        mesh *mymesh = (mesh *)arg;
        std::string remoteaddress = "";

        {
        nng_sockaddr obuf;
        nng_pipe_getopt_sockaddr(p,NNG_OPT_REMADDR,&obuf);
        qDebug() << NNG_OPT_REMADDR << obuf.s_family;
        QHostAddress qha;
        if ( obuf.s_family == NNG_AF_INET6) {
            qha.setAddress(obuf.s_in6.sa_addr);
            qha.setAddress(qha.toIPv4Address());
            remoteaddress = qha.toString().toStdString();
            qDebug() << "port" << obuf.s_in6.sa_port;
            qDebug() << "port2" << qFromBigEndian(obuf.s_in6.sa_port);
            qDebug() << "port3" << qToBigEndian(obuf.s_in6.sa_port);

        }
        else if ( obuf.s_family == NNG_AF_INET) {
            qha.setAddress(qFromBigEndian(obuf.s_in.sa_addr));
//            qha.setAddress(qha.toIPv4Address());
            remoteaddress = qha.toString().toStdString();
            qDebug() << "port" << obuf.s_in6.sa_port;
            qDebug() << "port2" << qFromBigEndian(obuf.s_in.sa_port);
            qDebug() << "port3" << qToBigEndian(obuf.s_in.sa_port);

        }

        qDebug() << "mesh::onNotify" << "remote address" << remoteaddress.data() ;
        }
        {
            char* obuf;
            nng_pipe_getopt_string(p,NNG_OPT_URL,&obuf);
            qDebug() << NNG_OPT_URL << "option::" << obuf << ":: done";
        }

        {
            nng_sockaddr obuf;
            nng_pipe_getopt_sockaddr(p,NNG_OPT_LOCADDR,&obuf);
            qDebug() << NNG_OPT_LOCADDR << obuf.s_family;
            std::string localaddress = "";

            QHostAddress qha;
            if ( obuf.s_family == NNG_AF_INET6) {
                qha.setAddress(obuf.s_in6.sa_addr);
                qha.setAddress(qha.toIPv4Address());
                localaddress = qha.toString().toStdString();
                qDebug() << "port" << obuf.s_in6.sa_port;
                qDebug() << "port2" << qFromBigEndian(obuf.s_in6.sa_port);
                qDebug() << "port3" << qToBigEndian(obuf.s_in6.sa_port);

            }
            else if ( obuf.s_family == NNG_AF_INET) {
                qha.setAddress(qFromBigEndian(obuf.s_in.sa_addr));
    //            qha.setAddress(qha.toIPv4Address());
                localaddress = qha.toString().toStdString();
                qDebug() << "port" << obuf.s_in6.sa_port;
                qDebug() << "port2" << qFromBigEndian(obuf.s_in.sa_port);
                qDebug() << "port3" << qToBigEndian(obuf.s_in.sa_port);

            }
            qDebug() << "mesh::onNotify" << "local address" << localaddress.data() ;

        }

        {
            int obuf;
            nng_pipe_getopt_int(p,NNG_OPT_PEER,&obuf);
            qDebug() << NNG_OPT_PEER << "option::" << obuf << ":: done";
        }
        {
            char* obuf;
            nng_pipe_getopt_string(p,NNG_OPT_PEERNAME,&obuf);
            qDebug() << NNG_OPT_PEERNAME << "option::" << obuf << ":: done";
        }
        {
            char* obuf;
            nng_pipe_getopt_string(p,NNG_OPT_SOCKNAME,&obuf);
            qDebug() << NNG_OPT_SOCKNAME << "option::" << obuf << ":: done";
        }


        int dialerid = nng_dialer_id(nng_pipe_dialer(p));
//        bool listenerid = nng_listener_id(nng_pipe_listener(p));

        switch (act) {
        case NNG_PIPE_ADD:
            qDebug() << "add";
            {
                mymesh->onConnected(remoteaddress,p,dialerid);
            }
            break;
        case NNG_PIPE_REM:
            qDebug() << "remove";
//            if ( dialerid != -1 )
//                nng_dialer_close(nng_pipe_dialer(p));

            mymesh->onDisconnected(remoteaddress,p,dialerid);
            break;
        default:
            return;
        }


    }

};

}
#endif // MESH_H

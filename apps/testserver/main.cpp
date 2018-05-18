#include <QCoreApplication>
#include <QDebug>

#include <nng/nng.h>
#include <nng/protocol/reqrep0/rep.h>
#include <nng/protocol/reqrep0/req.h>


void notify(nng_pipe p, nng_pipe_action act, void *arg) {
    qDebug() << "server  notify ";

    switch (act) {
    case NNG_PIPE_ADD:
        qDebug() << "add";
        break;
    case NNG_PIPE_REM:
        qDebug() << "remove";
        break;
    default:
        return;
    }

    {
        nng_sockaddr obuf;
        nng_pipe_getopt_sockaddr(p,NNG_OPT_REMADDR,&obuf);
        qDebug() << obuf.s_family;

        nng_sockaddr_in6 in = obuf.s_in6;
        qDebug() << NNG_OPT_REMADDR << in.sa_addr[12] << "." <<  in.sa_addr[13] << "." <<  in.sa_addr[14] << "." << in.sa_addr[15];
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

        nng_sockaddr_in6 in = obuf.s_in6;
        qDebug() << NNG_OPT_LOCADDR << in.sa_addr[12] << "." <<  in.sa_addr[13] << "." <<  in.sa_addr[14] << "." << in.sa_addr[15];

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
    qDebug() << "";

}



int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    nng_socket   sock;
    int rv;

    qDebug() << "server test out";

    rv = nng_rep0_open_raw(&sock);
//    nng_setopt_bool(sock, NNG_OPT_TCP_KEEPALIVE, false);

    nng_pipe_notify(sock, notify, NULL);

    if (rv != 0) {
//        fatal("nng_rep0_open", rv);
    }

    nng_msg *msg;

    nng_listener listener;
    rv = nng_listen(sock, "tcp://:10128", &listener, 0);

    nng_sockaddr obuf;

    nng_listener_getopt_sockaddr(listener,NNG_OPT_LOCADDR,&obuf);
    qDebug() << NNG_OPT_LOCADDR << obuf.s_family;

    nng_sockaddr_in6 in = obuf.s_in6;
    qDebug() << NNG_OPT_LOCADDR << in.sa_addr[12] << "." <<  in.sa_addr[13] << "." <<  in.sa_addr[14] << "." << in.sa_addr[15];


    rv = nng_recvmsg(sock,&msg,0);

    int sz = nng_msg_len(msg);

    char* buf = (char *)nng_msg_body(msg);
    std::string rec;
    rec.assign(buf,sz);
    qDebug() << rec.data();

    char* head = (char *)nng_msg_header(msg);
    rec.assign(head,nng_msg_header_len(msg));

    qDebug() << rec.data();


    nng_pipe p = nng_msg_get_pipe(msg);

    /*
    {
        nng_sockaddr obuf;
        nng_pipe_getopt_sockaddr(p,NNG_OPT_REMADDR,&obuf);
        qDebug() << obuf.s_family;

        nng_sockaddr_in6 in = obuf.s_in6;
        qDebug() << NNG_OPT_REMADDR << in.sa_addr[12] << "." <<  in.sa_addr[13] << "." <<  in.sa_addr[14] << "." << in.sa_addr[15];
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

        nng_sockaddr_in6 in = obuf.s_in6;
        qDebug() << NNG_OPT_LOCADDR << in.sa_addr[12] << "." <<  in.sa_addr[13] << "." <<  in.sa_addr[14] << "." << in.sa_addr[15];

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

    */

//    nng_listener_close(listener);
//    nng_close(sock);

//#define NNG_OPT_SOCKNAME "socket-name"
//#define NNG_OPT_RAW "raw"
//#define NNG_OPT_PROTO "protocol"
//#define NNG_OPT_PROTONAME "protocol-name"
//#define NNG_OPT_PEER "peer"
//#define NNG_OPT_PEERNAME "peer-name"
//#define NNG_OPT_RECVBUF "recv-buffer"
//#define NNG_OPT_SENDBUF "send-buffer"
//#define NNG_OPT_RECVFD "recv-fd"
//#define NNG_OPT_SENDFD "send-fd"
//#define NNG_OPT_RECVTIMEO "recv-timeout"
//#define NNG_OPT_SENDTIMEO "send-timeout"
//#define NNG_OPT_LOCADDR "local-address"
//#define NNG_OPT_REMADDR "remote-address"
//#define NNG_OPT_URL "url"
//#define NNG_OPT_MAXTTL "ttl-max"
//#define NNG_OPT_RECVMAXSZ "recv-size-max"
//#define NNG_OPT_RECONNMINT "reconnect-time-min"
//#define NNG_OPT_RECONNMAXT "reconnect-time-max"


    return a.exec();
}
/*




int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    qDebug() << "test out";

    nng_socket   sock;
    int rv;

    rv = nng_rep0_open(&sock);
    if (rv != 0) {
//        fatal("nng_rep0_open", rv);
    }

    nng_msg *msg;

    rv = nng_msg_alloc(&msg,0);

    rv = nng_msg_append(msg,"msg",3);

    rv = nng_listen(sock, "ipc:///tmp/async_demo", NULL, 0);

    rv = nng_recvmsg(sock,&msg,0);

    return app.exec();
}
*/

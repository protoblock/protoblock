#include <QCoreApplication>
#include <QDebug>

#include <nng/nng.h>
#include <nng/protocol/reqrep0/rep.h>
#include <nng/protocol/reqrep0/req.h>
#include <nng/supplemental/util/platform.h>


void notify(nng_pipe p, nng_pipe_action act, void *arg) {

    bool doclose = false;
    switch (act) {
    case NNG_PIPE_ADD:
        qDebug() << "add";
        break;
    case NNG_PIPE_REM:
        qDebug() << "remove";
        doclose = true;
//        nng_dialer *d = (nng_dialer *)arg;
        nng_dialer_close(*(nng_dialer *)arg);
        return;
        break;
    default:
        return;
    }

    qDebug() << "client  notify ";
    {
        nng_sockaddr obuf;
        int r = nng_pipe_getopt_sockaddr(p,NNG_OPT_REMADDR,&obuf);
        if ( r == 0 ) {
            qDebug() << obuf.s_family;

            nng_sockaddr_in6 in = obuf.s_in6;
            qDebug() << NNG_OPT_REMADDR << in.sa_addr[12] << "." <<  in.sa_addr[13] << "." <<  in.sa_addr[14] << "." << in.sa_addr[15];
        }
    }

    {
        char* obuf;
        if ( nng_pipe_getopt_string(p,NNG_OPT_URL,&obuf) == 0 )
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

    if ( doclose )
        nng_pipe_close(p);
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    nng_socket   sock;
    int rv;

    qDebug() << "client test out";

    rv = nng_req0_open(&sock);

//    nng_setopt_bool(sock, NNG_OPT_TCP_KEEPALIVE, false);



    nng_dialer dialer;
    nng_pipe_notify(sock, notify, &dialer);

    if (rv != 0) {
//        fatal("nng_rep0_open", rv);
    }

    nng_msg *msg;

    rv = nng_msg_alloc(&msg,0);
    rv = nng_msg_append(msg,"msg",3);
    rv = nng_dial(sock, "tcp://[50.0.50.206]:10128", &dialer, 0);

    nng_msleep(3600);
//    rv = nng_dial(sock, "tcp://50.0.50.206:10130", NULL, 0);
//    nng_msleep(3600);

    rv = nng_sendmsg(sock,msg,0);

//    nng_pipe p = nng_msg_get_pipe(msg);
//    {
//        nng_sockaddr obuf;
//        nng_pipe_getopt_sockaddr(p,NNG_OPT_REMADDR,&obuf);
//        qDebug() << obuf.s_family;

//        nng_sockaddr_in6 in = obuf.s_in6;
//        qDebug() << in.sa_addr[12] << "." <<  in.sa_addr[13] << "." <<  in.sa_addr[14] << "." << in.sa_addr[15];
//    }


    //    nng_listener_close(listener);
    nng_msleep(3600);
//    nng_dialer_close(dialer);
//    nng_close(sock);
    nng_msg_free(msg);
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

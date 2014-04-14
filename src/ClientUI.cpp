//
//  ClientUI.cpp
//  fantasybit
//
//  Created by Jay Berg on 4/5/14.
//
//

#include "ClientUI.h"
#include "ProtoData.pb.h"
#include <iostream>
#include "MsgSock.h"
#include <thread>

using namespace std;
using namespace nn;
namespace fantasybit
{

void ClientUI::init()
{
    havegui = true;
    
    myname.set_status(MyNameStatus::none);
    myname.set_name("");
}


void ClientUI::run()
{
    init();
  
    const int RECEIVE_BREAK_LOOP_COUNT = 50;
    const int GUI_TIMEOUT_SECONDS = 500;
    const int LONG_NAP_SECONDS = 30;
    const int SHORT_SLEEP_MILLIS = 1000;

    //const int bsize = 256;
    //char buf[bsize];
    //int flags=0;
    
    Receiver server{sockserv};
    Receiver gui{sockgui};
    OutData outdata{};
    InData indata{};

    std::chrono::time_point<std::chrono::system_clock> start, end;
    start = std::chrono::system_clock::now();
    int scount = -1;
    int gcount = -1;
    bool first=true;

    while (running)
    {
 
        scount = -1;
        while (server.receive(outdata,NN_DONTWAIT) && (++scount < RECEIVE_BREAK_LOOP_COUNT))
            process_server(outdata);
        
        if ( first && scount >= 0)
        {
            snapshot_gui();
            first = false;
        }
 
        gcount = -1;
        while ( gui.receive(indata,NN_DONTWAIT) && (++gcount < RECEIVE_BREAK_LOOP_COUNT))
        {
            if ( !havegui )
            {
                havegui = true;
                snapshot_gui();
            }
            start = std::chrono::system_clock::now();
            process_gui(indata);
        }
 
        if ( havegui && gcount <= 0 ) //check for gui timeout
        {
            end = std::chrono::system_clock::now();
            std::chrono::duration<double> elapsed_seconds = end-start;
            if ( elapsed_seconds.count() >= GUI_TIMEOUT_SECONDS  )
                havegui = false;
        }
        
        if (!havegui && scount <= 0) //no gui - take a nap
        {
            if ( !running ) break;
            std::this_thread::sleep_for( std::chrono::seconds{LONG_NAP_SECONDS} );
        }
        else if ( scount + gcount <= 0 ) //no data
            std::this_thread::sleep_for( std::chrono::milliseconds{SHORT_SLEEP_MILLIS} );
        
        /*
        if ( !retserver )
        {
         
            continue;
        }
        cout << " UI " << outdata.DebugString() << "\n";
        
        if ( outdata.type() == OutData::MYFANTASYNAME )
        {
            if ( !outdata.has_myfantasyname() )
                on(outdata.myfantasyname());
            
            {
                indata.set_type(InData::MineName);
                indata.set_data("jaybny");
                Sender::Send(sock, indata);
            }
        }
        */
    }
}

void ClientUI::process_server(const OutData &data)
{
    cout << data.DebugString() << " *** process_server\n";
    switch ( data.type())
    {
        case OutData_Type_MYFANTASYNAME:
            if ( data.has_myfantasyname() )
            {
                myname = data.myfantasyname();
                if ( havegui )
                    Sender::Send(sockgui,data);
            }
            break;
        default:
            break; 
    }
}

void ClientUI::process_gui(const InData &data)
{
    cout << data.DebugString() << "***** process_gui\n";
    switch ( data.type())
    {
        case InData_Type_MINENAME:
            Sender::Send(sockserv,data);
            break;
        case InData_Type_QUIT:
            Sender::Send(sockserv,data);
            running = false;
            break;
        case InData_Type_CONNECT:
            snapshot_gui();
            break;
        case InData_Type_HEARTBEAT:
            {
                OutData out{};
                out.set_type(OutData_Type_HEARTBEAT);
                Sender::Send(sockgui,out);
            }
            break;
        default:
            break;
    }
}

void ClientUI::snapshot_gui() 
{
    OutData out{};
    out.set_type(OutData_Type_SNAPSHOT);
    out.mutable_myfantasyname()->CopyFrom(myname);
    Sender::Send(sockgui,out);
}

}
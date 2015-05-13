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

#define LOG(logger, severity) LOGIT(logger, severity,  __FILE__, __LINE__, __FUNCTION__)

using namespace std;
using namespace nn;
namespace fantasybit
{

void ClientUI::init()
{
	LOG(lg, trace) << "init havegui = true";

    havegui = true;
    
    myname.set_status(MyNameStatus::none);
    myname.set_name("");
}


void ClientUI::run()
{
    init();
  
    const int RECEIVE_BREAK_LOOP_COUNT = 50;
    const int GUI_TIMEOUT_SECONDS = 120;
    const int LONG_NAP_SECONDS = 30;
    const int SHORT_SLEEP_MILLIS = 1000;
 
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
		//LOG(lg, trace) << "running ";

        scount = -1;
        while (server.receive(outdata,NN_DONTWAIT) && (++scount < RECEIVE_BREAK_LOOP_COUNT))
            process_server(outdata);
        
		//LOG(lg, trace) << "processed " << scount << " from server ";
        if ( first && scount >= 0)
        {
			LOG(lg, trace) << "first time - snapshot_gui ";

            snapshot_gui();
            first = false;
        }
 
        gcount = -1;
        while ( gui.receive(indata,NN_DONTWAIT) && (++gcount < RECEIVE_BREAK_LOOP_COUNT))
        {
			LOG(lg, trace) << "received from gui " << indata.DebugString();

            if ( !havegui )
            {
				LOG(lg, trace) << "snapshot_gui ";

                havegui = true;
                snapshot_gui();
            }
            start = std::chrono::system_clock::now();
            process_gui(indata);
			LOG(lg, trace) << "processed from gui ";

        }
 
        if ( havegui && gcount <= 0 ) //check for gui timeout
        {
            end = std::chrono::system_clock::now();
            std::chrono::duration<double> elapsed_seconds = end-start;
			//LOG(lg, trace) << "elapsed_seconds " << elapsed_seconds.count();
			if (elapsed_seconds.count() >= GUI_TIMEOUT_SECONDS)
			{
				LOG(lg, trace) << "gui timeout " << elapsed_seconds.count();
				havegui = false;
			}
        }

        
        if (!havegui && scount <= 0) //no gui - take a nap
        {
            if ( !running ) break;
			LOG(lg, trace) << "take long nap - no gui  " << LONG_NAP_SECONDS << " seconds";
            std::this_thread::sleep_for( std::chrono::seconds{LONG_NAP_SECONDS} );
			LOG(lg, trace) << "wake";
        }
		else if (scount + gcount <= 0) //no data
		{
			//LOG(lg, trace) << "take short nap - no data";
			std::this_thread::sleep_for(std::chrono::milliseconds{ SHORT_SLEEP_MILLIS });
		}

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
	LOG(lg, trace) << "from server " << data.DebugString();
    //cout << data.DebugString() << " *** process_server\n";
    switch ( data.type())
    {
        case OutData_Type_MYFANTASYNAME:
            if ( data.has_myfantasyname() )
            {
                myname = data.myfantasyname();
				if (havegui)
				{
					LOG(lg, trace) << "to gui ";
					int n = Sender::Send(sockgui, data);// , NN_DONTWAIT);
					LOG(lg, trace) << "sent " << n << "bytes to gui ";
				}
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
		case InData_Type_MAKE_BLOCK:
			Sender::Send(sockserv, data);
			break;
        default:
			Sender::Send(sockserv, data);
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
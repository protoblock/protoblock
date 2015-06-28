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

    havegui = false;
    
    myname.set_status(MyNameStatus::none);
    myname.set_name("");
}


void ClientUI::run()
{
    init();
  
	const int RECEIVE_DELTA_BREAK_COUNT = 500;
	const int RECEIVE_BREAK_LOOP_COUNT = 50;
    const int GUI_TIMEOUT_SECONDS = 120;
    const int LONG_NAP_SECONDS = 5;
    const int SHORT_SLEEP_MILLIS = 1000;
 
    Receiver server{sockserv};
    Receiver gui{sockgui};
	Receiver deltas{ sockdelta };
	OutData outdata{};
    InData indata{};
	DeltaData deltadata{};
	OutData *outgui = nullptr;
	//GuiData togui{};

    std::chrono::time_point<std::chrono::system_clock> start, end;
    start = std::chrono::system_clock::now();
    int scount = -1;
    int gcount = -1;
	int dcount = -1;
    bool first = true;
	bool have_data = false; 
	bool gui_heartbeat = false;

	delta_snap.set_type(DeltaData_Type_SNAPSHOT);

    while (running)
    {
        scount = -1;
		dcount = -1;
		gcount = -1;
		have_data = false;

		while (server.receive(outdata, NN_DONTWAIT)) {
			if (process_server(outdata)) 
				dcount++; //dcount used as flag to trigger send to gui

			if (++scount >= RECEIVE_BREAK_LOOP_COUNT)
				break;
			std::this_thread::sleep_for(std::chrono::milliseconds{ 10 });

			outdata.Clear();
		}

		while (deltas.receive(deltadata, NN_DONTWAIT)) { 
			LOG(lg, trace) << "after RECEIVE " << deltadata.DebugString();
			process_delta(deltadata, delta_update);
			LOG(lg, trace) << "after process  " << deltadata.DebugString();

			if (++dcount >= RECEIVE_DELTA_BREAK_COUNT)
				break;

			std::this_thread::sleep_for(std::chrono::milliseconds{ 100 });
		}

		if (dcount >= 0 ) { //have new data
			if (!first) {
				LOG(lg, trace) << "!first sending " << delta_update.DebugString();

				to_gui(delta_update);
			}
			process_delta(delta_update, delta_snap);

			have_data = true;
		}
		else if (scount >= 0)
			have_data = true;

		//LOG(lg, trace) << "processed " << scount << " from server ";
        if ( first && have_data)
        {
			LOG(lg, trace) << "first time - snapshot_gui ";
            snapshot_gui();
            first = false;
        }
 
		while (gui.receive(indata, NN_DONTWAIT) )
        {
			//LOG(lg, trace) << "received from gui " << indata.DebugString();

            if ( !havegui )
            {
				//LOG(lg, trace) << "!havegui snapshot_gui ";

                havegui = true;
                //snapshot_gui();
            }
            start = std::chrono::system_clock::now();
			gui_heartbeat = process_gui(indata);

			//LOG(lg, trace) << "processed from gui ";
			if (++gcount >= RECEIVE_BREAK_LOOP_COUNT)
				break;
			std::this_thread::sleep_for(std::chrono::milliseconds{ 10 });
        }
 
        if ( havegui && gcount < 0 ) //check for gui timeout
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

        
        if (!havegui && !have_data) //no gui - take a nap
        {
            if ( !running ) break;
			LOG(lg, trace) << "take long nap - no gui  " << LONG_NAP_SECONDS << " seconds";
            std::this_thread::sleep_for( std::chrono::seconds{LONG_NAP_SECONDS} );
			LOG(lg, trace) << "wake";
        }
		else if (!have_data && gcount < 0) //no data
		{
			//LOG(lg, trace) << "take short nap - no data";
			std::this_thread::sleep_for(std::chrono::milliseconds{ SHORT_SLEEP_MILLIS });
		}
    }
}

bool ClientUI::process_server(const OutData &data)
{
	bool ret = false;
	LOG(lg, trace) << "from server " << data.DebugString();
    //cout << data.DebugString() << " *** process_server\n";
    switch ( data.type())
    {
        case OutData_Type_MYFANTASYNAME:
			if (data.has_myfantasyname())  {
				delta_update.mutable_myfantasyname()->CopyFrom(data.myfantasyname());
				ret = true;
			}
            break;
        default:
            break; 
    }

	return ret;
}

bool ClientUI::process_gui(const InData &data)
{
	bool ret = false; 
	if (data.type() != InData_Type_HEARTBEAT)
		LOG(lg, trace) << "from gui: " << data.DebugString();

	switch ( data.type())
    {
        case InData_Type_QUIT:
            Sender::Send(sockserv,data);
            running = false;
            break;
        case InData_Type_CONNECT:	
            snapshot_gui();
            break;
        case InData_Type_HEARTBEAT:
			delta_update.set_type(DeltaData_Type_HEARTBEAT);
			ret = true;	
            break;
		case InData_Type_MAKE_BLOCK:
			Sender::Send(sockserv, data);
			break;
        default:
			Sender::Send(sockserv, data);
            break;
    }
	return ret;
}

void ClientUI::snapshot_gui() 
{
	LOG(lg, trace) << "snapshot_gui sending " << delta_snap.DebugString();

    to_gui(delta_snap);
}

bool ClientUI::process_delta(DeltaData &data,DeltaData &delta_update)
{
	//LOG(lg, trace) << "delatr " << data.DebugString();
	if (data.has_myfantasyname())
		delta_update.mutable_myfantasyname()->CopyFrom(data.myfantasyname());

	if (data.has_globalstate())
		delta_update.mutable_globalstate()->CopyFrom(data.globalstate());

	for (const auto &t : data.teamstates()) {
		auto *tt = delta_update.add_teamstates();
		tt->CopyFrom(t);
	}

	for (const auto &t : data.players()) {
		auto *tt = delta_update.add_players();
		tt->CopyFrom(t);
	}

	for (const auto &t : data.datas()) {
		auto *dd = delta_update.add_datas();
		dd->CopyFrom(t);
	}

	data.Clear();
	return true;
}

void ClientUI::to_gui(const DeltaData &delt) {
	Sender::Send(sockgui, delt);
}

}
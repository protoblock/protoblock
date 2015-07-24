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


void ClientUI::run()
{
    try {
        runit();
    }
    catch (std::exception &e)
    {
        LOG(lg,fatal) <<  e.what();
        return;
    }
}

void ClientUI::runit() {

    Receiver server{sockserv};
    Receiver gui{sockgui};
	Receiver deltas{ sockdelta };
	OutData outdata{};
    InData indata{};
	DeltaData deltadata{};
    while (running)
    {
        if (server.receive(outdata, NN_DONTWAIT))
            if (outdata.has_myfantasyname()) {
                deltadata.Clear();
                deltadata.set_type(DeltaData_Type_HEARTBEAT);
                deltadata.mutable_myfantasyname()->CopyFrom(outdata.myfantasyname());
                Sender::Send(sockgui, deltadata );
            }


        if (deltas.receive(deltadata, NN_DONTWAIT)) {
            deltadata.set_type(DeltaData_Type_HEARTBEAT);
            Sender::Send(sockgui, deltadata);
        }

        if (gui.receive(indata, NN_DONTWAIT) )
        {
			LOG(lg, trace) << "received from gui " << indata.DebugString();
            Sender::Send(sockserv, indata);
        }

        std::this_thread::sleep_for(std::chrono::milliseconds{ 100 });
    }
}
}

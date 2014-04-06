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

void ClientUI::run()
{
    init();
    //const int bsize = 256;
    //char buf[bsize];
    //int flags=0;
    
    Receiver r{sock};
    OutData outdata{};
    InData indata{};
    
    std::chrono::milliseconds dura{ 2000 };

    while (running)
    {
        bool ret = r.receive(outdata,NN_DONTWAIT);
        
        if ( !ret )
        {
            std::this_thread::sleep_for( dura );
            continue;
        }
        cout << " UI " << outdata.DebugString() << "\n";
        if ( outdata.type() == OutData::MYFANTASYNAME )
        {
            if ( !outdata.has_myfantasyname() )
            {
                indata.set_type(InData::MineName);
                indata.set_data("jaybny");
                Sender::Send(sock, indata);
            }
        }
    }
    
    //indata.set_type(OutData::STOP);
    //Sender::Send(sock, indata);
    
}

}
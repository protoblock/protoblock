//
//  ClientUI.h
//  fantasybit
//
//  Created by Jay Berg on 4/5/14.
//
//

#ifndef __fantasybit__ClientUI__
#define __fantasybit__ClientUI__

#include "nn.hpp"
#include <nanomsg/pair.h>
#include <assert.h>
#include <string>
#include "ProtoData.pb.h"
#include <iostream>
namespace fantasybit
{

class ClientUI
{
    nn::socket sockserv, sockgui;
public:
    ClientUI(std::string addrserv,std::string addrgui)
        : sockserv{AF_SP, NN_PAIR}, sockgui{AF_SP, NN_PAIR}
    {
        sockserv.connect(addrserv.c_str());
        sockgui.bind(addrgui.c_str());
    }
    void run();
    void stop()
    {
        running = false;
    }
    
private:
    volatile bool running = true;
    void init();
protected:
    MyFantasyName myname{};
    
    void process_server(const OutData &);
    void process_gui(const InData &);
    void snapshot_gui();
    bool havegui;
};

}

#endif /* defined(__fantasybit__ClientUI__) */

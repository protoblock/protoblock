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

namespace fantasybit
{




class ClientUI
{
    nn::socket sock;
    volatile bool running = true;
public:
    
    ClientUI(std::string addr) : sock{AF_SP, NN_PAIR}
    {
        sock.connect(addr.c_str());
    }
    
    void run();
    
    void init() {};
    void stop()
    {
        running = false;
    }
};

}

#endif /* defined(__fantasybit__ClientUI__) */

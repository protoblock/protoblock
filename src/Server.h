//
//  Server.h
//  fantasybit
//
//  Created by Jay Berg on 4/5/14.
//
//

#ifndef __fantasybit__Server__
#define __fantasybit__Server__

#include "nn.hpp"
#include <nanomsg/pair.h>
#include <assert.h>
#include <string>

namespace fantasybit
{

class Server
{
    nn::socket sock;
    volatile bool running = true;
public:
    
    Server(std::string addr) : sock{AF_SP, NN_PAIR}
    {
        sock.bind(addr.c_str());
    }
    
    void run()
    {
        init();
        while (running)
        {
        }
    }
    
    void init();
    void stop()
    {
        running = false;
    }
};

}

#endif /* defined(__fantasybit__Server__) */

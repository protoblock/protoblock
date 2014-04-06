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

#include "MsgSock.h"
#include "FantasyAgent.h"

namespace fantasybit
{

class Server
{
    nn::socket sock;
    Sender sender;
    volatile bool running = true;
    
    std::unique_ptr<FantasyAgent> agent{};
public:
    
    Server(std::string addr) : sock{AF_SP, NN_PAIR}, sender{sock}
    {
        sock.bind(addr.c_str());
    }
    
    void run();
    
    void mine(const std::string &name);
    
    void init();
    void stop()
    {
        running = false;
    }
};


    /*
    {
        init();
        
        std::chrono::milliseconds dura{ 2000 };
        while (running)
        {
            std::this_thread::sleep_for( dura );
        }
    }
    */

}

#endif /* defined(__fantasybit__Server__) */

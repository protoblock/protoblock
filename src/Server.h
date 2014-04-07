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
public:
    Server(std::string addr) : sock{AF_SP, NN_PAIR}, sender{sock}
    {
        sock.bind(addr.c_str());
    }
    
    void run();
    void stop()
    {
        running = false;
    }
private:
    volatile bool running = true;
    std::unique_ptr<FantasyAgent> agent{};
    
    void init();
protected:
    void mine(const std::string &name);
};

}

#endif /* defined(__fantasybit__Server__) */

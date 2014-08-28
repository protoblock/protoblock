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
    Server(std::string addr) : sock{AF_SP, NN_PAIR}, sender{sock}, agent{new FantasyAgent{}}
    {
        sock.bind(addr.c_str());
    }
    
    void run();
    void stop()
    {
        running = false;
        agent->kill();
    }
private:
    volatile bool running = true;
    std::unique_ptr<FantasyAgent> agent;
    
    void init();
protected:
    void claimName(const std::string &name,bool mine);
};

}

#endif /* defined(__fantasybit__Server__) */

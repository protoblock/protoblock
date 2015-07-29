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
#include <nanomsg\nn.h>
#include <nanomsg\reqrep.h>
#include <nanomsg/pair.h>
#include <nanomsg\pubsub.h>
#include <assert.h>
#include <string>

#include "MsgSock.h"
#include "FantasyAgent.h"

namespace fantasybit
{

class Server
{
    nn::socket sock;
	Sender sender, sender_blocks, sender_trans;
	nn::socket pubnewlocalblock, pubnewlocaltrans;
	int pubnewlocalblockid, pubnewlocaltransid;
		
public:
    Server(std::string addr) : 
		sock{AF_SP, NN_PAIR}, sender{sock}, 
		pubnewlocalblock{ AF_SP, NN_PUB }, sender_blocks{ pubnewlocalblock },
		pubnewlocaltrans{ AF_SP, NN_PUB }, sender_trans{ pubnewlocaltrans },
		agent{new FantasyAgent{}}
    {
        sock.bind(addr.c_str());

		pubnewlocalblockid = pubnewlocalblock.bind("inproc://newlocalblock");
		pubnewlocaltransid = pubnewlocaltrans.bind("inproc://newlocaltrans");
    }
    
    void run();
    void runit();

    void stop()
    {
        running = false;
    }
private:
    volatile bool running = true;
    std::unique_ptr<FantasyAgent> agent;
    
    void init();
protected:
    void claimName(const std::string &name);
};

class
{

};

}

#endif /* defined(__fantasybit__Server__) */

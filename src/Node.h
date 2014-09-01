//
//  Node.h
//  fantasybit
//
//  Created by Jay Berg on 8/31/14
//
//

#ifndef __fantasybit__Node__
#define __fantasybit__Node__

#include "nn.hpp"
#include <nanomsg/pair.h>
#include <assert.h>
#include <string>
#include "ProtoData.pb.h"
#include <iostream>
#include "fbutils.h"
#include <leveldb/db.h>
#include <set>
#include <mutex>
#include <vector>

namespace fantasybit
{

class Node
{
    //nn::socket sockserv, sockgui;
	std::auto_ptr<leveldb::DB> peers;
	std::auto_ptr<leveldb::DB> blockchain;
	std::auto_ptr<leveldb::DB> holdblocks;
	std::set<std::string> peerlist{};
	std::vector<std::string> newpeers{};
	std::vector<std::string> connected{};
	std::vector<std::string> higherpeers{};
	NodeRequest reqhs{};

	std::string myip{};
	int current_hight = 0;
	int maxhi = 0;
	int count = 0;
	int getLastBlockNum();
	volatile bool sync_running = true;
	volatile bool sync_req_running = true;
	volatile bool running_live = true;
	std::mutex blockchain_mutex{};
	std::mutex connected_mutex{};
public:
	Node() {} 
	bool doHandshake(const std::string &peer);
	void runHandShake();
    int init();
	bool getMyIp();
	void syncService();
	void syncRequest();
	void runLive();
	void pendingTransactions();
	std::string filedir(const std::string &in)
	{
		return ROOT_DIR + in;
	}

};



static std::string SEED_NODE("127.0.0.1");

}

#endif /* defined(__fantasybit__Node__) */

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
	std::auto_ptr<leveldb::DB> holdblocks;
	std::set<std::string> peerlist{};
	std::vector<std::string> newpeers{};
	std::vector<std::string> connected{};
	std::vector<std::string> newconnected{};
    std::vector<std::string> newconnected2{};
    std::vector<std::string> higherpeers{};
	NodeRequest reqhs{};

	std::string myip{};
	std::string myhost{};
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
	void stop()
	{
		sync_running = false;
		sync_req_running = false;
		running_live = false;	
	}
	bool doHandshake(const std::string &peer);
	void runHandShake();
    int init();
	bool getMyIp();
    void dosyncService();
    void dosyncRequest();
    void dorunLive();
    void dopendingTransactions();
    void syncService();
    void syncRequest();
    void runLive();
    void pendingTransactions();
    void CheckOrphanBlocks();

    std::string filedir(const std::string &in)
	{
        return GET_ROOT_DIR() + in;
	}

	static leveldb::Slice i2slice(int i)
	{
		leveldb::Slice value((char*)&i, sizeof(int) );
		return value;
	}

	static Block Node::getlastBLock() {
		Block b{};
		std::string value;
		auto *it = blockchain->NewIterator(leveldb::ReadOptions());
		it->SeekToLast();

		if (!it->Valid()) return b;

		auto str = it->value().ToString();

		b.ParseFromString(str);
		delete it;
		return b;
	}

	bool behind_nat = true;
	static void ClearTx(const Block &);
	static std::shared_ptr<leveldb::DB> blockchain;
	static std::shared_ptr<leveldb::DB> txpool;

};

static std::string SEED_NODE("162.254.27.226");
static std::string SEED_HOST("Jets");

}

#endif /* defined(__fantasybit__Node__) */

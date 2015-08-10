//
//  Node.h
//  fantasybit
//
//  Created by Jay Berg on 8/31/14
//
//

#ifndef __fantasybit__Node__
#define __fantasybit__Node__

#include <assert.h>
#include <string>
#include "ProtoData.pb.h"
#include <iostream>
#include "fbutils.h"
#include <leveldb/db.h>
#include <set>
#include <mutex>
#include <vector>
#include <fc/optional.hpp>
#include "boostLog.h"

#define LOG(logger, severity) LOGIT(logger, severity,  __FILE__, __LINE__, __FUNCTION__)

namespace fantasybit
{

class Node
{
	int current_hight = 0;
    int global_height = 0;
public:
    Node();

    bool Sync();
    bool SyncTo(int);

    fc::optional<int> getLastGlobalBlockNum();

    std::string filedir(const std::string &in)
	{
        return GET_ROOT_DIR() + in;
	}

	static leveldb::Slice i2slice(int i)
	{
		leveldb::Slice value((char*)&i, sizeof(int) );
		return value;
	}

    static Block Node::getlastLocalBlock() {
		Block b{};
		std::string value;
		auto *it = blockchain->NewIterator(leveldb::ReadOptions());
		it->SeekToLast();

        if (!it->Valid()) {
            //ToDo fc optional
            return b;
        }

		auto str = it->value().ToString();

		b.ParseFromString(str);
		delete it;
		return b;
	}

    static fc::optional<Block> Node::getLocalBlock(int num) {
        fc::optional<Block> block;
        if ( getLastLocalBlockNum() < num )
            return block;

        std::string value;
        leveldb::Slice snum((char*)&num, sizeof(int));
        if (blockchain->Get(leveldb::ReadOptions(), snum, &value).IsNotFound()) {
            LOG(lg,warning) << "block not found " << num;
            //ToDo
            return block;
        }

        (*block).ParseFromString(value);
        return block;
    }

    static fc::optional<Block> Node::getGlobalBlock(int num) {
        fc::optional<Block> block;
        return block;
    }

    static int Node::getLastLocalBlockNum();
    static int Node::myLastGlobalBlockNum();
    static void Node::setLastGlobalBlockNum(int num);
    static std::mutex blockchain_mutex;

    //static void ClearTx(const Block &);
	static std::shared_ptr<leveldb::DB> blockchain;
    static std::shared_ptr<leveldb::DB> txpool;
    static int GlobalHeight;

};

static std::string SEED_NODE("162.254.27.226");
static std::string SEED_HOST("Jets");

}

#endif /* defined(__fantasybit__Node__) */

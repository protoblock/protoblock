//
//  Node.h
//  fantasybit
//
//  Created by Jay Berg on 8/31/14
//
//

#ifndef __fantasybit__Node__
#define __fantasybit__Node__
/*
#include <assert.h>
#include <string>
#include <iostream>
#include <set>
#include <vector>
*/

#include <mutex>
#include "fbutils.h"

#include <fc/optional.hpp>
//#include "boostLog.h"
#include <leveldb/db.h>
#include "ProtoData.pb.h"
#include <memory>

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

    std::string filedir(const std::string &in);
    static leveldb::Slice i2slice(int i);

    static Block Node::getlastLocalBlock() ;

    static fc::optional<Block> Node::getLocalBlock(int num) ;

    static fc::optional<Block> Node::getGlobalBlock(int num);

    static int Node::getLastLocalBlockNum();
    static int Node::myLastGlobalBlockNum();
    static void Node::setLastGlobalBlockNum(int num);
    static std::mutex blockchain_mutex;

    static void ClearTx(const Block &);
	static std::shared_ptr<leveldb::DB> blockchain;
    static std::shared_ptr<leveldb::DB> txpool;
    static int GlobalHeight;
    static bool addTxPool(const std::string &id, std::string &txstr)  {
        return (txpool->Put(leveldb::WriteOptions(), id, txstr ).ok());

    }

};

//static std::string SEED_NODE("162.254.27.226");
//static std::string SEED_HOST("Jets");

}

#endif /* defined(__fantasybit__Node__) */

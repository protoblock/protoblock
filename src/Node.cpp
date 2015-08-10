//
//  Node.cpp
//  fantasybit
//
//  Created by Jay Berg on 8/31/14
//
//

#include "Node.h"
#include "ProtoData.pb.h"
#include <iostream>
#include "MsgSock.h"
#include <thread>

#include <iostream>
#include <istream>
#include <ostream>
#include <string>
#include "ProtoData.pb.h"
#include <set>
#include <fc/time.hpp>
#include "Processor.h"
#include "boostLog.h"


using namespace std;
	
#define LOG(logger, severity) LOGIT(logger, severity,  __FILE__, __LINE__, __FUNCTION__)

namespace fantasybit
{

Node::Node() {
    LOG(lg, trace) << "init node";
    leveldb::Options options;
    options.create_if_missing = true;
    leveldb::Status status;

    leveldb::DB *db2;
    status = leveldb::DB::Open(options, filedir("block/blockchain"), &db2);
    Node::blockchain.reset(db2);

//    leveldb::DB *db4;
//    status = leveldb::DB::Open(options, filedir("tx/txpool"), &db4);
//    Node::txpool.reset(db4);

    current_hight = getLastLocalBlockNum();
    if (current_hight == 0)
    {
        LOG(lg, info) << "no blocks - making Genesis";

        Block sb{Commissioner::makeGenesisBlock()};
        leveldb::Slice value((char*)&current_hight, sizeof(int));
        blockchain->Put(leveldb::WriteOptions(), value, sb.SerializeAsString());
        current_hight = getLastLocalBlockNum();
    }

    //assert(getLastBlockNum() > 0);

    LOG(lg, info) << " current_hight " << current_hight;
}

bool Node::Sync() {
    fc::optional<int> gh = getLastGlobalBlockNum();
    if ( !gh ) {
        LOG(lg,error) << " no getLastGlobalBlockNum";
        return false;
    }

    return SyncTo(*gh);
}

bool Node::SyncTo(int gh) {
    global_height = gh;

    while ( current_hight < global_height ) {

        fc::optional<Block> sb = getGlobalBlock(current_hight+1);
        if ( !sb ) {
            LOG(lg,error) << " no getGlobalBlockNum" << current_hight+1;
            return false;
        }

        LOG(lg, info) << "received " << (*sb).DebugString();

        if (!BlockProcessor::verifySignedBlock(*sb)) {
            LOG(lg,error) << " !BlockProcessor::verifySignedBlock(sb) ";
            return false; ;
        }

        if ((*sb).signedhead().head().num() > current_hight + 1) {
            LOG(lg,error) << "sb.signedhead().head().num() > current_hight + 1";
            return false; ;
        }

        if ((*sb).signedhead().head().num() == current_hight + 1) {
            LOG(lg,info) << "Received next " << current_hight+1;
            int myhight = current_hight+1;
            leveldb::Slice snum((char*)&myhight, sizeof(int));
            blockchain->Put(leveldb::WriteOptions(), snum, (*sb).SerializeAsString());
            current_hight = current_hight+1;

            //Node::ClearTx(*sb);

            //CheckOrphanBlocks();
        }
        else if ( (*sb).signedhead().head().num() > current_hight+1){
            LOG(lg,warning) << "Received gap in block " << (*sb).signedhead().head().num();
        }
    }

    return current_hight == global_height;
}

int Node::getLastLocalBlockNum() {
    std::string value;
    auto *it = blockchain->NewIterator(leveldb::ReadOptions());
    it->SeekToLast();

    if (!it->Valid()) return 0;

    auto slice = it->key();

    int num = *(reinterpret_cast<const int *>(slice.data()));
    delete it;
    return num;
}

int Node::myLastGlobalBlockNum() {
    int myglobalheight = 0;
    {
        std::lock_guard<std::mutex> lockg{ blockchain_mutex };
        myglobalheight = GlobalHeight;
    }

    return myglobalheight;
}

fc::optional<int> Node::getLastGlobalBlockNum() {
    return 0;
}

void Node::setLastGlobalBlockNum(int mylastglobalheight) {
    std::lock_guard<std::mutex> lockg{ blockchain_mutex };
    GlobalHeight = mylastglobalheight;
}

decltype(Node::blockchain) Node::blockchain{};
decltype(Node::blockchain) Node::txpool{};

decltype(Node::blockchain_mutex) Node::blockchain_mutex{};
decltype(Node::GlobalHeight) Node::GlobalHeight{};


}	

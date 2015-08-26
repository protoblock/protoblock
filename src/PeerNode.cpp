//
//  Node.cpp
//  fantasybit
//
//  Created by Jay Berg on 8/31/14
//
//

#include "PeerNode.h"
#include "ProtoData.pb.h"
#include <iostream>
//#include "MsgSock.h"
#include <thread>

#include <iostream>
#include <istream>
#include <ostream>
#include <string>
#include "ProtoData.pb.h"
#include <set>
#include <fc/time.hpp>
#include "Processor.h"
#include "Commissioner.h"
#include "RestFullCall.h"
#include "DataPersist.h"


#include "globals.h"
using namespace std;
	
namespace fantasybit
{

Node::Node() {
    qDebug() << "init node";
    leveldb::Options options;
    options.create_if_missing = true;
    leveldb::Status status;

    leveldb::DB *db2;
    status = leveldb::DB::Open(options, filedir("block/blockchain"), &db2);
    Node::blockchain.reset(db2);

    leveldb::DB *db4;
    status = leveldb::DB::Open(options, filedir("tx/txpool"), &db4);
    Node::txpool.reset(db4);

    current_hight = getLastLocalBlockNum();
    if (current_hight == 0)
    {
        qInfo() <<  "no blocks - making Genesis";

        current_hight = 1;
        Block sb{Commissioner::makeGenesisBlock()};

        {
        //    Writer<Block> writer{ GET_ROOT_DIR() + "genesisAlpha.out", ios::app };
        //    writer(sb);
        }
        qInfo() <<  "|" << QTD(sb.DebugString()) << "|";

        qInfo() <<  "done";

        leveldb::Slice value((char*)&current_hight, sizeof(int));
        blockchain->Put(leveldb::WriteOptions(), value, sb.SerializeAsString());
        current_hight = getLastLocalBlockNum();

        if (!BlockProcessor::verifySignedBlock(sb)) {
            qCritical() << " !BlockProcessor::verifySignedBlock(sb) ";
            return;
        }

    }

    //assert(getLastBlockNum() > 0);

    qInfo() <<  " current_hight " << current_hight;
}

bool Node::Sync() {
    qDebug() << "cureent thread" << QThread::currentThread();
    fc::optional<int> gh = getLastGlobalBlockNum();
    if ( !gh ) {
        qCritical() << " no getLastGlobalBlockNum";
        return false;
    }
    else setLastGlobalBlockNum(*gh);

    qInfo() << " global height " << *gh;
    if ( current_hight < (*gh) )
        return SyncTo(*gh);
    else
        return true;
}

bool Node::SyncTo(int gh) {
    global_height = gh;

    while ( current_hight < global_height ) {

        fc::optional<Block> sb = getGlobalBlock(current_hight+1);
        if ( !sb ) {
            qCritical() << " no getGlobalBlockNum" << current_hight+1;
            return false;
        }

        qInfo() <<  "received " << (*sb).DebugString();

        if (!BlockProcessor::verifySignedBlock(*sb)) {
            qCritical() << " !BlockProcessor::verifySignedBlock(sb) ";
            return false; ;
        }

        if ((*sb).signedhead().head().num() > current_hight + 1) {
            qCritical() << "sb.signedhead().head().num() > current_hight + 1";
            return false; ;
        }

        if ((*sb).signedhead().head().num() == current_hight + 1) {
            qInfo() << "Received next " << current_hight+1;
            int myhight = current_hight+1;
            leveldb::Slice snum((char*)&myhight, sizeof(int));
            blockchain->Put(leveldb::WriteOptions(), snum, (*sb).SerializeAsString());
            current_hight = current_hight+1;

            //Node::ClearTx(*sb);

            //CheckOrphanBlocks();
        }
        else if ( (*sb).signedhead().head().num() > current_hight+1){
            qWarning() << "Received gap in block " << (*sb).signedhead().head().num();
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
    //qDebug() << "cureent thread" << QThread::currentThread();
    int height = RestfullService::getHeight("http://192.96.159.216:4545");

    if ( myLastGlobalBlockNum() < height )
        setLastGlobalBlockNum(height);

    return height;
}

void Node::setLastGlobalBlockNum(int mylastglobalheight) {
    std::lock_guard<std::mutex> lockg{ blockchain_mutex };
    GlobalHeight = mylastglobalheight;
}


std::string Node::filedir(const std::string &in)
{
    return GET_ROOT_DIR() + in;
}

leveldb::Slice Node::i2slice(int i)
{
    leveldb::Slice value((char*)&i, sizeof(int) );
    return value;
}

Block Node::getlastLocalBlock() {
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

fc::optional<Block> Node::getLocalBlock(int num) {
    fc::optional<Block> block;
    if ( getLastLocalBlockNum() < num )
        return block;

    std::string value;
    leveldb::Slice snum((char*)&num, sizeof(int));
    if (blockchain->Get(leveldb::ReadOptions(), snum, &value).IsNotFound()) {
        qWarning() << "block not found " << num;
        //ToDo
        return block;
    }

    block = Block{};

    (*block).ParseFromString(value);
    return block;
}

fc::optional<Block> Node::getGlobalBlock(int num) {
    fc::optional<Block> block;

    //int height = RestfullService::getHeight("http://192.96.159.216:4545");

    //if ( height < num ) return;
    string bs = RestfullService::getBlk("http://192.96.159.216:4545",num);

    block = Block{};
    (*block).ParseFromString(bs);
    qInfo() << (*block).DebugString();

    return block;
}




decltype(Node::blockchain) Node::blockchain{};
decltype(Node::txpool) Node::txpool{};

decltype(Node::blockchain_mutex) Node::blockchain_mutex{};
decltype(Node::GlobalHeight) Node::GlobalHeight{};


}	

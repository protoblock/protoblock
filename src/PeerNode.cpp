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
#include "leveldb/write_batch.h"

#include "globals.h"
using namespace std;
	
namespace fantasybit
{

Node::Node() { }
void Node::init() {
    qDebug() << "kkk file" << filedir("block");
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
    qInfo() <<  "current_hight" << current_hight;

    if (current_hight == 0)
    {
  /*
        Block sb{};
        Reader<Block> b1r{GET_ROOT_DIR() +   "fantasybit-genesis-9-8-14-block.data"};
        if ( !b1r.good() )
            qCritical() << " No genesis ";
        else
            b1r.ReadNext(sb);
*/

        //qInfo() <<  "no blocks - making Genesis";
        auto  sb = Commissioner::makeGenesisBlock();

        {
        //    Writer<Block> writer{ GET_ROOT_DIR() + "genesisAlpha.out", ios::app };
        //    writer(sb);
        }
        //qInfo() <<  "|" << QTD(sb.DebugString()) << "|";

        qInfo() <<  "done";

        if (!BlockProcessor::verifySignedBlock(sb)) {
            qCritical() << " !BlockProcessor::verifySignedBlock(sb) ";
            //return;
        }
        else {
            current_hight = 1;

            leveldb::Slice value((char*)&current_hight, sizeof(int));
            blockchain->Put(leveldb::WriteOptions(), value, sb.SerializeAsString());
            current_hight = getLastLocalBlockNum();
        }
/*
        if ( !b1r.good() )
            qCritical() << " No num 1 genesis ";
        else
            b1r.ReadNext(sb);

        if (!BlockProcessor::verifySignedBlock(sb)) {
            qCritical() << " !BlockProcessor::verifySignedBlock(sb) ";
            return;
        }
        else {

        current_hight = 1;

        leveldb::Slice value1((char*)&current_hight, sizeof(int));
        blockchain->Put(leveldb::WriteOptions(), value1, sb.SerializeAsString());
        current_hight = getLastLocalBlockNum();
        }
*/

    }

    //assert(getLastBlockNum() > 0);

    qInfo() <<  " current_hight " << current_hight;
}


void Node::BackSync(int to) {

    leveldb::WriteBatch batch;
    for ( int i=getLastLocalBlockNum();i>to;i--) {
        leveldb::Slice key((char*)&i, sizeof(int));
        batch.Delete(key);
    }
    auto s = blockchain->Write(leveldb::WriteOptions(), &batch);

    current_hight = getLastLocalBlockNum();

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
    else if ( current_hight > (*gh) && current_hight > 1 ) {
        return false;
    }
    else
        return true;
}

bool Node::SyncTo(int gh) {
    global_height = gh;

    int count = 0;
    while ( current_hight < global_height ) {

        if (count > 50) return false;

        qDebug() << current_hight << global_height;
        fc::optional<Block> sb = getGlobalBlock(current_hight+1);
        if ( !sb ) {
            qCritical() << " no getGlobalBlockNum" << current_hight+1;
            QThread::currentThread()->sleep(100 * count++);
            continue;

        }

        qInfo() <<  "received " << (*sb).signedhead().head().num();

        if (!BlockProcessor::verifySignedBlock(*sb)) {
            qCritical() << " !SyncTo::verifySignedBlock(sb) ";
            QThread::currentThread()->sleep(100 * count++);
            continue;
        }

        if ((*sb).signedhead().head().num() > current_hight + 1) {
            qCritical() << "sb.signedhead().head().num() > current_hight + 1";
            //return false; ;
        }

        if ((*sb).signedhead().head().num() == current_hight + 1) {
            qInfo() << "Received next " << current_hight+1;
            int myhight = current_hight+1;
            leveldb::Slice snum((char*)&myhight, sizeof(int));
            blockchain->Put(leveldb::WriteOptions(), snum, (*sb).SerializeAsString());
            current_hight = current_hight+1;

            count = 0;
            Node::ClearTx(*sb);

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

    if (!it->Valid()) {
        delete it;
        return 0;
    }
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

    //return 20;
    qDebug() << " calling rest height";
    int height = RestfullService::getHeight(PAPIURL.data());
    qDebug() << " after rest height" << height;

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
        delete it;
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
    string bs = RestfullService::getBlk(PAPIURL.data(),num);
    Block bb{};
    bb.ParseFromString(bs);
    qInfo() << bb.SerializeAsString().size() << bb.DebugString();


    block = Block{};
    (*block).ParseFromString(bs);
    qInfo() << bs.size() << (*block).DebugString();

    return block;
}


void Node::ClearTx(const Block &b) {
    for (const auto &st : b.signed_transactions()) {
        Node::txpool->Delete(leveldb::WriteOptions(), st.id());
    }
}

void Node::Cleaner() {
    Block b{};
    std::vector<Block> replace{};
    auto *it = blockchain->NewIterator(leveldb::ReadOptions());
    for (it->SeekToFirst() ;it->Valid();it->Next() ) {
        b.ParseFromString(it->value().ToString());
        if ( Cleanit(&b) )
            replace.push_back(b);
    }
    delete it;

    for ( auto b : replace) {
        int32_t bnum = b.signedhead().head().num();
        leveldb::Slice snum((char*)&bnum, sizeof(int32_t));

        blockchain->Put(leveldb::WriteOptions(), snum, b.SerializeAsString());
        //  Cleanit(&b);
        qDebug() << "kkk" << bnum << b.DebugString();

        //string bdata = b.SerializeAsString();
        //RestfullClient rest(QUrl(LAPIURL.data()));
        //rest.postRawData("block/"+QString::number(bnum),"xxx",bdata.data(),bdata.size());

    }
}

bool Node::Cleanit(Block *b) {
    auto dt = //b.signed_transactions(0).trans().GetExtension(DataTransition::data_trans);
    b->mutable_signed_transactions(0)->mutable_trans()->MutableExtension(DataTransition::data_trans);
    bool replaceit = false;
    for ( int i=0;i<dt->data_size(); i++) {
        if ( dt->data(i).type() != Data::RESULT) continue;
        replaceit = true;

        Data *d = dt->mutable_data(i);
        ResultData *prd = d->MutableExtension(ResultData::result_data);
        for ( int i =0; i < prd->game_result().home_result_size(); i++) {
            PlayerResult *pr = prd->mutable_game_result()->mutable_home_result(i);
            if ( !pr->stats().has_kstats() ) continue;

            if ( pr->stats().kstats().fg_size() == 0) continue;

            double newres = BlockProcessor::CalcResults(pr->stats());
            qDebug() << "kkk" << newres << pr->result();
            pr->set_result(newres);
        }
        for ( int i =0; i < prd->game_result().away_result_size(); i++) {
            PlayerResult *pr = prd->mutable_game_result()->mutable_away_result(i);
            if ( !pr->stats().has_kstats() ) continue;

            if ( pr->stats().kstats().fg_size() == 0) continue;

            double newres = BlockProcessor::CalcResults(pr->stats());
            qDebug() << "kkk" << newres << pr->result();
            pr->set_result(newres);
        }

    }
    return replaceit;

}

decltype(Node::blockchain) Node::blockchain;
decltype(Node::txpool) Node::txpool;

decltype(Node::blockchain_mutex) Node::blockchain_mutex{};
decltype(Node::GlobalHeight) Node::GlobalHeight{};


}	

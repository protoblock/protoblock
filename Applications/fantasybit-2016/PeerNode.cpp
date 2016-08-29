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
#include "Commissioner.h"

#include "RestFullCall.h"
//#include <fc/time.hpp>
#include "Processor.h"

#include "DataPersist.h"
#include "leveldb/write_batch.h"

#include "globals.h"
using namespace std;
	
namespace fantasybit
{

Node::Node() { }
void Node::init() {
    write_sync.sync = true;

    Int32Comparator *cmp = new Int32Comparator();
    leveldb::Options optionsInt;
    optionsInt.create_if_missing = true;
    optionsInt.comparator = cmp;

    qDebug() << "init node";
    leveldb::Options options;
    options.create_if_missing = true;
    leveldb::Status status;

    leveldb::DB *db2;
    status = leveldb::DB::Open(optionsInt, filedir("block/blockchain"), &db2);
    if ( !status.ok()) {
        qCritical() << " error opening block/blockchain";
        return;
    }
    Node::blockchain.reset(db2);

    leveldb::DB *db4;
    status = leveldb::DB::Open(options, filedir("tx/txpool"), &db4);
    Node::txpool.reset(db4);

    current_hight = getLastLocalBlockNum();
    qInfo() <<  "current_hight" << current_hight;

#ifdef CHECKPOINTS
    if ( current_hight < Commissioner::DeveloperCheckpointHigh() ) {
        auto dc = Commissioner::getCheckPoint();

        current_hight = Commissioner::DeveloperCheckpointHigh();

        leveldb::Slice value((char*)&current_hight, sizeof(int32_t));
        blockchain->Put(write_sync, value, dc.SerializeAsString());
        current_hight = getLastLocalBlockNum();

        NFLStateData::InitCheckpoint();

        BlockRecorder::InitCheckpoint(current_hight);
    }
#endif

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

        qDebug() << sb.DebugString().data();
        if (!BlockProcessor::verifySignedBlock(sb)) {
            qCritical() << " !BlockProcessor::verifySignedBlock(sb) ";
            //return;
        }
        else {
            current_hight = 1;

            leveldb::Slice value((char*)&current_hight, sizeof(int32_t));
            blockchain->Put(write_sync, value, sb.SerializeAsString());
            current_hight = getLastLocalBlockNum();
        }
/*
        if ( !b1r.good() )
            qCritical() << " No num 1 genesis ";
        else
            b1r.odNext(sb);

        if (!BlockProcessor::verifySignedBlock(sb)) {
            qCritical() << " !BlockProcessor::verifySignedBlock(sb) ";
            return;
        }
        else {

        current_hight = 1;

        leveldb::Slice value1((char*)&current_hight, sizeof(int));
        blockchain->Put(write_sync, value1, sb.SerializeAsString());
        current_hight = getLastLocalBlockNum();
        }
*/

    }

    //assert(getLastBlockNum() > 0);

    qInfo() <<  " current_hight " << current_hight;
}


void Node::BackSync(int32_t to) {

    leveldb::WriteBatch batch;
    for ( int32_t i=getLastLocalBlockNum();i>to;i--) {
        leveldb::Slice key((char*)&i, sizeof(int32_t));
        batch.Delete(key);
    }
    auto s = blockchain->Write(write_sync, &batch);

    current_hight = getLastLocalBlockNum();

}

bool Node::Sync() {
    qDebug() << "cureent thread" << QThread::currentThread();
    fc::optional<int32_t> gh = getLastGlobalBlockNum();
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

bool Node::SyncTo(int32_t gh) {
    global_height = gh;
    bool forked  = false;
    string previd;
    int count = 0;
#ifdef CHECKPOINTS
    if ( current_hight == Commissioner::DeveloperCheckpointHigh())
        previd = Commissioner::DeveloperCheckPointId();
    else
#endif
    {
        auto ob = getLocalBlock(current_hight, true);
        if ( ob )
            previd = FantasyAgent::BlockHash(*ob);
    }
    while ( current_hight < global_height ) {

        if (count > 50) return false;

        qDebug() << current_hight << global_height;
#ifdef Q_OS_MAC
        auto bend = current_hight+200;
#endif
#ifdef Q_OS_WIN
        auto bend = current_hight+150;
#endif
        auto vsb = getGlobalBlock(current_hight+1, bend < global_height ? bend : global_height);

        if ( vsb.size() == 0 ) {
            qCritical() << " no getGlobalBlockNum" << current_hight+1;
            QThread::currentThread()->msleep(100 * count++);
            continue;
        }

        qInfo() <<  "received " << vsb.size(); //(*sb).signedhead().head().num();

        for ( auto ssb : vsb) {
        Block *sb = &ssb;
        if (!BlockProcessor::verifySignedBlock(*sb)) {
            qCritical() << " !SyncTo::verifySignedBlock(sb) ";
            QThread::currentThread()->msleep(100 * count++);
            break;
        }

        if ((*sb).signedhead().head().num() > current_hight + 1) {
            qCritical() << "sb.signedhead().head().num() > current_hight + 1";
            break;
        }

        //fork
        if ( (*sb).signedhead().head().prev_id() != previd ) {
            forking = true;
            if ( !BackFork((*sb).signedhead().head().prev_id(),current_hight) )
                return forking = false;
            else {
                auto ob = getLocalBlock(current_hight, true);
                if ( !ob ) return false;
                previd = FantasyAgent::BlockHash(*ob);
                if ( (*sb).signedhead().head().prev_id() != previd ) {
                    qCritical() << " bad prev after BackFork";
                }

                forked = true;
           }
           forking = false;
        }

        if ((*sb).signedhead().head().num() == current_hight + 1) {
            qInfo() << "Received next " << current_hight+1;
            int32_t myhight = current_hight+1;
            leveldb::Slice snum((char*)&myhight, sizeof(int32_t));
            blockchain->Put(write_sync, snum, (*sb).SerializeAsString());
            current_hight = current_hight+1;

            qInfo() << "Put next " << current_hight;

            //int32_t num = *(reinterpret_cast<const int32_t *>(snum.data()));

            //int32_t num2;
            //memcpy(&num2,snum.data(),snum.size());
            //qWarning() << myhight << num2 << num << snum.size() << snum.data() << sizeof(int32_t) << sizeof(char) << "yoyo getLastLocalBlockNum()" << getLastLocalBlockNum() << "current_hight" << current_hight;
            qInfo() << myhight  << snum.size() << snum.data() << sizeof(int32_t) << sizeof(char);
            qInfo() << "yoyo getLastLocalBlockNum()" << getLastLocalBlockNum() << "current_hight" << current_hight;

            count = 0;
            //Node::ClearTx(*sb);

            previd = FantasyAgent::BlockHash(*sb);

            //CheckOrphanBlocks();
        }
        else if ( (*sb).signedhead().head().num() > current_hight+1){
            qWarning() << "Received gap in block " << (*sb).signedhead().head().num();
            break;
        }
        }
    }

    return (!forked && (current_hight == global_height));
}

bool Node::BackFork(const string &goodid, int32_t num) {

    string prevprev = goodid;
    string id = "";
    int count = 0;
    do {
        if ( num == 1) {
            qCritical() << " bad genesis? ";

            return false;
        }
        if ( count > 15 ) return false;
        fc::optional<Block> gb = getGlobalBlock(num);
        if ( !gb ) {
            qCritical() << " no prev global block " << num;
            QThread::currentThread()->msleep(100 * count++);
            continue;
        }

        if (!BlockProcessor::verifySignedBlock(*gb))
            return false;

        id = FantasyAgent::BlockHash(*gb);
        if ( id != prevprev ) {
            qCritical() << " expect pre_id t equal id(prev)" << prevprev << id;
            return false;
        }

        leveldb::Slice snum((char*)&num, sizeof(int32_t));
        auto status =
                blockchain->Put(write_sync, snum, (*gb).SerializeAsString());
        if ( !status.ok() ) return false;

        prevprev = (*gb).signedhead().head().prev_id();

        num--;
        fc::optional<Block> lb = getLocalBlock(num, true);
        if ( !lb ) return false;

        id = FantasyAgent::BlockHash(*lb);

    } while (prevprev != id );

    return true;
}

int32_t Node::getLastLocalBlockNum() {

    //qWarning() << "yoyo" << "in getLastLocalBlockNum";

    std::string value;
    auto *it = blockchain->NewIterator(leveldb::ReadOptions());
    //qWarning() << "yoyo" << "in getLastLocalBlockNum 1";

    it->SeekToLast();

    //qWarning() << "yoyo" << "in getLastLocalBlockNum 2";

    if (!it->Valid()) {
        delete it;
        return 0;
    }
    auto slice = it->key();

    //qWarning() << "yoyo" << "in getLastLocalBlockNumxx";

    //Block sb{};
    //sb.ParseFromString(it->value().ToString());
    //qWarning() << "yoyo" << sb.signedhead().head().num();
    //auto previd = FantasyAgent::BlockHash(sb);

    int32_t num = *(reinterpret_cast<const int32_t *>(slice.data()));

    //qWarning() << "yoyo" << num << "getLastLocalBlockNum";

    delete it;

#ifdef STOP_HEIGHT_TEST
    if (num > 21199 )
        num = 21199;
#endif

    return num;
}

int32_t Node::myLastGlobalBlockNum() {
    int32_t myglobalheight = 0;
    {
        std::lock_guard<std::mutex> lockg{ blockchain_mutex };
        myglobalheight = GlobalHeight;
    }

    return myglobalheight;
}

fc::optional<int32_t> Node::getLastGlobalBlockNum() {
    //qDebug() << "cureent thread" << QThread::currentThread();


    //return 20;
      qDebug() << " calling rest height";
    int32_t height = RestfullService::getHeight(PAPIURL.data());
    //qDebug() << " after rest height" << height;

#ifdef STOP_HEIGHT_TEST
    height = 11321;
#endif

    if ( myLastGlobalBlockNum() < height )
        setLastGlobalBlockNum(height);

    return height;
}

void Node::setLastGlobalBlockNum(int32_t mylastglobalheight) {
    std::lock_guard<std::mutex> lockg{ blockchain_mutex };
    GlobalHeight = mylastglobalheight;
}


std::string Node::filedir(const std::string &in)
{
    return GET_ROOT_DIR() + in;
}

leveldb::Slice Node::i2slice(int32_t i)
{
    leveldb::Slice value((char*)&i, sizeof(int32_t) );
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

fc::optional<Block> Node::getLocalBlock(int32_t num, bool force) {

    fc::optional<Block> block;
    if ( forking && !force )
        return block;

    std::lock_guard<std::mutex> lockg{ blockchain_mutex };

    if ( getLastLocalBlockNum() < num )
        return block;

    std::string value;
    leveldb::Slice snum((char*)&num, sizeof(int32_t));
    auto ret = blockchain->Get(leveldb::ReadOptions(), snum, &value);
    if ( !ret.ok() ){
        qWarning() << "block not found " << num;
        //ToDo
        return block;
    }

    block = Block{};

    (*block).ParseFromString(value);
    return block;
}

fc::optional<Block> Node::getGlobalBlock(int32_t num) {
    fc::optional<Block> block;

    //if ( height < num  ) return;
    string bs = RestfullService::getBlk(PAPIURL.data(),num);
    Block bb{};
    bb.ParseFromString(bs);
    qInfo() << bb.SerializeAsString().size() << bb.DebugString();


    block = Block{};
    (*block).ParseFromString(bs);
    qInfo() << bs.size() << (*block).DebugString();

    return block;
}

std::vector<Block> Node::getGlobalBlock(int32_t num, int32_t bend) {
    auto strblks = RestfullService::getBlk(PAPIURL.data(),num,bend);

    std::vector<Block> ret{};

    for ( auto str : strblks) {
        Block bb{};
        if ( !bb.ParseFromString(str) )
            break;

        ret.push_back(bb);
    }

    return ret;
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

        blockchain->Put(write_sync, snum, b.SerializeAsString());
        //  Cleanit(&b);

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
            pr->set_result(newres);
        }
        for ( int i =0; i < prd->game_result().away_result_size(); i++) {
            PlayerResult *pr = prd->mutable_game_result()->mutable_away_result(i);
            if ( !pr->stats().has_kstats() ) continue;

            if ( pr->stats().kstats().fg_size() == 0) continue;

            double newres = BlockProcessor::CalcResults(pr->stats());
            pr->set_result(newres);
        }

    }
    return replaceit;

}

decltype(Node::blockchain) Node::blockchain;
decltype(Node::txpool) Node::txpool;

decltype(Node::blockchain_mutex) Node::blockchain_mutex{};
decltype(Node::GlobalHeight) Node::GlobalHeight{};
bool Node::forking = false;

}	

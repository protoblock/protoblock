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
#include <QFileInfo>
#include "globals.h"
using namespace std;

namespace fantasybit
{

#ifdef STOP_HEIGHT_TEST
//    int intSTOP_HEIGHT_TEST = 14918;
    int intSTOP_HEIGHT_TEST = -1;
#endif

Node::Node() { current_boot.set_blocknum(-1);}
void Node::init() {
#ifndef NO_DOSPECIALRESULTS
    doSpecialResults = false;
#endif

#ifndef NO_REMOVEALL_FORK1
    QFileInfo check_file( (GET_ROOT_DIR() + "postfork1.1").data ());
    if (!check_file.exists() ) {
        pb::remove_all(GET_ROOT_DIR() + "index/");
        pb::remove_all(GET_ROOT_DIR() + "block/");
        pb::remove_all(GET_ROOT_DIR() + "trade/");
        QFile file( (GET_ROOT_DIR() + "postfork1.1").data () );
        file.open(QIODevice::WriteOnly);
    }
#endif

#ifdef STARTSUPERBOWL52
        QFileInfo check_file1( (GET_ROOT_DIR() + "startSuperbowl52").data ());
        if (!check_file1.exists() ) {
            pb::remove_all(GET_ROOT_DIR() + "index/");
            pb::remove_all(GET_ROOT_DIR() + "block/");
            pb::remove_all(GET_ROOT_DIR() + "trade/");
            QFile file( (GET_ROOT_DIR() + "startSuperbowl52").data () );
            file.open(QIODevice::WriteOnly);
        }
#endif

#ifdef START2017WITH2014
    QFileInfo check_file( (GET_ROOT_DIR() + "start2017").data ());
    if (!check_file.exists() ) {
        pb::remove_all(GET_ROOT_DIR() + "index/");
        pb::remove_all(GET_ROOT_DIR() + "block/");
        pb::remove_all(GET_ROOT_DIR() + "trade/");
        QFile file( (GET_ROOT_DIR() + "start2017").data () );
        file.open(QIODevice::WriteOnly);
    }
#endif

#ifdef REMOVEALL_SEASON_TRADING
    QFileInfo check_file( (GET_ROOT_DIR() + "seasontrade2017wk2").data ());
    if (!check_file.exists() ) {
        pb::remove_all(GET_ROOT_DIR() + "index/");
        pb::remove_all(GET_ROOT_DIR() + "block/");
        pb::remove_all(GET_ROOT_DIR() + "trade/");
        QFile file( (GET_ROOT_DIR() + "seasontrade2017wk2").data () );
        file.open(QIODevice::WriteOnly);
    }
#endif

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

    leveldb::Options options2;
    options2.create_if_missing = true;
    leveldb::DB *db3;
    status = leveldb::DB::Open(options2, filedir("block/bootstrap"), &db3);
    if ( !status.ok()) {
        qCritical() << " error opening block/bootstrap";
        return;
    }
    Node::bootstrap.reset(db3);


//    leveldb::DB *db4;
//    status = leveldb::DB::Open(options, filedir("tx/txpool"), &db4);
//    Node::txpool.reset(db4);

    current_hight = getLastLocalBlockNum();
    qDebug() <<  "PeerNode current_hight" << current_hight;

#ifdef BLK18
    if ( current_hight == 0) {
            Block bl;
            Reader<Block> reader{ GET_ROOT_DIR(true) + "blk18.out"};
            //"D:\\work\\build-ProRoto2016-Desktop_Qt_5_10_1_MSVC2013_64bit-Debug\\Applications\\ProtoBlock2016\\debug\\storage\\newblocks.out"};
            if ( !reader.good() )
                qCritical() << "!good" << GET_ROOT_DIR() + "blk18.out";
            else {
                int count = 0;
                while ( reader.ReadNext(bl) ) {
                    int32_t height = bl.signedhead().head().num();
                    leveldb::Slice value((char*)&height, sizeof(int32_t));
                    blockchain->Put(write_sync, value, bl.SerializeAsString());
                    qDebug() << "jayberg blockchain put " << height << count++;
                }
                qDebug() << " jayberg wrote " << bl.signedhead().head().num();
            }
    }
    current_hight = getLastLocalBlockNum();
//    intSTOP_HEIGHT_TEST = current_hight;
#endif

#ifndef NOCHECK_LOCAL_BOOTSTRAP
    current_boot = getLastLocalBoot();
    qInfo() <<  "current_boot" << current_boot.DebugString().data();

    if ( current_boot.blocknum() > 0 ) {
        BlockRecorder::zeroblock = current_boot.blocknum();
        if ( current_boot.blocknum() >= current_hight ) {
            current_hight = current_boot.blocknum();
            LdbWriter ldb;
            ldb.init(Node::bootstrap.get());
            if (!BlockProcessor::verifyBootstrap(ldb,current_boot)) {
                qCritical() << " !BlockProcessor::verifySignedBlock(sb) ";
                //return;
            }
            else {
                current_hight = current_boot.blocknum();
                Block sb;
                leveldb::Slice value((char*)&current_hight, sizeof(int32_t));
                blockchain->Put(write_sync, value, sb.SerializeAsString());
                current_hight = getLastLocalBlockNum();

                pb::remove_all(Platform::instance()->getRootDir() + "index/");

                NFLStateData::InitCheckpoint();
                BlockRecorder::InitCheckpoint(current_hight);
            }
        }
#ifndef NO_DOSPECIALRESULTS
        else if ( doSpecialResults ) {
            NFLStateData::InitCheckpoint(true);
        }
#endif
    }
#endif


#ifndef NOUSE_GENESIS_BOOT
    if ( current_hight == 0 ) {
        LdbWriter ldb;
        ldb.init(Node::bootstrap.get());
        current_boot = Commissioner::makeGenesisBoot(ldb);
        if ( current_boot.blocknum() < 0 )
            qCritical() << " !current_boot.blocknum() <= 0 ";
        else if (current_boot.blocknum() < current_hight)
            qCritical() << " current_boot.blocknum() < current_hight" << current_boot.blocknum() <<  current_hight;
        else {
            qDebug() << " processing " << current_boot.DebugString().data();

            if (!BlockProcessor::verifyBootstrap(ldb,current_boot)) {
                qCritical() << " !BlockProcessor::verifySignedBlock(sb) ";
                //return;
            }
            else {
                ldb.write("head",current_boot.key());
                current_hight = current_boot.blocknum();
                Block sb;
                leveldb::Slice value((char*)&current_hight, sizeof(int32_t));
                blockchain->Put(write_sync, value, sb.SerializeAsString());
                current_hight = getLastLocalBlockNum();

                pb::remove_all(Platform::instance()->getRootDir() + "index/");
                NFLStateData::InitCheckpoint();

                BlockRecorder::InitCheckpoint(current_hight);
            }
        }
    }
#endif

#ifndef USE_LOCAL_GENESIS
    if (current_hight == 0)
        qDebug() << " ERROR? - NO Boot - No Local Genesis!";
    else
        qDebug() << " current_hight " << current_hight << current_boot.DebugString().data();
#else
    if (current_hight == 0) {
        auto  sb = Commissioner::makeGenesisBlock();

#ifdef TRACEDEBUG
        qDebug() << " current sb ";
        qDebug() << sb.DebugString().data();
        qDebug() << " 1.5 current sb ";
#endif
        if (!BlockProcessor::verifySignedBlock(sb))
            qCritical() << " !BlockProcessor::verifySignedBlock(sb) ";
        else {
            current_hight = 0;
            leveldb::Slice value((char*)&current_hight, sizeof(int32_t));
            blockchain->Put(write_sync, value, sb.SerializeAsString());
            current_hight = getLastLocalBlockNum();
        }
    }
#endif
    qInfo() <<  "current_hight " << current_hight;
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
    qDebug() << "Node::Sync() cureent thread" << QThread::currentThread();
    fc::optional<int32_t> gh = getLastGlobalBlockNum();
    if ( !gh || gh == 0 ) {
        qCritical() << " no getLastGlobalBlockNum";
        return true;
    }
    else setLastGlobalBlockNum(*gh);

    qInfo() << "Node::Sync() global height " << *gh;
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

#ifdef TRACE4
    qDebug() << " current_hight " << current_hight << "current_boot.blocknum()" <<current_boot.blocknum();
#endif

    if ( current_hight == current_boot.blocknum() ) {
        previd = current_boot.previd();
#ifdef CHECKPOINTS_2015
//    if ( current_hight == Commissioner::DeveloperCheckpointHigh())
//        previd = Commissioner::DeveloperCheckPointId();
//    else
#endif
    }
    else {
        auto ob = getLocalBlock(current_hight, true);
        if ( ob )
            previd = FantasyAgent::BlockHash(*ob);
    }
    while ( current_hight < global_height ) {

        if (count > 50) return false;
#ifdef TRACE4
        qDebug() << current_hight << global_height;
#endif

#ifdef Q_OS_MAC
        auto bend = current_hight+100;
#endif
#ifdef Q_OS_WIN
        auto bend = current_hight+100;
#endif
        auto vsb = getGlobalBlock(current_hight+1, bend < global_height ? bend : global_height);

        if ( vsb.size() == 0 ) {
            qCritical() << " no getGlobalBlockNum" << current_hight+1;
            QThread::currentThread()->msleep(100 * count++);
            continue;
        }
#ifdef TRACE4
        qInfo() <<  "received " << vsb.size(); //(*sb).signedhead().head().num();
#endif
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
                qWarning() << (*sb).signedhead().head().prev_id().data() << " != prev " << previd.data();
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
#ifdef TRACE4
                qInfo() << "Received next " << current_hight+1;
#endif
                int32_t myhight = current_hight+1;
                leveldb::Slice snum((char*)&myhight, sizeof(int32_t));
                blockchain->Put(write_sync, snum, (*sb).SerializeAsString());
                current_hight = current_hight+1;
#ifdef TRACE4
                qInfo() << "Put next " << current_hight;
                //int32_t num = *(reinterpret_cast<const int32_t *>(snum.data()));

                //int32_t num2;
                //memcpy(&num2,snum.data(),snum.size());
                //qWarning() << myhight << num2 << num << snum.size() << snum.data() << sizeof(int32_t) << sizeof(char) << "yoyo getLastLocalBlockNum()" << getLastLocalBlockNum() << "current_hight" << current_hight;
                qInfo() << myhight  << snum.size() << snum.data() << sizeof(int32_t) << sizeof(char);
                qInfo() << "yoyo getLastLocalBlockNum()" << getLastLocalBlockNum() << "current_hight" << current_hight;
#endif

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
    qWarning() << " back forking " << goodid.data() << num;
    string prevprev = goodid;
    string id = "";
    int count = 0;
    do {
        if ( num == 1) {
            qCritical() << " bad genesis? ";
            return false;
        }
        if ( count > 15 ) {
            qCritical() << " BackFork count > 15";
            return false;
        }
        fc::optional<Block> gb = getGlobalBlock(num);
        if ( !gb ) {
            qCritical() << " no prev global block " << num;
            QThread::currentThread()->msleep(100 * count++);
            continue;
        }

        if (!BlockProcessor::verifySignedBlock(*gb)) {
            qCritical() << "Node::BackFork !verifySignedBlock";
            return false;
        }

        id = FantasyAgent::BlockHash(*gb);
        if ( id != prevprev ) {
            qCritical() << " expect pre_id t equal id(prev)" << prevprev.data() << id.data();
            return false;
        }

        leveldb::Slice snum((char*)&num, sizeof(int32_t));
        auto status =
                blockchain->Put(write_sync, snum, (*gb).SerializeAsString());
        if ( !status.ok() ) {
            qCritical() << "Node::BackFork !status.ok()";
            return false;
        }

        prevprev = (*gb).signedhead().head().prev_id();

        num--;
        fc::optional<Block> lb = getLocalBlock(num, true);
        if ( !lb ) {
            qCritical() << "Node::BackFork !lb";
            return false;
        }

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
    if (num > intSTOP_HEIGHT_TEST && intSTOP_HEIGHT_TEST != -1) {
        qWarning() << " STOP_HEIGHT_TEST " << num << intSTOP_HEIGHT_TEST;
        num = intSTOP_HEIGHT_TEST;
    }
#endif

    return num;
}

int32_t Node::myLastGlobalBlockNum() {
    int32_t myglobalheight = 0;
    {
        std::lock_guard<std::mutex> lockg{ blockchain_mutex };
        myglobalheight = GlobalHeight;
    }


#ifdef TRACE4
    qDebug() << " myglobalheight " << myglobalheight;
#endif
    return myglobalheight;
}

int Node::getBootSeason() {
    QString links(PAPIURL.data());
    //QString links("https://158.222.102.83:4545");
    QString route("season");

    QMap<QString,QString>  headers;
    QMap<QString,QVariant> params;

    QUrl url;
    url.setUrl(links);
    RestfullClient rest (url);
    rest.getData(route);
    auto resp = rest.lastReply();

    qDebug() << resp;
    QJsonDocument ret = QJsonDocument::fromJson(resp);
    qDebug() << ret.isNull() << ret.isEmpty() << ret.isArray() << ret.isObject();

    QJsonObject jo = ret.object();
    auto sseason = jo.value("season").toString().toStdString();

    return stoi(sseason);
}

#ifndef NOCHECK_LOCAL_BOOTSTRAP
Bootstrap Node::getLastLocalBoot() {
    QString links(PAPIURL.data());
    //QString links("https://158.222.102.83:4545");
    QString route("week");

    QMap<QString,QString>  headers;
    QMap<QString,QVariant> params;

    QUrl url;
    url.setUrl(links);
    RestfullClient rest (url);
    rest.getData(route);
    auto resp = rest.lastReply();

    qDebug() << resp;
    QJsonDocument ret = QJsonDocument::fromJson(resp);
    qDebug() << ret.isNull() << ret.isEmpty() << ret.isArray() << ret.isObject();

    QJsonObject jo = ret.object();
    auto weekstr = jo.value("week").toString().toStdString();

    int week;
    if ( weekstr == "")
        week = -1;
    else
        week = stoi(weekstr);

    //todo: season
    int sseason = getBootSeason();

#ifdef NOCHECK_LOCAL_BOOTSTRAP_ONLY1
    week = 1;
#endif

#ifdef NOCHECK_LOCAL_BOOTSTRAP_ONLY0
    week = 0;
#endif

#ifdef NOCHECK_LOCAL_BOOTSTRAP_MINUS1
    week--;
#endif
    Bootstrap head;
    LdbWriter ldb;
    ldb.init(Node::bootstrap.get());
    string localhead = ldb.read("head");

    bool done = false;
    while ( !done ) {
        if ( week < 0 ) {
            sseason--;
            if ( sseason < 2017 ) {
                done = true;
                break;

            }
            else {
                week = 16;
            }
        }

        string globalhead = to_string(sseason) + (week < 10 ?  + "0" : "") + to_string(week);

#ifndef NO_DOSPECIALRESULTS
        if ( globalhead == "201613" && !Commissioner::BootStrapFileExists(globalhead) ) {
            doSpecialResults = true;
            qWarning() << "getLastLocalBoot  doSpecialResults " << doSpecialResults;
        }
#endif
        if ( globalhead > localhead  ) {
            head = Commissioner::makeGenesisBoot(ldb,globalhead);
            if ( head.blocknum() <= 0 ) {
                qCritical() << globalhead.data() <<  " !current_boot.blocknum() <= 0 ";
                week--;
            }
            else if (!BlockProcessor::verifyBootstrap(ldb,head)) {
                qCritical() << " !BlockProcessor::verifySignedBlock(sb) ";
                week--;
                //return;
            }
            else
                done = true;
                qDebug() << " getLastLocalBoot " << head.DebugString().data();
                ldb.write("head",head.key());
//                if ( head.key() == "201716" ) {
//                    head.set_week(21);
//                    head.set_season(2017);
//                    ldb.write(head.key(),ldb.write(head));
//                }
        }
        else {
            done = true;
#ifndef NO_DOSPECIALRESULTS
            if ( doSpecialResults ) {
                auto holdhead = ldb.read("head");
                Bootstrap temphead = Commissioner::makeGenesisBoot(ldb,globalhead);
                ldb.write("head",holdhead);
                qWarning() << "getLastLocalBoot  if doSpecialResults " << holdhead.data();
            }
#endif
        }
    }

    ldb.read(ldb.read(ldb.read("head")),head);
    return head;
}
#endif

fc::optional<int32_t> Node::getLastGlobalBlockNum() {
    //qDebug() << "cureent thread" << QThread::currentThread();


    //return 20;
//      qDebug() << " calling rest height" << PAPIURL.data();
    int32_t height = RestfullService::getHeight(PAPIURL.data());
#ifdef TRACE
    qDebug() << "Node::getLastGlobalBlockNum()" << height;
#endif

#ifdef STOP_HEIGHT_TEST
    if ( height != intSTOP_HEIGHT_TEST && intSTOP_HEIGHT_TEST != -1 ) {
        height = intSTOP_HEIGHT_TEST;
        qWarning() << "getLastGlobalBlockNum STOP_HEIGHT_TEST" << height;
    }
#endif

    if ( myLastGlobalBlockNum() != height )
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
        qCritical() << " getlastLocalBlock !it->Valid()";
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

    if ( getLastLocalBlockNum() < num ) {
        qDebug() << "getLastLocalBlockNum() < num " << num;
        return block;
    }

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

    block = Block{};
    (*block).ParseFromString(bs);
#ifdef TRACE
    qDebug() << "getGlobalBlock (" << num <<") size:" <<  bs.size();
    qDebug() << (*block).DebugString().data();
#endif

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

decltype(Node::blockchain) Node::blockchain;
decltype(Node::bootstrap) Node::bootstrap;

decltype(Node::blockchain_mutex) Node::blockchain_mutex{};
decltype(Node::GlobalHeight) Node::GlobalHeight{};
bool Node::forking = false;
#ifndef NO_DOSPECIALRESULTS
bool Node::doSpecialResults = false;
#endif
}

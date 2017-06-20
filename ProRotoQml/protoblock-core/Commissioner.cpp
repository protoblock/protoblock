//
//  Comissioner.cpp
//  fantasybit
//
//  Created by Jay Berg on 4/1/14.
//
//

#include "Commissioner.h"
#include "FantasyName.h"
#include <iostream>
#include <ctime>
//#include "playerloader.h"
#include "DataPersist.h"
#include "platform.h"
#include "appsettings.h"
#include "ldbwriter.h"
#include "qurl.h"
#include "RestFullCall.h"

using namespace std;

namespace fantasybit {




Block Commissioner::makeGenesisBlockRaw() { Block b; return b; } /*`
    //return;
    Block b;
    {
    Reader<Block> reader{GET_ROOT_DIR() +   "FantasyBit-Genesis-0-block.data"};
    if ( !reader.good() )
        qCritical() << " No genesis ";
    else
        if ( !reader.ReadNext(b) )
            qCritical() << " No genesis ";
    }
    qDebug() << b.DebugString();


    //return;
    Block b1;
    {
    Reader<Block> reader{GET_ROOT_DIR() +   "FantasyBit-Genesis-1-block.data"};
    if ( !reader.good() )
        qCritical() << " No genesis ";
    else
        if ( !reader.ReadNext(b) )
            qCritical() << " No genesis ";
    }
    qDebug() << b.DebugString();

*/
/*
    FantasyAgent dagent;
    dagent.beDataAgent();

    //dagent.signPlayer("FantasyAgent");
    dagent.writeNomNonic("data.no.out");

    Transaction dasn =
            makeFantasyAgent();

    SignedTransaction dasn =
            makeFantasyAgent();


    FantasyAgent magent {"master.out"};
    magent.signPlayer("FantasyMaster");
    magent.writeNomNonic("master.no.out");

    FantasyAgent dagent {"data.out"};
    dagent.signPlayer("FantasyAgent");
    dagent.writeNomNonic("data.no.out");

    SignedTransaction dasn =
            makeFantasyAgent(dagent);

    SignedTransaction
    masn = makeGenesisName(magent);

    SignedTransaction
    mansn = makeAgentName(magent,dasn);


    {
    Writer<SignedTransaction> writer{ GET_ROOT_DIR() + "dasn"};
    writer(dasn);
    }
    {
    Writer<SignedTransaction> writer{ GET_ROOT_DIR() + "masn"};
    writer(masn);
    }
    {
    Writer<SignedTransaction> writer{ GET_ROOT_DIR() + "mansn"};
    writer(mansn);
    }

    Transaction mtx = MasterGenesisTransition();
    auto smtx = magent.makeSigned(mtx);

    BlockHeader zerohead = MasterGenesisBlockHeader(magent,masn,smtx);

    SignedBlockHeader mastersbh{};
    mastersbh = magent.makeSigned(zerohead);
    //mastersbh = pr.second;
    //pr.first = id

    Block b{};
    b.mutable_signedhead()->CopyFrom(mastersbh);
    b.add_signed_transactions()->CopyFrom(smtx);
    b.add_signed_transactions()->CopyFrom(masn);
    b.add_signed_transactions()->CopyFrom(mansn);

    {
    Writer<Block> writer{ GET_ROOT_DIR() + "FantasyBit-Genesis-0-block.data"};
    writer(b);
    }


    FantasyAgent fa;
    fa.beDataAgent();



    auto gt = GenesisTransition();
    auto sgt = dagent.makeSigned(gt);
    BlockHeader onehead = GenesisBlockHeader(zerohead,dagent,dasn,sgt);

    SignedBlockHeader dasbh{};
    dasbh = dagent.makeSigned(onehead);
    //mastersbh = pr.second;
    //pr.first = id

    Block b1{};
    b.mutable_signedhead()->CopyFrom(mastersbh);
    b.add_signed_transactions()->CopyFrom(smtx);
    b.add_signed_transactions()->CopyFrom(masn);
    {
    Writer<Block> writer{ GET_ROOT_DIR() + "FantasyBit-Genesis-1-block.data"};
    writer(b1);
    }



    return b1;

}
*/

// pair<Block,Block>

#ifdef XXXSTAGINGFOOTBALL

Block Commissioner::makeGenesisBlock() {
    Block b{};

    /*
    uint32_t sz = sizeof(genesis_data);

    b.ParseFromArray(genesis_data,sizeof(genesis_data));

    return b;

    b.ParseFromArray(genesis_data,sizeof(genesis_data));

    return b;
*/
    Reader<Block> reader{GET_ROOT_DIR() + "genesisAlpha.out"};
    if ( !reader.good() )
        qCritical() << " No genesis ";
    else
        if ( !reader.ReadNext(b) )
            qCritical() << " No genesis ";

    return b;
}

#else

Block Commissioner::makeGenesisBlock() {
   // Block b{};

    /*
    Reader<Block> reader{GET_ROOT_DIR() + "FantasyBit-Genesis-1-block.data.out"};
    reader.ReadNext(b);
    if ( !reader.good() )
        qCritical() << " No genesis ";
    else
        if ( reader.ReadNext(b) )
            qCritical() << " No genesis ";

    return b;
*/

   // Writer<Block> writer{ GET_ROOT_DIR() + "FantasyBit-Genesis-1-block.data"};

    //FantasyAgent dagent;
    //dagent.beDataAgent();

    //dagent.signPlayer("FantasyAgent");
    //dagent.writeNomNonic("data.no.out");

  //  Transaction tx =
   //         makeFantasyAgent();
    //auto sn = dagent.makeSigned(tx);


    NameProof nameproof{};
    nameproof.set_type(NameProof_Type_ORACLE);

    NameTrans nametrans{};
    nametrans.set_public_key("25dTUQHwaPHdN2fXjpryz5jrrXxU6NNfKgrpJRA4VheJ4");
    nametrans.set_fantasy_name(FantasyAgentName());
    nametrans.mutable_proof()->CopyFrom(nameproof);

    Transaction trans{};
    trans.set_version(Commissioner::GENESIS_NUM);
    trans.set_type(TransType::NAME);
    trans.MutableExtension(NameTrans::name_trans)->CopyFrom(nametrans);


    SignedTransaction st{};
    st.mutable_trans()->CopyFrom(trans);
    //auto p = getIdSig(trans.SerializeAsString());
    st.set_id("ccae751b85a84496b78286e4ca3264d793fb5b248d1a145d3a377b1a0779fbeb");
    st.set_sig("iKx1CJNVRPp2LuCwkp66NfZTKxdZ98AknYYVzRtbuwT7RtDcATHc7WWruscbpkJ6qUcDBiDBjrkukRF7FmoJ97ikLFsb3bpiwG");
    st.set_fantasy_name("FantasyAgent");




    Transaction gt{};
    QString genesisDataFile = Platform::instance()->settings()->getSetting(AppSettings::GenesisTranactionLocation).toString();
    Reader<Transaction> treader{genesisDataFile.toStdString()};
    treader.ReadNext(gt);

//    qDebug() << " good " << treader.good() << genesisDataFile << gt.DebugString().data();


    //qDebug() << sn.sig() << sn.id();

    //dagent.makeGenesis();



    //auto p = dagent.getIdSig(gt.SerializeAsString());


    SignedTransaction sst;
    sst.mutable_trans()->CopyFrom(gt);
    sst.set_id("458bc71888897c2182a8c84b230d616198f57ec0600b527317fedd2280c5d3fb");
    sst.set_sig("iKx1CJMnxRcFL5jjJuQdEkYadpQZaBLVDgcMYkmPLrxNMTegRmHAnHb28NQQfvk5bLi4WPA5raFQJMq3XCdjbGCaD9xwJcgNQV");

    //qDebug() << p.first << p.second;


    BlockHeader bh{};
    bh.set_version(1);
    bh.set_num(1);
    bh.set_prev_id("000000000000");

    bh.set_timestamp(1441683084);

    bh.set_generator_pk("25dTUQHwaPHdN2fXjpryz5jrrXxU6NNfKgrpJRA4VheJ4");
    bh.set_generating_sig(pb::hashit(bh.generator_pk()).str());
    bh.set_basetarget(0); //todo
    bh.set_blocktype(BlockHeader_Type_DATA);

    auto root = pb::hashit(sst.id() + st.id());
    bh.set_transaction_id(root.str());


    //SignedBlockHeader dasbh{};
    // dasbh = dagent.makeSigned(bh);
    SignedBlockHeader sbh{};
    sbh.mutable_head()->CopyFrom(bh);
    sbh.set_sig("iKkkiBsva3XYQPGdGFLFudhPMJeyiYNhJ8Tp57UTwjdACbYpPLAJt2RLd8g8mpCVTqCwJUCNXKmo7KrfzSHAkHo6sqv7LskAP9");
    //qDebug() << dasbh.sig();


    Block b{};
    b.mutable_signedhead()->CopyFrom(sbh);
    b.add_signed_transactions()->CopyFrom(sst);
    b.add_signed_transactions()->CopyFrom(st);

#ifdef JAYHACK
    QString genesis2014DataFile = Platform::instance()->settings()->getSetting(AppSettings::GenesisTransition2014Location).toString();
    Reader<Transaction> readertx(genesis2014DataFile.toStdString());
    Transaction tx;
    while ( readertx.ReadNext(tx) ) {
//        qDebug() << tx.DebugString().data();
        SignedTransaction st;
        st.mutable_trans()->CopyFrom(tx);
        b.add_signed_transactions()->CopyFrom(st);
    }
#endif

   // b.add_signed_transactions()->CopyFrom(dasn);

   // Writer<Block> writer{ GET_ROOT_DIR() + "FantasyBit-Genesis-1-block.data", ios::app };
    //writer(b);
    //writer(b1);

    return b;

}

bool Commissioner::BootStrapFileExists(string genesiskey) {
    string prefix = "boot2strap";
//    if ( stoi(genesiskey) < 201607 )
//        prefix += "test";
    QString filename = string(prefix + genesiskey + ".out").data();
    QString genesisBootFile = Platform::instance()->settings()->getSetting(AppSettings::GenesisBootLocation2016).toString();
    genesisBootFile = genesisBootFile +  filename;
    QFileInfo check_file(genesisBootFile);
    return check_file.exists();
}

Bootstrap Commissioner::makeGenesisBoot(LdbWriter &ldb, string genesiskey) {
    Bootstrap head;
    string headhash;

    string prefix = "boot2strap";
//    if ( stoi(genesiskey) < 201607 )
//        prefix += "test";

    QString filename = string(prefix + genesiskey + ".out").data();
    QString genesisBootFile = Platform::instance()->settings()->getSetting(AppSettings::GenesisBootLocation2016).toString();
    genesisBootFile = genesisBootFile +  filename;
    QFileInfo check_file(genesisBootFile);
    if ( !check_file.exists() ) {
        qDebug() << "! check_file.exists() genesisBootFile" << genesisBootFile;

        QString links("http://protoblock.com");
        QString route(filename);

        QMap<QString,QString>  headers;
        QMap<QString,QVariant> params;


        QUrl url;
        url.setUrl(links);
        qDebug() << " chec url " << url.toString() << route << "|";
        RestfullClient rest (url);
        rest.getData(route);
        auto resp = rest.lastReply();
        if ( rest.lastCode() > 400 || resp.size() < 100 ) {
            qDebug() << rest.lastCode() << "did not got it url genesisBootFile" << filename;
            return head;
        }

//        qDebug() << "not got it url resp.size()" << resp.size() << resp;
//        return head;

        qDebug() << " got it url genesisBootFile" << filename;
        QFile *m_file = new QFile();
        m_file->setFileName(genesisBootFile);
        m_file->open(QIODevice::WriteOnly);

        m_file->write(resp);
        m_file->close(); //

    }
    check_file.refresh();
    if ( !check_file.exists() ) {
        qDebug() << " after !check_file.exists() genesisBootFile" << genesisBootFile;
        return head;
    }

    qDebug() << " reading genesisBootFile" << genesisBootFile;
    Reader<KeyValue> reader{genesisBootFile.toStdString()};
    qDebug() << "makeGenesisBoot good?" << reader.good() ;
    if ( !reader.good() )
        return head;
    KeyValue kv;
    while ( reader.ReadNext(kv)) {
        if ( kv.key() == genesiskey ) {
            headhash = kv.value();
        }
        string wrote = ldb.write(kv);
        if ( wrote != kv.key() || wrote == "" )
            qDebug() << " error writing bootstrap" << kv.DebugString().data();
    }

    qDebug() << kv.DebugString().data();


    if ( headhash != "" )
        ldb.read(headhash,head);

    return head;
}

#endif

Transaction Commissioner::GenesisTransition()
{
    DataTransition dt{};

    dt.set_type(TrType::HEARTBEAT);
    dt.set_season(2015);
    dt.set_week(1);

/*
    PlayerLoaderTR pltr{};
    auto players = pltr.loadPlayersFromTradeRadar(true);

    Data d{};

    d.Clear();
    d.set_type(Data::PLAYER);
    //PlayerData pd{};
    for ( auto pd : players ) {
        //TODOpd.set_teamid(t.second);
        d.MutableExtension(PlayerData::player_data)->CopyFrom(pd);
        Data *d2 = dt.add_data();
        d2->CopyFrom(d);
    }

    ScheduleLoader sl{};
    auto weeks = sl.loadScheduleFromJsonFile();

    for ( auto sd : weeks ) {
        d.Clear();
        d.set_type(Data::SCHEDULE);

        d.MutableExtension(ScheduleData::schedule_data)->CopyFrom(sd);
        Data *d2 = dt.add_data();
        d2->CopyFrom(d);
    }
*/
    Transaction trans{};
    trans.set_version(Commissioner::GENESIS_NUM);
    trans.set_type(TransType::DATA);
    //trans.MutableExtension(DataTransition::data_trans)->CopyFrom(dt);

    return trans;


    //PlayerLoader pl{};

    //auto players = pl.loadPlayersFromJsonFile();

}


/*
    uint32_t sz = sizeof(genesis_data);

    b.ParseFromArray(genesis_data,sizeof(genesis_data));

    return b;

    b.ParseFromArray(genesis_data,sizeof(genesis_data));

    return b;
*/



    /*
    FantasyAgent agent;
    agent.beDataAgent();

    auto gtrans = Commissioner::makeGenesisName(agent);
    auto gtransition = Commissioner::makeGenesisTransition();

    auto bh = GenesisBlockHeader();

    SignedBlockHeader sbh{};
    sbh.mutable_head()->CopyFrom(bh);

    auto p = agent.getIdSig(sbh.head().SerializeAsString());
    //sbh.set_sig("iKZWEJXwTRmfX8HYEAFfohcsArUk2a76faYE6jSMtVVKhzPXk89QC24eDuigNj6b1WcX28JHiFhWkEBm79akAZ7Q1UvnKdh5z7");

    //auto genesisblockhash = p.first;  "4f15942fc7cc2e418ee6d3f6bb4f0ef34d49863b21458d7445110783c8dcfd99"

    sbh.set_sig(p.second);
    b.mutable_signedhead()->CopyFrom(sbh);
    b.add_signed_transactions()->CopyFrom(gtransition);
    b.add_signed_transactions()->CopyFrom(gtrans);


*/


/*
    Writer<Block> reader{GET_ROOT_DIR() + "genesis2015Prod.out"};
    if ( !reader.good() )
        qCritical() << " cant write genesis ";
    else
        reader(b);
*/


    /*
    d.Clear();
    d.set_type(Data::SCHEDULE);
    ScheduleData sd{};
    WeeklySchedule ws{};
    sd.set_week(3);

    GameInfo gi{};
    gi.set_id("201500320");
    gi.set_home("NO");
    gi.set_away("HOU");
    gi.set_time(1440964800); //8/30  4:pm edt
    ws.add_games()->CopyFrom(gi);

    gi.Clear();
    gi.set_id("201500323");
    gi.set_home("ARZ");
    gi.set_away("OAK");
    gi.set_time(1440979200); //8/30  4:pm edt
    ws.add_games()->CopyFrom(gi);

    sd.mutable_weekly()->CopyFrom(ws);

*/

//const fc::bigint& max224()
//{
//    static fc::bigint m = [](){
//        char data[224/8+1];
//        memset( data, 0xff, sizeof(data) );
//        data[0] = 0;
//        return fc::bigint(data,sizeof(data));
//    }();
//    return m;
//}

//uint64_t difficulty( const fc::sha224& hash_value )
//{
//    if( hash_value == fc::sha224() ) return uint64_t(-1); // div by 0

//    auto dif = max224() / fc::bigint( (char*)&hash_value, sizeof(hash_value) );
//    int64_t tmp = dif.to_int64();
//    // possible if hash_value starts with 1
//    if( tmp < 0 ) tmp = 0;
//    return tmp;
//}

pb::public_key_data Commissioner::MASTER_PUB_KEY {

#ifndef PRODFOOTBALL
    Commissioner::str2pk(std::string("25dTUQHwaPHdN2fXjpryz5jrrXxU6NNfKgrpJRA4VheJ4"))
#else
    Commissioner::str2pk(std::string("qCxpbzdgBAMGiLYkcs1KhsMH2gXbTP27NJWV8eAgh4j9"))
#endif

};

pb::public_key_data Commissioner::GENESIS_PUB_KEY
{
#ifndef PRODFOOTBALL
      Commissioner::str2pk(std::string("mT1M2MeDjA1RsWkwT7cjE6bbjprcNi84cWyWNvWU1iBa"))
#else
      Commissioner::str2pk(std::string("25dTUQHwaPHdN2fXjpryz5jrrXxU6NNfKgrpJRA4VheJ4"))
#endif
};



std::unordered_map<pb::public_key_data,std::shared_ptr<FantasyName>> Commissioner::FantasyNames{};
std::map<hash_t,pb::public_key_data> Commissioner::Hash2Pk{};
//SignedBlock Commissioner::GenesisBlock{};
//Commissioner::makeGenesisBlock();

std::recursive_mutex Commissioner::name_mutex{};

decltype(Commissioner::GENESIS_NFL_TEAMS) Commissioner::GENESIS_NFL_TEAMS {
    "ARI" ,
    "ATL" ,
    "BAL" ,
    "BUF" ,
    "CAR" ,
    "CHI" ,
    "CIN" ,
    "CLE" ,
    "DAL" ,
    "DEN" ,
    "DET" ,
    "GB" ,
    "HOU" ,
    "IND" ,
    "JAC" ,
    "KC" ,
    "MIA" ,
    "MIN" ,
    "NE" ,
    "NO" ,
    "NYG" ,
    "NYJ" ,
    "OAK" ,
    "PHI" ,
    "PIT" ,
    "LAC" ,
    "SEA" ,
    "SF" ,
    "LAR" ,
    "TB" ,
    "TEN" ,
    "WAS"
};

}

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
    trans.set_version(Commissioner::TRANS_VERSION);
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
    Reader<Transaction> readertx("D:\\data\\Transition2014.out");
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
    string prefix = "bootstrap";
    if ( stoi(genesiskey) < 201607 )
        prefix += "test";
    QString filename = string(prefix + genesiskey + ".out").data();
    QString genesisBootFile = Platform::instance()->settings()->getSetting(AppSettings::GenesisBootLocation2016).toString();
    genesisBootFile = genesisBootFile +  filename;
    QFileInfo check_file(genesisBootFile);
    return check_file.exists();
}

Bootstrap Commissioner::makeGenesisBoot(LdbWriter &ldb, string genesiskey) {
    Bootstrap head;
    string headhash;

    string prefix = "bootstrap";
    if ( stoi(genesiskey) < 201607 )
        prefix += "test";
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
    trans.set_version(Commissioner::TRANS_VERSION);
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
    "SD" ,
    "SEA" ,
    "SF" ,
    "LA" ,
    "TB" ,
    "TEN" ,
    "WAS"
};
decltype(Commissioner::GENESIS_NFL_PLAYERS) Commissioner::GENESIS_NFL_PLAYERS{
    { "1" , "ARI" },
    { "2" , "ATL" },
    { "36" , "BAL" },
    { "37" , "BUF" },
    { "5" , "CAR" },
    { "38" , "CHI" },
    { "7" , "CIN" },
    { "8" , "CLE" },
    { "48" , "DAL" },
    { "49" , "DEN" },
    { "11" , "DET" },
    { "12" , "GB" },
    { "13" , "HOU" },
    { "14" , "IND" },
    { "52" , "JAX" },
    { "55" , "KC" },
    { "64" , "MIA" },
    { "67" , "MIN" },
    { "21" , "NE" },
    { "69" , "NO" },
    { "75" , "NYG" },
    { "82" , "NYJ" },
    { "83" , "OAK" },
    { "26" , "PHI" },
    { "28" , "PIT" },
    { "84" , "SD" },
    { "90" , "SEA" },
    { "93" , "SF" },
    { "32" , "STL" },
    { "95" , "TB" },
    { "34" , "TEN" },
    { "97" , "WAS" },
    { "2428" , "ARI" },
    { "4737" , "ARI" },
    { "16656" , "ARI" },
    { "16644" , "ARI" },
    { "14002" , "ARI" },
    { "15725" , "ARI" },
    { "5571" , "ARI" },
    { "16640" , "ARI" },
    { "15509" , "ARI" },
    { "14847" , "ARI" },
    { "14478" , "ARI" },
    { "16341" , "ARI" },
    { "15230" , "ARI" },
    { "16847" , "ARI" },
    { "15010" , "ARI" },
    { "15224" , "ARI" },
    { "14752" , "ARI" },
    { "17015" , "ARI" },
    { "16406" , "ARI" },
    { "15247" , "ARI" },
    { "16648" , "ARI" },
    { "15285" , "ARI" },
    { "732" , "ATL" },
    { "3388" , "ATL" },
    { "13291" , "ATL" },
    { "15182" , "ATL" },
    { "15268" , "ATL" },
    { "12956" , "ATL" },
    { "3601" , "ATL" },
    { "16868" , "ATL" },
    { "7082" , "ATL" },
    { "16529" , "ATL" },
    { "13284" , "ATL" },
    { "16524" , "ATL" },
    { "16834" , "ATL" },
    { "16541" , "ATL" },
    { "11439" , "ATL" },
    { "14784" , "ATL" },
    { "14786" , "ATL" },
    { "15378" , "ATL" },
    { "12326" , "ATL" },
    { "11221" , "ATL" },
    { "204" , "ATL" },
    { "852" , "ATL" },
    { "13162" , "ATL" },
    { "16536" , "ATL" },
    { "611" , "BAL" },
    { "17018" , "BAL" },
    { "4633" , "BAL" },
    { "14688" , "BAL" },
    { "16282" , "BAL" },
    { "12722" , "BAL" },
    { "16964" , "BAL" },
    { "15479" , "BAL" },
    { "16081" , "BAL" },
    { "13171" , "BAL" },
    { "16080" , "BAL" },
    { "16787" , "BAL" },
    { "9369" , "BAL" },
    { "17016" , "BAL" },
    { "16099" , "BAL" },
    { "16886" , "BAL" },
    { "16696" , "BAL" },
    { "16699" , "BAL" },
    { "14901" , "BAL" },
    { "16083" , "BAL" },
    { "16931" , "BAL" },
    { "16816" , "BAL" },
    { "13273" , "BAL" },
    { "13449" , "BAL" },
    { "11017" , "BAL" },
    { "4358" , "BAL" },
    { "1269" , "BUF" },
    { "12831" , "BUF" },
    { "14868" , "BUF" },
    { "15381" , "BUF" },
    { "14871" , "BUF" },
    { "13800" , "BUF" },
    { "9079" , "BUF" },
    { "16993" , "BUF" },
    { "16003" , "BUF" },
    { "14828" , "BUF" },
    { "14968" , "BUF" },
    { "8723" , "BUF" },
    { "5701" , "BUF" },
    { "11932" , "BUF" },
    { "12285" , "BUF" },
    { "13719" , "BUF" },
    { "10991" , "BUF" },
    { "16915" , "BUF" },
    { "733" , "BUF" },
    { "12751" , "BUF" },
    { "13856" , "BUF" },
    { "13486" , "BUF" },
    { "16954" , "BUF" },
    { "15994" , "BUF" },
    { "15371" , "BUF" },
    { "15447" , "BUF" },
    { "14865" , "BUF" },
    { "14866" , "BUF" },
    { "13320" , "CAR" },
    { "12083" , "CAR" },
    { "10333" , "CAR" },
    { "14275" , "CAR" },
    { "15714" , "CAR" },
    { "14817" , "CAR" },
    { "16550" , "CAR" },
    { "11422" , "CAR" },
    { "16553" , "CAR" },
    { "16802" , "CAR" },
    { "12109" , "CAR" },
    { "5451" , "CAR" },
    { "13102" , "CAR" },
    { "16934" , "CAR" },
    { "4738" , "CAR" },
    { "16561" , "CAR" },
    { "15616" , "CAR" },
    { "13653" , "CAR" },
    { "4104" , "CAR" },
    { "16559" , "CAR" },
    { "16741" , "CAR" },
    { "12153" , "CAR" },
    { "15561" , "CAR" },
    { "13817" , "CAR" },
    { "1658" , "CAR" },
    { "8972" , "CHI" },
    { "12752" , "CHI" },
    { "11447" , "CHI" },
    { "1410" , "CHI" },
    { "15118" , "CHI" },
    { "14795" , "CHI" },
    { "7389" , "CHI" },
    { "16430" , "CHI" },
    { "16768" , "CHI" },
    { "16416" , "CHI" },
    { "14187" , "CHI" },
    { "2699" , "CHI" },
    { "16427" , "CHI" },
    { "13299" , "CHI" },
    { "16441" , "CHI" },
    { "16867" , "CHI" },
    { "11084" , "CHI" },
    { "15281" , "CHI" },
    { "1181" , "CHI" },
    { "15962" , "CHI" },
    { "16460" , "CHI" },
    { "12415" , "CHI" },
    { "10291" , "CHI" },
    { "2332" , "CHI" },
    { "6489" , "CIN" },
    { "11735" , "CIN" },
    { "13693" , "CIN" },
    { "16116" , "CIN" },
    { "13878" , "CIN" },
    { "12841" , "CIN" },
    { "16997" , "CIN" },
    { "16347" , "CIN" },
    { "13069" , "CIN" },
    { "12845" , "CIN" },
    { "8373" , "CIN" },
    { "14916" , "CIN" },
    { "11737" , "CIN" },
    { "16112" , "CIN" },
    { "14917" , "CIN" },
    { "16703" , "CIN" },
    { "16846" , "CIN" },
    { "16325" , "CIN" },
    { "13870" , "CIN" },
    { "14918" , "CIN" },
    { "16122" , "CIN" },
    { "14920" , "CIN" },
    { "16917" , "CIN" },
    { "16111" , "CIN" },
    { "15979" , "CLE" },
    { "16135" , "CLE" },
    { "16139" , "CLE" },
    { "13697" , "CLE" },
    { "13887" , "CLE" },
    { "14587" , "CLE" },
    { "5282" , "CLE" },
    { "13282" , "CLE" },
    { "10961" , "CLE" },
    { "12847" , "CLE" },
    { "16129" , "CLE" },
    { "16142" , "CLE" },
    { "13158" , "CLE" },
    { "16838" , "CLE" },
    { "16127" , "CLE" },
    { "16740" , "CLE" },
    { "16955" , "CLE" },
    { "17021" , "CLE" },
    { "11555" , "CLE" },
    { "8330" , "CLE" },
    { "7203" , "CLE" },
    { "4577" , "CLE" },
    { "15350" , "CLE" },
    { "13406" , "CLE" },
    { "16884" , "CLE" },
    { "16958" , "CLE" },
    { "13910" , "DAL" },
    { "13109" , "DAL" },
    { "3867" , "DAL" },
    { "16382" , "DAL" },
    { "14141" , "DAL" },
    { "16357" , "DAL" },
    { "16381" , "DAL" },
    { "14954" , "DAL" },
    { "8066" , "DAL" },
    { "15085" , "DAL" },
    { "14731" , "DAL" },
    { "13425" , "DAL" },
    { "13527" , "DAL" },
    { "16123" , "DAL" },
    { "722" , "DAL" },
    { "15088" , "DAL" },
    { "14152" , "DAL" },
    { "17005" , "DAL" },
    { "11270" , "DAL" },
    { "15082" , "DAL" },
    { "10092" , "DEN" },
    { "15037" , "DEN" },
    { "17009" , "DEN" },
    { "14996" , "DEN" },
    { "11063" , "DEN" },
    { "12551" , "DEN" },
    { "3989" , "DEN" },
    { "14720" , "DEN" },
    { "16278" , "DEN" },
    { "16276" , "DEN" },
    { "14053" , "DEN" },
    { "7328" , "DEN" },
    { "11594" , "DEN" },
    { "16274" , "DEN" },
    { "15020" , "DEN" },
    { "14046" , "DEN" },
    { "14756" , "DEN" },
    { "15036" , "DEN" },
    { "16273" , "DEN" },
    { "16287" , "DEN" },
    { "1810" , "DEN" },
    { "16853" , "DEN" },
    { "13016" , "DEN" },
    { "8569" , "DEN" },
    { "16606" , "DEN" },
    { "14593" , "DEN" },
    { "16254" , "DEN" },
    { "11197" , "DEN" },
    { "16462" , "DET" },
    { "549" , "DET" },
    { "8590" , "DET" },
    { "9038" , "DET" },
    { "15123" , "DET" },
    { "14787" , "DET" },
    { "16456" , "DET" },
    { "16509" , "DET" },
    { "11611" , "DET" },
    { "6768" , "DET" },
    { "17024" , "DET" },
    { "13394" , "DET" },
    { "14219" , "DET" },
    { "15126" , "DET" },
    { "12323" , "DET" },
    { "15497" , "DET" },
    { "16815" , "DET" },
    { "14811" , "DET" },
    { "16928" , "DET" },
    { "17023" , "DET" },
    { "15129" , "DET" },
    { "15336" , "DET" },
    { "6029" , "DET" },
    { "16466" , "DET" },
    { "14203" , "DET" },
    { "16451" , "DET" },
    { "9588" , "DET" },
    { "1694" , "GB" },
    { "15695" , "GB" },
    { "16907" , "GB" },
    { "2593" , "GB" },
    { "13103" , "GB" },
    { "16470" , "GB" },
    { "13227" , "GB" },
    { "15816" , "GB" },
    { "16966" , "GB" },
    { "15138" , "GB" },
    { "1589" , "GB" },
    { "16485" , "GB" },
    { "17026" , "GB" },
    { "12506" , "GB" },
    { "16487" , "GB" },
    { "11400" , "GB" },
    { "16480" , "GB" },
    { "16469" , "GB" },
    { "16973" , "GB" },
    { "4556" , "GB" },
    { "16855" , "GB" },
    { "16491" , "GB" },
    { "16211" , "HOU" },
    { "13961" , "HOU" },
    { "8358" , "HOU" },
    { "14986" , "HOU" },
    { "16831" , "HOU" },
    { "16935" , "HOU" },
    { "13729" , "HOU" },
    { "12771" , "HOU" },
    { "14984" , "HOU" },
    { "16082" , "HOU" },
    { "15491" , "HOU" },
    { "16204" , "HOU" },
    { "11792" , "HOU" },
    { "16189" , "HOU" },
    { "13735" , "HOU" },
    { "16994" , "HOU" },
    { "14719" , "HOU" },
    { "16210" , "HOU" },
    { "16196" , "HOU" },
    { "13974" , "HOU" },
    { "13002" , "HOU" },
    { "14985" , "HOU" },
    { "8564" , "HOU" },
    { "16200" , "HOU" },
    { "11101" , "HOU" },
    { "3258" , "IND" },
    { "1034" , "IND" },
    { "17028" , "IND" },
    { "16232" , "IND" },
    { "14008" , "IND" },
    { "14005" , "IND" },
    { "4291" , "IND" },
    { "16790" , "IND" },
    { "14594" , "IND" },
    { "13078" , "IND" },
    { "5820" , "IND" },
    { "13991" , "IND" },
    { "16965" , "IND" },
    { "14788" , "IND" },
    { "16240" , "IND" },
    { "17030" , "IND" },
    { "17029" , "IND" },
    { "2429" , "IND" },
    { "13997" , "IND" },
    { "13987" , "IND" },
    { "15602" , "IND" },
    { "16227" , "IND" },
    { "16238" , "IND" },
    { "16245" , "JAX" },
    { "16261" , "JAX" },
    { "2405" , "JAX" },
    { "17033" , "JAX" },
    { "6902" , "JAX" },
    { "16258" , "JAX" },
    { "16899" , "JAX" },
    { "14019" , "JAX" },
    { "16263" , "JAX" },
    { "15018" , "JAX" },
    { "12816" , "JAX" },
    { "11501" , "JAX" },
    { "15019" , "JAX" },
    { "13595" , "JAX" },
    { "11413" , "JAX" },
    { "7308" , "JAX" },
    { "16797" , "JAX" },
    { "14687" , "JAX" },
    { "16255" , "JAX" },
    { "16988" , "JAX" },
    { "16576" , "JAX" },
    { "13027" , "JAX" },
    { "16242" , "JAX" },
    { "16250" , "JAX" },
    { "11317" , "JAX" },
    { "16979" , "JAX" },
    { "16253" , "JAX" },
    { "3061" , "JAX" },
    { "16992" , "KC" },
    { "16301" , "KC" },
    { "16300" , "KC" },
    { "15293" , "KC" },
    { "9902" , "KC" },
    { "6739" , "KC" },
    { "16308" , "KC" },
    { "16305" , "KC" },
    { "12978" , "KC" },
    { "17034" , "KC" },
    { "15470" , "KC" },
    { "16837" , "KC" },
    { "8914" , "KC" },
    { "7969" , "KC" },
    { "14070" , "KC" },
    { "15045" , "KC" },
    { "16307" , "KC" },
    { "13418" , "KC" },
    { "15261" , "KC" },
    { "16933" , "KC" },
    { "1986" , "KC" },
    { "16712" , "KC" },
    { "15764" , "KC" },
    { "15305" , "KC" },
    { "15296" , "KC" },
    { "15048" , "KC" },
    { "14836" , "KC" },
    { "13059" , "KC" },
    { "9226" , "MIA" },
    { "5834" , "MIA" },
    { "13429" , "MIA" },
    { "14877" , "MIA" },
    { "16775" , "MIA" },
    { "14806" , "MIA" },
    { "16020" , "MIA" },
    { "6597" , "MIA" },
    { "14471" , "MIA" },
    { "13799" , "MIA" },
    { "13788" , "MIA" },
    { "14872" , "MIA" },
    { "13791" , "MIA" },
    { "14501" , "MIA" },
    { "16909" , "MIA" },
    { "16031" , "MIA" },
    { "16916" , "MIA" },
    { "14876" , "MIA" },
    { "16012" , "MIA" },
    { "15196" , "MIA" },
    { "12861" , "MIA" },
    { "15757" , "MIA" },
    { "16022" , "MIA" },
    { "14716" , "MIA" },
    { "14463" , "MIN" },
    { "11323" , "MIN" },
    { "16497" , "MIN" },
    { "8529" , "MIN" },
    { "16504" , "MIN" },
    { "15141" , "MIN" },
    { "11992" , "MIN" },
    { "16906" , "MIN" },
    { "14465" , "MIN" },
    { "15534" , "MIN" },
    { "14549" , "MIN" },
    { "12985" , "MIN" },
    { "4807" , "MIN" },
    { "16510" , "MIN" },
    { "16518" , "MIN" },
    { "13255" , "MIN" },
    { "15555" , "MIN" },
    { "13275" , "MIN" },
    { "16903" , "MIN" },
    { "15150" , "MIN" },
    { "14445" , "MIN" },
    { "15697" , "MIN" },
    { "4276" , "MIN" },
    { "17038" , "MIN" },
    { "16962" , "NE" },
    { "2982" , "NE" },
    { "16621" , "NE" },
    { "16041" , "NE" },
    { "10034" , "NE" },
    { "8355" , "NE" },
    { "4314" , "NE" },
    { "15813" , "NE" },
    { "14886" , "NE" },
    { "16134" , "NE" },
    { "7651" , "NE" },
    { "11463" , "NE" },
    { "12708" , "NE" },
    { "16056" , "NE" },
    { "12296" , "NE" },
    { "16557" , "NE" },
    { "15565" , "NE" },
    { "13741" , "NE" },
    { "14352" , "NE" },
    { "14783" , "NE" },
    { "11535" , "NE" },
    { "9906" , "NE" },
    { "15367" , "NE" },
    { "14883" , "NE" },
    { "2640" , "NE" },
    { "12548" , "NE" },
    { "15566" , "NE" },
    { "10974" , "NE" },
    { "16412" , "NO" },
    { "15190" , "NO" },
    { "14867" , "NO" },
    { "6276" , "NO" },
    { "7242" , "NO" },
    { "16568" , "NO" },
    { "5094" , "NO" },
    { "13341" , "NO" },
    { "4511" , "NO" },
    { "16567" , "NO" },
    { "16074" , "NO" },
    { "15922" , "NO" },
    { "16836" , "NO" },
    { "5537" , "NO" },
    { "13337" , "NO" },
    { "14821" , "NO" },
    { "10949" , "NO" },
    { "15645" , "NO" },
    { "15191" , "NO" },
    { "12392" , "NO" },
    { "15765" , "NO" },
    { "13861" , "NO" },
    { "16989" , "NO" },
    { "11756" , "NO" },
    { "16141" , "NO" },
    { "14368" , "NO" },
    { "15866" , "NO" },
    { "13048" , "NYG" },
    { "16191" , "NYG" },
    { "2695" , "NYG" },
    { "15093" , "NYG" },
    { "16946" , "NYG" },
    { "4932" , "NYG" },
    { "15254" , "NYG" },
    { "16389" , "NYG" },
    { "13117" , "NYG" },
    { "15834" , "NYG" },
    { "15836" , "NYG" },
    { "8649" , "NYG" },
    { "16008" , "NYG" },
    { "12780" , "NYG" },
    { "16402" , "NYG" },
    { "13141" , "NYG" },
    { "16120" , "NYG" },
    { "12246" , "NYG" },
    { "14244" , "NYG" },
    { "14242" , "NYG" },
    { "11517" , "NYG" },
    { "15782" , "NYG" },
    { "14097" , "NYG" },
    { "7007" , "NYG" },
    { "16657" , "NYG" },
    { "16759" , "NYG" },
    { "11694" , "NYJ" },
    { "15982" , "NYJ" },
    { "14895" , "NYJ" },
    { "16719" , "NYJ" },
    { "16864" , "NYJ" },
    { "13763" , "NYJ" },
    { "13981" , "NYJ" },
    { "12794" , "NYJ" },
    { "14715" , "NYJ" },
    { "8283" , "NYJ" },
    { "14819" , "NYJ" },
    { "11667" , "NYJ" },
    { "16651" , "NYJ" },
    { "6828" , "NYJ" },
    { "12774" , "NYJ" },
    { "12800" , "NYJ" },
    { "15221" , "NYJ" },
    { "12239" , "NYJ" },
    { "14696" , "NYJ" },
    { "14891" , "NYJ" },
    { "15416" , "NYJ" },
    { "16360" , "NYJ" },
    { "16064" , "NYJ" },
    { "16063" , "NYJ" },
    { "16798" , "NYJ" },
    { "10992" , "NYJ" },
    { "11182" , "NYJ" },
    { "8142" , "NYJ" },
    { "16057" , "NYJ" },
    { "16311" , "OAK" },
    { "15758" , "OAK" },
    { "13270" , "OAK" },
    { "17041" , "OAK" },
    { "3253" , "OAK" },
    { "15066" , "OAK" },
    { "15746" , "OAK" },
    { "9331" , "OAK" },
    { "17049" , "OAK" },
    { "13264" , "OAK" },
    { "16765" , "OAK" },
    { "15071" , "OAK" },
    { "13163" , "OAK" },
    { "13902" , "OAK" },
    { "16309" , "OAK" },
    { "3382" , "OAK" },
    { "14789" , "OAK" },
    { "14111" , "OAK" },
    { "15072" , "OAK" },
    { "16327" , "OAK" },
    { "16040" , "OAK" },
    { "16980" , "OAK" },
    { "16326" , "OAK" },
    { "15417" , "OAK" },
    { "12777" , "OAK" },
    { "15056" , "OAK" },
    { "16829" , "OAK" },
    { "16236" , "PHI" },
    { "14855" , "PHI" },
    { "8415" , "PHI" },
    { "18" , "PHI" },
    { "11527" , "PHI" },
    { "15976" , "PHI" },
    { "11196" , "PHI" },
    { "15971" , "PHI" },
    { "11313" , "PHI" },
    { "11243" , "PHI" },
    { "16781" , "PHI" },
    { "3330" , "PHI" },
    { "11256" , "PHI" },
    { "13124" , "PHI" },
    { "15185" , "PHI" },
    { "14853" , "PHI" },
    { "6198" , "PHI" },
    { "15965" , "PHI" },
    { "17043" , "PHI" },
    { "15974" , "PHI" },
    { "14856" , "PHI" },
    { "5511" , "PHI" },
    { "12944" , "PHI" },
    { "17042" , "PHI" },
    { "14971" , "PIT" },
    { "16058" , "PIT" },
    { "8769" , "PIT" },
    { "12508" , "PIT" },
    { "3807" , "PIT" },
    { "16146" , "PIT" },
    { "14973" , "PIT" },
    { "16144" , "PIT" },
    { "16848" , "PIT" },
    { "14967" , "PIT" },
    { "7580" , "PIT" },
    { "16758" , "PIT" },
    { "14620" , "PIT" },
    { "16920" , "PIT" },
    { "7635" , "PIT" },
    { "11056" , "PIT" },
    { "14641" , "PIT" },
    { "8771" , "PIT" },
    { "16145" , "PIT" },
    { "6293" , "PIT" },
    { "17045" , "SD" },
    { "17046" , "SD" },
    { "17047" , "SD" },
    { "17048" , "SD" },
    { "16333" , "SD" },
    { "15078" , "SD" },
    { "16343" , "SD" },
    { "4354" , "SD" },
    { "5054" , "SD" },
    { "5100" , "SD" },
    { "2950" , "SD" },
    { "15076" , "SD" },
    { "16728" , "SD" },
    { "8244" , "SD" },
    { "13391" , "SD" },
    { "16776" , "SD" },
    { "8604" , "SD" },
    { "12317" , "SD" },
    { "16345" , "SD" },
    { "7711" , "SD" },
    { "13929" , "SD" },
    { "8848" , "SD" },
    { "7884" , "SD" },
    { "14768" , "SD" },
    { "8508" , "SD" },
    { "14122" , "SD" },
    { "14536" , "SEA" },
    { "12594" , "SEA" },
    { "15235" , "SEA" },
    { "14759" , "SEA" },
    { "16694" , "SEA" },
    { "16731" , "SEA" },
    { "14749" , "SEA" },
    { "16830" , "SEA" },
    { "16734" , "SEA" },
    { "14533" , "SEA" },
    { "12386" , "SEA" },
    { "16732" , "SEA" },
    { "15256" , "SEA" },
    { "17050" , "SEA" },
    { "15631" , "SEA" },
    { "12649" , "SEA" },
    { "15772" , "SEA" },
    { "11488" , "SEA" },
    { "16689" , "SEA" },
    { "15263" , "SEA" },
    { "13434" , "SEA" },
    { "13471" , "SEA" },
    { "15630" , "SEA" },
    { "16679" , "SEA" },
    { "11603" , "SEA" },
    { "13460" , "SEA" },
    { "12982" , "SF" },
    { "17052" , "SF" },
    { "13443" , "SF" },
    { "5714" , "SF" },
    { "16664" , "SF" },
    { "15241" , "SF" },
    { "17051" , "SF" },
    { "16893" , "SF" },
    { "15550" , "SF" },
    { "16887" , "SF" },
    { "4640" , "SF" },
    { "13130" , "SF" },
    { "16668" , "SF" },
    { "13440" , "SF" },
    { "16673" , "SF" },
    { "15467" , "SF" },
    { "16663" , "SF" },
    { "15640" , "SF" },
    { "17013" , "SF" },
    { "13446" , "SF" },
    { "11712" , "SF" },
    { "7857" , "SF" },
    { "12830" , "SF" },
    { "16878" , "SF" },
    { "5084" , "SF" },
    { "14496" , "SF" },
    { "15239" , "SF" },
    { "16395" , "SF" },
    { "14697" , "STL" },
    { "14404" , "STL" },
    { "13723" , "STL" },
    { "11097" , "STL" },
    { "15215" , "STL" },
    { "15216" , "STL" },
    { "14405" , "STL" },
    { "16850" , "STL" },
    { "15270" , "STL" },
    { "15694" , "STL" },
    { "14087" , "STL" },
    { "14421" , "STL" },
    { "16626" , "STL" },
    { "16771" , "STL" },
    { "15596" , "STL" },
    { "15207" , "STL" },
    { "17053" , "STL" },
    { "16636" , "STL" },
    { "14764" , "STL" },
    { "14807" , "STL" },
    { "15005" , "STL" },
    { "16961" , "STL" },
    { "8532" , "STL" },
    { "16614" , "STL" },
    { "14424" , "STL" },
    { "13386" , "STL" },
    { "8534" , "STL" },
    { "16762" , "TB" },
    { "16228" , "TB" },
    { "16603" , "TB" },
    { "15201" , "TB" },
    { "16597" , "TB" },
    { "16598" , "TB" },
    { "16944" , "TB" },
    { "8777" , "TB" },
    { "14385" , "TB" },
    { "15202" , "TB" },
    { "16990" , "TB" },
    { "16609" , "TB" },
    { "13852" , "TB" },
    { "14538" , "TB" },
    { "14193" , "TB" },
    { "16922" , "TB" },
    { "8778" , "TB" },
    { "3519" , "TB" },
    { "15038" , "TB" },
    { "16593" , "TB" },
    { "16608" , "TB" },
    { "13370" , "TB" },
    { "14850" , "TB" },
    { "14657" , "TEN" },
    { "16175" , "TEN" },
    { "16763" , "TEN" },
    { "8750" , "TEN" },
    { "12754" , "TEN" },
    { "12137" , "TEN" },
    { "13957" , "TEN" },
    { "9739" , "TEN" },
    { "14978" , "TEN" },
    { "17004" , "TEN" },
    { "16801" , "TEN" },
    { "11230" , "TEN" },
    { "16178" , "TEN" },
    { "11220" , "TEN" },
    { "8399" , "TEN" },
    { "16163" , "TEN" },
    { "11100" , "TEN" },
    { "16898" , "TEN" },
    { "16869" , "TEN" },
    { "1545" , "TEN" },
    { "16180" , "TEN" },
    { "7175" , "TEN" },
    { "6211" , "TEN" },
    { "15303" , "TEN" },
    { "13881" , "TEN" },
    { "3" , "TEN" },
    { "14740" , "WAS" },
    { "16947" , "WAS" },
    { "14252" , "WAS" },
    { "14257" , "WAS" },
    { "3943" , "WAS" },
    { "11565" , "WAS" },
    { "15659" , "WAS" },
    { "16411" , "WAS" },
    { "16115" , "WAS" },
    { "11047" , "WAS" },
    { "15314" , "WAS" },
    { "15102" , "WAS" },
    { "16856" , "WAS" },
    { "16421" , "WAS" },
    { "11969" , "WAS" },
    { "14269" , "WAS" },
    { "16866" , "WAS" },
    { "11345" , "WAS" },
    { "13170" , "WAS" },
    { "15100" , "WAS" },
    { "16574" , "WAS" },
    { "6767" , "WAS" },
};
decltype(Commissioner::STATS_ID) Commissioner::STATS_ID {
"5228",
"24171",
"8780",
"7200",
"4256",
"5479",
"6770",
"6624",
"25718",
"25711",
"6760",
"24793",
"6763",
"25785",
"6337",
"24823",
"23976",
"7760",
"24788",
"27540",
"8795",
"28390",
"23997",
"27560",
"9265",
"8821",
"27531",
"28389",
"24851",
"7426",
"8261",
"23999",
"7177",
"25812",
"24822",
"26684",
"8256",
"25755",
"24967",
"27564",
"24017",
"8850",
"27548",
"9496",
"24791",
"26878",
"9547",
"24057",
"25798",
"9317",
"27583",
"27535",
"9010",
"26699",
"25807",
"8266",
"24858",
"26810",
"27570",
"25773",
"26650",
"25802",
"24815",
"27591",
"28392",
"27589",
"7868",
"7241",
"9283",
"8826",
"24035",
"5521",
"27585",
"26702",
"7203",
"27619",
"6390",
"8982",
"27532",
"24400",
"24062",
"6762",
"6339",
"8838",
"8001",
"5967",
"8790",
"7237",
"28408",
"26660",
"27603",
"28461",
"26804",
"26686",
"27709",
"25723",
"7206",
"25730",
"27581",
"24845",
"24070",
"8285",
"26839",
"8819",
"28442",
"28014",
"7776",
"25741",
"6405",
"25883",
"9560",
"25105",
"7801",
"24936",
"7149",
"26774",
"28424",
"28403",
"9001",
"9274",
"25703",
"7924",
"9348",
"24262",
"27631",
"24843",
"9514",
"28465",
"9037",
"25743",
"27921",
"26708",
"26697",
"27874",
"25379",
"26767",
"24889",
"7492",
"27584",
"25820",
"7847",
"8781",
"26644",
"9286",
"27582",
"26060",
"25991",
"24961",
"8861",
"24830",
"27627",
"26389",
"27167",
"27938",
"9294",
"7755",
"27566",
"24866",
"26658",
"7306",
"9353",
"24901",
"8916",
"27538",
"25876",
"24373",
"7751",
"27626",
"26",
"27597",
"29",
"8332",
"25",
"6791",
"26064",
"15",
"9372",
"8277",
"26859",
"28474",
"26807",
"28483",
"25744",
"6867",
"24774",
"23987",
"24011",
"10",
"27422",
"24063",
"27911",
"6558",
"5",
"28417",
"28428",
"7867",
"26682",
"27756",
"28513",
"26758",
"8263",
"28685",
"24913",
"28618",
"24303",
"28046",
"26488",
"6896",
"9066",
"27120",
"8416",
"27737",
"25774",
"26664",
"27",
"27567",
"6243",
"21",
"25777",
"28103",
"24892",
"28457",
"8801",
"28227",
"8447",
"9271",
"9",
"4269",
"7777",
"26631",
"2",
"26534",
"26950",
"27658",
"28429",
"25885",
"25427",
"28907",
"9032",
"25871",
"22",
"9520",
"25881",
"8",
"7520",
"25648",
"30",
"9526",
"7223",
"16",
"14",
"20",
"3727",
"28",
"8432",
"4",
"27641",
"28402",
"28482",
"25793",
"9388",
"27958",
"26719",
"11",
"24940",
"27625",
"34",
"26428",
"1",
"8383",
"28378",
"27378",
"5046",
"27624",
"8565",
"17",
"27634",
"25828",
"28878",
"9043",
"28527",
"24093",
"24834",
"7",
"28026",
"27652",
"8063",
"28593",
"24169",
"24135",
"19",
"24",
"28115",
"26657",
"27746",
"25706",
"25234",
"6",
"26822",
"23",
"28443",
"24856",
"27618",
"28493",
"12",
"26416",
"26729",
"3",
"18",
"33",
"26006",
"26670",
"26756",
"28543",
"13",
"26612",
"24100",
"28495",
"26751",
"24991",
"28234",
"26794",
"23996",
"24026",
"24130",
"26715",
"25594",
"26753",
"28456",
"24912",
"24045",
"24932",
"26652",
"28985",
"27946",
"28272",
"25238",
"28562",
"27593",
"26783",
"8028",
"25771",
"28464",
"27832",
"9158",
"28458",
"7806",
"26455",
"27614",
"9416",
"28494",
"8926",
"25880",
"27299",
"25937",
"9505",
"25120",
"26781",
"8907",
"27074",
"27050",
"24998",
"28561",
"28531",
"9466",
"28475",
"26813",
"26824",
"7858",
"24970",
"26678",
"24149",
"26777",
"24208",
"26601",
"9666",
"27213",
"7695",
"24108",
"8935",
"28574",
"27580",
"9444",
"28592",
"26108",
"28547",
"27622",
"8021",
"24946",
"28832",
"7802",
"8862",
"28473",
"24923",
"25178",
"27055",
"27277",
"24963",
"28714",
"25158",
"26614",
"25106",
"26347",
"28953",
"27764",
"27651",
"25125",
"26763",
"26548",
"9284",
"26596",
"28044",
"25650",
"8912",
"25481",
"24891",
"28496",
"26024",
"28021",
"28548",
"8331",
"9705",
"8874",
"28514",
"28559",
"26838",
"25896",
"8327",
"27674",
"25681",
"28379",
"27670",
"7544",
"27927",
"25838",
"26832",
"24436",
"24798",
"25939",
"26253",
"0",
"28534",
"25810",
"28563",
"25806",
"26299",
"28583",
"28926",
"28752",
"28556",
"9291",
"8609",
"28348",
"27739",
"0",
"28601",
"27789",
"29024",
"26218",
"24198",
"24435",
"27717",
"27174",
"26000",
"26812",
"28545",
"28887",
"27135",
"27103",
"25831",
"28626",
"25760",
"26518",
"9347",
"9149",
"29034",
"26701",
"24082",
"25767",
"9269",
"24797",
"7389",
"0",
"5448",
"24023",
"28847",
"25171",
"4416",
"25732",
"8297",
"7798",
"6865",
"27590",
"8544",
"26872",
"7321",
"26696",
"26733",
"6849",
"26805",
"9449",
"7813",
"27429",
"24060",
"24861",
"6169",
"8834",
"27706",
"7406",
"9678",
"28737",
"28491",
"26483",
"27692",
"27550"
};

}

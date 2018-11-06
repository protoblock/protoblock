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

Block Commissioner::makeGenesisBlock()
{
    Block genesis_block{};
    std::string GENESIS_PUB_KEY_STR = pk2str(GENESIS_PUB_KEY);

    //GenesisTransition
    SignedTransaction &genesis_transition_st = *genesis_block.add_signed_transactions();
    Transaction &genesis_transition_tx = *genesis_transition_st.mutable_trans();
    genesis_transition_tx.set_version(1);
    genesis_transition_tx.set_type(TransType::DATA);

    DataTransition &genesis_transition = *genesis_transition_tx.MutableExtension(DataTransition::data_trans);
    genesis_transition.set_type(TrType::SEASONSTART);
    genesis_transition.set_season(2014);
    genesis_transition.set_week(0);

    Data &data_msg = *genesis_transition.add_data();
    data_msg.set_type(Data_Type_MESSAGE);
    data_msg.MutableExtension(MessageData::message_data)->set_msg(
        "Distributed Engineered Autonomous Agents : Satoshi Fantasy. April 2014. "
        "dcc76458aeaab9ebe5e17abb704f292511ce2f7b8b30336b8f12cf46da2ad71e"
    );

    genesis_transition_st.set_id("c96ca89a0b9b219836f1d9ae7599120236b446dd96196d190034cb5747ac1ef9");
    genesis_transition_st.set_fantasy_name("FantasyAgent");
#ifndef PRODFOOTBALL
    genesis_transition_st.set_sig("J5CdCsogMw2a5ZUH5BLCH58RLtMYTXGwd3aUheFWuvPj63DbKSkvQdDxysZAnLRnwLmah4MSWT2oWaax3hsHYrg");
    genesis_transition_st.set_sig("2ktZ5Um2TVkrUD6iWJqAWPEfCHWzQ9Z6id5rnRnwSHpRe6fT4Ra5GRiY8NcSLGEa1WyfPB5d3GT3XwTi1X7J1sfY");
#else
    genesis_transition_st.set_sig("2ktZ5Um2TVkrUD6iWJqAWPEfCHWzQ9Z6id5rnRnwSHpRe6fT4Ra5GRiY8NcSLGEa1WyfPB5d3GT3XwTi1X7J1sfY");
#endif

    //makeFantasyAgent
    SignedTransaction &agent_st = *genesis_block.add_signed_transactions();
    Transaction &agenttrans = *agent_st.mutable_trans();
    agenttrans.set_version(1);
    agenttrans.set_type(TransType::NAME);

    NameTrans &nametrans = *agenttrans.MutableExtension(NameTrans::name_trans);
    //genesis.name_hash = 10576213825162658308;// FantasyName::name_hash("FantasyAgent");
    nametrans.set_fantasy_name("FantasyAgent");
    nametrans.set_public_key(GENESIS_PUB_KEY_STR);//"25dTUQHwaPHdN2fXjpryz5jrrXxU6NNfKgrpJRA4VheJ4");
    nametrans.set_recovery_key("xXGLATXkcfrk96dnky3USYVMnEw6RpaiRv4XKFZF3kEE");
    nametrans.set_am_agent(true);

    agent_st.set_fantasy_name("FantasyAgent");
#ifndef PRODFOOTBALL
    agent_st.set_id("65399b01d283f15deafc1bf5c86d71e4f67afd26cd69e30d3bf282489c1b769b");
    agent_st.set_sig("59daD7UtP7pGPaU4avZHZcyjCKMwSGL8RMP1m3KtWwiuVkX3XL3bAC8YxyPTpo6d1Fyyu7UmpsaByQXgm8XY1eM2");
    agent_st.set_id("14aaf2551bb80878446ea3cf1638b55ab9092ab9959c82dc76fa6433f230511c");
    agent_st.set_sig("2NA9KRUCRWAEHycjcvY2ZNxfUpKaUmeF7W63uxK8ZedZRGB1zZdsd6ezUzs7V7tLFXkWi3Za8ACPKvr7TV1pggDV");
#else
    agent_st.set_id("14aaf2551bb80878446ea3cf1638b55ab9092ab9959c82dc76fa6433f230511c");
    agent_st.set_sig("2NA9KRUCRWAEHycjcvY2ZNxfUpKaUmeF7W63uxK8ZedZRGB1zZdsd6ezUzs7V7tLFXkWi3Za8ACPKvr7TV1pggDV");
#endif

    //GenesisBlockHeader
    SignedBlockHeader &signedhead = *genesis_block.mutable_signedhead();
    BlockHeader &bh = *signedhead.mutable_head();
    bh.set_version(1);
    bh.set_num(0);
    bh.set_prev_id("0000000000000000000000000000000000000000000000000000000000000000");
    bh.set_timestamp(1408989677); // 0 - Genesis - 1408989677 converts to Monday August 25, 2014 14:01:17 (pm) in time zone America/New York (EDT)
    bh.set_generator_pk(GENESIS_PUB_KEY_STR);//"25dTUQHwaPHdN2fXjpryz5jrrXxU6NNfKgrpJRA4VheJ4");
    bh.set_basetarget(230584300921369395); // (9223372036854775808 / 40) = 100% = (2^64 / 40)
    bh.set_blocktype(BlockHeader_Type_DATA);

#ifndef PRODFOOTBALL
    bh.set_generating_sig("33319199662b78c55f0def95399a67d6dda4dc920958b7209dc65da2dbc01801");
    bh.set_transaction_id("4859318116a237b5c369b4c253122d3731ad846ad099ead9c18149668c2895d3");
    signedhead.set_sig("4LYuzs6PYu9QDD1sRg8NKg6UK6pd5X815Hb2FEZWw8ekCL9QoepWL5WpT1vVeeBcE757Ny5pqPxVjhLcngEsyftZ");
    bh.set_generating_sig("61b198fd17d49893e324cd7e840f6bfc87bdf4da54afea0515b40e200b87dab6"); //pb::hashit(bh.generator_pk()).str()
    bh.set_transaction_id("d2501817a50746a32327c348078cac9932b51511917f335726ce730a6c7bedc5"); //pb::makeMerkleRoot(genesis_block.signed_transactions())
    signedhead.set_sig("5BRi8K5hyKtpSodvjzNWGeaoianxWJmbG5TpeCLgs7YvtzG3hfEjAWsNbr9qzoso6nf4SZVkbZdHViLGrqQGV9JV");

#else
    bh.set_generating_sig("61b198fd17d49893e324cd7e840f6bfc87bdf4da54afea0515b40e200b87dab6"); //pb::hashit(bh.generator_pk()).str()
    bh.set_transaction_id("d2501817a50746a32327c348078cac9932b51511917f335726ce730a6c7bedc5"); //pb::makeMerkleRoot(genesis_block.signed_transactions())
    signedhead.set_sig("5BRi8K5hyKtpSodvjzNWGeaoianxWJmbG5TpeCLgs7YvtzG3hfEjAWsNbr9qzoso6nf4SZVkbZdHViLGrqQGV9JV");
#endif

    return genesis_block;

/********************************************************************************************
signedhead {
  head {
    version: 1
    num: 0
    prev_id: "0000000000000000000000000000000000000000000000000000000000000000"
    timestamp: 1408989677
    generator_pk: "25dTUQHwaPHdN2fXjpryz5jrrXxU6NNfKgrpJRA4VheJ4"
    generating_sig: "61b198fd17d49893e324cd7e840f6bfc87bdf4da54afea0515b40e200b87dab6"
    basetarget: 230584300921369395
    blocktype: DATA
    transaction_id: "d2501817a50746a32327c348078cac9932b51511917f335726ce730a6c7bedc5"
  }
  sig: "5BRi8K5hyKtpSodvjzNWGeaoianxWJmbG5TpeCLgs7YvtzG3hfEjAWsNbr9qzoso6nf4SZVkbZdHViLGrqQGV9JV"
}
signed_transactions {
  trans {
    version: 1
    type: DATA
    [fantasybit.DataTransition.data_trans] {
      type: SEASONSTART
      season: 2014
      week: 0
      data {
        type: MESSAGE
        [fantasybit.MessageData.message_data] {
          msg: "Distributed Engineered Autonomous Agents : Satoshi Fantasy. April 2014. dcc76458aeaab9ebe5e17abb704f292511ce2f7b8b30336b8f12cf46da2ad71e"
        }
      }
    }
  }
  id: "c96ca89a0b9b219836f1d9ae7599120236b446dd96196d190034cb5747ac1ef9"
  sig: "2ktZ5Um2TVkrUD6iWJqAWPEfCHWzQ9Z6id5rnRnwSHpRe6fT4Ra5GRiY8NcSLGEa1WyfPB5d3GT3XwTi1X7J1sfY"
  fantasy_name: "FantasyAgent"
}
signed_transactions {
  trans {
    version: 1
    type: NAME
    [fantasybit.NameTrans.name_trans] {
      fantasy_name: "FantasyAgent"
      public_key: "25dTUQHwaPHdN2fXjpryz5jrrXxU6NNfKgrpJRA4VheJ4"
      recovery_key: "xXGLATXkcfrk96dnky3USYVMnEw6RpaiRv4XKFZF3kEE"
      am_agent: true
    }
  }
  id: "14aaf2551bb80878446ea3cf1638b55ab9092ab9959c82dc76fa6433f230511c"
  sig: "2NA9KRUCRWAEHycjcvY2ZNxfUpKaUmeF7W63uxK8ZedZRGB1zZdsd6ezUzs7V7tLFXkWi3Za8ACPKvr7TV1pggDV"
  fantasy_name: "FantasyAgent"
}
********************************************************************************************/

/*
    NameTrans nametrans{};
    nametrans.set_hash(10576213825162658308);
    nametrans.set_public_key(std::string("mT1M2MeDjA1RsWkwT7cjE6bbjprcNi84cWyWNvWU1iBa"));
    nametrans.set_nonce(57428892);
    nametrans.set_utc_sec(1408989677);
    nametrans.set_prev_id("00000000000000000000000000000000000000000000000000000000");
    nametrans.set_sig("iKkkiYrzqzRo4Cgz1TeZty4JY4KUrDWyPgeF5tKpeRoRD14zWubsFneY8fW7UodCpP3JXXrFvWh6UkSWD7NcktHDK9gb4i9D3m");
    nametrans.set_sigid("19cacff77cae784ada296272e43b6dd6f22975d1");

    Transaction trans{};
    trans.set_version(1);
    trans.set_type(TransType::NAME);
    //[fantasybit.NameTrans.name_trans]
    trans.MutableExtension(NameTrans::name_trans)->CopyFrom(nametrans);
    SignedTransaction st{};
    st.mutable_trans()->CopyFrom(trans);
    st.set_id("6ca607c105f8f9adfa652a89c285e58a1848f35caef132267e0385f79c453eb4");
    st.set_sig("iKkki4FAQFoNR4foHVv1KNqfnJ1Fm1xuTToW3LgRjfAem2PSuPU3cH7ZPiJNm3xyTLt2bJx5kdRMfn1aEhfCGiTsHbE3PHBeis");
    st.set_fantasy_name("FantasyAgent");

    BlockHeader bh{};
    bh.set_prev_id("00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000");
    bh.set_num(0);

    Block b{};
    b.mutable_head()->CopyFrom(bh);
    SignedTransaction* st2 = b.add_signed_transactions();
    st2->CopyFrom(st);

    SignedBlock sb{};
    sb.mutable_block()->CopyFrom(b);
    sb.set_id("c9348ceb2551871534121114cd707c40653303250602aad6c6e0c67c522e5e9c");
    sb.set_sig("iKkkiYr6vYFtkRtxCeWQvu7iZ9oFdLwrpRe1P3XYUwZz3BvBuwiufWTFj1JSRJ3d1zjvp9W2whNVTWtT5Jxtn1ByyiW3qQYMyy");

    //std::cout << sb.DebugString();
    //Commissioner::GenesisBlock = sb;
    */
}

bool Commissioner::BootStrapFileExists(string genesiskey) {
    string prefix = "boot4strap";
    QString filename = string(prefix + genesiskey + ".out").data();
    QString genesisBootFile = Platform::instance()->settings()->getSetting(AppSettings::ResourceLocation).toString();
    genesisBootFile = genesisBootFile +  filename;
    QFileInfo check_file(genesisBootFile);
    return check_file.exists();
}

Bootstrap Commissioner::makeGenesisBoot(LdbWriter &ldb, string genesiskey) {
    Bootstrap head;
    string headhash;

    string prefix = "boot4strap";

    QString filename = string(prefix + genesiskey + ".out").data();
    QString genesisBootFile = Platform::instance()->settings()->getSetting(AppSettings::ResourceLocation).toString();
    genesisBootFile = genesisBootFile +  filename;
    QFileInfo check_file(genesisBootFile);

#ifndef NOCHECK_WEB_BOOT
    if ( !check_file.exists() ) {

        QString links("http://protoblock.com");
        QString route(filename);

        QMap<QString,QString>  headers;
        QMap<QString,QVariant> params;


        QUrl url;
        url.setUrl(links);
        RestfullClient rest (url);
        rest.getData(route);
        auto resp = rest.lastReply();
        if ( rest.lastCode() > 400 || resp.size() < 100 ) {
            qDebug() << rest.lastCode() << "did not got it url genesisBootFile" << filename;
            return head;
        }

        QFile *m_file = new QFile();
        m_file->setFileName(genesisBootFile);
        m_file->open(QIODevice::WriteOnly);

        m_file->write(resp);
        m_file->close(); //

    }
    check_file.refresh();
#endif

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


    if ( headhash != "" )
        ldb.read(headhash,head);

    return head;
}

pb::public_key_data Commissioner::GENESIS_PUB_KEY
{
#ifndef PRODFOOTBALL
//      Commissioner::str2pk(std::string("mT1M2MeDjA1RsWkwT7cjE6bbjprcNi84cWyWNvWU1iBa"))
     Commissioner::str2pk(std::string("25dTUQHwaPHdN2fXjpryz5jrrXxU6NNfKgrpJRA4VheJ4"))

#else
      Commissioner::str2pk(std::string("25dTUQHwaPHdN2fXjpryz5jrrXxU6NNfKgrpJRA4VheJ4"))
#endif
};

#ifndef PRODFOOTBALL
pb::public_key_data Commissioner::PROD_GENESIS_PUB_KEY
{
      Commissioner::str2pk(std::string("25dTUQHwaPHdN2fXjpryz5jrrXxU6NNfKgrpJRA4VheJ4"))
};
#endif

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

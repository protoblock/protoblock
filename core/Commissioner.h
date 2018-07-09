//
//  Comissioner.h
//  fantasybit
//
//  Created by Jay Berg on 4/1/14.
//
//

#ifndef __fantasybit__Comissioner__
#define __fantasybit__Comissioner__

#include "ProtoData.pb.h"
#include "utils/utils.h"


namespace fantasybit {

class Commissioner
{
public:
    static pb::public_key_data GENESIS_PUB_KEY;
#ifndef PRODFOOTBALL
    static pb::public_key_data PROD_GENESIS_PUB_KEY;
#endif

    static Block Commissioner::makeGenesisBlock()
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
#else
        bh.set_generating_sig("61b198fd17d49893e324cd7e840f6bfc87bdf4da54afea0515b40e200b87dab6"); //pb::hashit(bh.generator_pk()).str()
        bh.set_transaction_id("d2501817a50746a32327c348078cac9932b51511917f335726ce730a6c7bedc5"); //pb::makeMerkleRoot(genesis_block.signed_transactions())
        signedhead.set_sig("5BRi8K5hyKtpSodvjzNWGeaoianxWJmbG5TpeCLgs7YvtzG3hfEjAWsNbr9qzoso6nf4SZVkbZdHViLGrqQGV9JV");
#endif
        return genesis_block;
    }

    static std::string pk2str(const pb::public_key_data &pk)
    {
        return pb::to_base58(pk);
    }

    static pb::public_key_data privStr2Pub(const std::string &priv_key) {
        return pb::secp256k1_privkey::regenerate(priv_key).get_public_key().serialize();
    }

    static pb::public_key_data str2pk(const std::string &str)
    {
        pb::public_key_data pbb;
        pb::from_base58(str, (char *)pbb.key_data, 33);
        return pbb;
    }

    static std::string sig2str(const pb::signature &sig)
    {
        return pb::to_base58((char *)sig.data, 64);
    }

    static pb::signature str2sig(const std::string &str)
    {
        unsigned char data[72];
        pb::signature sig;
        if ( pb::from_base58(str, (char *)data, 64) > 64 ) {
            //Warning: still dealing with "bad" sigs from 2015
            auto sz = pb::from_base58(str, (char *)data, 72);
            sig = pb::parse_der(data,sz < 72 ? sz : 72);
            sig = pb::signature_normalize(sig);
        }
        else memcpy(sig.data,data,64);

        return sig;
    }


};

}

#endif /* defined(__fantasybit__Comissioner__) */

/****************************GENESIS BLOCK***************************************
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
/***********************************************************
type: MYFANTASYNAME
myfantasyname{
    name: "FantasyAgent"
    status : found
       nametransaction{
            hash: 10576213825162658308
            public_key : ""
            nonce : 57428892
            utc_sec : 1408989677
            prev_id : "0000000000000000000000000000000000000000000000000000000000000000"
            sig : ""
            sigid : ""
        }
}
*************************************************************/

/*
static name_transaction createGenesisName()
{
    name_transaction genesis(nameid_t{ "00000000000000000000000000000000000000000000000000000000" });
    genesis.name_hash = 10576213825162658308;// FantasyName::name_hash("FantasyAgent");
    genesis.pubkey = Commissioner::str2pk(std::string("mT1M2MeDjA1RsWkwT7cjE6bbjprcNi84cWyWNvWU1iBa"));
    genesis.utc_sec = fc::time_point_sec(1408989677);// = fc::time_point_sec(fc::time_point::from_iso_string("20140401T134233")); // 1408989677
    genesis.nonce = 57428892;
    genesis.sig = Commissioner::str2sig(std::string("iKkkiYrzqzRo4Cgz1TeZty4JY4KUrDWyPgeF5tKpeRoRD14zWubsFneY8fW7UodCpP3JXXrFvWh6UkSWD7NcktHDK9gb4i9D3m"));

    assert(Commissioner::verify(genesis.sig,genesis.digest(),genesis.pubkey));

    GENESIS_PUB_KEY = genesis.pubkey;

    return genesis;
}
*/

/********************************************************************************************
SignedBlock
{
	block
	{
		head
		{
			num: 1
			prev_id : "00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
		}
		signed_transactions
		{
			trans	
			{
				version: 1
				type : NAME
				[fantasybit.NameTrans.name_trans] 
				{
					hash: 10576213825162658308
					public_key : "mT1M2MeDjA1RsWkwT7cjE6bbjprcNi84cWyWNvWU1iBa"
					nonce : 57428892
					utc_sec : 1408989677
					prev_id : "00000000000000000000000000000000000000000000000000000000"
					sig : "iKkkiYrzqzRo4Cgz1TeZty4JY4KUrDWyPgeF5tKpeRoRD14zWubsFneY8fW7UodCpP3JXXrFvWh6UkSWD7NcktHDK9gb4i9D3m"
					sigid : "19cacff77cae784ada296272e43b6dd6f22975d1"
				}
			}
			id: "6ca607c105f8f9adfa652a89c285e58a1848f35caef132267e0385f79c453eb4"
			sig: "iKkki4FAQFoNR4foHVv1KNqfnJ1Fm1xuTToW3LgRjfAem2PSuPU3cH7ZPiJNm3xyTLt2bJx5kdRMfn1aEhfCGiTsHbE3PHBeis"
			fantasy_name : "FantasyAgent"
		}
	}
	id: "c9348ceb2551871534121114cd707c40653303250602aad6c6e0c67c522e5e9c"
	sig : "iKkkiYr6vYFtkRtxCeWQvu7iZ9oFdLwrpRe1P3XYUwZz3BvBuwiufWTFj1JSRJ3d1zjvp9W2whNVTWtT5Jxtn1ByyiW3qQYMyy"
}
*********************************************************************************************************************************/

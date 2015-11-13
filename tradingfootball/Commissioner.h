//
//  Comissioner.h
//  fantasybit
//
//  Created by Jay Berg on 4/1/14.
//
//

#ifndef __fantasybit__Comissioner__
#define __fantasybit__Comissioner__

#include "FantasyName.h"
#include <fc/crypto/base58.hpp>
#include <iostream>

//#include <../cute-fantasy/playerloader.h>

#include "FantasyAgent.h"

namespace fantasybit {

uint64_t difficulty( const fc::sha224& hash_value );

class Commissioner
{
    volatile bool running = true;
    static std::unordered_map<pubkey_t,std::shared_ptr<FantasyName>> FantasyNames;
    static std::map<hash_t,pubkey_t> Hash2Pk;


public:
	Commissioner() {	}

    static std::recursive_mutex name_mutex;
    static fc::ecc::public_key_data MASTER_PUB_KEY;
	static fc::ecc::public_key_data GENESIS_PUB_KEY;
    static bool isAliasAvailable(alias_t alias)
    {
        std::lock_guard<std::recursive_mutex> lockg{ name_mutex };
		return Commissioner::Hash2Pk.find(FantasyName::name_hash(alias)) == end(Commissioner::Hash2Pk);
    }


    static void clearAll()
    {
        std::lock_guard<std::recursive_mutex> lockg{ name_mutex };
        FantasyNames.clear();
        Hash2Pk.clear();
    }

    static std::vector<std::shared_ptr<FantasyName>> GetFantasyNames() {
        std::vector<std::shared_ptr<FantasyName>> names;

        for (auto p : FantasyNames)
            names.emplace_back(p.second);

        return names;
    }
    
    static std::shared_ptr<FantasyName> getName(const pubkey_t &pub  ) {
        std::lock_guard<std::recursive_mutex> lockg{ name_mutex };
        auto iter = Commissioner::FantasyNames.find(pub);
        if ( iter == end(Commissioner::FantasyNames) ) return nullptr;
        
        return iter->second;
    }

	static std::shared_ptr<FantasyName> getName(const hash_t &hash) {
        std::lock_guard<std::recursive_mutex> lockg{ name_mutex };
		auto iter = Hash2Pk.find(hash);
		if (iter == end(Hash2Pk)) return nullptr;

		return getName(iter->second);
	}

	static std::shared_ptr<FantasyName> getName(const alias_t &alias) {
        std::lock_guard<std::recursive_mutex> lockg{ name_mutex };
		return getName(FantasyName::name_hash(alias));
	}


    static std::shared_ptr<FantasyName> AddName(const alias_t &alias, const std::string &pubkey)
    {
        auto pfn = makeName(alias,pubkey);
        {
            std::lock_guard<std::recursive_mutex> lockg{ name_mutex };
            Commissioner::Hash2Pk.emplace(pfn->hash(), pfn->pubkey());
            Commissioner::FantasyNames.emplace(pfn->pubkey(), pfn);
        }
        return pfn;

    }

	static std::shared_ptr<FantasyName> makeName(const alias_t &alias, const std::string &pubkey)
	{
        auto pk  = str2pk(pubkey);
        auto pfn = std::make_shared< FantasyName >(alias,pk);
		return pfn;
	}

    static std::string FantasyMasterName() { return "FantasyMaster"; }
    static std::string FantasyAgentName() { return "FantasyAgent"; }


/*

    //static SignedBlock GenesisBlock;
    static SignedTransaction makeGenesisName() {
        //make and sign "FantasyAgent" name transaction
		NameProof nameproof{};
		nameproof.set_type(NameProof_Type_ORACLE);

		NameTrans nametrans{};
		nametrans.set_public_key(std::string("mT1M2MeDjA1RsWkwT7cjE6bbjprcNi84cWyWNvWU1iBa"));
        nametrans.set_fantasy_name(FantasyAgentName());
		nametrans.mutable_proof()->CopyFrom(nameproof);

		Transaction trans{};
		trans.set_version(1);
		trans.set_type(TransType::NAME);
		trans.MutableExtension(NameTrans::name_trans)->CopyFrom(nametrans);

		SignedTransaction st{};
		st.mutable_trans()->CopyFrom(trans);
		st.set_id("4ebd75db20bab394d28d6bdd2740d075aeb4ea4f5222cc50e6e3f5e6d3901b0e");
		st.set_sig("iKkkiYsL3fadfz6KoM7Rs3SGsyyG2vmxVtxRKEBU9nsbnQQLjJxFtJ7iCPT4xMZcm41Q8aJByJZiaF2qde1Q7YdzvtRdWwxDFD");
		st.set_fantasy_name("FantasyAgent");

		return st;
	}
*/

    static SignedTransaction makeAgentName(FantasyAgent &agent,SignedTransaction agentname) {
        //make and sign "FantasyAgent" name transaction


        MasterProof mp{};
        mp.set_season("2105");
        mp.set_week(1);
        mp.set_timestamp(1441676880); // 9/8/2015 1:47:28
        mp.mutable_new_oracle_name()->CopyFrom(agentname);

        NameProof np{};
        np.set_type(NameProof_Type_MASTER);
        np.MutableExtension(MasterProof::master_proof)->CopyFrom(mp);

        NameTrans nt = agentname.trans().GetExtension(NameTrans::name_trans);

        NameTrans nametrans{};
        nametrans.set_public_key(nt.public_key());
        nametrans.set_fantasy_name(nt.fantasy_name());
        nametrans.mutable_proof()->CopyFrom(np);

        Transaction trans{};
        trans.set_version(Commissioner::TRANS_VERSION);
        trans.set_type(TransType::NAME);
        trans.MutableExtension(NameTrans::name_trans)->CopyFrom(nametrans);

        auto st = agent.makeSigned(trans);
        return st;
    }

    static SignedTransaction makeGenesisName(FantasyAgent &agent) {
        //make and sign "FantasyAgent" name transaction
        NameProof nameproof{};
        nameproof.set_type(NameProof_Type_ORACLE);

        NameTrans nametrans{};
        nametrans.set_public_key(agent.pubKeyStr());
        nametrans.set_fantasy_name(FantasyMasterName());
        nametrans.mutable_proof()->CopyFrom(nameproof);

        Transaction trans{};
        trans.set_version(Commissioner::TRANS_VERSION);
        trans.set_type(TransType::NAME);
        trans.MutableExtension(NameTrans::name_trans)->CopyFrom(nametrans);

        auto st = agent.makeSigned(trans);
        return st;
    }

    static Transaction makeFantasyAgent() {
        //make and sign "FantasyAgent" name transaction
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

//        auto st = agent.makeSigned(trans);
        return trans;
    }

    static Transaction MasterGenesisTransition() {
        DataTransition dt{};

        dt.set_type(DataTransition_Type_SEASONSTART);
        dt.set_season(2015);
        dt.set_week(1);

        Transaction trans{};
        trans.set_version(Commissioner::TRANS_VERSION);
        trans.set_type(TransType::DATA);
        trans.MutableExtension(DataTransition::data_trans)->CopyFrom(dt);

        return trans;
    }

    static Transaction GenesisTransition();

    static GlobalState InitialGlobalState () {
        GlobalState gs{};
        gs.set_season(2015);
        gs.set_state(GlobalState::INSEASON);
        gs.set_week(1);

        return gs;
    }
/*
    static SignedTransaction makeGenesisTransition() {
        SignedTransaction st{};

        Transaction tx = GenesisTransition();
        st.mutable_trans()->CopyFrom(tx);

        st.set_id(HashIt(tx.SerializeAsString()));
        //st.set_id("9792644b12d72d0d76019dab91cbcddf71a54a9ea46acf5fa206f4db1d276a3b");
        st.set_sig("iKkki472jcQ8tKtr75dV8i5Aei4dSuudRQL4R2YX9bT3erRgnQ27cJh9hoefTU8rp3GVCPNxCGM1epkUKAW9cmwiTsDVW38bSw");
        st.set_fantasy_name("FantasyAgent");
        return st;
    }
*/

    static BlockHeader MasterGenesisBlockHeader(FantasyAgent fa,
                                                SignedTransaction name,
                                                SignedTransaction transition) {
        BlockHeader bh{};
        bh.set_version(Commissioner::BLOCK_VERSION);
        bh.set_num(0);
        bh.set_prev_id("0000000000000000000000000000000000000000000000000000000000000000");


        bh.set_timestamp(1441680862);
        bh.set_generator_pk(fa.pubKeyStr());

        bh.set_generating_sig(fc::sha256::hash(bh.generator_pk()).str());
        bh.set_basetarget(0); //todo
        bh.set_blocktype(BlockHeader_Type_DATA);
        auto root = fc::sha256::hash(transition.id() + name.id()).str();
        bh.set_transaction_id(root);

        return bh;
    }

    static BlockHeader GenesisBlockHeader(BlockHeader mbh,
                                          //FantasyAgent da,
                                          SignedTransaction name,
                                          SignedTransaction  & transition) {
        BlockHeader bh{};
        bh.set_version(1);
        bh.set_num(1);
        bh.set_prev_id(fc::sha256::hash(mbh.SerializeAsString()).str());

        bh.set_timestamp(1441683084);

        bh.set_generator_pk("25dTUQHwaPHdN2fXjpryz5jrrXxU6NNfKgrpJRA4VheJ4");
        bh.set_generating_sig(fc::sha256::hash(mbh.generating_sig() + bh.generator_pk()).str());
        bh.set_basetarget(0); //todo
        bh.set_blocktype(BlockHeader_Type_DATA);

        auto root = fc::sha256::hash(transition.id() + name.id()).str();
        bh.set_transaction_id(root);

        return bh;
    }

    static Block makeGenesisBlockRaw();
    static Block makeGenesisBlock();

	static const int BLOCK_VERSION = 1;
	static const int TRANS_VERSION = 1;
	static const int GENESIS_NUM = 1;
    

    static bool verify(const fc::ecc::signature &sig, const fc::sha256 &digest, pubkey_t& pk) {
          return fc::ecc::public_key(pk).verify(digest, sig);// fc::ecc::public_key(sig, digest) == pub;
	}

	static bool verifyOracle(const fc::ecc::signature &sig, const fc::sha256 &digest)
	{
        return verify(sig, digest, GENESIS_PUB_KEY) || verify(sig, digest, MASTER_PUB_KEY);
	}

	static bool verifyByName(const fc::ecc::signature &sig, const fc::sha256 &digest,const std::string &fn)
	{
        std::lock_guard<std::recursive_mutex> lockg{ name_mutex };
		auto iter = Hash2Pk.find(FantasyName::name_hash(fn));
        if (iter == Hash2Pk.end()) {
            qCritical() << "cant find fantasyname: " << fn;
            return false;
        }
        else {
            auto ret = verify(sig, digest, iter->second);
            if ( !ret )
                qCritical()  << "!verify(sig, digest, iter->second)";
            return ret;
        }
	}

	static std::string pk2str(const pubkey_t &pk)
	{
		return fc::to_base58(pk.data, pk.size());
	}

    static fc::ecc::public_key_data privStr2Pub(const std::string &priv_key) {
        auto pk = fc::sha256{ priv_key };
        return fc::ecc::private_key::regenerate(pk).get_public_key().serialize();
    }

	static pubkey_t str2pk(const std::string &str)
	{
		pubkey_t pbb;
		fc::from_base58(str, pbb.data, pbb.size());
		return pbb;
	}

	static std::string sig2str(const fc::ecc::signature &sig)
	{
		return fc::to_base58(sig.data, sig.size());
	}

	static fc::ecc::signature str2sig(const std::string &str)
	{
		fc::ecc::signature sig;
		fc::from_base58(str, sig.data, sig.size());
		return sig;
	}

    static std::vector<std::string> STATS_ID;

    static std::vector<std::string> GENESIS_NFL_TEAMS;
    static std::set<std::pair<std::string,std::string>> GENESIS_NFL_PLAYERS;
    static std::vector<std::string> GET_GENESIS_NFL_TEAMS() {
        return GENESIS_NFL_TEAMS;
    }

    static std::set<std::pair<std::string,std::string>> GET_GENESIS_NFL_PLAYERS(){
        return GENESIS_NFL_PLAYERS;
    }

	//static std::string DATA_DIR("data");
	//static std::string DB_DIR("db");
};

}

#endif /* defined(__fantasybit__Comissioner__) */


/****************************GENESIS BLOCK***************************************
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


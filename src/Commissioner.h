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

namespace fantasybit {

uint64_t difficulty( const fc::sha224& hash_value );

class Commissioner
{
    volatile bool running = true;
public:
	Commissioner() {
	}

    void stop() { running = false; }
    static std::unordered_map<pubkey_t,std::shared_ptr<FantasyName>> FantasyNames;
    static std::map<hash_t,pubkey_t> Hash2Pk;
			
	//static std::string m_genesis_id;
	static fc::ecc::public_key_data GENESIS_PUB_KEY;
    static bool isAliasAvailable(alias_t alias)
    {
		return Commissioner::Hash2Pk.find(FantasyName::name_hash(alias)) == end(Commissioner::Hash2Pk);
    }
    
    static std::shared_ptr<FantasyName> getName(const pubkey_t &pub  ) {
        auto iter = Commissioner::FantasyNames.find(pub);
        if ( iter == end(Commissioner::FantasyNames) ) return nullptr;
        
        return iter->second;
    }

	static std::shared_ptr<FantasyName> getName(const hash_t &hash) {
		auto iter = Hash2Pk.find(hash);
		if (iter == end(Hash2Pk)) return nullptr;

		return getName(iter->second);
	}

	static std::shared_ptr<FantasyName> getName(const alias_t &alias) {
		return getName(FantasyName::name_hash(alias));
	}

	static std::shared_ptr<FantasyName> makeName(const alias_t &alias, const std::string &pubkey)
	{
		auto pfn = std::make_shared < FantasyName >();
		pfn->alias = alias;
		pfn->pubkey = str2pk(pubkey);
		return pfn;
	}
    
	//static SignedBlock GenesisBlock;
	static SignedTransaction makeGenesisName() {
		//make and sign "FantasyAgent" name transaction
		NameProof nameproof{};
		nameproof.set_type(NameProof_Type_ORACLE);

		NameTrans nametrans{};
		nametrans.set_public_key(std::string("mT1M2MeDjA1RsWkwT7cjE6bbjprcNi84cWyWNvWU1iBa"));
		nametrans.set_fantasy_name("FantasyAgent");
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

	static Block makeGenesisBlock() 
	{
		Block b{};
		auto gtrans = Commissioner::makeGenesisName();

		BlockHeader bh{};
		bh.set_version(1);
		bh.set_num(1);
		bh.set_prev_id("0000000000000000000000000000000000000000000000000000000000000000");

		//1433116800  June 1 2015 00:00 UTC 
		//auto time = fc::time_point_sec(fc::time_point::from_iso_string("20150601T000000")).sec_since_epoch();
		bh.set_timestamp(1433116800);

		bh.set_generator_pk(std::string("mT1M2MeDjA1RsWkwT7cjE6bbjprcNi84cWyWNvWU1iBa"));

		//fc::sha256::hash(bh.generator_pk()).str());
		bh.set_generating_sig("33319199662b78c55f0def95399a67d6dda4dc920958b7209dc65da2dbc01801"); 

		bh.set_basetarget(0); //todo
		bh.set_blocktype(BlockHeader_Type_NORMAL);

		//merkle root - single transaction 
		//auto root = fc::sha256::hash(gtrans.id() + gtrans.id()).str();
		bh.set_transaction_id("5d8147106af959c07fb4dbcabc4602135824ab5f0be5dd8a36275914de333b8e"); 

		SignedBlockHeader sbh{};
		sbh.mutable_head()->CopyFrom(bh);

		//auto p = agent.getIdSig(sbh.head().SerializeAsString());
		sbh.set_sig("iKkkiYqRdeNyz4cuTZ9BhyXLRxFcrFxtYzqfUjPRkf4FFivNvjCnNGhGoU2oDX5raGZrKiQUTUNDP2XU2te6k2TppqNmfB5EfH");

		//auto genesisblockhash = p.first; "aa5ee9a1667e86f23b0c16cef9c7721403ca1bab7545073b20bc5f3385a34b99"

		b.mutable_signedhead()->CopyFrom(sbh);
		b.add_signed_transactions()->CopyFrom(gtrans);

		return b;
	}

	static const int BLOCK_VERSION = 1;
	static const int TRANS_VERSION = 1;
	static const int GENESIS_NUM = 1;
    

	static bool verify(const fc::ecc::signature &sig, const fc::sha256 &digest, pubkey_t& pk)
	{
		return fc::ecc::public_key(pk).verify(digest, sig);// fc::ecc::public_key(sig, digest) == pub;
	}

	static bool verifyOracle(const fc::ecc::signature &sig, const fc::sha256 &digest)
	{
		return verify(sig, digest, GENESIS_PUB_KEY);
	}

	static bool verifyByName(const fc::ecc::signature &sig, const fc::sha256 &digest,const std::string &fn)
	{
		auto iter = Hash2Pk.find(FantasyName::name_hash(fn));
		if (iter == Hash2Pk.end())
			return fbutils::LogFalse(std::string("cant find fantasyname").append(fn));
		
		return verify(sig, digest, iter->second);
	}

	static std::string pk2str(pubkey_t &pk)
	{
		return fc::to_base58(pk.data, pk.size());
	}

	static pubkey_t str2pk(const std::string &str)
	{
		pubkey_t pbb;
		fc::from_base58(str, pbb.data, pbb.size());
		return pbb;
	}

	static std::string sig2str(fc::ecc::signature &sig)
	{
		return fc::to_base58(sig.data, sig.size());
	}

	static fc::ecc::signature str2sig(const std::string &str)
	{
		fc::ecc::signature sig;
		fc::from_base58(str, sig.data, sig.size());
		return sig;
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
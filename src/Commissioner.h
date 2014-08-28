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

    name_transaction generateName(alias_t,pubkey_t) ;

	nameid_t lastId() const { return m_genesis_id; }
    void stop() { running = false; }
    static std::unordered_map<pubkey_t,std::shared_ptr<FantasyName>> FantasyNames;
    static std::map<hash_t,pubkey_t> Aliases;
			
    static nameid_t m_genesis_id;
	static fc::ecc::public_key_data GENESIS_PUB_KEY;
    static bool isAliasAvailable(alias_t alias)
    {
        return Commissioner::Aliases.find(FantasyName::name_hash(alias)) == end(Commissioner::Aliases);
    }
    
    static std::shared_ptr<FantasyName> getName(const pubkey_t &pub  ) {
        auto iter = Commissioner::FantasyNames.find(pub);
        if ( iter == end(Commissioner::FantasyNames) ) return nullptr;
        
        return iter->second;
    }

	static std::shared_ptr<FantasyName> getName(const hash_t &hash) {
		auto iter = Aliases.find(hash);
		if (iter == end(Aliases)) return nullptr;

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
    
    static uint64_t target(uint64_t in) { return std::max(in,Commissioner::min_difficulty); }
  
	/***********************************************************
	type: MYFANTASYNAME
	myfantasyname{
		name: "FantasyAgent"
		status : found
		   nametransaction{
				hash: 10576213825162658308
				public_key : "mT1M2MeDjA1RsWkwT7cjE6bbjprcNi84cWyWNvWU1iBa"
				nonce : 57428892
				utc_sec : 1408989677
				prev_id : "00000000000000000000000000000000000000000000000000000000"
				sig : "iKkkiYrzqzRo4Cgz1TeZty4JY4KUrDWyPgeF5tKpeRoRD14zWubsFneY8fW7UodCpP3JXXrFvWh6UkSWD7NcktHDK9gb4i9D3m"
				sigid : "19cacff77cae784ada296272e43b6dd6f22975d1"
			}
	}
	*************************************************************/
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
    };	
	
	//static SignedBlock GenesisBlock;

	static SignedBlock makeGenesisBlock() 
	{
		NamePOW namepow{};
		namepow.set_hash(10576213825162658308);
		namepow.set_public_key(std::string("mT1M2MeDjA1RsWkwT7cjE6bbjprcNi84cWyWNvWU1iBa"));
		namepow.set_nonce(57428892);
		namepow.set_utc_sec(1408989677);
		namepow.set_prev_id("00000000000000000000000000000000000000000000000000000000");
		namepow.set_sig("iKkkiYrzqzRo4Cgz1TeZty4JY4KUrDWyPgeF5tKpeRoRD14zWubsFneY8fW7UodCpP3JXXrFvWh6UkSWD7NcktHDK9gb4i9D3m");
		namepow.set_sigid("19cacff77cae784ada296272e43b6dd6f22975d1");

		NameProof nameproof{};
		nameproof.set_type(NameProof_Type_POW);
		nameproof.MutableExtension(NamePOW::name_pow)->CopyFrom(namepow);
		NameTrans nametrans{};		
		nametrans.set_public_key(namepow.public_key());
		nametrans.set_fantasy_name("FantasyAgent");
		nametrans.mutable_proof()->CopyFrom(nameproof);

		Transaction trans{};
		trans.set_version(1);
		trans.set_type(TransType::NAME);
		//[fantasybit.NameTrans.name_trans]
		trans.MutableExtension(NameTrans::name_trans)->CopyFrom(nametrans);

		SignedTransaction st{};
		st.mutable_trans()->CopyFrom(trans);
		st.set_id("3ed1aff4fea939ae372b2347e3af121f22daf745c1731eb280fac460d39547ca");
		st.set_sig("iKkkiYpPLSsXDLtGZGcjNWAoHj3cTRuUiKWEd5FWZ2oQ3oczZwdnXKM74KKtpwxwnp7bQgr7vWKYRzKJGG252UrQjZD7HtBkWb	");
		st.set_fantasy_name("FantasyAgent");

		BlockHeader bh{};
		bh.set_prev_id("00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000");
		bh.set_num(GENESIS_NUM);

		Block b{};
		b.mutable_head()->CopyFrom(bh);
		SignedTransaction* st2 = b.add_signed_transactions();
		st2->CopyFrom(st);

		SignedBlock sb{};
		sb.mutable_block()->CopyFrom(b);
		sb.set_version(BLOCK_VERSION);
		sb.set_id("487836348a0265cc675343c4cb3e541afcf07ce97b11656732798929f4233e8a");
		sb.set_sig("iKZWEQEEmJRKFhgZ1PGwYyXchWfqd9qhNCWVPy42t9Fua6XtMvqA2JVb46VfwjchKvhzqotf2MjGRRTw2dDTh3arkVkQnHwDVh");

		//std::cout << sb.DebugString();
		//Commissioner::GenesisBlock = sb;
		return sb;
	}

	static const int BLOCK_VERSION = 1;
	static const int TRANS_VERSION = 1;
	static const int GENESIS_NUM = 1;

    static std::vector<name_transaction> createGenesisChild()
    {
        return std::vector<name_transaction>{};
    }
    
    static int hashmineindex()
    {
#ifdef EASY_TEST_MINING
        return 2;
#else
        return 3;
#endif
    }
    
    static fc::sha224 max_hash() {
        fc::sha224 mining_hash{};
        char* tmpPtr = (char*)&mining_hash;
        memset( tmpPtr, 0xff, sizeof(mining_hash) );
        for (int i=0;i<hashmineindex();++i)
            tmpPtr[i] = 0;
        tmpPtr[hashmineindex()] = 0xff;
        return mining_hash;
    }
    
    static uint64_t min_difficulty;

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
		auto iter = Aliases.find(FantasyName::name_hash(fn));
		if (iter == Aliases.end()) 
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
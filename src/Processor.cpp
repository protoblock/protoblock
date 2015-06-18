//
//  Processor.cpp
//  fantasybit
//
//  Created by Jay Berg on 8/24/14.
//
//

#include  "Processor.h"

namespace fantasybit
{

bool BlockProcessor::verify_name(const SignedTransaction &st, const NameTrans &nt, 
	const fc::ecc::signature& sig, const fc::sha256 &digest) { 

	auto iter = Commissioner::Hash2Pk.find(FantasyName::name_hash(nt.fantasy_name()));
	if (iter != end(Commissioner::Hash2Pk)) {
		auto fn = Commissioner::FantasyNames[iter->second]->alias;
		return fbutils::LogFalse(std::string("Processor::process failure: FantasyName(").append(fn + ")  hash already used use.\n")
										.append(st.DebugString()));
	}

	auto proof = nt.proof();
	switch (proof.type())
	{
		case NameProof_Type_TWEET:
		{
			auto tp = proof.GetExtension(TweetProof::tweet_proof);
			//TODO verify tweet
			return true;
		}
		break;

		case NameProof_Type_ORACLE:
		{
			return true;//TODO
			//verify oracle
			if (!Commissioner::verifyOracle(sig, digest))
#ifdef NO_ORACLE_CHECK_TESTING
			if (!Commissioner::verifyByName(sig, digest, st.fantasy_name()))
#endif

				return fbutils::LogFalse(std::string("Processor::process name proof oracle failed").append(st.DebugString()));

			return true;
		}
		break;

		default:
			return false;
			break;
	}
	/*
	assert(pow.hash() == pfn->hash());
	Commissioner::Hash2Pk.emplace(pow.hash(), pfn->pubkey);
	Commissioner::FantasyNames.emplace(pfn->pubkey, pfn);
	if (nt.has_name_pow())

	else if (nt.has_tweet_proof())
	{
	auto top = nt.tweet_proof();
	pfn->pubkey = Commissioner::str2pk(top.public_key());
	pfn->alias = nt.fantasy_name();
	//assert(pow.hash() == pfn->hash());
	Commissioner::Hash2Pk.emplace(pfn->hash(), pfn->pubkey);
	Commissioner::FantasyNames.emplace(pfn->pubkey, pfn);
	continue;
	}
	*/
}


}


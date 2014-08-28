//
//  Source.h
//  fantasybit
//
//  Created by Jay Berg on 8/24/14.
//
//

#ifndef __fantasybit__Processor__
#define __fantasybit__Processor__

#include "Commissioner.h"
#include "ProtoData.pb.h"
#include <fc/crypto/sha256.hpp>
#include <fc/crypto/elliptic.hpp>
#include <algorithm>
#include <iostream>
#include "fbutils.h";
#include "Source.h"

namespace fantasybit
{

using comish = fantasybit::Commissioner;

class BlockRecorder {};

class BlockProcessor
{
	//BlockRecorder mRecorder{};
	bool verify_name(const SignedTransaction &, const NameTrans &, const fc::ecc::signature&, const fc::sha256 &);
public:
	BlockProcessor() {
		//mRecorder.init();
	}
		
	void init() 
	{
		//mRecorder.init();
	}


	bool process(SignedBlock &sblock)
	{
		if (sblock.version() != Commissioner::BLOCK_VERSION ) 
			return fbutils::LogFalse(std::string("Processor::process wrong block version ").append(sblock.DebugString()));

		fc::sha256 digest = fc::sha256::hash(sblock.block().SerializeAsString() );
		if (digest.str() != sblock.id())
			return 
			fbutils::LogFalse(std::string("Processor::process block hash error digest \n").append(sblock.DebugString()).append(digest.str()));
		assert(digest.str() == sblock.id());

		fc::ecc::signature sig = Commissioner::str2sig(sblock.sig());
		//assert(Commissioner::verifyOracle(sig, digest));
		if (!Commissioner::verifyOracle(sig, digest))
#ifdef NO_ORACLE_CHECK_TESTING
			if (!Commissioner::GENESIS_NUM == sblock.block().head().num())
#endif
			return fbutils::LogFalse(std::string("Processor::process only oracle can sign blocks!! ").append(sblock.DebugString()));

		//mRecorder.newBlock(block);
		for (const auto &st : sblock.block().signed_transactions())
		{
			Transaction t{ st.trans() };

			fc::sha256 digest = fc::sha256::hash(t.SerializeAsString());
			if (digest.str() != st.id()) {
				fbutils::LogFalse(std::string("Processor::process signed transaction hash error digest ").append(st.DebugString()));
				continue;
			}

			if (t.version() != Commissioner::TRANS_VERSION) {
				fbutils::LogFalse(std::string("Processor::process wrong transaction version ").append(st.DebugString()));
				continue;
			}

			fc::ecc::signature sig = Commissioner::str2sig(st.sig());

			if (t.type() == TransType::NAME)
			{
				auto nt = t.GetExtension(NameTrans::name_trans);
				if (verify_name(st, nt, sig, digest))
				{
					auto pfn = Commissioner::makeName(nt.fantasy_name(), nt.public_key());
					Commissioner::Aliases.emplace(pfn->hash(), pfn->pubkey);
					Commissioner::FantasyNames.emplace(pfn->pubkey, pfn);
				}

				continue;
			}

			if (!Commissioner::verifyByName(sig, digest, st.fantasy_name()))
			{
				fbutils::LogFalse(std::string("Processor::process cant verify trans sig").append(st.DebugString()));
				continue;
			}

			switch (t.type())
			{
				case TransType::PROJECTION:
				{
					auto pt = t.GetExtension(ProjectionTrans::proj_trans);
					auto iter = Source::EventProjectionResult.find(pt.game_id());
					if (iter != end(Source::EventProjectionResult))
					{
						auto pp = pt.fpp_projection();
						iter->second.addProjection(st.fantasy_name(), pp.fantasy_player_id(), pp.points());
					}
					break;
				}
				case TransType::RESULT:
				{
					auto rt = t.GetExtension(ResultTrans::result_trans);
					//auto iter = Source::EventProjectionResult.find(rt.game_id());
					//if (iter == end(Source::EventProjectionResult)) {
					//}

					auto iter = Source::EventProjectionResult.emplace(make_pair(rt.game_id(), rt.game_id()));
					iter.first->second.setDataAgent(st.fantasy_name());
					//auto &source = Source::EventProjectionResult[rt.game_id()];
					for (const auto &fpp : rt.fpp_results())
					{
						iter.first->second.addResult(fpp.fantasy_player_id(), fpp.points());
						//auto pp = pt.fpp_projection();
						//iter->second.addProjection(st.fantasy_name(), pp.fantasy_player_id(), pp.points());
					}
					break;
				}
				default:
					break;
			}
		}

		std::cout << " BLOCK(" << sblock.block().head().num() << ") processed! \n";
		return true;
			//std::cout << t.DebugString() << "\n";//process(t)
		//mRecorder.comitBlock(block);	
	}

};

}


#endif

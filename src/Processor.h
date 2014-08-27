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

class BlockRecorder {};

class BlockProcessor
{
	//BlockRecorder mRecorder{};
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
		assert(digest.str() == sblock.id());
		if (digest.str() != sblock.id())
			return fbutils::LogFalse(std::string("Processor::process block hash error digest ").append(sblock.DebugString()));

		fc::ecc::signature sig = Commissioner::str2sig(sblock.sig());
		assert(Commissioner::verifyOracle(sig, digest));
		if (!Commissioner::verifyOracle(sig, digest))
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

			if (t.type() == TransType::NAME)
			{ //TODO: fix this mess
				auto pfn = std::make_shared < FantasyName >();
				auto nt = t.GetExtension(NameTrans::name_trans);
				pfn->pubkey = Commissioner::str2pk(nt.public_key());
				pfn->alias = st.fantasy_name();
				assert(nt.hash() == pfn->hash());
				Commissioner::Aliases.emplace(nt.hash(), pfn->pubkey);
				Commissioner::FantasyNames.emplace(pfn->pubkey, pfn);
				continue;
			}

			fc::ecc::signature sig = Commissioner::str2sig(st.sig());
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

		return true;
			//std::cout << t.DebugString() << "\n";//process(t)
		//mRecorder.comitBlock(block);
	}

};

}


#endif

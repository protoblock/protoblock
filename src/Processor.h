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

	void process(SignedBlock &sblock)
	{
		fc::sha256 digest = fc::sha256::hash(sblock.block().SerializeAsString() );
		//std::cout << "1 2 " << digest.str() << " sid " << sblock.id() << "\n";

		assert(digest.str() == sblock.id());
		
		//sblock.sig().copy(sig.begin(), sig.size(), 0);
		//strcpy(sig.data, static_cast<unsigned char*>(sblock.mutable_sig();
		//sblock.sig().copy(sig.begin(), sig.size());
		//std::copy(sblock.sig().begin(), sblock.sig().end(), sig.data + 10);

		fc::ecc::signature sig = Commissioner::str2sig(sblock.sig());
		if (!Commissioner::verifyOracle(sig, digest))
		{
			std::cout << " only oracle can sign blocks!!" << "\n";
			assert(Commissioner::verifyOracle(sig, digest));
		}

		//std::copy(sblock.sig().begin(), sig.begin());
		//SerializeToArray(sig.begin(), sig.size());
		//verify block header
		//if (verified)
		{
			//mRecorder.newBlock(block);
			for (const auto &st : sblock.block().signed_transactions())
			{
				Transaction t{ st.trans() };
				switch (t.type())
				{
					case TransType::NAME:
					{
						auto pfn = std::make_shared < FantasyName >();
						auto nt = t.GetExtension(NameTrans::name_trans);
						pfn->pubkey = Commissioner::str2pk(nt.public_key());
						pfn->alias = st.fantasy_name();
						assert(nt.hash() == pfn->hash());
						Commissioner::Aliases.emplace(nt.hash(), pfn->pubkey);
						Commissioner::FantasyNames.emplace(pfn->pubkey, pfn);
					}
					break;
					
					default:
						break;
				}
				fc::sha256 digest = fc::sha256::hash(st.trans().SerializeAsString());
				/*
				if (!Commissioner::verify(Commissioner::str2sig(st.sig()), digest)
				{
					std::cout << " only oracle can sign blocks!!" << "\n";
					assert(Commissioner::verifyOracle(sig, digest));
				}
				*/
			}
				//std::cout << t.DebugString() << "\n";//process(t)
			//mRecorder.comitBlock(block);
		}
	}

};

}


#endif

//
//  Source.h
//  fantasybit
//
//  Created by Jay Berg on 8/24/14.
//
//

#ifndef __fantasybit__Processor__
#define __fantasybit__Processor__

#include <utility>
#include "Commissioner.h"
#include "ProtoData.pb.h"
#include <fc/crypto/sha256.hpp>
#include <fc/crypto/elliptic.hpp>
#include <algorithm>
#include <iostream>
#include "fbutils.h";
#include "Source.h";
#include <leveldb/db.h>
#include <fc/filesystem.hpp>
//#include <vector>


#include "nn.hpp"
#include "nn.h"
#include <nanomsg\reqrep.h>
#include <nanomsg/pair.h>
#include <nanomsg\pubsub.h>
#include <assert.h>
#include <string>
#include "MsgSock.h"
#include "boostLog.h"

#define LOG(logger, severity) LOGIT(logger, severity,  __FILE__, __LINE__, __FUNCTION__)


namespace fantasybit
{

	//using namespace leveldb;

//using comish = fantasybit::Commissioner;
//using ldbP = std::unique_ptr < leveldb::DB > ;
class BlockRecorder 
{
	leveldb::DB *blockstatus;
	leveldb::DB *namehashpub;
	leveldb::DB *pubfantasyname;
	leveldb::DB *pubbalance;
	leveldb::DB *gamesids;
	//leveldb::DB *projections;
	leveldb::WriteOptions write_sync{};

	std::map<Uid, leveldb::DB*> game_projections_db{};
	//std::vector<leveldb::DB*> alldbs{};
	int lastBlock = 0;
public:
	BlockRecorder() {}

	void init()
	{
		write_sync.sync = true;
		leveldb::Options options;
		options.create_if_missing = true;
		leveldb::Status status;
		status = leveldb::DB::Open(options, filedir("blockstatus"), &blockstatus);
		assert(status.ok());
		status = leveldb::DB::Open(options, filedir("namehashpub"), &namehashpub);
		assert(status.ok());
		status = leveldb::DB::Open(options, filedir("pubfantasyname"), &pubfantasyname);
		assert(status.ok());
		status = leveldb::DB::Open(options, filedir("pubbalance"), &pubbalance);
		assert(status.ok());
		status = leveldb::DB::Open(options, filedir("gameids"), &gamesids);
		assert(status.ok());
		//status = leveldb::DB::Open(options, filedir("projections"), &pubbalance);
		//status = leveldb::DB::Open(options, filedir("last-projections"), &pubbalance);
		leveldb::Iterator* it = gamesids->NewIterator(leveldb::ReadOptions());
		for (it->SeekToFirst(); it->Valid(); it->Next())
		{
			leveldb::DB* db;
			Uid id = it->key().ToString();
			if (leveldb::DB::Open(options, filedir("gameids/")+id, &db).ok())
			{
				game_projections_db[id] = db;
			}
		}
		delete it;

		std::string value;
		status = blockstatus->Get(leveldb::ReadOptions(), "lastblock", &value);
		if (!status.ok())
			lastBlock =  0;
		else 
			lastBlock = *(reinterpret_cast<const int *>(value.data()));

		it = namehashpub->NewIterator(leveldb::ReadOptions());
		std::string fname;
		for (it->SeekToFirst(); it->Valid(); it->Next()) {
			if (pubfantasyname->Get(leveldb::ReadOptions(), it->value(), &fname).IsNotFound())
				continue;
			auto pfn = Commissioner::makeName(fname, it->value().ToString());
			Commissioner::Aliases.emplace(pfn->hash(), pfn->pubkey);
			Commissioner::FantasyNames.emplace(pfn->pubkey, pfn);
			uint64_t newval = 0;
			std::string temp;
			if (pubbalance->Get(leveldb::ReadOptions(), it->value(), &temp).IsNotFound())
				continue;
			
			newval = *(reinterpret_cast<const uint64_t *>(temp.data()));
			pfn->addBalance(newval);
		}
		delete it;
	}

	void startBlock(int num) 
	{
		leveldb::Slice value((char*)&num, sizeof(int));
		blockstatus->Put(write_sync, "processing", value);
		blockstatus->Put(leveldb::WriteOptions(), "lastblock", value);
	}

	int endBlock(int num)
	{
		int none = -1;
		leveldb::Slice value((char*)&none, sizeof(int));
		blockstatus->Put(write_sync, "processing", value);
		return num;
	}

	bool isValid()
	{
		std::string value;
		if (blockstatus->Get(leveldb::ReadOptions(), "processing", &value).IsNotFound())
			return false; 

		int num = *(reinterpret_cast<const int *>(value.data()));
		return num < 0;
	}

	int getLastBlockId()
	{
		return lastBlock;
	}

	void recordName(const hash_t &hash,const std::string &pubkey,const std::string &name)
	{
		leveldb::Slice hkey((char*)&hash, sizeof(hash_t));
		namehashpub->Put(leveldb::WriteOptions(), hkey, pubkey);
		pubfantasyname->Put(leveldb::WriteOptions(), pubkey, name);

		std::string temp;
		if (pubbalance->Get(leveldb::ReadOptions(), pubkey, &temp).IsNotFound())
		{
			uint64_t bal = 0;
			leveldb::Slice bval((char*)&bal, sizeof(uint64_t));
			pubbalance->Put(leveldb::WriteOptions(), pubkey, bval);
		}
	}

	void addProjection(Uid &game, Uid &player, std::string &fname, int16_t points)
	{
		std::string temp;
		//if (pubbalance->Get(leveldb::ReadOptions(), pubkey, &temp).IsNotFound())
		leveldb::Slice pval((char*)&points, sizeof(int16_t));

		auto iter = game_projections_db.find(game);
		if (iter != end(game_projections_db))
		{
			iter->second->Put(leveldb::WriteOptions(), player.append(":").append(fname), pval);
		}
	}

	void removeGameId(Uid &game)
	{
		auto iter = game_projections_db.find(game);
		if (iter != end(game_projections_db))
		{
			delete iter->second;
			leveldb::DestroyDB(filedir(iter->first),leveldb::Options());
			gamesids->Delete(leveldb::WriteOptions(), game);
		}
	}

	void addGameId(Uid &game)
	{
		gamesids->Put(leveldb::WriteOptions(), game, game);
		leveldb::Options options;
		options.create_if_missing = true;
		leveldb::DB* db;
		if (leveldb::DB::Open(options, filedir(game), &db).ok())
			game_projections_db[game] = db;
	}

	void addBalance(std::string &pubkey,uint64_t add)
	{
		uint64_t newval = 0;
		std::string temp;

		if (pubbalance->Get(leveldb::ReadOptions(), pubkey, &temp).ok())
		{
			newval = *(reinterpret_cast<const uint64_t *>(temp.data()));
		}

		add += newval;
		leveldb::Slice bval((char*)&add, sizeof(uint64_t));
		pubbalance->Put(leveldb::WriteOptions(), pubkey, bval);
		auto it = Commissioner::FantasyNames.find(Commissioner::str2pk(pubkey));
		if (it != end(Commissioner::FantasyNames))
			it->second->addBalance(add);
	}

	std::string filedir(const std::string &in)
	{
		return ROOT_DIR + "index/" + in;
	}

	void closeAll()
	{
		delete blockstatus;
		delete namehashpub;
		delete pubfantasyname;
		delete pubbalance;
		delete gamesids;
		for (auto p : game_projections_db)
			delete p.second;
		game_projections_db.clear();
		//delete projections;
	}
};

class BlockProcessor
{
	BlockRecorder mRecorder{};
	bool verify_name(const SignedTransaction &, const NameTrans &, const fc::ecc::signature&, const fc::sha256 &);
	NodeRequest nodereq{};
	NodeReply noderep{};
	nn::socket syncserv;
	std::pair<Sender, Receiver> syncradio;
	int syncservid;
	int realHeight = 0;

	Receiver rec_block;
	nn::socket subblock;
	int pubblockid;
	int lastidprocessed = 0;
public:
	volatile bool running = true;
	BlockProcessor() :
		syncserv{ AF_SP, NN_REQ },
		syncradio{ std::make_pair(Sender(syncserv), Receiver(syncserv)) } ,
		subblock{ AF_SP, NN_SUB },
		rec_block{ Receiver(subblock) }

	{
		syncservid = syncserv.connect("inproc://syncserv");
		subblock.setsockopt(NN_SUB, NN_SUB_SUBSCRIBE, "", 0);
		subblock.connect("inproc://pubblock");
	}

	void stop() { running = false; }

	void run()
	{	
		init();
		Block sb{};
		while (running && !isInSync())
			GetInSync(lastidprocessed + 1, realHeight);

		while (running)
		{
			if (!rec_block.receive(sb)) continue;

			if (sb.signedhead().head().num() > lastidprocessed + 1)
				GetInSync(lastidprocessed + 1, sb.signedhead().head().num());
			
			if (sb.signedhead().head().num() == lastidprocessed + 1)
				process(sb);
		}
	}
		
	void init() 
	{
		mRecorder.init();
		if (!mRecorder.isValid()) {
			LOG(lg, info) << "mRecorder not valid! ";
			mRecorder.closeAll();
			fc::remove_all(ROOT_DIR + "index/");
			mRecorder.init();
		}

		lastidprocessed =  mRecorder.getLastBlockId();

		if (!isInSync()) {
			LOG(lg, info) << "! isInSync lastidprocessed " << lastidprocessed;
			GetInSync(lastidprocessed + 1, realHeight);
		}
		else 
			LOG(lg, info) << "lastidprocessed " << lastidprocessed;

	}

	bool isInSync()
	{
		NodeRequest nrq{};
		nrq.set_type(NodeRequest_Type_HIGHT_REQUEST);
		LOG(lg, trace) << "send " << NodeRequest_Type_Name(nrq.type());
		syncradio.first.send(nrq);
		NodeReply nrp{};
		if (!syncradio.second.receive(nrp)) {
			LOG(lg, error) << "!syncradio.second.receive(nrp) ";
			return true;
		}

		LOG(lg, info) << "rec " << nrp.DebugString();

		realHeight = nrp.hight();
		return (realHeight == lastidprocessed);

	}

	void GetInSync(int start,int end)
	{
		LOG(lg, info) << "GetInSync from " << start << " to " << end;

		int lastid = start;
		while (true)
		{
			NodeRequest nrq{};
			nrq.set_type(NodeRequest_Type_BLOCK_REQUEST);
			nrq.set_num(lastid);
			LOG(lg, trace) << "send " << nrq.DebugString();
			syncradio.first.send(nrq);
			Block sb{};
			if (!syncradio.second.receive(sb)) {
				LOG(lg, error) << "!syncradio.second.receive(sb) ";
				break;
			}

			LOG(lg, trace) << "rec " << sb.DebugString();

			if (sb.signedhead().head().num() != lastid) {
				LOG(lg, error) << "sb.signedhead().head().num() != lastid " << lastid;
				break;
			}

			if (!process(sb)) {
				LOG(lg, error) << "!process(sb) ";
				break;
			}
			if (end == lastid) break;
			lastid++;
		}
	}

	bool process(Block &sblock)
	{
		LOG(lg, trace) << "process: " << sblock.DebugString();

		if (!verifySignedBlock(sblock)) {
			LOG(lg, error) << "verifySignedBlock failed! ";
			return false;
		}

		mRecorder.startBlock(sblock.signedhead().head().num());
		for (const auto &st : sblock.signed_transactions())
		{
			Transaction t{ st.trans() };
			LOG(lg, trace) << "processing tx " << t.DebugString();// TransType_Name(t.type());

			fc::sha256 digest = fc::sha256::hash(t.SerializeAsString());
			if (digest.str() != st.id()) {
				LOG(lg, error) << "digest.str() != st.id() ";
				//fbutils::LogFalse(std::string("Processor::process signed transaction hash error digest ").append(st.DebugString()));
				continue;
			}

			if (t.version() != Commissioner::TRANS_VERSION) {
				LOG(lg, error) << "t.version() != Commissioner::TRANS_VERSION";
				//fbutils::LogFalse(std::string("Processor::process wrong transaction version ").append(st.DebugString()));
				continue;
			}

			fc::ecc::signature sig = Commissioner::str2sig(st.sig());

			if (t.type() == TransType::NAME)
			{
				auto nt = t.GetExtension(NameTrans::name_trans);
				if (verify_name(st, nt, sig, digest))
				{
					mRecorder.recordName(FantasyName::name_hash(nt.fantasy_name()), nt.public_key(), nt.fantasy_name());
					auto pfn = Commissioner::makeName(nt.fantasy_name(), nt.public_key());
					Commissioner::Aliases.emplace(pfn->hash(), pfn->pubkey);
					Commissioner::FantasyNames.emplace(pfn->pubkey, pfn);
					LOG(lg, error) << "verified! " << FantasyName::name_hash(nt.fantasy_name());

				}
				else {
					LOG(lg, error) << "!verify_name ";
				}

				continue;
			}

			if (!Commissioner::verifyByName(sig, digest, st.fantasy_name()))
			{
				LOG(lg, error) << "!Commissioner::verifyByName";
				//fbutils::LogFalse(std::string("Processor::process cant verify trans sig").append(st.DebugString()));
				continue;
			}

			switch (t.type())
			{
				case TransType::PROJECTION:
				{
					auto pt = t.GetExtension(ProjectionTrans::proj_trans);
					auto iter = Source::EventProjectionResult.find(Source::PlayerTeam[pt.fpp().playerid()]);
					if (iter != end(Source::EventProjectionResult))
					{
						auto pp = pt.fpp();
						iter->second.addProjection(st.fantasy_name(), pp.playerid(), pp.points());
					}
					break;
				}
				case TransType::RESULT:
				{
					/*todo: result is no longer a transaction - is part of data 
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
					*/
					break;
				}
				default:
					break;
			}
		}

		std::cout << " BLOCK(" << sblock.signedhead().head().num() << ") processed! \n";
		lastidprocessed = mRecorder.endBlock(sblock.signedhead().head().num());
		//sblock.block().head().num());
		return true;
			//std::cout << t.DebugString() << "\n";//process(t)
		//mRecorder.comitBlock(block);	
	}

	static bool verifySignedBlock(Block &sblock)
	{
		if (sblock.signedhead().head().version() != Commissioner::BLOCK_VERSION)
			return fbutils::LogFalse(std::string("Processor::process wrong block version ").append(sblock.DebugString()));

		fc::sha256 digest = fc::sha256::hash(sblock.signedhead().head().SerializeAsString());
		//if (digest.str() != sblock.signedhead().id())
		//	return
		//fbutils::LogFalse(std::string("Processor::process block hash error digest \n").append(sblock.DebugString()).append(digest.str()));
		//assert(digest.str() == sblock.id());

		fc::ecc::signature sig = Commissioner::str2sig(sblock.signedhead().sig());
		//assert(Commissioner::verifyOracle(sig, digest));
		if (!Commissioner::verifyOracle(sig, digest))
#ifdef NO_ORACLE_CHECK_TESTING
			if (!Commissioner::GENESIS_NUM == sblock.block().head().num())
#endif
				return fbutils::LogFalse(std::string("Processor::process only oracle can sign blocks!! ").append(sblock.DebugString()));

		return true;
	}
};

}


#endif

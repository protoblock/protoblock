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
#include "fbutils.h"
#include "Source.h"
#include <leveldb/db.h>
#include <fc/filesystem.hpp>
#include "DistributionAlgo.h"
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
	leveldb::DB *state;
	leveldb::DB *players;
	leveldb::DB *teams;
	leveldb::DB *projections;

	//leveldb::DB *gamesids;
	//leveldb::DB *projections;
	leveldb::WriteOptions write_sync{};

	//std::map<Uid, leveldb::DB*> game_projections_db{};
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
		status = leveldb::DB::Open(options, filedir("state"), &state);
		assert(status.ok());
		status = leveldb::DB::Open(options, filedir("players"), &players);
		assert(status.ok());
		status = leveldb::DB::Open(options, filedir("teams"), &teams);
		assert(status.ok());
		status = leveldb::DB::Open(options, filedir("projections"), &projections);
		assert(status.ok());


		std::string value;
		status = blockstatus->Get(leveldb::ReadOptions(), "lastblock", &value);
		if (!status.ok())
			lastBlock =  0;
		else 
			lastBlock = *(reinterpret_cast<const int *>(value.data()));

		auto *it = namehashpub->NewIterator(leveldb::ReadOptions());
		std::string fname;
		for (it->SeekToFirst(); it->Valid(); it->Next()) {
			if (pubfantasyname->Get(leveldb::ReadOptions(), it->value(), &fname).IsNotFound())
				continue;
			auto pfn = Commissioner::makeName(fname, it->value().ToString());
			Commissioner::Hash2Pk.emplace(pfn->hash(), pfn->pubkey);
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

	DeltaData DeltaSnap() {
		DeltaData dd{};

		FantasyPlayer fp{};
		for (auto pp : Commissioner::FantasyNames) {
			fp.set_name(pp.second->alias);
			fp.set_bits(pp.second->getBalance().amount());
			auto *p = dd.add_players();
			p->CopyFrom(fp);
		}

		dd.mutable_globalstate()->CopyFrom(GetGlobalState());

		/*
		TeamState ts{};
		auto it = state->NewIterator(leveldb::ReadOptions());
		for (it->SeekToFirst(); it->Valid(); it->Next()) {
			if (it->key().ToString() == "globalstate")
				continue;
			ts.ParseFromString(it->value().ToString());
			auto *p = dd.add_teamstates();
			p->CopyFrom(ts);
		}
		*/

		//std::unordered_map<std::string, std::string> PlayerTeam{};
		//std::map<std::string, std::unordered_set<std::string>> TeamPlayers{};

		Data d{};
		PlayerData pd{};
		auto it = players->NewIterator(leveldb::ReadOptions());
		for (it->SeekToFirst(); it->Valid(); it->Next()) {
			pd.ParseFromString(it->value().ToString());
			d.MutableExtension(PlayerData::player_data)->CopyFrom(pd);
			auto *p = dd.add_datas();
			p->CopyFrom(d);
			(Source::TeamPlayers[pd.teamid()]).insert(pd.playerid());
			Source::PlayerTeam.emplace(pd.playerid(), pd.teamid());
		}

		TeamData td{};
		it = teams->NewIterator(leveldb::ReadOptions());
		for (it->SeekToFirst(); it->Valid(); it->Next()) {
			td.ParseFromString(it->value().ToString());
			d.MutableExtension(TeamData::team_data)->CopyFrom(td);
			auto *p = dd.add_datas();
			p->CopyFrom(d);
		}

		for (auto tps : Source::TeamPlayers) {
			auto ts = GetTeamState(tps.first);
			auto *p = dd.add_teamstates();
			p->CopyFrom(ts);
		}

		return dd;
	}

	void OnGlobalState(const GlobalState &gs) {
		state->Put(leveldb::WriteOptions(), "globalstate", gs.SerializeAsString());
	}

	GlobalState GetGlobalState() {
		GlobalState gs{};
		std::string temp;
		if (state->Get(leveldb::ReadOptions(), "globalstate", &temp).ok()) {
			gs.ParseFromString(temp);
		}

		return gs;
	}

	void OnTeamState(const TeamState &gs) {
		state->Put(leveldb::WriteOptions(), gs.teamid(), gs.SerializeAsString());
	}

	TeamState GetTeamState(const std::string &teamid) {
		TeamState gs{};
		std::string temp;
		if (state->Get(leveldb::ReadOptions(),teamid, &temp).ok()) {
			gs.ParseFromString(temp);
		}

		return gs;
	}

	void OnPlayerData(const PlayerData &gs) {
		players->Put(leveldb::WriteOptions(), gs.playerid(), gs.SerializeAsString());
		Source::PlayerTeam[gs.playerid()] = gs.teamid();
		(Source::TeamPlayers[gs.teamid()]).insert(gs.playerid());
	}

	PlayerData GetPlayerData(const std::string &pid) {
		PlayerData gs{};
		std::string temp;
		if (players->Get(leveldb::ReadOptions(), pid, &temp).ok()) {
			gs.ParseFromString(temp);
		}

		return gs;
	}

	void OnTeamData(const TeamData &gs) {
		teams->Put(leveldb::WriteOptions(), gs.teamid(), gs.SerializeAsString());
	}

	TeamData GetTeamData(const std::string &pid) {
		TeamData gs{};
		std::string temp;
		if (teams->Get(leveldb::ReadOptions(), pid, &temp).ok()) {
			gs.ParseFromString(temp);
		}

		return gs;
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

	void addProjection(const std::string &fname, const FantasyPlayerPoints &fpp)
	{
		uint64_t bal = fpp.points();
		leveldb::Slice bval((char*)&bal, sizeof(uint64_t));

		std::string key{ fpp.playerid() };
		std::string fn{ fname };

		projections->Put(leveldb::WriteOptions(), key.append(":").append(fname), bval);
		auto iter = FantasyProjections::Projections.find(key);
		if (iter == end(FantasyProjections::Projections)) {
			//FantasyProjections::Projections.insert(key, { fn, bal });
			FantasyProjections::Projections[key][fn] = bal;
		}
		else
			iter->second[fname] = bal;	
	}

	void clearProjections() {
		auto *it = projections->NewIterator(leveldb::ReadOptions());
		for (it->SeekToFirst(); it->Valid(); it->Next())
			projections->Delete(leveldb::WriteOptions(), it->key());

		FantasyProjections::Projections.clear();
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
		delete state;
		delete players;
		delete teams;
		delete projections;

	}
};

class BlockProcessor
{
	BlockRecorder mRecorder{};
	bool verify_name(const SignedTransaction &, const NameTrans &, const fc::ecc::signature&, const fc::sha256 &);
	NodeRequest nodereq{};
	NodeReply noderep{};
	nn::socket syncserv, delasrv;
	std::pair<Sender, Receiver> syncradio;
	int syncservid;
	int realHeight = 0;

	Receiver rec_block;
	nn::socket subblock;
	int pubblockid;
	int lastidprocessed = 0;
	bool seen_insync = false;
	DeltaData outDelta{};
	GlobalState mGlobalState{};
public:
	volatile bool running = true;
	BlockProcessor(std::string deltaserveraddr) :
		delasrv{ AF_SP, NN_PAIR },
		syncserv{ AF_SP, NN_REQ },
		syncradio{ std::make_pair(Sender(syncserv), Receiver(syncserv)) } ,
		subblock{ AF_SP, NN_SUB },
		rec_block{ Receiver(subblock) }

	{
		delasrv.connect(deltaserveraddr.c_str());
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

		seen_insync = true;
		Sender::Send(delasrv,mRecorder.DeltaSnap());

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
		int count = 0;
		mRecorder.init();
		while (!mRecorder.isValid()) {
			LOG(lg, info) << "mRecorder not valid! ";
			mRecorder.closeAll();
			fc::remove_all(ROOT_DIR + "index/");
			mRecorder.init();
			if (count++ >= 5 && !mRecorder.isValid()) {
				std::string sfatal{ "mRecorder not valid after 5 tries " };
				LOG(lg, fatal) << sfatal;
				throw sfatal;
			}
		}

		lastidprocessed =  mRecorder.getLastBlockId();
		mGlobalState = mRecorder.GetGlobalState();

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
		if (sblock.signedhead().head().blocktype() == BlockHeader::Type::BlockHeader_Type_DATA) 
			processDataBlock(sblock);
		else 
			processTxfrom(sblock);

		
		std::cout << " BLOCK(" << sblock.signedhead().head().num() << ") processed! \n";
		lastidprocessed = mRecorder.endBlock(sblock.signedhead().head().num());
		//sblock.block().head().num());

		Sender::Send(delasrv, outDelta);
		outDelta.Clear();
		return true;
	}


	bool processDataBlock(const Block &sblock) {
		if (sblock.signed_transactions_size() < 1) {
			LOG(lg, error) << "expect Transition for Data block";
			return false;
		}

		auto st = sblock.signed_transactions(0);
		if (st.trans().type() != TransType::DATA)  {
			LOG(lg, error) << "expect first Transaction for Data block to be Data";
			return false;
		}

		auto dt = st.trans().GetExtension(DataTransition::data_trans);
		if (dt.data_size() > 0)
			process(dt.data(), st.fantasy_name());

		if (sblock.signed_transactions_size() > 1)
			processTxfrom(sblock, 1);

		process(dt);

		return true;
	}

	/*
	enum Data_Type {
		Data_Type_SCHEDULE = 0,
		Data_Type_PLAYER = 1,
		Data_Type_TEAM = 2,
		Data_Type_RESULT = 3
	};
	*/

	//process data array from DataTransition transaction
	void process(decltype(DataTransition::default_instance().data()) in, 
				const std::string &blocksigner )  {

		//outDelta.mutable_datas()->CopyFrom(in);

		for (const auto d : in) {
			Data *nd;
			PlayerData tpd{};
			TeamData ttd{};
			ResultData rd{};
			switch (d.type()) {
				case Data_Type_PLAYER:
					nd = outDelta.add_datas();
					nd->CopyFrom(d);
					tpd = d.GetExtension(PlayerData::player_data);
					mRecorder.OnPlayerData(tpd);
					break;
				case Data_Type_TEAM:
					nd = outDelta.add_datas();
					nd->CopyFrom(d);
					ttd = d.GetExtension(TeamData::team_data);
					mRecorder.OnTeamData(ttd);
					break;
				case Data_Type_RESULT:
				{
					rd = d.GetExtension(ResultData::result_data);

					//	auto it = FantasyProjections::Projections.find(rd.fpp().playerid());
					//	if (it == end(FantasyProjections::Projections))

					auto namevalpair = FantasyProjections::Projections[rd.fpp().playerid()];
					DistribuePointsAvg dist(namevalpair);
					auto rewards = dist.distribute(rd.fpp().points(), blocksigner);

					for (auto nr : rewards) {
						auto fn = Commissioner::getName(nr.first);
						uint64_t reward = static_cast<uint64_t>(nr.second);
						mRecorder.addBalance(Commissioner::pk2str(fn->pubkey), reward);
						LOG(lg, trace) << " reward " << fn->alias << " with "
							<< nr.second << " bal " << fn->getBalance().amount();

						FantasyPlayer fp{};
						fp.set_name(fn->alias);
						fp.set_bits(reward);
						outDelta.add_players()->CopyFrom(fp);
					}
					break;
				}

				default:
					LOG(lg, error) << "unexpedted type" << d.type();
					break;
			}
		}
	}

	void process(const DataTransition &indt) {
		switch (indt.type())
		{
		case DataTransition_Type_ROSTER:
			if (mGlobalState.state() != GlobalState_State_PRESEASON) 
				LOG(lg, warning) << indt.type() << " baad transition for current state " << mGlobalState.state();
	
			{	
				LOG(lg, info) << "53 man roster Transition!";
				if (mGlobalState.season() != indt.season()) {
					LOG(lg, warning) << "wrong season! " << indt.DebugString();
					mGlobalState.set_season(indt.season());
				}
				mGlobalState.set_state(GlobalState_State_ROSTER53MAN);
				mRecorder.OnGlobalState(mGlobalState);
				outDelta.mutable_globalstate()->CopyFrom(mGlobalState);
			}
			break;
		case DataTransition_Type_SEASONSTART:
			if (mGlobalState.state() != GlobalState_State_ROSTER53MAN) 
				LOG(lg, warning) << indt.type() << " baad transition for current state " << mGlobalState.state();

			{
				LOG(lg, info) << indt.season() << " Season Start ";
				if (mGlobalState.season() != indt.season()) {
					LOG(lg, warning) << "wrong season! " << indt.DebugString();
					mGlobalState.set_season(indt.season());
				}
				mGlobalState.set_state(GlobalState_State_INSEASON);
				mRecorder.OnGlobalState(mGlobalState);
				outDelta.mutable_globalstate()->CopyFrom(mGlobalState);
			}
			break;
		case DataTransition_Type_SEASONEND:
			if (mGlobalState.state() != GlobalState_State_INSEASON)
				LOG(lg, warning) << indt.type() << " baad transition for current state " << mGlobalState.state();

			
			LOG(lg, info) << indt.season() << " Season End :( ";

			if (mGlobalState.season() == indt.season() - 1) {}
			else if (mGlobalState.season() != indt.season()) {
				LOG(lg, warning) << "wrong season! " << indt.DebugString();
				mGlobalState.set_season(indt.season());
			}

			mGlobalState.set_season(mGlobalState.season() + 1);

			mGlobalState.set_state(GlobalState_State_PREDRAFT);
			mRecorder.OnGlobalState(mGlobalState);

			outDelta.mutable_globalstate()->CopyFrom(mGlobalState);
			break;

		case DataTransition_Type_DRAFTOVER:
			if (mGlobalState.state() != GlobalState_State_PRESEASON)
				LOG(lg, warning) << indt.type() << " baad transition for current state " << mGlobalState.state();

			{
				LOG(lg, info) << indt.season() << " nfl Draft over ";
				if (mGlobalState.season() != indt.season()) {
					LOG(lg, warning) << "wrong season! " << indt.DebugString();
					mGlobalState.set_season(indt.season());
				}
				mGlobalState.set_state(GlobalState_State_PRESEASON);
				mRecorder.OnGlobalState(mGlobalState);
				outDelta.mutable_globalstate()->CopyFrom(mGlobalState);
			}
			break;
		case DataTransition_Type_HEARTBEAT:
			//todo: deal w data in this msg
			break;
		case DataTransition_Type_GAMESTART:
			for (auto t : indt.teamid()) {
				auto ts = mRecorder.GetTeamState(t);
				ts.set_state(TeamState_State_INGAME);
				ts.set_week(indt.week());
				ts.set_teamid(t);
				mRecorder.OnTeamState(ts);
				LOG(lg, info) << " Kickoff for team " << t;
				outDelta.add_teamstates()->CopyFrom(ts);
			}
			break;
		case DataTransition_Type_WEEKOVER:
		{
			if (mGlobalState.state() != GlobalState_State_INSEASON)
				LOG(lg, warning) << indt.type() << " baad transition for current state " << mGlobalState.state();


			int newweek = indt.week() + 1;
			LOG(lg, info) << "week " << indt.week() << " Over ";
			if (indt.week() == 16) {
				newweek = 1;
				LOG(lg, info) << "season " << indt.season() << " Over ";
				mGlobalState.set_state(GlobalState_State_PRESEASON);
				mGlobalState.set_season(mGlobalState.season() + 1);
				mRecorder.OnGlobalState(mGlobalState);
				outDelta.mutable_globalstate()->CopyFrom(mGlobalState);
			}

			for (auto t : indt.teamid()) {
				auto ts = mRecorder.GetTeamState(t);
				ts.set_state(TeamState_State_PREGAME);
				ts.set_week(newweek);
				ts.set_teamid(t);
				mRecorder.OnTeamState(ts);
				outDelta.add_teamstates()->CopyFrom(ts);
			}

			mRecorder.clearProjections();
			break;
		}
		default:
			break;
		}
	}

	bool isValidTx(const SignedTransaction &st) {
		Transaction t{ st.trans() };
		fc::sha256 digest = fc::sha256::hash(t.SerializeAsString());
		if (digest.str() != st.id()) {
			LOG(lg, error) << "digest.str() != st.id() ";
			//fbutils::LogFalse(std::string("Processor::process signed transaction hash error digest ").append(st.DebugString()));
			return false;
		}

		if (t.version() != Commissioner::TRANS_VERSION) {
			LOG(lg, error) << "t.version() != Commissioner::TRANS_VERSION";
			//fbutils::LogFalse(std::string("Processor::process wrong transaction version ").append(st.DebugString()));
			return false;
		}

		fc::ecc::signature sig = Commissioner::str2sig(st.sig());
		if (t.type() == TransType::NAME) {
			auto nt = t.GetExtension(NameTrans::name_trans);
			if (!verify_name(st, nt, sig, digest))
				return false;
		}

		else if (!Commissioner::verifyByName(sig, digest, st.fantasy_name()))
		{
			LOG(lg, error) << "!Commissioner::verifyByName";
			//fbutils::LogFalse(std::string("Processor::process cant verify trans sig").append(st.DebugString()));
			return false;;
		}

		return true;
	}

	void processTxfrom(const Block &b,int start = 0) {

		for (int i = start; i < b.signed_transactions_size(); i++)
		{
			auto st = b.signed_transactions(i);
			Transaction t{ st.trans() };
			fc::sha256 digest = fc::sha256::hash(t.SerializeAsString());

			LOG(lg, trace) << "processing tx " << t.DebugString();// TransType_Name(t.type());

			if (!isValidTx(st)) continue;

			switch (t.type())
			{
			case TransType::PROJECTION:
			{
				auto pt = t.GetExtension(ProjectionTrans::proj_trans);
				mRecorder.addProjection(st.fantasy_name(), pt.fpp());
				LOG(lg, trace) << "new projection ";
				break;
			}

			case TransType::NAME:
			{
				auto nt = t.GetExtension(NameTrans::name_trans);
				mRecorder.recordName(FantasyName::name_hash(nt.fantasy_name()), nt.public_key(), nt.fantasy_name());
				auto pfn = Commissioner::makeName(nt.fantasy_name(), nt.public_key());
				Commissioner::Hash2Pk.emplace(pfn->hash(), pfn->pubkey);
				Commissioner::FantasyNames.emplace(pfn->pubkey, pfn);
				LOG(lg, error) << "verified! " << FantasyName::name_hash(nt.fantasy_name());
				FantasyPlayer fp{};
				fp.set_name(nt.fantasy_name());
				fp.set_bits(0);
				outDelta.add_players()->CopyFrom(fp);
			}
			break;
			default:
				break;
			}
		}
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
//iter->second.	addProjection(st.fantasy_name(), pp.fantasy_player_id(), pp.points());
}
*/

//outDelta.players(); - balance 

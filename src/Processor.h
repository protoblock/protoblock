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
#include <nanomsg\nn.h>
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
	leveldb::DB *blockstatus;  // lastblock -> status // last block processed 
	leveldb::DB *namehashpub;  // FantasyName::Hash -> public_key  
	leveldb::DB *pubfantasyname; // public_key ->FantasyName 
	leveldb::DB *pubbalance; // public_key -> fantasybit balance
	leveldb::DB *state; // GlobalState and TeamState
	leveldb::DB *players; // NFL playerid -> PlayerData
	leveldb::DB *teams; // NFL teamid -> TeamData
	leveldb::DB *projections; // playerid:FantasyName -> projection

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
        if (!status.ok()) {
			lastBlock =  0;
            LOG(lg,warning) << "!ok no blocks";
        }
        else {
			lastBlock = *(reinterpret_cast<const int *>(value.data()));
            LOG(lg,info) << "lastBLock: " << lastBlock;
        }
	}

    void initAllData() {
        initFantasyNames();
        initProjections();
        initData();
    }

    void initFantasyNames() {
        auto *it = namehashpub->NewIterator(leveldb::ReadOptions());
        std::string fname;
        for (it->SeekToFirst(); it->Valid(); it->Next()) {
            if (pubfantasyname->Get(leveldb::ReadOptions(), it->value(), &fname).IsNotFound()) {
                LOG(lg,warning) << it->value().ToString()
                                << " found namehash but NO fantasyname ";
                continue;
            }

            auto pfn = Commissioner::makeName(fname, it->value().ToString());
            Commissioner::Hash2Pk.emplace(pfn->hash(), pfn->pubkey);
            Commissioner::FantasyNames.emplace(pfn->pubkey, pfn);
            uint64_t newval = 0;
            std::string temp;
            if (pubbalance->Get(leveldb::ReadOptions(), it->value(), &temp).IsNotFound())
            {
                LOG(lg,warning) << "nno bal " << pfn->ToString();
                continue;
            }

            newval = *(reinterpret_cast<const uint64_t *>(temp.data()));
            pfn->addBalance(newval);
            LOG(lg,trace) << pfn->ToString();
        }
        delete it;
    }

	void initProjections() {
		auto *it = projections->NewIterator(leveldb::ReadOptions());
		for (it->SeekToFirst(); it->Valid(); it->Next()) {

			auto str = it->key().ToString();
			int pos =  str.find_first_of(':');
			auto nflplayer = str.substr(0, pos);
			auto fantasyname = str.substr(pos + 1);

			uint64_t bal = *(reinterpret_cast<const uint64_t *>(it->value().data()));

			FantasyProjections::Projections[nflplayer][fantasyname] = bal;
            LOG(lg,trace) << str << ":" << bal;
		}
        delete it;
	}

    void initData() {
        PlayerData pd{};
        auto it = players->NewIterator(leveldb::ReadOptions());
        for (it->SeekToFirst(); it->Valid(); it->Next()) {
            pd.ParseFromString(it->value().ToString());
            (Source::TeamPlayers[pd.teamid()]).insert(pd.playerid());
            Source::PlayerTeam.emplace(pd.playerid(), pd.teamid());
            LOG(lg,trace) << "Player:" << pd.playerid() << ":" << pd.teamid();
        }

        TeamData td{};
        it = teams->NewIterator(leveldb::ReadOptions());
        for (it->SeekToFirst(); it->Valid(); it->Next()) {
            td.ParseFromString(it->value().ToString());
            Source::TeamIds.insert(td.teamid());
            LOG(lg,trace) << td.teamid();
        }

        TeamState ts{};
        it = state->NewIterator(leveldb::ReadOptions());
        for (it->SeekToFirst(); it->Valid(); it->Next()) {
            if (it->key().ToString() == "globalstate")
                continue;
            ts.ParseFromString(it->value().ToString());
            Source::TeamWeek[ts.teamid()] =
                    std::make_pair(ts.week(), ts.state() == TeamState::INGAME);
            LOG(lg,trace) << ts.teamid() << " " << ts.state() << " week" << ts.week();
        }
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
		
		TeamState ts{};
		auto it = state->NewIterator(leveldb::ReadOptions());
		for (it->SeekToFirst(); it->Valid(); it->Next()) {
			if (it->key().ToString() == "globalstate")
				continue;
			ts.ParseFromString(it->value().ToString());
			auto *p = dd.add_teamstates();
			p->CopyFrom(ts);
		}

		//std::unordered_map<std::string, std::string> PlayerTeam{};
		//std::map<std::string, std::unordered_set<std::string>> TeamPlayers{};

		Data d{};
		PlayerData pd{};
		it = players->NewIterator(leveldb::ReadOptions());
		for (it->SeekToFirst(); it->Valid(); it->Next()) {
			pd.ParseFromString(it->value().ToString());
			d.set_type(Data_Type::Data_Type_PLAYER);
			d.MutableExtension(PlayerData::player_data)->CopyFrom(pd);
			auto *p = dd.add_datas();
			p->CopyFrom(d);
			(Source::TeamPlayers[pd.teamid()]).insert(pd.playerid());
			Source::PlayerTeam.emplace(pd.playerid(), pd.teamid());
		}

		d.Clear();
		TeamData td{};
		it = teams->NewIterator(leveldb::ReadOptions());
		for (it->SeekToFirst(); it->Valid(); it->Next()) {
			td.ParseFromString(it->value().ToString());
			d.set_type(Data_Type::Data_Type_TEAM);
			d.MutableExtension(TeamData::team_data)->CopyFrom(td);
			auto *p = dd.add_datas();
			p->CopyFrom(d);
            Source::TeamIds.insert(td.teamid());
		}

		/*
		for (auto tps : Source::TeamPlayers) {
			auto ts = GetTeamState(tps.first);
			auto *p = dd.add_teamstates();
			p->CopyFrom(ts);
		}
		*/

		return dd;
	}


	void OnGlobalState(const GlobalState &gs) {
		state->Put(leveldb::WriteOptions(), "globalstate", gs.SerializeAsString());
        LOG(lg,trace) << gs.DebugString();
	}

	GlobalState GetGlobalState() {
		GlobalState gs{};
		std::string temp;
		if (state->Get(leveldb::ReadOptions(), "globalstate", &temp).ok()) {
			gs.ParseFromString(temp);
		}
        else {
            LOG(lg,error) << "No GlobalState";
        }

		return gs;
	}

	void OnTeamState(const TeamState &gs) {
		state->Put(leveldb::WriteOptions(), gs.teamid(), gs.SerializeAsString());
        Source::TeamWeek[gs.teamid()] = std::make_pair(gs.week(), gs.state() == TeamState::INGAME);
        LOG(lg,trace) << gs.DebugString();
    }

	TeamState GetTeamState(const std::string &teamid) {
		TeamState gs{};
		std::string temp;
		if (state->Get(leveldb::ReadOptions(),teamid, &temp).ok()) {
            gs.ParseFromString(temp);
		}
        else {
            LOG(lg,error) << teamid << ": No TeamState";
        }

		return gs;
	}

	void OnPlayerData(const PlayerData &gs) {
		players->Put(leveldb::WriteOptions(), gs.playerid(), gs.SerializeAsString());
		Source::PlayerTeam[gs.playerid()] = gs.teamid();
		(Source::TeamPlayers[gs.teamid()]).insert(gs.playerid());
        LOG(lg,trace) << gs.DebugString();
	}

	PlayerData GetPlayerData(const std::string &pid) {
		PlayerData gs{};
		std::string temp;
		if (players->Get(leveldb::ReadOptions(), pid, &temp).ok()) {
			gs.ParseFromString(temp);
		}
        else LOG(lg,error) << pid << " no PlayerData";

		return gs;
	}

	void OnTeamData(const TeamData &gs) {
		teams->Put(leveldb::WriteOptions(), gs.teamid(), gs.SerializeAsString());
        Source::TeamIds.insert(gs.teamid());
        LOG(lg,trace) << gs.DebugString();
	}

	TeamData GetTeamData(const std::string &pid) {
		TeamData gs{};
		std::string temp;
		if (teams->Get(leveldb::ReadOptions(), pid, &temp).ok()) {
			gs.ParseFromString(temp);
		}
        else LOG(lg,error) << pid << ": no TeamData";

		return gs;
	}


	void startBlock(int num) 
	{
		leveldb::Slice value((char*)&num, sizeof(int));
		blockstatus->Put(write_sync, "processing", value);
		blockstatus->Put(leveldb::WriteOptions(), "lastblock", value);
        LOG(lg,info) << "starting block: " << num;
	}

	int endBlock(int num)
	{
		int none = -1;
		leveldb::Slice value((char*)&none, sizeof(int));
		blockstatus->Put(write_sync, "processing", value);
        LOG(lg,info) << "end block: " << num;
		return num;
	}

	bool isValid()
	{
		std::string value;
		if (blockstatus->Get(leveldb::ReadOptions(), "processing", &value).IsNotFound())
			return true; 

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

        LOG(lg,trace) << name << ":" << hash << ":" << pubkey;
	}

	void addProjection(const std::string &fname, const FantasyPlayerPoints &fpp)
	{
		uint64_t bal = fpp.points();
		leveldb::Slice bval((char*)&bal, sizeof(uint64_t));

		std::string key{ fpp.playerid() };
		std::string fn{ fname };

		projections->Put(leveldb::WriteOptions(), key.append(":").append(fname), bval);
		auto iter = FantasyProjections::Projections.find(fpp.playerid());
		if (iter == end(FantasyProjections::Projections)) {
			//FantasyProjections::Projections.insert(key, { fn, bal });
			FantasyProjections::Projections[fpp.playerid()][fn] = bal;
		}
		else
			iter->second[fname] = bal;	

        LOG(lg,trace) << "proj: " << key << ":" << fname << ":" << bal;
	}

	void clearProjections() {
		auto *it = projections->NewIterator(leveldb::ReadOptions());
		for (it->SeekToFirst(); it->Valid(); it->Next())
			projections->Delete(leveldb::WriteOptions(), it->key());

		FantasyProjections::Projections.clear();
        LOG(lg,trace) << " clearProjections ";
	}

	void addBalance(std::string &pubkey,uint64_t add)
	{
        uint64_t curr = 0;
		std::string temp;

		if (pubbalance->Get(leveldb::ReadOptions(), pubkey, &temp).ok())
		{
            curr = *(reinterpret_cast<const uint64_t *>(temp.data()));
		}

        LOG(lg,trace) << pubkey << " bal " << curr << "+" << add;

        uint64_t newval = curr + add;
        leveldb::Slice bval((char*)&newval, sizeof(uint64_t));
		pubbalance->Put(leveldb::WriteOptions(), pubkey, bval);
		auto it = Commissioner::FantasyNames.find(Commissioner::str2pk(pubkey));
		if (it != end(Commissioner::FantasyNames))
			it->second->addBalance(add);
        else
            LOG(lg,error) << " cant find FantasyName";
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
        int timeout = 60000;
        subblock.setsockopt(NN_SOL_SOCKET, NN_RCVTIMEO, &timeout, sizeof(timeout));

		subblock.connect("inproc://pubblock");
	}

    void stop() {
        running = false;
        LOG(lg,info) << "stop";
    }

    void run() {
        try {
            runit();
        }
        catch (std::exception &e)
        {
            LOG(lg,fatal) <<  e.what();
            return;
        }
    }

    void runit() {
        LOG(lg,info) << "run";

		init();
        if (!running) {
            LOG(lg,warning) <<  " !running";
            return;
        }

		Block sb{};
        while (running && !isInSync()) {
            LOG(lg, info) << "!in sync last" << lastidprocessed << " real " << realHeight;
            GetInSync(lastidprocessed + 1, realHeight);
        }

        if (!running) {
            LOG(lg,warning) <<  " !running";
            return;
        }

		seen_insync = true;
        LOG(lg, info) << "YES in sync last" << lastidprocessed << " real " << realHeight;

		auto ds = mRecorder.DeltaSnap();
		LOG(lg, trace) << "sending DeltaSnap" << ds.DebugString();
		Sender::Send(delasrv,ds);

		while (running)
		{
            LOG(lg, trace) << "waiting for live block" << lastidprocessed + 1;
            if (!rec_block.receive(sb)) {
                LOG(lg, trace) << "timedout";

                while (running && !isInSync()) {
                    LOG(lg, info) << "!in sync last" << lastidprocessed << " real " << realHeight;
                    GetInSync(lastidprocessed + 1, realHeight);
                }
            }
            else {
                LOG(lg,info) << "Received live block" << sb.signedhead().head().num();
            }

            if (sb.signedhead().head().num() > lastidprocessed + 1) {
                LOG(lg,info) << "gap in block num last" << lastidprocessed;
				GetInSync(lastidprocessed + 1, sb.signedhead().head().num());
            }

            if (!running) {
                LOG(lg,warning) <<  " !running";
                return;
            }

            if (sb.signedhead().head().num() == lastidprocessed + 1) {
                LOG(lg, trace) << "received live block" << lastidprocessed + 1;
                if (!process(sb))
                    LOG(lg,error) <<  " !process live block";
                else
                    LOG(lg,trace) <<  " YES processed live block";
            }
		}

        LOG(lg,info) <<  "ok !running";
	}


	void init() 
	{
		int count = 0;
		mRecorder.init();
        while (!mRecorder.isValid() && running) {
			LOG(lg, info) << "mRecorder not valid! ";
			mRecorder.closeAll();
			fc::remove_all(ROOT_DIR + "index/");
            LOG(lg, info) << "delete all leveldb, should have nothing";
            mRecorder.init();
			if (count++ >= 5 && !mRecorder.isValid()) {
				std::string sfatal{ "mRecorder not valid after 5 tries " };
				LOG(lg, fatal) << sfatal;
				throw sfatal;
			}
		}

        if (!running) return;

        mRecorder.initAllData();
        LOG(lg, info) << "YES mRecorder is valid";

		lastidprocessed =  mRecorder.getLastBlockId();
		mGlobalState = mRecorder.GetGlobalState();

        LOG(lg, info) << "last: " << lastidprocessed << "globalstate " <<
                         mGlobalState.DebugString();

        if (!isInSync()) {
            LOG(lg, info) << "! isInSync lastidprocessed " << lastidprocessed << "real " << realHeight;
            GetInSync(lastidprocessed + 1, realHeight);
        }
        else
            LOG(lg, info) << "YES in sync lastidprocessed " << lastidprocessed;

	}

	bool isInSync()
	{
        while (running) {
            //std::this_thread::sleep_for(std::chrono::milliseconds{ 10 });

            NodeRequest nrq{};
            nrq.set_type(NodeRequest_Type_HIGHT_REQUEST);
            nrq.set_num(lastidprocessed);
            LOG(lg, trace) << "check in sync send " << NodeRequest_Type_Name(nrq.type());
            if ( syncradio.first.send(nrq) <= 0) {
                LOG(lg,error) << " sync radio send timeout";
                continue;
            }

            NodeReply nrp{};
            if (!syncradio.second.receive(nrp)) {
                LOG(lg, error) << "!syncradio.second.receive(nrp) ";
                continue;
            }

            LOG(lg, info) << "rec " << nrp.DebugString();

            realHeight = nrp.hight();

            if ( realHeight < lastidprocessed ) {
                std::string sfatal{ "real < last" };
                LOG(lg,warning) << " real < last " << realHeight << " < " << lastidprocessed;
                //throw sfatal;
            }
            break;
        }
        return (realHeight == lastidprocessed);
	}

    void GetInSync(int start,int end)
	{
		LOG(lg, info) << "GetInSync from " << start << " to " << end;

		int lastid = start;
        while (running)
		{
			NodeRequest nrq{};
			nrq.set_type(NodeRequest_Type_BLOCK_REQUEST);
			nrq.set_num(lastid);
			LOG(lg, trace) << "send " << nrq.DebugString();
            if ( syncradio.first.send(nrq) <= 0 ) {
                LOG(lg,error) << " syncradio send timeout ";
                continue;
            }
			Block sb{};
			if (!syncradio.second.receive(sb)) {
				LOG(lg, error) << "!syncradio.second.receive(sb) ";
                continue;
			}

			LOG(lg, trace) << "rec " << sb.DebugString();

			if (sb.signedhead().head().num() != lastid) {
				LOG(lg, error) << "sb.signedhead().head().num() != lastid " << lastid;
                continue;
			}

			if (!process(sb)) {
				LOG(lg, error) << "!process(sb) ";
                continue;
			}
            if (end == lastid)
                break;

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

		LOG(lg, trace) << " outDelta " << outDelta.DebugString();

		if (seen_insync)
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

    bool isInWeekGame(const std::string &id, int week ) {
        auto it = Source::TeamWeek.find(id);
        if ( it == end(Source::TeamWeek) )
            return false;

        return  (it->second.first == week ) && (it->second.second);
    }

    bool sanity(const FantasyPlayerPoints &fpp) {
        if ( !fpp.has_season() || !fpp.has_week()
             || !fpp.has_playerid() || !fpp.has_points() )
            return false;

        if ( fpp.season() != mGlobalState.season() ) return false;

        auto it = Source::PlayerTeam.find(fpp.playerid());
        if (it == end(Source::PlayerTeam))
            return false;

        return isInWeekGame(it->second,fpp.week());
    }


    //std::unordered_map<std::string,TeamState> mTeamStates{}

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
				case Data_Type_RESULT: {
					rd = d.GetExtension(ResultData::result_data);

                    if ( !sanity(rd.fpp()) ) {
                        LOG(lg, error) << " invalid result skipping" << rd.DebugString();
                        break;
                    }
					//	auto it = FantasyProjections::Projections.find(rd.fpp().playerid());
					//	if (it == end(FantasyProjections::Projections))

					LOG(lg, info) << " process result " << rd.fpp().DebugString();
					auto namevalpair = FantasyProjections::Projections[rd.fpp().playerid()];
					DistribuePointsAvg dist(namevalpair);
					auto rewards = dist.distribute(rd.fpp().points(), blocksigner);

					for (auto nr : rewards) {
						auto fn = Commissioner::getName(nr.first);
                        if ( fn == nullptr ) {
                            LOG(lg, error) << "cant find name" << nr.first;
                            continue;
                        }
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
                setPreGameWeek(1);
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

            setPreGameWeek(newweek);
            /*
			for (auto t : indt.teamid()) {
				auto ts = mRecorder.GetTeamState(t);
				ts.set_state(TeamState_State_PREGAME);
				ts.set_week(newweek);
				ts.set_teamid(t);
				mRecorder.OnTeamState(ts);
				outDelta.add_teamstates()->CopyFrom(ts);
			}
            */

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

        if ( st.fantasy_name() == "") {
            LOG(lg, error) << " Blank FantasyName";
            return false;;
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
				LOG(lg, trace) << st.fantasy_name() << "new projection " << pt.DebugString();
				mRecorder.addProjection(st.fantasy_name(), pt.fpp());
				break;
			}

			case TransType::NAME:
			{
				auto nt = t.GetExtension(NameTrans::name_trans);
				mRecorder.recordName(FantasyName::name_hash(nt.fantasy_name()), nt.public_key(), nt.fantasy_name());
				auto pfn = Commissioner::makeName(nt.fantasy_name(), nt.public_key());
				Commissioner::Hash2Pk.emplace(pfn->hash(), pfn->pubkey);
				Commissioner::FantasyNames.emplace(pfn->pubkey, pfn);
                LOG(lg, info) << "verified " << FantasyName::name_hash(nt.fantasy_name());
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
	
    void setPreGameWeek(int week) {
        TeamState ts{};
        ts.set_state(TeamState_State_PREGAME);
        ts.set_week(week);

        for ( auto id : Source::TeamIds) {
            ts.set_teamid(id);
            mRecorder.OnTeamState(ts);
            outDelta.add_teamstates()->CopyFrom(ts);
        }
    }

    static bool verifySignedBlock(const Block &sblock)
	{
        LOG(lg,trace) << sblock.DebugString();

		if (sblock.signedhead().head().version() != Commissioner::BLOCK_VERSION)
        {
            LOG(lg,error) << " !verifySignedBlock wrong block version! ";
            return false;
        }
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
            {
                LOG(lg,error) << " Invalid Block Signer!";
                return false;
            }

        return true;
	}

    static bool verifySignedTransaction(const SignedTransaction &st) {
        LOG(lg,trace) << st.DebugString();

        if (st.trans().version() != Commissioner::TRANS_VERSION)
        {
            LOG(lg,error) << " !verifySignedTransaction wrong trans version! ";
            return false;
        }

        fc::sha256 digest = fc::sha256::hash(st.trans().SerializeAsString());
        if (digest.str() != st.id()) {
            LOG(lg, error) << "digest.str() != st.id() ";
            return false;
        }

        if ( st.fantasy_name() == "") {
            LOG(lg, error) << " Blank FantasyName";
            return false;;
        }

        fc::ecc::signature sig = Commissioner::str2sig(st.sig());
        if (st.trans().type() != TransType::NAME)
            if (!Commissioner::verifyByName(sig, digest, st.fantasy_name()))
            {
                LOG(lg, error) << "!Commissioner::verifyByName";
                return false;;
            }

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

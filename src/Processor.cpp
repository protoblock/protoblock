//
//  Processor.cpp
//  fantasybit
//
//  Created by Jay Berg on 8/24/14.
//
//
#include <utility>
#include "Commissioner.h"
#include "ProtoData.pb.h"
#include <fc/crypto/sha256.hpp>
#include <fc/crypto/elliptic.hpp>
#include <algorithm>
#include <iostream>
#include "fbutils.h"
#include "Source.h"
#include "DistributionAlgo.h"
//#include <vector>
#include <assert.h>
#include <string>
//#include "MsgSock.h"
//#include "boostLog.h"

#include "blockrecorder.h"

#include "Processor.h"
#include "boostLog.h"
#define LOG(logger, severity) LOGIT(logger, severity,  __FILE__, __LINE__, __FUNCTION__)

namespace fantasybit
{
int BlockProcessor::init() {
    mRecorder.init();
    if (!mRecorder.isValid() ) {
        LOG(lg, info) << "mRecorder not valid! ";
        mRecorder.closeAll();
        fc::remove_all(GET_ROOT_DIR() + "index/");
        LOG(lg, info) << "delete all leveldb, should have nothing";
        mRecorder.init();
        if (!mRecorder.isValid() ) {
            LOG(lg, info) << "mRecorder not valid! ";
            return -1;
        }
    }

    mRecorder.initAllData();
    LOG(lg, info) << "YES mRecorder is valid";

    lastidprocessed =  mRecorder.getLastBlockId();
    mGlobalState = mRecorder.GetGlobalState();

    LOG(lg, info) << "last: " << lastidprocessed << "globalstate " <<
                     mGlobalState.DebugString();

    outDelta = mRecorder.DeltaSnap();
    return lastidprocessed;
}

DeltaData BlockProcessor::GetandClear() {
    auto hold = outDelta;
    outDelta.Clear();
    return hold;
}

bool BlockProcessor::process(Block &sblock) {
    LOG(lg, trace) << "process: " << sblock.DebugString();
    if (!verifySignedBlock(sblock)) {
        LOG(lg, error) << "verifySignedBlock failed! ";
        return -1;
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

    return lastidprocessed;
}

bool BlockProcessor::processDataBlock(const Block &sblock) {
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

bool BlockProcessor::isInWeekGame(const std::string &id, int week ) {
    auto it = Source::TeamWeek.find(id);
    if ( it == end(Source::TeamWeek) )
        return false;

    return  (it->second.first == week ) && (it->second.second);
}

bool BlockProcessor::sanity(const FantasyPlayerPoints &fpp) {
    if ( !fpp.has_season() || !fpp.has_week()
         || !fpp.has_playerid() || !fpp.has_points() )
        return false;

    if ( fpp.season() != mGlobalState.season() ) return false;

    auto it = Source::PlayerTeam.find(fpp.playerid());
    if (it == end(Source::PlayerTeam))
        return false;

    return isInWeekGame(it->second,fpp.week());
}

void BlockProcessor::process(decltype(DataTransition::default_instance().data()) in,
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

                double result = 0.0;
                if ( rd.fpp().has_result() )
                    result = rd.fpp().result();//static_cast<int>(rd.fpp().result() * 100.0);
                else if ( rd.fpp().has_points())
                    result = rd.fpp().points();// * 100;
                else {
                    LOG(lg,error) << " no points or results found";
                    break;
                }
                auto rewards = dist.distribute(result, blocksigner);

                for (auto nr : rewards) {
                    auto fn = Commissioner::getName(nr.first);
                    if ( fn == nullptr ) {
                        LOG(lg, error) << "cant find name" << nr.first;
                        continue;
                    }
                    int reward = nr.second;
                    mRecorder.addBalance(Commissioner::pk2str(fn->pubkey()), reward);
                    LOG(lg, trace) << " reward " << fn->alias() << " with "
                        << nr.second << " bal " << fn->getBalance();

                    FantasyPlayer fp{};
                    fp.set_name(fn->alias());
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

void BlockProcessor::process(const DataTransition &indt) {
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
        if (mGlobalState.state() != GlobalState_State_PREDRAFT)
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

bool BlockProcessor::isValidTx(const SignedTransaction &st) {
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

void BlockProcessor::processTxfrom(const Block &b,int start) {

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
            Commissioner::AddName(nt.fantasy_name(), nt.public_key());
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

void BlockProcessor::setPreGameWeek(int week) {
    TeamState ts{};
    ts.set_state(TeamState_State_PREGAME);
    ts.set_week(week);

    for ( auto id : Source::TeamIds) {
        ts.set_teamid(id);
        mRecorder.OnTeamState(ts);
        outDelta.add_teamstates()->CopyFrom(ts);
    }
}

bool BlockProcessor::verifySignedBlock(const Block &sblock)
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

bool BlockProcessor::verifySignedTransaction(const SignedTransaction &st) {
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
            LOG(lg, error) << "sig :" << st.sig();
            LOG(lg, error) << "st.fantasy_name() :" << st.fantasy_name();

            return false;;
        }

    return true;
}

bool BlockProcessor::verify_name(const SignedTransaction &st, const NameTrans &nt, 
	const fc::ecc::signature& sig, const fc::sha256 &digest) { 

    if ( !Commissioner::isAliasAvailable(nt.fantasy_name()) )
    {
        LOG(lg,error) << std::string("Processor::process failure: FantasyName(").
                        append(nt.fantasy_name() + ")  hash already used ");
        return false;
    }


    auto pk = Commissioner::str2pk(nt.public_key());
    auto name = Commissioner::getName(pk);
    if ( name != nullptr ) {
        LOG(lg,error) << std::string("verfiy_name failure: FantasyName(").
                        append(nt.fantasy_name() + ")  pubkey already n use") +
                         name->ToString();
                        //.append(st.DebugString());

        return false;
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
            {
                LOG(lg,error) << std::string("Processor::process name proof oracle failed")
                                 .append(st.DebugString());

                return false;
            }
            else
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


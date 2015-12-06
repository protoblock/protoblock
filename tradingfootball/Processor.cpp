//
//  Processor.cpp
//  fantasybit
//
//  Created by Jay Berg on 8/24/14.
//
//
#include <QApplication>

#include <utility>
#include "Commissioner.h"
#include "ProtoData.pb.h"
#include <fc/crypto/sha256.hpp>
#include <fc/crypto/elliptic.hpp>
#include <fc/filesystem.hpp>
#include <algorithm>
#include <iostream>
#include "fbutils.h"
#include "Source.h"
#include "DistributionAlgo.h"
#include "blockrecorder.h"
#include "Processor.h"
#include "platform.h"
#include "globals.h"
#include "ApiData.pb.h"
#include "RestFullCall.h"
#include "fbutils.h"
#ifdef DATAAGENTWRITENAMES
#include "playerloader.h"
#endif

namespace fantasybit
{

void BlockProcessor::hardReset() {
    mRecorder.closeAll();
    mData.closeAll();
    mNameData.closeAll();
#ifdef TRADE_FEATURE
    mExchangeData.closeAll();

    mExchangeData.removeAll();
#endif


    fc::remove_all(Platform::instance()->getRootDir() + "index/");
}

int32_t BlockProcessor::init() {
    mRecorder.init();
    if (!mRecorder.isValid() ) {
        emit InvalidState(mRecorder.getLastBlockId());
        qInfo() <<  "mRecorder not valid! ";
        mRecorder.closeAll();
        fc::remove_all(Platform::instance()->getRootDir() + "index/");
        qInfo() <<  "delete all leveldb, should have nothing";
        mRecorder.init();
        if (!mRecorder.isValid() ) {
            qInfo() <<  "mRecorder not valid! ";
            InvalidState(mRecorder.getLastBlockId());
            return -1;
        }
    }

    mData.init();
    mNameData.init();
#ifdef TRADE_FEATURE
    mExchangeData.init();
#endif


    qInfo() <<  "YES mRecorder is valid";

    lastidprocessed =  mRecorder.getLastBlockId();
    return lastidprocessed;
}


int32_t BlockProcessor::process(Block &sblock) {
    qDebug() << "process: " << sblock.signedhead().head().num();
    if (!verifySignedBlock(sblock)) {
        //qCritical() << "verifySignedBlock failed! ";
        qCritical() << "verifySignedBlock failed! ";
        return -1;
    }
    else {
        qInfo() << "yes verifySignedBlock " <<  sblock.signedhead().head().num();
    }

    mRecorder.startBlock(sblock.signedhead().head().num());
    if (sblock.signedhead().head().blocktype() == BlockHeader::Type::BlockHeader_Type_DATA)
        processDataBlock(sblock);
    else
        processTxfrom(sblock);

    qInfo() << " BLOCK(" << sblock.signedhead().head().num() << ") processed! ";
    lastidprocessed = mRecorder.endBlock(sblock.signedhead().head().num());

    //qDebug() << " outDelta " << outDelta.DebugString();

    return lastidprocessed;
}

bool BlockProcessor::processDataBlock(const Block &sblock) {
    if (sblock.signed_transactions_size() < 1) {
        qCritical() << "expect Transition for Data block";
        return false;
    }

    auto st = sblock.signed_transactions(0);
    if (st.trans().type() != TransType::DATA)  {
        qCritical() << "expect first Transaction for Data block to be Data";
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
*/
void BlockProcessor::process(decltype(DataTransition::default_instance().data()) in,
            const std::string &blocksigner )  {

    //outDelta.mutable_datas()->CopyFrom(in);

    for (const auto d : in) {
        Data *nd;
        PlayerData tpd{};
        GameData ttd{};
        ResultData rd{};
        switch (d.type()) {
            case Data_Type_PLAYER:
                tpd = d.GetExtension(PlayerData::player_data);
                if ( !tpd.has_playerid() ) {
                    qCritical() << "no playerid" + QTD(tpd.DebugString());
                    break;
                }
                if ( tpd.has_player_base() )
                    mData.AddNewPlayer(tpd.playerid(),tpd.player_base());
                if ( tpd.has_player_status() )
                    mData.UpdatePlayerStatus(tpd.playerid(),tpd.player_status());
                if ( tpd.has_player_game_status() )
                    ;//ToDo

                break;
            case Data_Type_GAME:
                ttd = d.GetExtension(GameData::game_data);
                if ( ttd.has_status() && ttd.has_gameid() )
                    mData.UpdateGameStatus(ttd.gameid(),ttd.status());
                else {
                    qCritical() << "no data" + QTD(ttd.DebugString());
                }
                break;
            case Data_Type_RESULT: {
                rd = d.GetExtension(ResultData::result_data);

                if ( !rd.has_game_result()) {
                    qCritical() << "no data" + QTD(ttd.DebugString());
                    break;
                }

                /*
                if ( !sanity(rd.fpp()) ) {
                    qCritical() << " invalid result skipping" << rd.DebugString();
                    break;
                }
                */
                auto allprojs = mNameData.GetGameProj(rd.game_result().gameid());
                unordered_map<string,std::unordered_map<std::string,int>> projmaps;
                for ( auto fpj : allprojs.home())
                   projmaps[fpj.playerid()].insert(make_pair(fpj.name(),fpj.proj()));

                for ( auto fpj : allprojs.away())
                   projmaps[fpj.playerid()].insert(make_pair(fpj.name(),fpj.proj()));

                if( rd.game_result().home_result_size() <= 0 )
                    qCritical() << "no home result" + QTD(rd.DebugString());
                else {
                    for ( int i =0; i < rd.game_result().home_result_size(); i++) {
                        qDebug() << rd.game_result().home_result(i).playerid()
                                 << rd.game_result().home_result(i).result();

                        auto proj = projmaps[rd.game_result().home_result(i).playerid()];
                        //if ( proj.size() == 0 )
                        //    continue;

                        processResultProj(rd.mutable_game_result()->mutable_home_result(i)
                                          ,proj,blocksigner);
                    }
                }


                if( rd.game_result().away_result_size() <= 0 )
                    qCritical() << "no away result" + QTD(rd.DebugString());
                else
                    //for (auto result : rd.game_result().away_result() ) {
                    for ( int i =0; i < rd.game_result().away_result_size(); i++) {
                        qDebug() << rd.game_result().away_result(i).playerid()
                                 << rd.game_result().away_result(i).result();

                        auto proj = projmaps[rd.game_result().away_result(i).playerid()];
                        //if ( proj.size() == 0 )
                        //    continue;
                        processResultProj(rd.mutable_game_result()->mutable_away_result(i),
                                          proj,blocksigner);
                        //result.mutable_fantaybitaward()->CopyFrom(delta.fantaybitaward());
                        //rd.mutable_game_result()->
                        //        mutable_away_result(i)->mutable_fantaybitaward()->CopyFrom(delta.fantaybitaward());
                    }

                //for (auto result : rd.game_result().away_result() )
                //    qDebug() << result.playerid() << result.fantaybitaward_size();

                mData.AddGameResult(rd.game_result().gameid(),rd.game_result());
#ifdef TRADE_FEATURE
                mExchangeData.OnGameResult(rd.game_result());
#endif
#ifdef DATAAGENTWRITENAMES
                {
#ifndef DATAAGENTWRITENAMES_FORCE
                if ( !amlive ) break;
#endif
                SqlStuff sql("satoshifantasy","distribution");
                Distribution dist{};
                dist.set_gameid(rd.game_result().gameid());
                auto gs = mData.GetGlobalState();
                dist.set_season(gs.season());
                dist.set_week(gs.week());

                auto gi = mData.GetGameInfo(dist.gameid());
                dist.set_teamid(gi.home());
                for ( auto res : rd.game_result().home_result()) {
                    dist.set_playerid(res.playerid());
                    dist.set_result(res.result());

                    for ( auto fba : res.fantaybitaward()) {
                        dist.set_fantasy_nameid(FantasyName::name_hash(fba.name()));
                        dist.set_proj(fba.proj());
                        dist.set_award(fba.award());
                        emit new_dataDistribution(dist);

                        auto ds = dist.SerializeAsString();

                        sql.distribute(dist);
                        //RestfullClient rest(QUrl(LAPIURL.data()));
                        //rest.postRawData("distribution","shit",ds.data(),((size_t)ds.size()));

                    }
                }

                dist.set_teamid(gi.away());
                for ( auto res : rd.game_result().away_result()) {
                    dist.set_playerid(res.playerid());
                    dist.set_result(res.result());

                    for ( auto fba : res.fantaybitaward()) {
                        dist.set_fantasy_nameid(FantasyName::name_hash(fba.name()));
                        dist.set_proj(fba.proj());
                        dist.set_award(fba.award());
                        emit new_dataDistribution(dist);
                        auto ds = dist.SerializeAsString();

                        sql.distribute(dist);
                        //RestfullClient rest(QUrl(LAPIURL.data()));
                        //rest.postRawData("distribution","shit",ds.data(),((size_t)ds.size()));

                    }
                }
                }
#endif
                break;
            }
            case Data_Type_SCHEDULE: {
                auto sd = d.GetExtension(ScheduleData::schedule_data);
                if ( sd.has_week() && sd.has_weekly() )
                    mData.AddNewWeeklySchedule(sd.week(),sd.weekly());
                else {
                    qCritical() << "no data" + QTD(sd.DebugString());
                }
                break;
            }

            case Data_Type_MESSAGE: {
                auto msg = d.GetExtension(MessageData::message_data);
                if ( msg.has_msg() ) {
                    qWarning() << "Control messgae" << msg.DebugString();
#ifdef Q_OS_MAC
                    if ( msg.msg().find("win64") != string::npos )
#endif
#ifdef Q_OS_WIN
                    if ( msg.msg().find("osx64") != string::npos )
#endif
                        break;

                    //if ( !amlive )
                    //    break;
                    //else
                    if ( msg.has_gt() || msg.has_lt()) {
                        int version =
                                MAJOR_VERSION * 1000 +
                                MINOR_VERSION * 100 +
                                REVISION_NUMBER * 10+
                                BUILD_NUMBER;

                        if ( (msg.has_gt() && version > msg.gt())
                             ||
                              (msg.has_lt() && version < msg.lt()))
                        onControlMessage(QString::fromStdString(msg.msg()));

                    }
                    else
                        onControlMessage(QString::fromStdString(msg.msg()));

                }
                break;
            }
            default:
                qWarning() << "unexpedted type" << d.type();
                break;
        }
    }
}


void BlockProcessor::processResultProj(PlayerResult* playerresultP,
                                       std::unordered_map<std::string,int> &proj,
                                       const std::string &blocksigner) {
    auto &playerresult = *playerresultP;
    DistribuePointsAvg dist(proj);
    if ( !playerresult.has_result() && playerresult.has_stats()) {
        //auto calc = CalcResults(playerresult.stats());
        //playerresult.set_result(calc);
    }

    auto rewards = dist.distribute(playerresult.result(), blocksigner);
    //decltype(PlayerResult::default_instance().fantaybitaward())
    PlayerResult awards;
    for (auto r : rewards ) {
        FantasyBitAward fba{};
        fba.set_name(r.first);
        auto m = proj[r.first];
        fba.set_proj(m);
        fba.set_award(r.second);
        awards.add_fantaybitaward()->CopyFrom(fba);
        mNameData.AddBalance(r.first,r.second);
    }
    playerresult.mutable_fantaybitaward()->CopyFrom(awards.fantaybitaward());
    //return awards;
}

void BlockProcessor::process(const DataTransition &indt) {
    auto mGlobalState = mData.GetGlobalState();
    switch (indt.type())
    {
    case DataTransition_Type_SEASONSTART:
        if (mGlobalState.state() != GlobalState_State_OFFSEASON)
            qWarning() << indt.type() << " bad transition for current state " << mGlobalState.state();

        {
            qInfo() <<  indt.season() << " Season Start week" << indt.week();
            if (mGlobalState.season() != indt.season()) {
                qWarning() << "wrong season! " << indt.DebugString();
                mGlobalState.set_season(indt.season());
            }
            mGlobalState.set_week(indt.week());
            mGlobalState.set_state(GlobalState_State_INSEASON);
            mData.OnGlobalState(mGlobalState);
            OnWeekStart(indt.week());
        }
        break;
    case DataTransition_Type_SEASONEND:
        if (mGlobalState.state() != GlobalState_State_INSEASON)
            qWarning() << indt.type() << " baad transition for current state " << mGlobalState.state();


        qInfo() <<  indt.season() << " Season End :( ";

        if (mGlobalState.season() == indt.season() - 1) {}
        else if (mGlobalState.season() != indt.season()) {
            qWarning() << "wrong season! " << indt.DebugString();
            mGlobalState.set_season(indt.season());
        }

        mGlobalState.set_season(mGlobalState.season() + 1);

        mGlobalState.set_state(GlobalState_State_OFFSEASON);
        mData.OnGlobalState(mGlobalState);

        //outDelta.mutable_globalstate()->CopyFrom(mGlobalState);
        break;

    case DataTransition_Type_HEARTBEAT:
        //todo: deal w data in this msg
        if (mGlobalState.season() != indt.season()) {
            qWarning() << "wrong season! " << indt.DebugString();
            //mGlobalState.set_season(indt.season());
        }

        if (mGlobalState.week() != indt.week()) {
            qWarning() << "wrong week! " << indt.DebugString();
            //mGlobalState.set_week(indt.week());
        }

        break;
    case DataTransition_Type_GAMESTART:
        for (auto t : indt.gamedata()) {
            mData.OnGameStart(t.gameid(),t.status());
            qInfo() <<  "Kickoff for game " << t.DebugString();
            auto gi =  mData.GetGameInfo(t.gameid());
            auto homeroster = mData.GetTeamRoster(gi.home());
            auto awayroster = mData.GetTeamRoster(gi.away());
            vector<string> homep, awayp;
            for ( auto hr : homeroster)
                homep.push_back(hr.first);
            for ( auto hr : awayroster)
                awayp.push_back(hr.first);

            mNameData.OnGameStart(t.gameid(),homep,awayp);
#ifdef TRADE_FEATURE
            mExchangeData.OnGameStart(t.gameid(),homep,awayp);
#endif

        }
        break;
    case DataTransition_Type_WEEKOVER:
    {
        if (mGlobalState.state() != GlobalState_State_INSEASON)
            qWarning() << indt.type() << " baad transition for current state " << mGlobalState.state();

        if ( indt.week() != mGlobalState.week())
            qWarning() << indt.type() << " wrong week" << mGlobalState.week() << indt.week();

        OnWeekOver(indt.week());
        int newweek = indt.week() + 1;
        qInfo() <<  "week " << indt.week() << " Over ";
        if (indt.week() == 16) {
            newweek = 0;
            qInfo() <<  "season " << indt.season() << " Over ";
            mGlobalState.set_state(GlobalState_State_OFFSEASON);
            mGlobalState.set_season(mGlobalState.season() + 1);
            mGlobalState.set_week(0);
            mData.OnGlobalState(mGlobalState);
        }
        else {
            mGlobalState.set_week(newweek);
            mData.OnGlobalState(mGlobalState);
            OnWeekStart(newweek);
        }
        break;
    }
    default:
        break;
    }
}

bool BlockProcessor::isValidTx(const SignedTransaction &st) {
    const Transaction &t = st.trans();
    fc::sha256 digest = fc::sha256::hash(t.SerializeAsString());
    if (digest.str() != st.id()) {
        qCritical() << "digest.str() != st.id() ";
        //fbutils::LogFalse(std::string("Processor::process signed transaction hash error digest ").append(st.DebugString()));
        return false;
    }

    if (t.version() != Commissioner::TRANS_VERSION) {
        qCritical() << "t.version() != Commissioner::TRANS_VERSION";
        //fbutils::LogFalse(std::string("Processor::process wrong transaction version ").append(st.DebugString()));
        return false;
    }

    fc::ecc::signature sig = Commissioner::str2sig(st.sig());
    if ( st.trans().type() == TransType::STAMPED) {
        if ( Commissioner::verifyOracle(sig, digest) )
            return true;

        qCritical() << "TransType::STAMPED !Commissioner::verifyOracle";
        return false;
    }

    if ( st.fantasy_name() == "") {
        qCritical() << " Blank FantasyName";
        return false;
    }

    if (t.type() == TransType::NAME) {
        auto nt = t.GetExtension(NameTrans::name_trans);
        if (!verify_name(st, nt, sig, digest)) {
            qInfo() << " !verify name";
            return false;
        }

        return true;
    }

    if (!Commissioner::verifyByName(sig, digest, st.fantasy_name())) {
        qCritical() << "!Commissioner::verifyByName";
        //fbutils::LogFalse(std::string("Processor::process cant verify trans sig").append(st.DebugString()));
        return false;;
    }

    return true;
}

void BlockProcessor::processTxfrom(const Block &b,int start) {

    //first do name transactions
    for (int i = start; i < b.signed_transactions_size(); i++) {

        if ( b.signed_transactions(i).trans().type() != TransType::NAME)
            continue;

        qDebug() << "processing name tx " << b.signed_transactions(i).trans().DebugString();// TransType_Name(t.type());

        if ( !isValidTx(b.signed_transactions(i))) {
            qDebug() << " imvalid tx 1"  ;
            continue;
        }

        const NameTrans & nt = b.signed_transactions(i).trans().GetExtension(NameTrans::name_trans);
        mNameData.AddNewName(nt.fantasy_name(), nt.public_key() );
        qInfo() <<  "verified " << FantasyName::name_hash(nt.fantasy_name());

    }

   // for (const SignedTransaction &st : b.signed_transactions()) //
    for ( int i = start; i < b.signed_transactions_size(); i++)
    {
        if ( b.signed_transactions(i).trans().type() == TransType::NAME)
            continue;

        const SignedTransaction &st = b.signed_transactions(i);
        const Transaction &t = b.signed_transactions(i).trans();
        //fc::sha256 digest = fc::sha256::hash(t.SerializeAsString());

        qDebug() << "processing tx " << t.DebugString();// TransType_Name(t.type());

        if (!isValidTx(st)) {
            qDebug() << " imvalid tx 2";
            continue;
        }

        switch (t.type())
        {
        case TransType::PROJECTION_BLOCK: {
            const ProjectionTransBlock & ptb = t.GetExtension(ProjectionTransBlock::proj_trans_block);
            qDebug() << st.fantasy_name() << "new projection block";// << ptb.DebugString();
            for (const PlayerPoints & pt : ptb.player_points() ) {
                mNameData.AddProjection(st.fantasy_name(), pt.playerid(), pt.points());
            }

            break;

        }

        case TransType::PROJECTION:
        {
            auto pt = t.GetExtension(ProjectionTrans::proj_trans);
            qDebug() << st.fantasy_name() << "new projection " << pt.DebugString();
            mNameData.AddProjection(st.fantasy_name(), pt.playerid(), pt.points());
            break;
        }

        case TransType::TIME: {
            break;
        }

        case TransType::STAMPED: {
            auto stamped = t.GetExtension(StampedTrans::stamped_trans);
            qDebug() << "new StampedTrans " << stamped.timestamp() << stamped.seqnum();

            ProcessInsideStamped(stamped.signed_orig(),stamped.seqnum());

            break;
        }

        default:
            break;
        }
    }
}

void BlockProcessor::ProcessInsideStamped(const SignedTransaction &inst,int32_t seqnum) {
    auto fn = BlockProcessor::getFNverifySt(inst);
    if ( !fn ) {
        qWarning() << "invalid tx inside stamped" << inst.trans().type();
        return;
    }

    const Transaction &t = inst.trans();
    switch (t.type()) {
        case TransType::EXCHANGE:
        {
#ifndef TRADE_FEATURE
            break;
#endif

            auto emdg = t.GetExtension(ExchangeOrder::exchange_order);
            qDebug() << "new ExchangeOrder " << emdg.DebugString();

            mExchangeData.OnNewOrderMsg(emdg,seqnum,fn);
            break;
        }
        default:
            break;
    }

}

void BlockProcessor::OnWeekOver(int week) {
    mNameData.OnWeekOver(week);
    mData.OnWeekOver(week);
#ifdef TRADE_FEATURE
    mExchangeData.OnWeekOver(week);
#endif

    emit WeekOver(week);
}

void BlockProcessor::OnWeekStart(int week) {
    mNameData.OnWeekStart(week);
    mData.OnWeekStart(week);
    emit WeekStart(week);
}

bool BlockProcessor::verifySignedBlock(const Block &sblock)
{
    //qDebug() << sblock.DebugString();

    if (sblock.signedhead().head().version() != Commissioner::BLOCK_VERSION)
    {
        qCritical() << " !verifySignedBlock wrong block version! ";
    //    return false;
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
            qCritical() << " Invalid Block Signer!";
            return false;
        }

    return true;
}

bool BlockProcessor::verifySignedTransaction(const SignedTransaction &st) {
    qDebug() << st.DebugString();

    if (st.trans().version() != Commissioner::TRANS_VERSION)
    {
        qCritical() << " !verifySignedTransaction wrong trans version! ";
        return false;
    }

    fc::sha256 digest = fc::sha256::hash(st.trans().SerializeAsString());
    if (digest.str() != st.id()) {
        qCritical() << "digest.str() != st.id() ";
        return false;
    }

    if ( st.fantasy_name() == "") {
        qCritical() << " Blank FantasyName";
        return false;;
    }

    fc::ecc::signature sig = Commissioner::str2sig(st.sig());
    if (st.trans().type() != TransType::NAME)
        if (!Commissioner::verifyByName(sig, digest, st.fantasy_name()))
        {
            qCritical() << "!Commissioner::verifyByName";
            qCritical() << "sig :" << st.sig();
            qCritical() << "st.fantasy_name() :" << st.fantasy_name();

            return false;;
        }

    return true;
}

std::shared_ptr<FantasyName> BlockProcessor::getFNverifySt(const SignedTransaction &st) {

    std::shared_ptr<FantasyName> ret;
    if (st.trans().version() != Commissioner::TRANS_VERSION) {
        qCritical() << " !verifySignedTransaction wrong trans version! ";
        return ret;
    }

    fc::sha256 digest = fc::sha256::hash(st.trans().SerializeAsString());
    if (digest.str() != st.id()) {
        qCritical() << "digest.str() != st.id() ";
        return ret;
    }

    if ( st.fantasy_name() == "") {
        qCritical() << " Blank FantasyName";
        return ret;;
    }

    ret = Commissioner::getName(st.fantasy_name());
    if ( !ret )
        qCritical() << " cant find FantasyName" << st.fantasy_name();
    else {
        fc::ecc::signature sig = Commissioner::str2sig(st.sig());
        if ( !Commissioner::verify(sig,digest,ret->pubkey()) )
            ret.reset();
    }
    return ret;
}

bool BlockProcessor::verify_name(const SignedTransaction &st, const NameTrans &nt, 
	const fc::ecc::signature& sig, const fc::sha256 &digest) { 

    if ( !Commissioner::isAliasAvailable(nt.fantasy_name()) )
    {
        qCritical() << std::string("Processor::process failure: FantasyName(").
                        append(nt.fantasy_name() + ")  hash already used ");
        return false;
    }


    auto pk = Commissioner::str2pk(nt.public_key());
    auto name = Commissioner::getName(pk);
    if ( name != nullptr ) {
        qCritical() << std::string("verfiy_name failure: FantasyName(").
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
                qCritical() << std::string("Processor::process name proof oracle failed")
                                 .append(st.DebugString());

                return false;
            }
            else
                return true;
		}
		break;

		default:
            return true;
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


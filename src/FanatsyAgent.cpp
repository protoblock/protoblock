//
//  FanatsyAgent.cpp
//  fantasybit
//
//  Created by Jay Berg on 4/6/14.
//
//

#include "FantasyAgent.h"
#include "Processor.h"
#include <iostream>
#include "Node.h"

#define LOG(logger, severity) LOGIT(logger, severity,  __FILE__, __LINE__, __FUNCTION__)

namespace fantasybit {

FantasyAgent::status FantasyAgent::signPlayer(alias_t name)
{
    status ret = NOTAVAILABLE;
   
    if ( Commissioner::isAliasAvailable(name) )
    {
        ret = REQUESTED;	
		client.alias = name;
		client.pubkey = m_priv.get_public_key().serialize();
    }
    else if ( auto p = Commissioner::getName(m_priv.get_public_key()) )
    {
        if ( p->isAlias(name) )
        {
            ret = OWNED;
            client = *p;
        }
    }
        
    return ret;
}

//static SignedBlock makeGenesisBlock() {
/*
bool FantasyAgent::makeGenesis()
{
	LOG(lg, trace) << "genesis ";

	Block sb = Commissioner::makeGenesisBlock();

	BlockProcessor bp{};
	bp.init();
	bp.process(sb);
	return true;
}
*/

bool FantasyAgent::beOracle()
{
#ifdef NO_ORACLE_CHECK_TESTING	
	m_oracle = m_priv;
	Commissioner::GENESIS_PUB_KEY = m_oracle.get_public_key()	;
	m_oracle = m_priv;
	return true;
#endif	

	if (Commissioner::GENESIS_PUB_KEY == m_priv.get_public_key().serialize())
		m_oracle = m_priv;
	else
	{
		Secret oracle{};
		Reader<Secret> read{ ROOT_DIR + "oracle.txt" };
		if (!read.good())
			return false;
		if (read.ReadNext(oracle))
		{
			LOG(lg, trace) << "oracle.private_key()" << oracle.private_key();
			auto pk = str2priv(oracle.private_key());
			if (pk.get_public_key() == Commissioner::GENESIS_PUB_KEY)
			{
				m_oracle = pk;
				m_priv = m_oracle;
			}
			else return false;
		}
		else return false;
	}

	return true;
}


Block FantasyAgent::makeNewBlockAsDataAgent(const SignedTransaction &dt)
{
	Block b{};
	
	if (!amDataAgent()) {
		if (!beDataAgent()) {
			fbutils::LogFalse(std::string("cant makeNewBlockAsDataAgent am not agent"));
			return b;
		}
	}

	auto prev = Node::getlastBLock().signedhead().head();

	BlockHeader bh{};
	bh.set_version(Commissioner::BLOCK_VERSION);
	bh.set_num(prev.num() + 1);
	bh.set_prev_id(fc::sha256::hash(prev.SerializeAsString()).str());
	bh.set_timestamp( std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count());
	//todo: t
	bh.set_generator_pk(pubKeyStr());
	bh.set_generating_sig(fc::sha256::hash(prev.generating_sig() + bh.generator_pk()).str());
	bh.set_basetarget(0); //todo
	bh.set_blocktype(BlockHeader_Type_DATA);
	bh.set_transaction_id(""); //todo: merkle root of tx 

	SignedBlockHeader sbh{};
	sbh.mutable_head()->CopyFrom(bh);

	auto p = getIdSig(sbh.head().SerializeAsString(),m_oracle);
	sbh.set_sig(p.second);
	//todo: store block hash from p.first 

	b.mutable_signedhead()->CopyFrom(sbh);

	SignedTransaction st{};
	SignedTransaction* st2 = b.add_signed_transactions();
	st2->CopyFrom(dt);


	leveldb::ReadOptions options;
	options.snapshot = Node::txpool->GetSnapshot();
	leveldb::Iterator *iter = Node::txpool->NewIterator(options);

	for (iter->SeekToFirst(); iter->Valid(); iter->Next()) {
		
		st.ParseFromString(iter->value().ToString());

		SignedTransaction* st2 = b.add_signed_transactions();
		st2->CopyFrom(st);
	}

	/*
	if (pendingTrans.size() == 0)
		return b;

	for (auto &pt : pendingTrans)
	{
		SignedTransaction* st2 = b.add_signed_transactions();
		st2->CopyFrom(pt);
	}

	pendingTrans.clear();
	*/
	delete iter;
	Node::txpool->ReleaseSnapshot(options.snapshot);


	//sb.set_id(p.first);	
	//BlockProcessor bp{};
	//bp.init();
	//bp.process(sb);


	return b;

}

Block FantasyAgent::makeNewBlockAsOracle()
{
	Block b{};

	if (pendingTrans.size() == 0)
		return b;

	if (!beOracle())
	{
		fbutils::LogFalse(std::string("cant makeNewBlockAsOracle am not him"));
		return b;
	}

	BlockHeader bh{};
	bh.set_prev_id
		("00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000");
	bh.set_num(2);
	bh.set_version(Commissioner::BLOCK_VERSION);

	SignedBlockHeader sbh{};
	sbh.mutable_head()->CopyFrom(bh);

	for (auto &pt : pendingTrans)
	{
		SignedTransaction* st2 = b.add_signed_transactions();
		st2->CopyFrom(pt);
	}

	b.mutable_signedhead()->CopyFrom(sbh);

	auto p = getIdSig(sbh.head().SerializeAsString(), m_oracle);
	//sb.set_id(p.first);
	sbh.set_sig(p.second);


	//BlockProcessor bp{};
	//bp.init();
	//bp.process(sb);
	pendingTrans.clear();

	return b;

}



/*
NameTrans nametrans{};
nametrans.set_hash(10576213825162658308);
nametrans.set_public_key(std::string("mT1M2MeDjA1RsWkwT7cjE6bbjprcNi84cWyWNvWU1iBa"));
nametrans.set_nonce(57428892);
nametrans.set_utc_sec(1408989677);
nametrans.set_prev_id("00000000000000000000000000000000000000000000000000000000");
nametrans.set_sig("iKkkiYrzqzRo4Cgz1TeZty4JY4KUrDWyPgeF5tKpeRoRD14zWubsFneY8fW7UodCpP3JXXrFvWh6UkSWD7NcktHDK9gb4i9D3m");
nametrans.set_sigid("19cacff77cae784ada296272e43b6dd6f22975d1");

Transaction trans{};
trans.set_version(1);
trans.set_type(TransType::NAME);
//[fantasybit.NameTrans.name_trans]
trans.MutableExtension(NameTrans::name_trans)->CopyFrom(nametrans);

SignedTransaction st{};
st.mutable_trans()->CopyFrom(trans);
st.set_id("6ca607c105f8f9adfa652a89c285e58a1848f35caef132267e0385f79c453eb4");
st.set_sig("iKkki4FAQFoNR4foHVv1KNqfnJ1Fm1xuTToW3LgRjfAem2PSuPU3cH7ZPiJNm3xyTLt2bJx5kdRMfn1aEhfCGiTsHbE3PHBeis");
st.set_fantasy_name("FantasyAgent");

BlockHeader bh{};
bh.set_prev_id("00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000");
bh.set_num(1);

Block b{};
b.mutable_head()->CopyFrom(bh);
SignedTransaction* st2 = b.add_signed_transactions();
st2->CopyFrom(st);

SignedBlock sb{};
sb.mutable_block()->CopyFrom(b);
sb.set_id("c9348ceb2551871534121114cd707c40653303250602aad6c6e0c67c522e5e9c");
sb.set_sig("iKkkiYr6vYFtkRtxCeWQvu7iZ9oFdLwrpRe1P3XYUwZz3BvBuwiufWTFj1JSRJ3d1zjvp9W2whNVTWtT5Jxtn1ByyiW3qQYMyy");

//std::cout << sb.DebugString();
//Commissioner::GenesisBlock = sb;
/*
auto nt = Commissioner::createGenesisName();
NameTrans nt2{};
nt2.set_hash(nt.name_hash);
nt2.set_public_key(Commissioner::pk2str(nt.pubkey));
nt2.set_nonce(nt.nonce);
nt2.set_utc_sec(nt.utc_sec.sec_since_epoch());
nt2.set_prev_id(nt.prev.str());
nt2.set_sig(Commissioner::sig2str(nt.sig));
nt2.set_sigid(nt.sigid());

Transaction trans{};
trans.set_version(1);
trans.set_type(TransType::NAME);
//trans.SetExtension(NameTrans::name_trans, );
trans.MutableExtension(NameTrans::name_trans)->CopyFrom(nt2);
//trans.SetExtension(NameTrans::name_trans, nt2);

SignedTransaction st{};
st.mutable_trans()->CopyFrom(trans);
auto p = getIdSig(trans.SerializeAsString());
st.set_id(p.first);
st.set_sig(p.second);
st.set_fantasy_name("FantasyAgent");

BlockHeader bh{};
bh.set_prev_id
("00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000");
bh.set_num(1);

Block b{};
b.mutable_head()->CopyFrom(bh);
SignedTransaction* st2 = b.add_signed_transactions();
st2->CopyFrom(st);

SignedBlock sb{};
sb.mutable_block()->CopyFrom(b);
auto pp = getIdSig(sb.block().SerializeAsString());
sb.set_id(pp.first);
sb.set_sig(pp.second);
//std::cout << "1 id " << pp.first << " sig " << pp.second << "\n";
*/
/*auto a = sb.block().signed_transactions().Get(0);
auto p1 = getIdSig(a.trans().SerializeAsString());
a.set_id(p1.first);
a.set_sig(p1.second);

std::cout << "\n" << a.DebugString() << "\n";

auto p = getIdSig(sb.block().SerializeAsString());
sb.set_id(p.first);
sb.set_sig(p.second);
std::cout << "\n" << sb.DebugString() << "\n";
*/

}
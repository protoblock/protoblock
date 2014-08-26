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
namespace fantasybit {

FantasyAgent::status FantasyAgent::signPlayer(alias_t name)
{
    status ret = NOTAVAILABLE;
   
    if ( Commissioner::isAliasAvailable(name) )
    {
        fut = std::async(&Commissioner::generateName, &comish, name, m_priv.get_public_key() );
        ret = REQUESTED;	
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
	bool FantasyAgent::makeBlock()

		{
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
	SignedBlock sb = Commissioner::makeGenesisBlock();
		
	std::cout << "\n" << sb.DebugString() << "\n";

	BlockProcessor bp{};
	bp.init();
	bp.process(sb);
	return true;
}



}
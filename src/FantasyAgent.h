//
//  FanatsyAgent.h
//  fantasybit
//
//  Created by Jay Berg on 4/6/14.
//
//

#ifndef __fantasybit__FanatsyAgent__
#define __fantasybit__FanatsyAgent__

#include "Commissioner.h" 
#include <utility>
#include "DataPersist.h"

namespace fantasybit
{

class FantasyAgent
{
    fc::ecc::private_key m_priv,m_oracle;
    std::future<name_transaction> fut{};
	FantasyName client{};
	std::vector<SignedTransaction> pendingTrans{};

public:
    enum status { REQUESTED, NOTAVAILABLE, OWNED };
    Commissioner comish{};
    
    FantasyAgent(const fc::sha256& secret)
    {
        m_priv = fc::ecc::private_key::regenerate(secret);
    }
    
    FantasyAgent(bool generate = true) : m_priv()
    {
        if ( generate )
            m_priv = m_priv.generate();
    }
	
	void resetPrivateKey()
	{
		m_priv = m_priv.generate();
	}
	bool makeGenesis();
	void onSignedTransaction(SignedTransaction &sn)
	{
		pendingTrans.emplace_back(sn);
		fbutils::LogFalse(std::string("onSignedTransaction").append(sn.DebugString()));
	}

	SignedTransaction makeSigned(Transaction &trans)
	{
		SignedTransaction st{};
		st.mutable_trans()->CopyFrom(trans);
		auto p = getIdSig(trans.SerializeAsString());
		st.set_id(p.first);
		st.set_sig(p.second);
		st.set_fantasy_name(client.alias);
		return st;
	}
	status signPlayer(alias_t name,bool mine=false);

	bool beOracle();

	bool makeNewBlockAsOracle();

	fc::ecc::private_key str2priv(const std::string &in)
	{
		return fc::ecc::private_key::regenerate(fc::sha256{ in });
	}
    name_transaction getRequested()
    {
        auto nt = fut.get();
        nt.sig =  m_priv.sign(nt.digest());
        return nt;
    }
    
    void kill()
    {
        comish.stop();
    }
    
    std::string getSecret() const
    {
        return m_priv.get_secret().str();
    }

	std::string pubKeyStr()
	{
		return Commissioner::pk2str(m_priv.get_public_key().serialize());
	}

	std::pair<std::string, std::string> getIdSig(std::string &in)
	{
		return getIdSig(in, m_priv);
	}

	std::pair<std::string, std::string> getIdSig(std::string &in, fc::ecc::private_key &pk)
	{
		fc::sha256 sha = fc::sha256::hash( in );
		return make_pair(sha.str(), Commissioner::sig2str(pk.sign(sha)));
	}
};

}

#endif /* defined(__fantasybit__FanatsyAgent__) */

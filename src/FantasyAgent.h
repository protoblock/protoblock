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
#include <fc/optional.hpp>
#include "boostLog.h"
#define LOGH(logger, severity) LOGIT(logger, severity,  __FILE__, __LINE__, __FUNCTION__)

namespace fantasybit
{

class FantasyAgent
{
    fc::optional<fc::ecc::private_key> m_priv, m_oracle;
    std::unique_ptr<FantasyName> client{nullptr};
	std::vector<SignedTransaction> pendingTrans{};
	Block prevBlock{};

    std::vector<Secret2> m_secrets;
    std::string secretfilename = "secret2.out";
public:
    enum status { AVAIL, NOTAVAILABLE, OWNED };
    
    FantasyAgent() {
        Reader<Secret2> read{ GET_ROOT_DIR() +  secretfilename};
        if ( !read.good() )
            return;

        Secret2 secret{};
        while (read.ReadNext(secret)) {
            m_secrets.push_back(secret);
            LOG(lg,info) << secret.fantasy_name() << " have key";
            if ( AmFantasyAgent(secret.public_key())) {
                m_oracle = str2priv(secret.private_key());
                LOG(lg,info) << " is oracle key";
            }
        }
    }
    /*
    FantasyAgent(const fc::sha256& secret, const std::string &name)
    {
        m_priv = fc::ecc::private_key::regenerate(secret);
        client = std::make_unique<FantasyName> (name,m_priv.get_public_key().serialize());
        LOGH(lg, trace) << "have secret " << client->ToString();
    }
		
    FantasyAgent(bool generate = true) : m_priv()
    {
		LOGH(lg, trace) << "generate=" << generate;

        if ( generate )
            m_priv = m_priv.generate();
    }
	
	void resetPrivateKey()
	{
		m_priv = m_priv.generate();
	}
    */

    std::multimap<std::string,std::string> getMyNames() {
        std::multimap<std::string,std::string> ret;
        for ( auto s2 : m_secrets ) {
            ret.emplace(s2.fantasy_name(),s2.public_key());
        }

        return ret;
    }

	bool makeGenesis();
	void onSignedTransaction(SignedTransaction &sn)
	{
		pendingTrans.emplace_back(sn);
		LOG(lg,trace) << sn.DebugString();
	}

    bool HaveClient() {
        return client != nullptr;
    }

	bool amDataAgent() {
		//Todo: fix
        if (m_oracle && m_priv)
            return (*m_oracle).get_secret() == (*m_priv).get_secret();
        else
            return false;
	}

    static bool AmFantasyAgent(fc::ecc::public_key_data pubkey) {
        return Commissioner::GENESIS_PUB_KEY == pubkey;
    }

    static bool AmFantasyAgent(std::string pubkey) {
        return Commissioner::GENESIS_PUB_KEY == Commissioner::str2pk(pubkey);
    }

	SignedTransaction makeSigned(Transaction &trans)
	{
		SignedTransaction st{};
		st.mutable_trans()->CopyFrom(trans);
		auto p = getIdSig(trans.SerializeAsString());
		st.set_id(p.first);
		st.set_sig(p.second);
        st.set_fantasy_name(client->alias());
		return st;
	}

	template <class T>
	Transaction toTransaction(T &t) {
		Transaction tr{};
		tr.MutableExtension(T::trans)->CopyFrom(t);
		return tr;
	}

	template <class T>
	SignedTransaction toSignedTransaction(T &t) {
		return makeSigned(toTransaction(t));
	}

    status signPlayer(std::string name);

    //bool beOracle();
    bool beDataAgent();// { return beOracle();  }

    //Block makeNewBlockAsOracle();
	Block makeNewBlockAsDataAgent(const SignedTransaction &);

    std::string getSecret() const {
        return m_priv ? (*m_priv).get_secret().str() : "";
    }

    fc::ecc::public_key_data pubKey() {
        return (*m_priv).get_public_key();
	}

    std::string pubKeyStr() {
        return m_priv ?
                    Commissioner::pk2str((*m_priv).get_public_key().serialize())
                  : "";
    }


    std::pair<std::string, std::string> getIdSig(std::string &in) {
        return m_priv ? getIdSig(in, *m_priv)
                       :  std::make_pair("","");
	}


    std::pair<std::string, std::string>
    getIdSig(std::string &in, fc::ecc::private_key &pk) {
		fc::sha256 sha = fc::sha256::hash( in );
		return make_pair(sha.str(), Commissioner::sig2str(pk.sign(sha)));
	}

    static fc::ecc::private_key str2priv(const std::string &in) {
        return fc::ecc::private_key::regenerate(fc::sha256{ in });
    }

};

}

#endif /* defined(__fantasybit__FanatsyAgent__) */

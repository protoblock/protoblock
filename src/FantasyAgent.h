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

namespace fantasybit
{

class FantasyAgent
{
    fc::ecc::private_key m_priv;
    std::future<name_transaction> fut{};
    FantasyName client;
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
	
	bool makeBlock();
	

	status signPlayer(alias_t name);

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

	std::pair<std::string, std::string> getIdSig(std::string in)
	{
		fc::sha256 sha = fc::sha256::hash( in );
		return make_pair(sha.str(), Commissioner::sig2str(m_priv.sign(sha)));
	}
};

}

#endif /* defined(__fantasybit__FanatsyAgent__) */

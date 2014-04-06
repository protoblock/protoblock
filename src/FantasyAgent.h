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
//#include "DataPersist.h"
//#include "ProtoData.pb.h"

namespace fantasybit
{

class FantasyAgent
{
    fc::ecc::private_key m_priv;

    std::future<name_transaction> fut{};
    FantasyName client;
public:
    Commissioner comish{};
    enum status { REQUESTED, NOTAVAILABLE, OWNED };
    
    FantasyAgent(const fc::sha256& secret) {
        m_priv = fc::ecc::private_key::regenerate(secret);
    }
    
    FantasyAgent(bool generate = true) : m_priv() {
        if ( generate )
        {
            m_priv = m_priv.generate();
            //secretdata.set_private_key(m_priv.get_secret().str());
            //Writer<Secret> write{"secret.out",std::ios::app};
            //write(secretdata);
        }
        /*
        else {
            Reader<Secret> read{"secret.out"};
            if ( read.ReadNext(secretdata) )
                m_priv = fc::ecc::private_key::regenerate(fc::sha256{secretdata.private_key()});
        }
        */
    }
    
    status signPlayer(alias_t name);

    name_transaction getRequested() {
        auto nt = fut.get();
        nt.sig =  m_priv.sign(nt.digest());
        return nt;
    }
    
    std::string getSecret() const
    {
        return m_priv.get_secret().str();
    }

};

}

#endif /* defined(__fantasybit__FanatsyAgent__) */

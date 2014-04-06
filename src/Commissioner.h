//
//  Comissioner.h
//  fantasybit
//
//  Created by Jay Berg on 4/1/14.
//
//

#ifndef __fantasybit__Comissioner__
#define __fantasybit__Comissioner__

#include "FantasyName.h"

namespace fantasybit {

class Commissioner
{
public:
    FantasyName generateName(alias_t,pubkey_t) ;

    nameid_t lastId() const { return m_genesis_id; }
    
    static std::unordered_map<pubkey_t,std::shared_ptr<FantasyName>> FantasyNames;
    static std::map<hash_t,pubkey_t> Aliases;
      
    static nameid_t m_genesis_id;
    static bool isAliasAvailable(alias_t alias)
    {
        return Commissioner::Aliases.find(FantasyName::name_hash(alias)) == end(Commissioner::Aliases);
    }
    
    static std::shared_ptr<FantasyName> getName(const pubkey_t &pub  ) {
        auto iter = Commissioner::FantasyNames.find(pub);
        if ( iter == end(Commissioner::FantasyNames) ) return nullptr;
        
        return iter->second;
    }
    
    static name_transaction createGenesisName()
    {
        name_transaction genesis(nameid_t{});
        genesis.name_hash = FantasyName::name_hash("SatoshiFantasy");
        genesis.pubkey = fc::ecc::private_key::regenerate(fc::sha256::hash("SatoshiFantasy",7)).get_public_key();
        genesis.utc_sec = fc::time_point_sec(fc::time_point::from_iso_string( "20140401T134233" ));
        return genesis;
    };
    
    static fc::sha224 max_hash() {
        fc::sha224 mining_hash;
        char* tmpPtr = (char*)&mining_hash;
        memset( tmpPtr, 0xff, sizeof(mining_hash) );
        tmpPtr[0] = 0;
        tmpPtr[1] = 0;
        tmpPtr[2] = 0;
        tmpPtr[2] = 0;
        tmpPtr[3] = 0x0f;
        return mining_hash;
    }
    
    static uint64_t min_difficulty;
};


class FantasyPlayer;
class FantasyAgent
{
    fc::ecc::private_key m_priv;

    std::future<FantasyName> fut{};
    FantasyName client;
public:
    Commissioner comish{};
    enum status { REQUESTED, NOTAVAILABLE, OWNED };
    
    FantasyAgent(const fc::sha256& secret) {
        m_priv = fc::ecc::private_key::regenerate(secret);
    }
    
    FantasyAgent(bool generate = true) : m_priv() {
        if ( generate ) m_priv = m_priv.generate();
    }
    
    status signPlayer(alias_t name)
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
    
    FantasyName getRequested() {
        return fut.get();
    }

};


}

#endif /* defined(__fantasybit__Comissioner__) */

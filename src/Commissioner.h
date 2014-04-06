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

uint64_t difficulty( const fc::sha224& hash_value );

class Commissioner
{
public:
    name_transaction generateName(alias_t,pubkey_t) ;

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
    
    static uint64_t target(uint64_t in) { return std::max(in,Commissioner::min_difficulty); }
    
    static name_transaction createGenesisName()
    {
        name_transaction genesis(nameid_t{});
        genesis.name_hash = FantasyName::name_hash("SatoshiFantasy");
        genesis.pubkey = fc::ecc::private_key::regenerate(fc::sha256::hash("SatoshiFantasy",7)).get_public_key();
        genesis.utc_sec = fc::time_point_sec(fc::time_point::from_iso_string( "20140401T134233" ));
        return genesis;
    };
    
    static constexpr int hashmineindex()
    {
#ifdef HARD_MINING 
        return 3;
#else
        return 2;
#endif
    }
    
    static fc::sha224 max_hash() {
        fc::sha224 mining_hash;
        char* tmpPtr = (char*)&mining_hash;
        memset( tmpPtr, 0xff, sizeof(mining_hash) );
        for (int i=0;i<hashmineindex()-1;i++)
            tmpPtr[0] = 0;
         tmpPtr[hashmineindex()] = 0x0f;
        return mining_hash;
    }
    
    static uint64_t min_difficulty;
};




}

#endif /* defined(__fantasybit__Comissioner__) */

//
//  Comissioner.cpp
//  fantasybit
//
//  Created by Jay Berg on 4/1/14.
//
//

#include "Comissioner.h"
#include "FantasyName.h"

namespace fantasybit {

  const fc::bigint& max224()
  {
     static fc::bigint m = [](){ 
        char data[224/8+1];
        memset( data, 0xff, sizeof(data) );
        data[0] = 0;
        return fc::bigint(data,sizeof(data));
     }();
     return m;
  }

  uint64_t difficulty( const fc::sha224& hash_value )
  {
      if( hash_value == fc::sha224() ) return uint64_t(-1); // div by 0

      auto dif = max224() / fc::bigint( (char*)&hash_value, sizeof(hash_value) );
      int64_t tmp = dif.to_int64();
      // possible if hash_value starts with 1
      if( tmp < 0 ) tmp = 0;
      return tmp;
  }


FantasyName Comissioner::generateName(alias_t alias, pubkey_t pub)
{
    name_transaction nt{lastId()};

    nt.pubkey = pub;
    nt.name_hash = FantasyName::name_hash(alias);
    
    nt.nonce = 0;
    nt.utc_sec = fc::time_point::now();
    uint64_t hit = difficulty(nt.id());
    uint64_t target = std::max(difficulty(nt.prev),Comissioner::min_difficulty);
    
    while ( hit < target ) {
        if ( nt.nonce == std::numeric_limits<nonce_t>::max()) {
            nt.utc_sec = fc::time_point::now();
            nt.nonce = 0;
        }
        else ++nt.nonce;
        hit = difficulty(nt.id());
    }
    
    return FantasyName{pub,alias};
}

nameid_t Comissioner::m_genesis_id = Comissioner::createGenesisName().id();
std::unordered_map<pubkey_t,std::shared_ptr<FantasyName>> Comissioner::FantasyNames{};
std::map<hash_t,pubkey_t> Comissioner::Aliases{};
uint64_t Comissioner::min_difficulty = difficulty(Comissioner::max_hash());


}
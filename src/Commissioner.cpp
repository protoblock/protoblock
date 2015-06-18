//
//  Comissioner.cpp
//  fantasybit
//
//  Created by Jay Berg on 4/1/14.
//
//

#include "Commissioner.h"
#include "FantasyName.h"
#include <iostream> 
#include <ctime>

using namespace std;

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

fc::ecc::public_key_data Commissioner::GENESIS_PUB_KEY
{ Commissioner::str2pk(std::string("mT1M2MeDjA1RsWkwT7cjE6bbjprcNi84cWyWNvWU1iBa")) };
std::unordered_map<pubkey_t,std::shared_ptr<FantasyName>> Commissioner::FantasyNames{};
std::map<hash_t,pubkey_t> Commissioner::Hash2Pk{};
//SignedBlock Commissioner::GenesisBlock{};
//Commissioner::makeGenesisBlock();

}
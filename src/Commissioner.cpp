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

name_transaction Commissioner::generateName(alias_t alias, pubkey_t pub)
{
	name_transaction nt{ lastId() };

    nt.pubkey = pub;
    nt.name_hash = FantasyName::name_hash(alias);
    
    nt.nonce = 0;
    nt.utc_sec = fc::time_point::now();
    //fc::time_point_sec(fc::time_point::from_iso_string( "20140401T134233" ));
    uint64_t hit = difficulty(nt.id());
    uint64_t target = std::max(difficulty(nt.prev),Commissioner::min_difficulty);
    //nt = createGenesisName();

#ifdef TRACE    
    cout << "id " << nt.id() << " target " << target << "\n";

    cout << "before " << nt.nonce << " utc " << nt.utc_sec.sec_since_epoch() << "\n";

    cout << "in max " << nt.nonce << " utc " << nt.utc_sec.sec_since_epoch() << "\n";
#endif
    nonce_t reset = std::numeric_limits<nonce_t>::max()-1000;

/**/
        std::chrono::time_point<std::chrono::system_clock> start, end;
        start = std::chrono::system_clock::now();
        std::time_t start_time = std::chrono::system_clock::to_time_t(start);
        std::cout << "started computation at " << std::ctime(&start_time);
/**/
    while ( running && hit < target ) {
        if ( nt.nonce >= reset   ) //214748364)
        {
            nt.utc_sec = fc::time_point::now();
            nt.nonce = 0;
            cout << "in max " << nt.nonce << " utc " << nt.utc_sec.sec_since_epoch() << "\n";
        }
        else { ++nt.nonce; }
        
        hit = difficulty(nt.id());
    }

/**/
        end = std::chrono::system_clock::now();
        std::chrono::duration<double> elapsed_seconds = end-start;
        std::time_t end_time = std::chrono::system_clock::to_time_t(end);
 
        std::cout << "finished computation at " << std::ctime(&end_time)
             << "elapsed time: " << elapsed_seconds.count() << "s\n";
/**/
    
#ifdef TRACE
    cout << "after loop hit " << hit << " target " << target << "\n";
    cout << "afer max " << nt.nonce << " utc " << nt.utc_sec.sec_since_epoch() << "\n";
#endif

    return nt;
}

fc::ecc::public_key_data Commissioner::GENESIS_PUB_KEY;
nameid_t Commissioner::m_genesis_id = Commissioner::createGenesisName().id();
std::unordered_map<pubkey_t,std::shared_ptr<FantasyName>> Commissioner::FantasyNames{};
std::map<hash_t,pubkey_t> Commissioner::Aliases{};
uint64_t Commissioner::min_difficulty = difficulty(Commissioner::max_hash());
//SignedBlock Commissioner::GenesisBlock{};
//Commissioner::makeGenesisBlock();

}
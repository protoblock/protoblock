//
//  main.cpp
//  satoshifantasy
//
//  Created by User on 3/17/14.
//  Copyright (c) 2014 User. All rights reserved.
//
/*
#include <iostream>
#include <string>
#include <memory>
#include "FantasyName.h"
#include "DistributionAlgo.h"
#include "fbutils.h"
#include <numeric>
#include <fc/crypto/aes.hpp>
#include <fc/crypto/city.hpp>
#include <fc/exception/exception.hpp>
#include <fc/crypto/elliptic.hpp>
#include <fc/crypto/sha224.hpp>
#include <fc/io/raw.hpp>
//#include "../cryptopp/integer.h"
//using CryptoPP::Integer;

using namespace fantasybit;
using namespace std;


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

  struct name_type {

    uint64_t name_hash;
    fc::ecc::public_key_data  pkey;
    uint64_t       prev;
    uint32_t nonce;
    fc::uint128    thash;
    fc::time_point_sec                       utc_sec;
    void name(std::string in) {
      fc::sha256 h = fc::sha256::hash( in.c_str(), in.size() );

    // compress it down to 64 bits
        name_hash =  fc::city_hash64( (char*)&h, sizeof(h) );
        prev = 8347;
        ttt();
        
    }
    fc::sha224 id() {
        fc::sha224::encoder enc;
        fc::raw::pack(enc,name_hash);
        fc::raw::pack(enc,pkey);
        fc::raw::pack(enc,prev);
        fc::raw::pack(enc,nonce);
        fc::raw::pack(enc,thash);
                fc::raw::pack(enc,utc_sec);
        return enc.result();
    }
    
    void ttt() {
         fc::sha512::encoder enc;
     fc::raw::pack( enc, prev );
     auto result = enc.result();
     // city hash isn't crypto secure, but its input is sha512 which is.
     // we use city to compress the hash for bandwidth purposes
     thash =  fc::city_hash128( (char*)&result, sizeof(result) );
     
     }
    
  };
*/

#include "Comissioner.h"
#include "FantasyName.h"
#include <iostream>

using namespace fantasybit;
using namespace std;



int main(int argc, const char * argv[])
{
    FantasyName me;
    FantasyAgent fa{true};
    auto ret = fa.signPlayer("SatoshiFantasy");
    if ( ret == FantasyAgent::REQUESTED ) {
        me = fa.getRequested();
    
        cout << me.alias << "\n";
    }
/*
  //Integer i = 0;

  //auto a =   i.GetBit(3);
  //std::cout << "i: " << i << endl;
  
  fc::sha256 seed = fc::sha256::hash( "FantasyMaster", 13 );
  fc::ecc::private_key priv = fc::ecc::private_key::generate();
  fc::ecc::private_key priv2 = fc::ecc::private_key::generate();
  //fc::ecc::private_key::generate_from_seed(seed);
  fc::ecc::public_key pub = priv.get_public_key();
  fc::sha256 secret = priv.get_secret();
  
  fc::sha224 aa;
   fc::sha256 digest  = fc::sha256::hash( "secretworld", 11 );
  fc::ecc::signature sig = priv.sign(digest);
   bool ver = priv.verify(digest, sig);
   
   fc::sha224 mining_hash;
   char* tmpPtr = (char*)&mining_hash;
   memset( tmpPtr, 0xff, sizeof(mining_hash) );
   tmpPtr[0] = 0;
   tmpPtr[1] = 0;
   tmpPtr[2] = 0;
   tmpPtr[3] = 0;
   tmpPtr[4] = 0x0f;

    name_type me;
    me.name("FantasyMaster");
    me.nonce = 0;
    me.pkey = pub;
    me.utc_sec = fc::time_point::now();
    uint64_t target = difficulty(mining_hash);
    uint64_t hit = difficulty(me.id());
    fc::time_point_sec start = fc::time_point::now();
    while ( hit < target ) {
        me.nonce++;
        //me.thash++;
        hit = difficulty(me.id());
        if ( me.nonce == std::numeric_limits<uint32_t>::max()) {
            me.utc_sec = fc::time_point::now();
            me.nonce = 0;
        }
    }
    fc::time_point_sec stop = fc::time_point::now();
    auto timed = stop.sec_since_epoch()  - start.sec_since_epoch();
   
    NameValuePairs<Int> proj {
       make_pair(shared_ptr<FantasyName>(new FantasyName{fc::ecc::private_key::generate().get_public_key(),"alias1"}), 10),
       make_pair(shared_ptr<FantasyName>(new FantasyName{fc::ecc::private_key::generate().get_public_key(),"alias2"}), 15),
       make_pair(shared_ptr<FantasyName>(new FantasyName{fc::ecc::private_key::generate().get_public_key(),"alias3"}), 16),
       make_pair(shared_ptr<FantasyName>(new FantasyName{fc::ecc::private_key::generate().get_public_key(),"alias4"}), 19),
       make_pair(shared_ptr<FantasyName>(new FantasyName{fc::ecc::private_key::generate().get_public_key(),"alias5"}), 1),
       make_pair(shared_ptr<FantasyName>(new FantasyName{fc::ecc::private_key::generate().get_public_key(),"alias6"}), 33),
       make_pair(shared_ptr<FantasyName>(new FantasyName{fc::ecc::private_key::generate().get_public_key(),"alias7"}), 11)
    };

    Int points = 14;
    DistribuePointsAvg d{proj};
    auto results = d.distribute(points);

    Int sum = accumulate(begin(results), end(results), 0.0,
        [](const double rsum, decltype(results)::const_reference pair){return rsum + pair.second;});
    
    if ( sum > points )
        throw exception();
    
    //std::string line;
    //std::getline( std::cin, line );
  std::string line;
  std::getline( std::cin, line );
  auto key = fc::sha512::hash( "hello", 5 );
  while( std::cin && line != "q" )
  {
    try {
      std::vector<char> data( line.c_str(),line.c_str()+line.size()+1 ); 
      std::vector<char> crypt = fc::aes_encrypt( key, data );
      std::vector<char> dcrypt = fc::aes_decrypt( key, crypt );

      std::cout<<"line.size:     '"<<line.size()<<"'\n";
      std::cout<<"data.size:     '"<<data.size()<<"'\n";
      std::cout<<"crypt.size:    '"<<crypt.size()<<"'\n";
      std::cout<<"dcrypt.size:   '"<<dcrypt.size()<<"'\n";
      std::cout<<"line:          '"<<line<<"'\n";
      std::cout<<"dcrypt:        '"<<dcrypt.data()<<"'\n";
      std::cout<<"dcrypt: "<<fc::variant(dcrypt).as_string()<<"\n";
      std::cout<<"crypt: "<<fc::variant(crypt).as_string()<<"\n";
      memset( crypt.data(), 0, crypt.size() );

      fc::aes_encoder enc;
      enc.init( fc::sha256::hash((char*)&key,sizeof(key) ), fc::city_hash_crc_128( (char*)&key, sizeof(key) ) ); 
      auto len = enc.encode( dcrypt.data(), dcrypt.size(), crypt.data() );
    //  enc.final_encode( crypt.data() + len );
      std::cout<<"crypt: "<<fc::variant(crypt).as_string()<<"\n";
      

      fc::aes_decoder dec;
      dec.init( fc::sha256::hash((char*)&key,sizeof(key) ), fc::city_hash_crc_128( (char*)&key, sizeof(key) ) ); 
    } 
    catch ( fc::exception& e ) 
    {
       std::cout<<e.to_detail_string()<<"\n";
    }
    std::getline( std::cin, line );
}
*/
    return 0;
};


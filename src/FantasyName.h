//
//  FantasyName.h
//  fantasybit
//
//  Created by Jay Berg on 3/19/14.
//
//

#ifndef __fantasybit__FantasyName__
#define __fantasybit__FantasyName__

#include <string>
#include <unordered_map>
#include <map>
#include "fbutils.h"
#include <fc/crypto/elliptic.hpp>
#include <future>
#include <fc/crypto/sha224.hpp>
#include <fc/crypto/ripemd160.hpp>
#include <fc/time.hpp>
#include "ProtoData.pb.h"

namespace fantasybit
{

class Bits
{ 
    Int64 base;
public:		
    Bits(Int64 b) : base(b) {}
    
    double points() { return base * .00000001; }
    double bits() { return points() * 100; }
    Int64 amount() { return base; }
    void add(Bits b) { base+=b.amount(); }
};

using pubkey_t = fc::ecc::public_key_data;
using alias_t = std::string;
using hash_t = uint64_t;

struct FantasyName
{
    pubkey_t pubkey;
    alias_t  alias;  
	Bits balance{ 0 };
    
    Bits getBalance() { return balance; }
    
	void addBalance(Bits b) {
		balance.add(b);

	}
    hash_t hash() const {
        return name_hash(alias);
    }
    
    bool isAlias(const alias_t &that) const {
        return hash() == name_hash(that);
    }
    
    static hash_t  name_hash( const alias_t& n );
};


/*
class KeyPair
{
public:
	KeyPair() {}
	KeyPair(Secret _k);

	static KeyPair create();

	Secret const& secret() const { return m_secret; }
	Secret const& sec() const { return m_secret; }
	Public const& pub() const { return m_public; }

	Address const& address() const { return m_address; }

private:
	Secret m_secret;
	Public m_public;
	Address m_address;
};
*/

}

#endif /* defined(__fantasybit__FantasyName__) */

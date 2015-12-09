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
#include <fc/crypto/base58.hpp>
#include <mutex>
#include <thread>

#define alias_t std::string


namespace fantasybit
{

template <class T>
class Bits
{ 
    T base;
public:		
    Bits(T b) : base(b) {}
    
    double points() { return base * .00000001; }
    double bits() { return points() * 100; }
    T amount() { return base; }
    void add(Bits b) { base+=b.amount(); }
    void subtract(Bits b) { base-=b.amount(); }
};

using pubkey_t = fc::ecc::public_key_data;
using hash_t = uint64_t;
using skillbits  =Bits<UInt64>;
using stakebits = Bits<Int64>;
struct FantasyName
{
private:
    alias_t  mAlias;
    pubkey_t mPubkey;
    skillbits balance;
    stakebits stake;
    std::recursive_mutex bal_mutex{};
public:
    FantasyName(const alias_t &inalias, const pubkey_t &inpubkey)
        : mAlias(inalias) , mPubkey(inpubkey) , balance (0), stake(0) {}
    
    FantasyName ( const FantasyName &in )
        : mAlias(in.alias()) , mPubkey(in.pubkey()) , balance (in.balance), stake(in.stake) {}

    UInt64 getBalance()  {
        std::lock_guard<std::recursive_mutex> lockg{ bal_mutex };
        return balance.amount();
    }

    Int64 getStakeBalance()  {
        std::lock_guard<std::recursive_mutex> lockg{ bal_mutex };
        return stake.amount();
    }

    void addBalance(skillbits b) {
        std::lock_guard<std::recursive_mutex> lockg{ bal_mutex };
        balance.add(b);
        stake.add(b.amount());
	}

    void addProfitLoss(stakebits b) {
        std::lock_guard<std::recursive_mutex> lockg{ bal_mutex };
        stake.add(b);
    }

    /*
    void addLoss(Bits b) {
        std::lock_guard<std::recursive_mutex> lockg{ bal_mutex };
        stake.subtract(b);
    }

    void addProfit(Bits b) {
        std::lock_guard<std::recursive_mutex> lockg{ bal_mutex };
        stake.add(b);
    }
    */



    void newBalance(skillbits b) {
        std::lock_guard<std::recursive_mutex> lockg{ bal_mutex };
        balance = b;
    }

    hash_t hash() const {
        return name_hash(alias());
    }
    
    pubkey_t pubkey() const {
        return mPubkey;
    }

    std::string alias() const {
        return mAlias;
    }

    bool isAlias(const alias_t &that) const {
        return hash() == name_hash(that);
    }
    
    std::string ToString() {
      return "alias(" + alias() + ") hash(" + std::to_string(hash()) + ") pk(" +
              fc::to_base58(pubkey().data, pubkey().size()) + ") balance(" +
              std::to_string(getBalance()) + ")";
    }

    static hash_t name_hash( const alias_t& n );
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

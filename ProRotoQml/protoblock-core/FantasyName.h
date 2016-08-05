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
#include <future>
//#include <fc/time.hpp>
#include "ProtoData.pb.h"
#include <mutex>
#include <thread>
#include "utils/utils.h"

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

using hash_t = uint64_t;
using skillbits  =Bits<UInt64>;
using stakebits = Bits<Int64>;
struct FantasyName
{
private:
    alias_t  mAlias;
    pb::public_key_data mPubkey;
    skillbits balance;
    stakebits stakedelta;
    std::recursive_mutex bal_mutex{};
public:
    FantasyName(const alias_t &inalias, const pb::public_key_data &inpubkey)
        : mAlias(inalias) , mPubkey(inpubkey) , balance (0), stakedelta(0) {}

    FantasyName ( const FantasyName &in )
        : mAlias(in.alias()) , mPubkey(in.pubkey()) , balance (in.balance), stakedelta(in.stakedelta) {}

    UInt64 getBalance()  {
        std::lock_guard<std::recursive_mutex> lockg{ bal_mutex };
        return balance.amount();
    }

    Int64 getStakeBalance()  {
        std::lock_guard<std::recursive_mutex> lockg{ bal_mutex };
        return (Int64)balance.amount() + stakedelta.amount();
    }

    void addBalance(skillbits b) {
        std::lock_guard<std::recursive_mutex> lockg{ bal_mutex };
        balance.add(b);
    }

    void addProfitLoss(stakebits b) {
        std::lock_guard<std::recursive_mutex> lockg{ bal_mutex };
        stakedelta.add(b);
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

    void initBalance(skillbits b) {
        std::lock_guard<std::recursive_mutex> lockg{ bal_mutex };
        balance = b;
    }

    void initStakePNL(stakebits b) {
        std::lock_guard<std::recursive_mutex> lockg{ bal_mutex };
        stakedelta = b;
    }


    hash_t hash() const {
        return name_hash(alias());
    }

    pb::public_key_data pubkey() const {
        return mPubkey;
    }

    std::string alias() const {
        return mAlias;
    }

    bool isAlias(const alias_t &that) const {
        return hash() == name_hash(that);
    }

    template <typename T>
    std::string To_String(T val)
    {
        std::stringstream stream;
        stream << val;
        return stream.str();
    }


    std::string ToString() {
      return "alias(" + alias() + ") hash(" + To_String(hash()) + ") public-key(" +
              pb::to_base58(pubkey()) + ") skill-balance(" +
              To_String(getBalance()) + ") stake-delta(" +
              To_String(stakedelta.bits()) + ") stake-balance(" +
              To_String(getStakeBalance()) +")";

    }

    static hash_t name_hash( const alias_t& n );
};

struct FantasyNameCHash : FantasyName {
    FantasyNameCHash(const alias_t &inalias, const pb::public_key_data &inpubkey) :
            FantasyName(inalias, inpubkey), chash(hash()) {}

    FantasyNameCHash ( const FantasyName &in ) :
        FantasyName(in), chash(hash()) {}

private:
    hash_t chash;

    hash_t hash() const {
        return chash;
    }
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

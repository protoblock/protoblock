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
#include <iostream>
#include <set>

#include "FantasyAgent.h"
#include "StateData.pb.h"
#include "ldbwriter.h"

namespace fantasybit {

//uint64_t difficulty( const fc::sha224& hash_value );

class Commissioner
{
    volatile bool running = true;
    static std::unordered_map<pb::public_key_data,std::shared_ptr<FantasyName>> FantasyNames;
    static std::map<hash_t,pb::public_key_data> Hash2Pk;


public:
    static std::recursive_mutex name_mutex;
#ifndef PRODFOOTBALL
    static pb::public_key_data PROD_GENESIS_PUB_KEY;
#endif

    static pb::public_key_data GENESIS_PUB_KEY;
    static bool isAliasAvailable(alias_t alias) {
        std::lock_guard<std::recursive_mutex> lockg{ name_mutex };
        return Commissioner::Hash2Pk.find(FantasyName::name_hash(alias)) == end(Commissioner::Hash2Pk);
    }


    static void clearAll()
    {
        std::lock_guard<std::recursive_mutex> lockg{ name_mutex };
        FantasyNames.clear();
        Hash2Pk.clear();
    }

    static std::vector<std::shared_ptr<FantasyName>> GetFantasyNames() {
        std::vector<std::shared_ptr<FantasyName>> names;

        for (auto p : FantasyNames)
            names.emplace_back(p.second);

        return names;
    }

    static std::shared_ptr<FantasyName> getName(const pb::public_key_data &pub  ) {
        std::lock_guard<std::recursive_mutex> lockg{ name_mutex };
        auto iter = Commissioner::FantasyNames.find(pub);
        if ( iter == end(Commissioner::FantasyNames) ) return nullptr;

        return iter->second;
    }

    static std::shared_ptr<FantasyName> getName(const hash_t &hash) {
        std::lock_guard<std::recursive_mutex> lockg{ name_mutex };
        auto iter = Hash2Pk.find(hash);
        if (iter == end(Hash2Pk)) return nullptr;

        return getName(iter->second);
    }

    static std::shared_ptr<FantasyName> getName(const alias_t &alias) {
        std::lock_guard<std::recursive_mutex> lockg{ name_mutex };
        return getName(FantasyName::name_hash(alias));
    }


    static std::shared_ptr<FantasyName> AddName(const alias_t &alias, const std::string &pubkey)
    {
        auto pfn = makeName(alias,pubkey);
        {
            std::lock_guard<std::recursive_mutex> lockg{ name_mutex };
            Commissioner::Hash2Pk.emplace(pfn->hash(), pfn->pubkey());
            Commissioner::FantasyNames.emplace(pfn->pubkey(), pfn);
        }
        return pfn;

    }

    static std::shared_ptr<FantasyName> makeName(const alias_t &alias, const std::string &pubkey)
    {
        auto pk  = str2pk(pubkey);
        auto pfn = std::make_shared< FantasyName >(alias,pk);
        return pfn;
    }

    static std::string FantasyAgentName() { return "FantasyAgent"; }


    static GlobalState InitialGlobalState () {
        GlobalState gs{};
        gs.set_season(2014);
        gs.set_state(GlobalState::OFFSEASON);
        gs.set_week(0);

        return gs;
    }

    static Block makeGenesisBlock();
    static Bootstrap makeGenesisBoot(LdbWriter &ldb,string = "202100");
    static bool BootStrapFileExists(string);

    static const int BLOCK_VERSION = 1;
    // 4/12/2018 -
    //  TRANS_VERSION - 4  version4 requires timestamps
    //
    static const int TRANS_VERSION = 4;
    static const int GENESIS_NUM = 1;


    static bool verify(const pb::signature &sig, const pb::sha256 &digest, const pb::public_key_data& pk) {
          return pb::public_key(pk).verify(digest, sig);// pb::public_key(sig, digest) == pub;
    }

    static bool verifyOracle(const pb::signature &sig, const pb::sha256 &digest)
    {
#ifndef PRODFOOTBALL
        return verify(sig, digest, GENESIS_PUB_KEY);// || verify(sig, digest, MASTER_PUB_KEY);

//        return verify(sig, digest, GENESIS_PUB_KEY) || verify(sig, digest, PROD_GENESIS_PUB_KEY);
#else
        return verify(sig, digest, GENESIS_PUB_KEY);// || verify(sig, digest, MASTER_PUB_KEY);
#endif
    }

    static bool verifyByName(const pb::signature &sig, const pb::sha256 &digest,const std::string &fn)
    {
        std::lock_guard<std::recursive_mutex> lockg{ name_mutex };
        auto iter = Hash2Pk.find(FantasyName::name_hash(fn));
        if (iter == Hash2Pk.end()) {
            qCritical() << "error cant find fantasyname: " << fn.data();
            return false;
        }
        else {
            auto ret = verify(sig, digest, iter->second);
            if ( !ret )
                qCritical()  << "error !verify(sig, digest, iter->second)";
            return ret;
        }
    }

    static std::string pk2str(const pb::public_key_data &pk)
    {
        return pb::to_base58(pk);
    }

    static pb::public_key_data privStr2Pub(const std::string &priv_key) {
        return pb::secp256k1_privkey::regenerate(priv_key).get_public_key().serialize();
    }

    static pb::public_key_data str2pk(const std::string &str)
    {
        pb::public_key_data pbb;
        pb::from_base58(str, (char *)pbb.key_data, 33);
        return pbb;
    }

    static std::string sig2str(const pb::signature &sig)
    {
        return pb::to_base58((char *)sig.data, 64);
    }

    static pb::signature str2sig(const std::string &str)
    {
        unsigned char data[72];
        pb::signature sig;
        if ( pb::from_base58(str, (char *)data, 64) > 64 ) {
            //Warning: still dealing with "bad" sigs from 2015
            auto sz = pb::from_base58(str, (char *)data, 72);
            sig = pb::parse_der(data,sz < 72 ? sz : 72);
            sig = pb::signature_normalize(sig);
        }
        else memcpy(sig.data,data,64);

        return sig;
    }

    static std::vector<std::string> GENESIS_NFL_TEAMS;
    static std::vector<std::string> GET_GENESIS_NFL_TEAMS() {
        return GENESIS_NFL_TEAMS;
    }

    static NFL_Weeks WK;
};

}

#endif /* defined(__fantasybit__Comissioner__) */


/****************************GENESIS BLOCK***************************************
SignedBlock
{
    block
    {
        head
        {
            num: 1
            prev_id : "00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
        }
        signed_transactions
        {
            trans
            {
                version: 1
                type : NAME
                [fantasybit.NameTrans.name_trans]
                {
                    hash: 10576213825162658308
                    public_key : "mT1M2MeDjA1RsWkwT7cjE6bbjprcNi84cWyWNvWU1iBa"
                    nonce : 57428892
                    utc_sec : 1408989677
                    prev_id : "00000000000000000000000000000000000000000000000000000000"
                    sig : "iKkkiYrzqzRo4Cgz1TeZty4JY4KUrDWyPgeF5tKpeRoRD14zWubsFneY8fW7UodCpP3JXXrFvWh6UkSWD7NcktHDK9gb4i9D3m"
                    sigid : "19cacff77cae784ada296272e43b6dd6f22975d1"
                }
            }
            id: "6ca607c105f8f9adfa652a89c285e58a1848f35caef132267e0385f79c453eb4"
            sig: "iKkki4FAQFoNR4foHVv1KNqfnJ1Fm1xuTToW3LgRjfAem2PSuPU3cH7ZPiJNm3xyTLt2bJx5kdRMfn1aEhfCGiTsHbE3PHBeis"
            fantasy_name : "FantasyAgent"
        }
    }
    id: "c9348ceb2551871534121114cd707c40653303250602aad6c6e0c67c522e5e9c"
    sig : "iKkkiYr6vYFtkRtxCeWQvu7iZ9oFdLwrpRe1P3XYUwZz3BvBuwiufWTFj1JSRJ3d1zjvp9W2whNVTWtT5Jxtn1ByyiW3qQYMyy"
}
*********************************************************************************************************************************/


//
//  FanatsyAgent.h
//  fantasybit
//
//  Created by Jay Berg on 4/6/14.
//
//

#ifndef __fantasybit__FanatsyAgent__
#define __fantasybit__FanatsyAgent__

#include <map>

//#include "optional.hpp"
#include "ProtoData.pb.h"
#include "FantasyName.h"
#include "utils/utils.h"

using namespace std;

namespace fantasybit
{

class FantasyAgent
{
    pb::secp256k1_privkey m_priv, m_oracle;
    std::shared_ptr<FantasyName> client;
    std::shared_ptr<FantasyNameCHash> client2;
    std::vector<SignedTransaction> pendingTrans{};
    Block prevBlock{};

    std::vector<Secret3> m_secrets;
    std::string secretfilename4 = "secret4.out";
    std::string secretfilename3 = "secret3.out";

    std::unordered_map<std::string,Secret3> m_pending;

public:
    enum status { AVAIL, NOTAVAILABLE, OWNED };

    template<typename T, typename... Args>
    unique_ptr<T> make_unique(Args&&... args) {
        return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
    }

    FantasyAgent(string = "");

    std::multimap<std::string,std::string> getMyNames();
    std::vector<MyFantasyName> getMyNamesStatus(bool selectlast = false);

    bool makeGenesis();
    void onSignedTransaction(SignedTransaction &sn);
    bool HaveClient() const;
    bool amDataAgent();

    secp256k1_ecdsa_signature sign_raw(const pb::sha256 &in) {
        if ( HaveClient() )
            return m_priv.sign(in);

        else
            return secp256k1_ecdsa_signature();
    }

    static bool AmFantasyAgent(pb::public_key_data pubkey);

    static bool AmFantasyAgent(std::string pubkey);

    void writeNomNonic(string in) ;
    SignedTransaction makeSigned(Transaction &trans);
    template <class T>
    Transaction toTransaction(T &t) {
        Transaction tr{};
        tr.MutableExtension(T::trans)->CopyFrom(t);
        return tr;
    }

    SignedBlockHeader makeSigned(BlockHeader &bh);

    template <class T>
    SignedTransaction toSignedTransaction(T &t) {
        return makeSigned(toTransaction(t));
    }

    // this is a push test




    bool testIt(Secret3 secret);

    status signPlayer(std::string name);

    status useName(std::string name);

    //bool beOracle();
//    bool beDataAgent();// { return beOracle();  }

    //Block makeNewBlockAsOracle();
//    Block makeNewBlockAsDataAgent(const SignedTransaction &, fc::optional<BlockHeader>);
//    fc::optional<Block> makeNewBlockAsDataAgent(Transaction &tdt,
//                                                fc::optional<BlockHeader> myprev);
    std::string getSecret() const ;
    pb::public_key_data pubKey();

    std::string getMnemonic(std::string fname);
    std::string pubKeyStr();

    std::pair<std::string, std::string> getIdSig(const std::string &in);

    std::pair<std::string, std::string>
    getIdSig(const std::string &in, pb::secp256k1_privkey &pk) ;

    static pb::secp256k1_privkey str2priv(const std::string &in);

    MyFantasyName getCurrentNamesStatus();

    bool UseName(const std::string &name);

    static pair<pb::secp256k1_privkey,string> makePrivMnemonic();
    static pb::secp256k1_privkey fromMnemonic(const string &in);

    std::pair<pb::sha256, pb::signature>
    getRawIdSig(const std::string &in, pb::secp256k1_privkey &pk) ;

    MyFantasyName UseMnemonic(std::string mn, bool store=true);
    std::string startImportMnemonic(std::string mn);


    static pb::sha256 BlockHash(const Block &b) {
        return pb::hashit(b.signedhead().head().SerializeAsString());
    }

    std::string currentClient();
    uint64_t currentClient2();

    bool finishImportMnemonic(const std::string &pk, const std::string &name);
    std::string defaultName();
    bool readFromSecret(const std::string &readfrom, bool transfer = false);
};

}

#endif /* defined(__fantasybit__FanatsyAgent__) */

//
//  FanatsyAgent.h
//  fantasybit
//
//  Created by Jay Berg on 4/6/14.
//
//

#ifndef __fantasybit__FanatsyAgent__
#define __fantasybit__FanatsyAgent__

#include <fc/optional.hpp>
#include <fc/crypto/elliptic.hpp>
#include "ProtoData.pb.h"
#include "FantasyName.h"
#include <map>

using namespace std;

namespace fantasybit
{

class FantasyAgent
{
    fc::optional<fc::ecc::private_key> m_priv, m_oracle;
    std::shared_ptr<FantasyName> client;
	std::vector<SignedTransaction> pendingTrans{};
	Block prevBlock{};

    std::vector<Secret3> m_secrets;
    std::string secretfilename3 = "secret3.out";
    std::string secretfilename2 = "secret2.out";

public:
    enum status { AVAIL, NOTAVAILABLE, OWNED };
    
    FantasyAgent(string = "");

    std::multimap<std::string,std::string> getMyNames();
    std::map<std::string,MyFantasyName> getMyNamesStatus();

	bool makeGenesis();
    void onSignedTransaction(SignedTransaction &sn);
    bool HaveClient();
    bool amDataAgent();

    static bool AmFantasyAgent(fc::ecc::public_key_data pubkey);

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

    bool testIt(Secret3 secret);

    status signPlayer(std::string name);

    status useName(std::string name);

    //bool beOracle();
    bool beDataAgent();// { return beOracle();  }

    //Block makeNewBlockAsOracle();
    Block makeNewBlockAsDataAgent(const SignedTransaction &, fc::optional<BlockHeader>);
    fc::optional<Block> FantasyAgent::makeNewBlockAsDataAgent(Transaction &tdt,
                                                fc::optional<BlockHeader> myprev);
    std::string getSecret() const ;
    fc::ecc::public_key_data pubKey();

    std::string pubKeyStr();

    std::pair<std::string, std::string> getIdSig(std::string &in);

    std::pair<std::string, std::string>
    getIdSig(std::string &in, fc::ecc::private_key &pk) ;

    static fc::ecc::private_key str2priv(const std::string &in);

    MyFantasyName FantasyAgent::getCurrentNamesStatus();

    bool FantasyAgent::UseName(std::string name);

    static pair<fc::ecc::private_key,string> FantasyAgent::makePrivMnemonic();
    static fc::ecc::private_key FantasyAgent::fromMnemonic(const string &in);

    std::pair<fc::sha256, fc::ecc::signature>
    FantasyAgent::getRawIdSig(std::string &in, fc::ecc::private_key &pk) ;

    MyFantasyName UseMnemonic(std::string mn, bool store=true);


};

}

#endif /* defined(__fantasybit__FanatsyAgent__) */

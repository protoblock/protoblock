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

namespace fantasybit
{

class FantasyAgent
{
    fc::optional<fc::ecc::private_key> m_priv, m_oracle;
    std::unique_ptr<FantasyName> client;
	std::vector<SignedTransaction> pendingTrans{};
	Block prevBlock{};

    std::vector<Secret2> m_secrets;
    std::string secretfilename = "secret2.out";

public:
    enum status { AVAIL, NOTAVAILABLE, OWNED };
    
    FantasyAgent();

    std::multimap<std::string,std::string> getMyNames();
    std::map<std::string,MyFantasyName> getMyNamesStatus();

	bool makeGenesis();
    void onSignedTransaction(SignedTransaction &sn);
    bool HaveClient();
    bool amDataAgent();

    static bool AmFantasyAgent(fc::ecc::public_key_data pubkey);

    static bool AmFantasyAgent(std::string pubkey);

    SignedTransaction makeSigned(Transaction &trans);
	template <class T>
	Transaction toTransaction(T &t) {
		Transaction tr{};
		tr.MutableExtension(T::trans)->CopyFrom(t);
		return tr;
	}

	template <class T>
	SignedTransaction toSignedTransaction(T &t) {
		return makeSigned(toTransaction(t));
	}

    status signPlayer(std::string name);

    //bool beOracle();
    bool beDataAgent();// { return beOracle();  }

    //Block makeNewBlockAsOracle();
	Block makeNewBlockAsDataAgent(const SignedTransaction &);

    std::string getSecret() const ;
    fc::ecc::public_key_data pubKey();

    std::string pubKeyStr();

    std::pair<std::string, std::string> getIdSig(std::string &in);

    std::pair<std::string, std::string>
    getIdSig(std::string &in, fc::ecc::private_key &pk) ;

    static fc::ecc::private_key str2priv(const std::string &in);

};

}

#endif /* defined(__fantasybit__FanatsyAgent__) */

//
//  FanatsyAgent.cpp
//  fantasybit
//
//  Created by Jay Berg on 4/6/14.
//
//


#include <iostream>
#include <utility>
#include <memory>
#include "FantasyAgent.h"
#include "Commissioner.h"
#include "DataPersist.h"
//#include "optional.hpp"
#include "FantasyName.h"
#include "mnemonic.h"
#include "utils/utils.h"
#include <openssl/rand.h>
//#include <QFile>
//#include <QString>
#include <QStandardPaths>

using namespace fantasybit;

FantasyAgent::FantasyAgent(string filename ) : client{nullptr} {
    if ( filename != "" )
        secretfilename4 = filename;

    if ( !readFromSecret( GET_ROOT_DIR() +  secretfilename4, false) ) {
        #ifdef Q_OS_MAC
            QString path = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
//            path.append("/tradingfootball/storage/" + secretfilename3);
            std::string oldosxsecret = path.toStdString() + "/tradingfootball/storage/" + secretfilename3;
            qDebug() << " oldosxsecret " << oldosxsecret.data();
            readFromSecret( oldosxsecret, true);
        #endif

         readFromSecret( GET_ROOT_DIR() +  secretfilename3, true);
    }
}

bool FantasyAgent::readFromSecret(const std::string &readfrom, bool transfer) {

    qDebug() << "readFromSecret " << readfrom.data();
    std::vector<Secret3> temp;
    {
        Reader<Secret3> read{ readfrom };
            if ( !read.good() )
                return false;

        Secret3 secret;
        while (read.ReadNext(secret)) {
            if (transfer) temp.push_back(secret);

            if ( !testIt(secret) ) {
                qCritical() << " secret verify fail" << secret.fantasy_name();
                continue;
            }

            if ( secret.has_mnemonic_key() )
                secret.clear_mnemonic_key();

            m_secrets.push_back(secret);
//            qDebug() << secret.fantasy_name().data() << " have key";
            if ( AmFantasyAgent(secret.public_key())) {
                  auto pr = str2priv(secret.private_key());
                  m_oracle = pr;
                qInfo() << " is oracle key";
            }
        }
    }

    if ( transfer ) {
        Writer<Secret3> writer{ GET_ROOT_DIR() + secretfilename4, ios::app };
        for ( auto &sec : temp)
            writer(sec);
    }

    return true;

}

std::string FantasyAgent::getMnemonic(std::string fname) {
    Reader<Secret3> read{ GET_ROOT_DIR() +  secretfilename4};
    if ( !read.good() ) {
        qDebug() << "getMnemonic bad read";
        return "bad read";
    }

    string ret = fname;
    Secret3 secret{};
    while (read.ReadNext(secret)) {

        if ( secret.fantasy_name() == fname ) {
            if ( !testIt(secret) ) {
                qCritical() << " secret verify fail" << secret.fantasy_name();
                ret += " verify fail";
                continue;
            }

            if ( secret.has_mnemonic_key() )
                return secret.mnemonic_key();
            else
                ret += " no has_mnemonic_key";
        }
    }

    return ret + " not found";
}

bool FantasyAgent::testIt(Secret3 secret) {
    //qDebug() << OPENSSL_VERSION_NUMBER;
    auto mnpriv = fromMnemonic(secret.mnemonic_key());

    auto ret = secp256k1_ec_seckey_verify(pb::CTX, mnpriv.data());

    string in = "sdfsfsdfs80999nbb b^&%#^  *(*& 65  \\00xx  *(&(54cV f" + secret.SerializeAsString();
    auto idsig1 = getRawIdSig(in,mnpriv);
    pb::secp256k1_privkey privateKey = str2priv(secret.private_key());
    auto idsig2 = getRawIdSig(in,privateKey);



//    auto pubk1 = mnpriv.get_public_key().serialize();
//    auto pubk2 = Commissioner::privStr2Pub(secret.private_key());

//    ret = secp256k1_ec_seckey_verify(pb::CTX,
//                                          reinterpret_cast<const unsigned char *>(secret.private_key().data()));
//    secp256k1_pubkey pubkey;
//    ret = secp256k1_ec_pubkey_create(pb::CTX, &pubkey,
//                                          reinterpret_cast<const unsigned char *>(secret.private_key().data()));

//    unsigned char pubkeyc[65];
//    size_t pubkeyclen = 65;
//    ret = secp256k1_ec_pubkey_serialize(pb::CTX, pubkeyc, &pubkeyclen, &pubkey, SECP256K1_EC_COMPRESSED);
//    memset(&pubkey, 0, sizeof(pubkey));
//    ret = secp256k1_ec_pubkey_parse(pb::CTX, &pubkey, pubkeyc, pubkeyclen);


//    unsigned char privkey[32];
//    unsigned char privkey2[32];
//    unsigned char privkeyc[300];
//    size_t prickeyclen = 300;
//    memcpy(privkey,secret.private_key().data(),32);

////    CHECK(ec_privkey_export_der(pb::CTX, privkeyc, &prickeyclen, privkey, 1) == 1);
////    CHECK(ec_privkey_import_der(pb::CTX, privkey2, privkeyc, prickeyclen) == 1);
////    CHECK(memcmp(privkey, privkey2, 32) == 0);

    auto pubk1 = mnpriv.get_public_key().serialize();

    auto priv2 = str2priv(secret.private_key());
    auto pubk2 = Commissioner::privStr2Pub(secret.private_key());
    auto pubk22 = priv2.get_public_key().serialize();

    auto pubk3 = privateKey.get_public_key().serialize();

    auto s1 = Commissioner::pk2str(pubk1);
    auto s2 = Commissioner::pk2str(pubk2);
    auto s3 = Commissioner::pk2str(pubk3);
    auto s4 = Commissioner::pk2str(pubk22);


    auto ss1 = privateKey.get_secret().str();
    auto s21 = mnpriv.get_secret().str();


//    if ( pubk1 == pubk2 ) {
//        qDebug() << " == " << Commissioner::pk2str(pubk1) << Commissioner::pk2str(pubk2);
//    }
//    else
//        qDebug() << " != " << Commissioner::pk2str(pubk1) << Commissioner::pk2str(pubk2);

    for ( auto h : {idsig1.second, idsig2.second}) {
        for ( auto s : {idsig1.first, idsig2.first})
            for ( auto p : {pubk1, pubk2})

        if ( !Commissioner::verify(h,s,p) )
            return false;
    }

    return true;
}

std::multimap<std::string,std::string> FantasyAgent::getMyNames() {
    std::multimap<std::string,std::string> ret;
    for ( auto s2 : m_secrets ) {
        ret.emplace(s2.fantasy_name(),s2.public_key());
    }

    return ret;
}

std::map<string,MyFantasyName> FantasyAgent::getMyNamesStatus(bool selectlast) {
    MyFantasyName selected;
    selected.set_status(MyNameStatus::none);
    std::map<string,MyFantasyName> ret{};
    for ( auto s2 : m_secrets ) {
        MyFantasyName fn{};
        fn.set_name(s2.fantasy_name());
        auto fname = Commissioner::getName(Commissioner::str2pk(s2.public_key()));
        if ( fname == nullptr ) {
            if ( Commissioner::isAliasAvailable(s2.fantasy_name()))
                fn.set_status(MyNameStatus::requested);
            else
                fn.set_status(MyNameStatus::notavil);
        }
        else if ( fname->alias() == s2.fantasy_name() )
            fn.set_status(MyNameStatus::confirmed);
        else if ( FantasyName::name_hash(s2.fantasy_name()) == fname->hash() ) {
            fn.set_status(MyNameStatus::confirmed);
            fn.set_name(fname->alias());
        }
        else
            fn.set_status(MyNameStatus::none);

        auto s = ret.find(fn.name());
        if ( s != end(ret) ) {
            if ( s->second.status() >= fn.status() )
                continue;
        }

        ret[fn.name()] = fn;
        if (fn.status() >= selected.status()) {
            selected = fn;
        }
    }

    if ( selectlast && selected.name() != "") {
       UseName(selected.name());
    }
    return ret;
}

MyFantasyName FantasyAgent::getCurrentNamesStatus() {
    MyFantasyName fn{};
    fn.set_status(MyNameStatus::none);

    if ( !HaveClient()) return fn;

    fn.set_name(client->alias());

    auto fname = Commissioner::getName(client->pubkey());
    if ( fname == nullptr ) {
        if ( Commissioner::isAliasAvailable(fn.name()))
            fn.set_status(MyNameStatus::requested);
        else
            fn.set_status(MyNameStatus::notavil);
    }
    else if ( fname->alias() == fn.name() )
        fn.set_status(MyNameStatus::confirmed);
    else if ( FantasyName::name_hash(fn.name()) == fname->hash() ) {
        fn.set_status(MyNameStatus::confirmed);
        fn.set_name(fname->alias());
    }
    else
        fn.set_status(MyNameStatus::none);

    return fn;
}

void FantasyAgent::onSignedTransaction(SignedTransaction &sn)
{
    pendingTrans.emplace_back(sn);
//    qDebug() << sn.DebugString();
}

bool FantasyAgent::HaveClient() const {

    return client2 != nullptr || client != nullptr;
}


/*!
 * \brief FantasyAgent::currentClient
 * \return
 * returns the current fantasyName of the end user.
 * if the "client" is not running it returns NOT GOOD
 */
std::string FantasyAgent::currentClient() {

    if ( !HaveClient() ) {
        return "NOT GOOD";
    }
    return client->alias();
}

uint64_t FantasyAgent::currentClient2() {
    return client2->hash();
}


bool FantasyAgent::amDataAgent() {
    //Todo: fix
    if (m_oracle.data() != nullptr && m_priv.data() != nullptr)
        return m_oracle.get_secret() == m_priv.get_secret();
    else
        return false;
}

bool FantasyAgent::AmFantasyAgent(pb::public_key_data pubkey) {
    return Commissioner::GENESIS_PUB_KEY == pubkey;
}

bool FantasyAgent::AmFantasyAgent(std::string pubkey) {
    return Commissioner::GENESIS_PUB_KEY == Commissioner::str2pk(pubkey);
}

SignedTransaction FantasyAgent::makeSigned(Transaction &trans) {
   SignedTransaction st{};
    st.mutable_trans()->CopyFrom(trans);
    string strTrans = string(trans.SerializeAsString());
    std::pair<std::string, std::string> p = getIdSig(strTrans);
    st.set_id(p.first);
    st.set_sig(p.second);
    st.set_fantasy_name(client->alias());
    return st;
}

SignedBlockHeader FantasyAgent::makeSigned(BlockHeader &bh) {
   SignedBlockHeader sbh{};
   sbh.mutable_head()->CopyFrom(bh);
    string strHeader = string(bh.SerializeAsString());
   std::pair<std::string, std::string> p = getIdSig(strHeader);
   sbh.set_sig(p.second);
//    qDebug() << p.first;
   return sbh;
}


std::string FantasyAgent::getSecret() const {
    if ( !HaveClient()) return "";
    return m_priv.get_secret().str();
}

pb::public_key_data FantasyAgent::pubKey() {
//    pb::public_key_data retpk;
//    size_t pubkeyclen = 33;
//    secp256k1_pubkey pubkey;
//    unsigned char pubkeyc[33];
//    auto ret = secp256k1_ec_pubkey_create(pb::CTX, &pubkey,
//                 reinterpret_cast<const unsigned char *>(m_priv.get_secret().data()));


//    ret = secp256k1_ec_pubkey_serialize(pb::CTX, pubkeyc, &pubkeyclen, &pubkey, SECP256K1_EC_COMPRESSED);

//    memcpy(retpk.key_data,pubkeyc,33);
//    return retpk;

    return m_priv.get_public_key().serialize();
//    return (*m_priv).get_public_key().serialize();
}

std::string FantasyAgent::pubKeyStr() {
//    qDebug() << "priv|" << m_priv.data() << "|" ;
    return (m_priv.data() != nullptr) ?
                Commissioner::pk2str(m_priv.get_public_key().serialize())
              : "";
}


std::pair<std::string, std::string> FantasyAgent::getIdSig(const std::string &in) {
    return (m_priv.data() != nullptr) ? getIdSig(in, m_priv)
                   :  std::make_pair("","");
}


std::pair<std::string, std::string>
FantasyAgent::getIdSig(const std::string &in,pb::secp256k1_privkey &pk) {
    auto sha = pb::hashit( in );
    return make_pair(sha.str(), Commissioner::sig2str(pk.sign(sha)));
}


std::pair<pb::sha256, secp256k1_ecdsa_signature>
FantasyAgent::getRawIdSig(const std::string &in,pb::secp256k1_privkey &pk) {
    const auto sha = pb::hashit( in );
    return make_pair(sha, pk.sign(sha));
}


pb::secp256k1_privkey FantasyAgent::str2priv(const std::string &in) {
//    int ret = secp256k1_ec_seckey_verify(pb::CTX, reinterpret_cast<const unsigned char *>(in.data()));
//    qDebug() << ret;
    return pb::secp256k1_privkey::regenerate(in);
}

std::string FantasyAgent::startImportMnemonic(std::string mn) {
    try {
        auto priv = fromMnemonic(mn);
        Secret3 secret{};
        secret.set_private_key(priv.get_secret().str());
        secret.set_public_key(Commissioner::pk2str(priv.get_public_key().serialize()));
        secret.set_mnemonic_key(mn);

        m_pending[secret.public_key()] = secret;

        return secret.public_key();
    }
    catch (MnemonicException) {
        return "";
    }
}

bool FantasyAgent::finishImportMnemonic(const std::string &pk,
                                               const std::string &name) {
    auto it = m_pending.find(pk);
    if ( it == end(m_pending) ) return false;

    Secret3 &sec = it->second;
    sec.set_fantasy_name(name);

    for ( auto s2 : m_secrets ) {
        if ( s2.public_key() == sec.public_key() &&
             s2.fantasy_name() == sec.fantasy_name() ) {
                m_pending.erase(it);
                return true;
        }
    }

    Writer<Secret3> writer{ GET_ROOT_DIR() + secretfilename4, ios::app };
    writer(sec);
    m_secrets.push_back(sec);

    m_pending.erase(it);
    return true;
}

MyFantasyName FantasyAgent::UseMnemonic(std::string mn, bool store) {

    MyFantasyName mfn;
    mfn.set_status(MyNameStatus::none);
    mfn.set_name("");
    pb::secp256k1_privkey priv;
    try {
        priv = fromMnemonic(mn);
//        m_priv = priv;
    }
    catch (MnemonicException) {
        return mfn;
    }


    auto pk = priv.get_public_key().serialize();
    auto fn = Commissioner::getName(pk);

    if ( !fn ) {
        qDebug() << "name not found from mnemonic" << Commissioner::pk2str(pk).data();
        return mfn;
    }

    string name = fn->alias();
    qInfo() << "name found" << fn->ToString();

//    client = make_unique<FantasyName>(name, m_priv.get_public_key().serialize());
//    client2 = make_unique<FantasyNameCHash>(name, m_priv.get_public_key().serialize());
    mfn.set_status(MyNameStatus::confirmed);
    mfn.set_name(name);


    if ( UseName(name) ) //dont write if already have it
        return mfn;

    Secret3 secret{};
    secret.set_private_key(priv.get_secret().str());
    secret.set_public_key(Commissioner::pk2str(pk));
    secret.set_fantasy_name(name);
    m_secrets.push_back(secret);

    if ( store ) {
        Writer<Secret3> writer{ GET_ROOT_DIR() + secretfilename4, ios::app };
        secret.set_mnemonic_key(mn);
        writer(secret);
    }
    qInfo() << secretfilename4 << "name available saving secret to file " << name;

    UseName(name);
    return mfn;
}

void FantasyAgent::writeNomNonic(string in) {
    Secret3 secret{};
    secret.set_private_key(getSecret());
    secret.set_public_key(pubKeyStr());
    secret.set_fantasy_name(client->alias());

    Writer<Secret3> writer{ GET_ROOT_DIR() + in};
    writer(secret);
}

bool FantasyAgent::UseName(const std::string &name) {
    for ( auto s : m_secrets ) {

        if ( s.fantasy_name() == name) {
            auto str = s.private_key();
            auto pr = str2priv(str);
            m_priv = pr;
            client = make_unique<FantasyName>
                    (name, m_priv.get_public_key().serialize());
            client2 = make_unique<FantasyNameCHash>
                    (name, m_priv.get_public_key().serialize());

            return true;
        }
    }

    return false;
}

std::string FantasyAgent::defaultName() {

    std::string name = "";
    for (int i = m_secrets.size(); i > 0; i--) {
        if ( m_secrets[i-1].fantasy_name() != "" ) {
            name = m_secrets[i-1].fantasy_name();
            auto str = m_secrets[i-1].private_key();
            auto pr = str2priv(str);
            m_priv = pr;
            client = make_unique<FantasyName>
                    (name, m_priv.get_public_key().serialize());
            client2 = make_unique<FantasyNameCHash>
                    (name, m_priv.get_public_key().serialize());

            return name;
        }
    }

    return name;
}


pair<pb::secp256k1_privkey,string> FantasyAgent::makePrivMnemonic() {
//    auto priv = pb::secp256k1_privkey::generate();

//    static int init = init_openssl();
    uint8_t buf[32];

    int result = RAND_bytes((unsigned char*)buf, 32);
    string m_mnemonic = createMnemonic(buf);
    return make_pair(fromMnemonic(m_mnemonic),m_mnemonic);
}

pb::secp256k1_privkey FantasyAgent::fromMnemonic(const string &in) {
    auto hseed = mnemonicToSeed(in);
    pb::sha256 secret;
    // 32 bytes is 256bits
    memcpy(secret.begin(), hseed.data(), 32);
    int ret = secp256k1_ec_seckey_verify(pb::CTX, hseed.data());

    return  pb::secp256k1_privkey::regenerate(secret);
}

FantasyAgent::status FantasyAgent::signPlayer(std::string name) {
    status ret = NOTAVAILABLE;

    //if ( Commissioner::isAliasAvailable(name) )
    {
        for (auto fn : m_secrets) {
            if (fn.fantasy_name() == name) {
                auto pr = str2priv(fn.private_key());
                m_priv = pr;
                client = make_unique<FantasyName>(name, m_priv.get_public_key().serialize());
                client2 = make_unique<FantasyNameCHash>(name, m_priv.get_public_key().serialize());

                ret = AVAIL;
                //LOG(lg, info) << "name available, already have it in secret file " << name;
                qInfo() <<"name available, already have it in secret file " << name;
            }
        }

        if (ret != AVAIL) {

            auto pm = makePrivMnemonic();
//            qDebug() << "makePrivMnemonic " << pm.second.data() << " priv | " << pm.first.data() << "|";

            m_priv = pm.first;
            auto m_mnemonic = pm.second;
//            client = make_unique<FantasyName>(name, m_priv.get_public_key().serialize());
            client = std::unique_ptr<FantasyName>(new FantasyName(name, m_priv.get_public_key().serialize()));

//            qDebug() << "pubKeyStr |" << pubKeyStr().data() << "|";


            Writer<Secret3> writer{ GET_ROOT_DIR() + secretfilename4, ios::app };
//            qDebug() << writer.good() << "file name " << (GET_ROOT_DIR() + secretfilename3).data();
            Secret3 secret{};
            secret.set_private_key(getSecret());
            secret.set_mnemonic_key(m_mnemonic);
            secret.set_public_key(pubKeyStr());
            secret.set_fantasy_name(name);
            writer(secret);
//            qDebug() << "\n"<< secret.DebugString ().data ();
            secret.clear_mnemonic_key();
            m_secrets.push_back(secret);

            //LOG(lg, info) << "name available saving secret to file " << name;
//            qDebug() << "name available saving secret to file " << name.data();

            ret = AVAIL;




//            QString mnF =  QString::fromStdString (GET_ROOT_DIR());
//            mnF.append (QString::fromStdString (secretfilename3.data ()));
//            QFile f(mnF);
//            if (!f.open (QFile::ReadOnly | QFile::Text)){
//                qDebug() << "could not open the  " << mnF;
//            }
//            QTextStream in(&f);
//                  while (!in.atEnd()) {
//                      QString line = in.readLine();
//                      qDebug() << line;
//                  }
//            f.close ();
//            f.flush ();

        }

    }

    //lets see if I already have the keys for this name
//    else for ( auto fn : m_secrets) {
//        if ( FantasyName::name_hash(fn.fantasy_name()) == FantasyName::name_hash(name)) {
//            if ( auto p = Commissioner::getName(name) ) {
//                if ( p->pubkey() == Commissioner::str2pk(fn.public_key()) ) {
//                    ret = OWNED;
//                    m_priv = str2priv(fn.private_key());
//                    client = make_unique<FantasyName>(*p);
//                    if ( AmFantasyAgent(p->pubkey()))
//                        m_oracle = m_priv;
//                    //LOG(lg, info) << "I already own it " << name;
//                    qInfo()<< "I already own it " << name;
//                }
//                else {
//                    qInfo() << "I have wrong pub" << name;
//                    //LOG(lg, warning) << "I have wrong pub" << name;
//                }

//            }
//            else {
//                //LOG(lg, warning) << "Cant find name?" << name;
//                qInfo() << "Cant find name?" << name;
//            }
//        }
//    }

//    if ( ret == NOTAVAILABLE )
//        qInfo() << "name not avaiable" << name;

    return ret;
}


//static SignedBlock makeGenesisBlock() {
/*
bool FantasyAgent::makeGenesis()
{
    LOG(lg, trace) << "genesis ";

    Block sb = Commissioner::makeGenesisBlock();

    BlockProcessor bp{};
    bp.init();
    bp.process(sb);
    return true;
}
*/

//bool FantasyAgent::beDataAgent() {
//#ifdef NO_ORACLE_CHECK_TESTING
//    qInfo() << " no oracle test";

//    if ( !m_priv )
//        m_priv = pb::secp256k1_privkey::generate();

//    if ( !HaveClient() )
//        client = std::make_unique<FantasyName>
//                ("NO_ORACLE_CHECK_TESTING",m_priv.get_public_key().serialize());

//	m_oracle = m_priv;
//    Commissioner::GENESIS_PUB_KEY = m_oracle.get_public_key();
//	return true;
//#endif

//    bool ret = false;

//    if (m_priv && AmFantasyAgent(pubKey())) {
//        qInfo() << " is oracle key";
//		m_oracle = m_priv;
//        ret = true;
//    }
//    else for ( auto fn : m_secrets) {
//        if ( AmFantasyAgent(fn.public_key())) {
//            qInfo() << " found agent key";
//            m_priv = str2priv(fn.private_key());
//            m_oracle = m_priv;
//            client = std::make_unique<FantasyName>
//                    (Commissioner::FantasyAgentName(),(*m_priv).get_public_key());
//            ret = true;
//            break;
//        }
//    }

//    if (!ret) {
//        Secret2 oracle{};
//        Reader<Secret2> read{ GET_ROOT_DIR() + "oracle.txt" };
//        if (!read.good())
//            qWarning() << " no oracle.txt";
//        else if (read.ReadNext(oracle)) {
//            if ( !oracle.has_public_key() && oracle.has_private_key()) {
//                auto pk = str2priv(oracle.private_key()).get_public_key().serialize();
//                oracle.set_public_key(Commissioner::pk2str(pk));
//            }
//            if (AmFantasyAgent(oracle.public_key())) {
//                qInfo() << " have oracle.txt";
//                m_priv = str2priv(oracle.private_key());
//                m_oracle = m_priv;
//                client = std::make_unique<FantasyName>
//                        (Commissioner::FantasyAgentName(),(*m_priv).get_public_key());
//                ret = true;
//            }
//            else
//                qWarning() << "oracle is NOT Commissioner::GENESIS_PUB_KEY";
//		}
//        else
//            qCritical() << " bad read oracle.txt";
//	}

//    return ret;
//}

//fc::optional<Block> FantasyAgent::makeNewBlockAsDataAgent(Transaction &tdt,
//                                            fc::optional<BlockHeader> myprev) {

//    fc::optional<Block> b;
//    if (!amDataAgent()) {
//        qWarning() << "am not DataAgent - try tp beDataAgent";

//        if (!beDataAgent()) {
//            qCritical() << "cant makeNewBlockAsDataAgent am not agent";
//            return b;
//        }
//    }
//    qInfo() << "I am DataAgent";

//    auto dt = makeSigned(tdt);

//    b = makeNewBlockAsDataAgent(dt,myprev);
//    return b;

//}


//Block FantasyAgent::makeNewBlockAsDataAgent(const SignedTransaction &dt, fc::optional<BlockHeader> myprev) {
//	Block b{};

//	if (!amDataAgent()) {
//        qWarning() << "am not DataAgent - try tp beDataAgent";

//		if (!beDataAgent()) {
//            qCritical() << "cant makeNewBlockAsDataAgent am not agent";
//			return b;
//		}
//	}
//    qInfo() << "I am DataAgent";

//    auto prev = Node::getlastLocalBlock().signedhead().head();
//    if ( myprev )
//        if ( (*myprev).num() > prev.num() )
//            prev = *myprev;

//	BlockHeader bh{};
//	bh.set_version(Commissioner::BLOCK_VERSION);
//	bh.set_num(prev.num() + 1);
//	bh.set_prev_id(pb::hashit(prev.SerializeAsString()).str());
//    bh.set_timestamp( std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count());
//	//todo: t
//	bh.set_generator_pk(pubKeyStr());
//	bh.set_generating_sig(pb::hashit(prev.generating_sig() + bh.generator_pk()).str());
//	bh.set_basetarget(0); //todo
//	bh.set_blocktype(BlockHeader_Type_DATA);
//	bh.set_transaction_id(""); //todo: merkle root of tx

//	SignedBlockHeader sbh{};
//	sbh.mutable_head()->CopyFrom(bh);

//    std::pair<std::string, std::string> p = getIdSig(sbh.head().SerializeAsString(),*m_oracle);
//    sbh.set_sig(p.second);
//    //todo: store block hash from p.first

//	b.mutable_signedhead()->CopyFrom(sbh);

//	SignedTransaction st{};
//	SignedTransaction* st2 = b.add_signed_transactions();
//	st2->CopyFrom(dt);


//	leveldb::ReadOptions options;
//	options.snapshot = Node::txpool->GetSnapshot();
//	leveldb::Iterator *iter = Node::txpool->NewIterator(options);
//	for (iter->SeekToFirst(); iter->Valid(); iter->Next()) {

//		st.ParseFromString(iter->value().ToString());

//		SignedTransaction* st2 = b.add_signed_transactions();
//		st2->CopyFrom(st);
//	}
//    delete iter;
//    Node::txpool->ReleaseSnapshot(options.snapshot);


//	/*
//	if (pendingTrans.size() == 0)
//		return b;

//	for (auto &pt : pendingTrans)
//	{
//		SignedTransaction* st2 = b.add_signed_transactions();
//		st2->CopyFrom(pt);
//	}

//	pendingTrans.clear();
//	*/


//	//sb.set_id(p.first);
//	//BlockProcessor bp{};
//	//bp.init();
//	//bp.process(sb);


//	return b;

//}


/*
Block FantasyAgent::makeNewBlockAsOracle() {
    Block b{};

    if (pendingTrans.size() == 0)
        return b;

    if (!beOracle())
    {
        fbutils::LogFalse(std::string("cant makeNewBlockAsOracle am not him"));
        return b;
    }

    BlockHeader bh{};
    bh.set_prev_id
        ("00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000");
    bh.set_num(2);
    bh.set_version(Commissioner::BLOCK_VERSION);

    SignedBlockHeader sbh{};
    sbh.mutable_head()->CopyFrom(bh);

    for (auto &pt : pendingTrans)
    {
        SignedTransaction* st2 = b.add_signed_transactions();
        st2->CopyFrom(pt);
    }

    b.mutable_signedhead()->CopyFrom(sbh);

    auto p = getIdSig(sbh.head().SerializeAsString(), m_oracle);
    //sb.set_id(p.first);
    sbh.set_sig(p.second);


    //BlockProcessor bp{};
    //bp.init();
    //bp.process(sb);
    pendingTrans.clear();

    return b;

}


/*
NameTrans nametrans{};
nametrans.set_hash(10576213825162658308);
nametrans.set_public_key(std::string("mT1M2MeDjA1RsWkwT7cjE6bbjprcNi84cWyWNvWU1iBa"));
nametrans.set_nonce(57428892);
nametrans.set_utc_sec(1408989677);
nametrans.set_prev_id("00000000000000000000000000000000000000000000000000000000");
nametrans.set_sig("iKkkiYrzqzRo4Cgz1TeZty4JY4KUrDWyPgeF5tKpeRoRD14zWubsFneY8fW7UodCpP3JXXrFvWh6UkSWD7NcktHDK9gb4i9D3m");
nametrans.set_sigid("19cacff77cae784ada296272e43b6dd6f22975d1");

Transaction trans{};
trans.set_version(1);
trans.set_type(TransType::NAME);
//[fantasybit.NameTrans.name_trans]
trans.MutableExtension(NameTrans::name_trans)->CopyFrom(nametrans);

SignedTransaction st{};
st.mutable_trans()->CopyFrom(trans);
st.set_id("6ca607c105f8f9adfa652a89c285e58a1848f35caef132267e0385f79c453eb4");
st.set_sig("iKkki4FAQFoNR4foHVv1KNqfnJ1Fm1xuTToW3LgRjfAem2PSuPU3cH7ZPiJNm3xyTLt2bJx5kdRMfn1aEhfCGiTsHbE3PHBeis");
st.set_fantasy_name("FantasyAgent");

BlockHeader bh{};
bh.set_prev_id("00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000");
bh.set_num(1);

Block b{};
b.mutable_head()->CopyFrom(bh);
SignedTransaction* st2 = b.add_signed_transactions();
st2->CopyFrom(st);

SignedBlock sb{};
sb.mutable_block()->CopyFrom(b);
sb.set_id("c9348ceb2551871534121114cd707c40653303250602aad6c6e0c67c522e5e9c");
sb.set_sig("iKkkiYr6vYFtkRtxCeWQvu7iZ9oFdLwrpRe1P3XYUwZz3BvBuwiufWTFj1JSRJ3d1zjvp9W2whNVTWtT5Jxtn1ByyiW3qQYMyy");

//std::cout << sb.DebugString();
//Commissioner::GenesisBlock = sb;
/*
auto nt = Commissioner::createGenesisName();
NameTrans nt2{};
nt2.set_hash(nt.name_hash);
nt2.set_public_key(Commissioner::pk2str(nt.pubkey));
nt2.set_nonce(nt.nonce);
nt2.set_utc_sec(nt.utc_sec.sec_since_epoch());
nt2.set_prev_id(nt.prev.str());
nt2.set_sig(Commissioner::sig2str(nt.sig));
nt2.set_sigid(nt.sigid());

Transaction trans{};
trans.set_version(1);
trans.set_type(TransType::NAME);
//trans.SetExtension(NameTrans::name_trans, );
trans.MutableExtension(NameTrans::name_trans)->CopyFrom(nt2);
//trans.SetExtension(NameTrans::name_trans, nt2);

SignedTransaction st{};
st.mutable_trans()->CopyFrom(trans);
auto p = getIdSig(trans.SerializeAsString());
st.set_id(p.first);
st.set_sig(p.second);
st.set_fantasy_name("FantasyAgent");

BlockHeader bh{};
bh.set_prev_id
("00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000");
bh.set_num(1);

Block b{};
b.mutable_head()->CopyFrom(bh);
SignedTransaction* st2 = b.add_signed_transactions();
st2->CopyFrom(st);

SignedBlock sb{};
sb.mutable_block()->CopyFrom(b);
auto pp = getIdSig(sb.block().SerializeAsString());
sb.set_id(pp.first);
sb.set_sig(pp.second);
//std::cout << "1 id " << pp.first << " sig " << pp.second << "\n";
*/
/*auto a = sb.block().signed_transactions().Get(0);
auto p1 = getIdSig(a.trans().SerializeAsString());
a.set_id(p1.first);
a.set_sig(p1.second);

std::cout << "\n" << a.DebugString() << "\n";

auto p = getIdSig(sb.block().SerializeAsString());
sb.set_id(p.first);
sb.set_sig(p.second);
std::cout << "\n" << sb.DebugString() << "\n";
*/


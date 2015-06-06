//
//  Server.cpp
//  fantasybit
//
//  Created by Jay Berg on 4/5/14.
//
//

#include "Server.h"
#include "DataPersist.h"
#include "ProtoData.pb.h"
#include "MsgSock.h"
#include "FantasyName.h"
#include "FantasyAgent.h"
#include <memory> 


using namespace std;
using namespace nn;
namespace fantasybit
{

void verify(const string priv_key,const NameTransaction &nt)
{
    fc::sha256 f2{priv_key};
    fc::ecc::private_key pk{fc::ecc::private_key::regenerate(f2)};
    //pk.verify(, nt.sig())
    
    name_transaction nt2(fc::sha224(nt.prev_id()));
    nt2.name_hash = nt.hash();
    //std::copy(begin(nt.public_key()),end(nt.public_key()),begin(nt2.pubkey));
	nt2.pubkey = Commissioner::str2pk(nt.public_key());
    nt2.nonce = nt.nonce();
    nt2.utc_sec = fc::time_point_sec{nt.utc_sec()};
    //std::copy(begin(nt.sig()),end(nt.sig()),begin(nt2.sig));
	nt2.sig = Commissioner::str2sig(nt.sig());
	bool ver = pk.verify(nt2.digest(),nt2.sig);

#ifdef TRACE
    cout << "priv_key{"<<priv_key<<"}\n hash " << nt.hash() << "\n";
    cout << "sig verify=" << ver <<
            " hit " << difficulty(nt2.id()) <<
            " target " << Commissioner::target(difficulty(nt2.prev)) << "\n\n";
#endif
}

void Server::init()
{
	Reader<Secret> read{ ROOT_DIR + "secret.out" };
    string priv_key{""};
    map<string,MyFantasyName> bestsecret{};
    Secret secret{};
    secret.set_private_key(priv_key);

    if ( read.good() )
    while (read.ReadNext(secret))
    {
        cout << secret.DebugString() << "\n";
        if ( secret.has_myfantasyname() )
        {
            if ( secret.private_key() == "" && secret.myfantasyname().has_nametransaction()
                    && priv_key != "")
                verify(priv_key,secret.myfantasyname().nametransaction());
            else
                verify(secret.private_key(),secret.myfantasyname().nametransaction());
            
            if ( bestsecret.find(secret.myfantasyname().name()) == end(bestsecret))
                bestsecret[secret.myfantasyname().name()] = secret.myfantasyname();
            else if ( secret.myfantasyname().status() >
                        bestsecret[secret.myfantasyname().name()].status())
            {
                bestsecret[secret.myfantasyname().name()] = secret.myfantasyname();
                priv_key = secret.private_key();
            }
        }
        else if (priv_key == "")
            priv_key = secret.private_key();
    }

#ifdef TRACE
    for(auto p :bestsecret)
        cout << p.second.DebugString() << "\n" ;
#endif

    OutData o;
    o.set_type( OutData_Type::OutData_Type_MYFANTASYNAME);
    
    if ( priv_key != "")
        agent.reset(new FantasyAgent{fc::sha256{priv_key}});
        
    if ( bestsecret.size() == 0)
    {
        secret.set_private_key(agent->getSecret());
        
        sender.send(o);
		Writer<Secret> writer{ ROOT_DIR + "secret.out", ios::trunc };
        writer(secret);
    }
    else for ( const auto& pair : bestsecret)
    {
        o.mutable_myfantasyname()->CopyFrom(pair.second);
        sender.send(o);
    }
}

void Server::run()
{
    init();

    Receiver rec{sock};
    InData indata;
    while (running)
    {
        rec.receive(indata);
      
        switch (indata.type())
        {
            case InData_Type_MINENAME:
                claimName(indata.data(),true);
                break;
            case InData_Type_QUIT:
                stop();
                break;
			case InData_Type_MAKE_BLOCK:
				//agent->makeGenesis();
			{
				Block sb = agent->makeNewBlockAsOracle();
				sender_blocks.send(sb);
				break;
			}
			case InData_Type_NEWNAME:
				agent->resetPrivateKey();
				claimName(indata.data(), false);
				break;
			case InData_Type_PROJ:
				{
				FantasyPlayerPoints fpp{};
				fpp.set_fantasy_player_id(indata.data2());
				fpp.set_points(indata.num());
				ProjectionTrans pj{};
				pj.set_game_id(indata.data());
				pj.mutable_fpp_projection()->CopyFrom(fpp);

				Transaction trans{};
				trans.set_version(Commissioner::TRANS_VERSION);
				trans.set_type(TransType::PROJECTION);
				trans.MutableExtension(ProjectionTrans::proj_trans)->CopyFrom(pj);
				SignedTransaction sn = agent->makeSigned(trans);
				agent->onSignedTransaction(sn);
				sender_trans.send(sn);

				}
				break;
			case InData_Type_RESULT:
				{
				FantasyPlayerPoints fpp{};
				fpp.set_fantasy_player_id(indata.data2());
				fpp.set_points(indata.num());
				ResultTrans rj{};
				rj.set_game_id(indata.data());
				FantasyPlayerPoints *fpp2 = rj.add_fpp_results();
				fpp2->CopyFrom(fpp);

				Transaction trans{};
				trans.set_version(Commissioner::TRANS_VERSION);
				trans.set_type(TransType::RESULT);
				trans.MutableExtension(ResultTrans::result_trans)->CopyFrom(rj);
				SignedTransaction sn = agent->makeSigned(trans);
				agent->onSignedTransaction(sn);
				sender_trans.send(sn);
				}
				break;
            default:
                break;
        }
        //cout << "server " << indata.DebugString() << "\n";
    }
}

void Server::claimName(const std::string &name,bool mine)
{
    //FantasyName me;
    //agent fa{true};
    
    cout << "Server::mine  {" << Commissioner::hashmineindex() << "}\n";
    auto ret = agent->signPlayer(name,mine);

    OutData o;
    o.set_type( OutData_Type::OutData_Type_MYFANTASYNAME);
    MyFantasyName mfn;
    mfn.set_name(name);
    mfn.set_status(MyNameStatus::none);
    switch (ret)
    {
        case FantasyAgent::REQUESTED:
            {
                mfn.set_status(MyNameStatus::requested);
                o.mutable_myfantasyname()->CopyFrom(mfn);
                sender.send(o);
            
				NameTrans nametrans{};
				NameProof np{};
				if (!mine)
				{
					np.set_type(NameProof_Type_ORACLE);
					nametrans.set_public_key(agent->pubKeyStr());
					nametrans.set_fantasy_name(name);
					nametrans.mutable_proof()->CopyFrom(np);
				}
				else
				{
					name_transaction nt = agent->getRequested();
					cout << "Server::after mine  {" << ret << "}\n";

					if (!running)
					{
						if (difficulty(nt.id()) < Commissioner::target(difficulty(nt.prev)))
							return;
					}

					fc::ecc::public_key pk{ nt.pubkey };
					if (!pk.verify(nt.digest(), nt.sig))
						cout << "error key not verfied \n";
					else {
						mfn.set_status(MyNameStatus::found);
						NamePOW nt2;
						nt2.set_hash(nt.name_hash);
						nt2.set_public_key(Commissioner::pk2str(nt.pubkey));
						nt2.set_nonce(nt.nonce);
						nt2.set_utc_sec(nt.utc_sec.sec_since_epoch());
						nt2.set_prev_id(nt.prev.str());
						nt2.set_sig(Commissioner::sig2str(nt.sig));
						nt2.set_sigid(nt.sigid());
						mfn.mutable_nametransaction()->CopyFrom(nt2);

						Writer<Secret> writer{ "secret.out", ios::app };
						Secret secret{};
						secret.set_private_key(agent->getSecret());
						secret.mutable_myfantasyname()->CopyFrom(mfn);
						writer(secret);

						o.mutable_myfantasyname()->CopyFrom(mfn);
						sender.send(o);

						np.set_type(NameProof_Type_POW);
						np.MutableExtension(NamePOW::name_pow)->CopyFrom(nt2);
						nametrans.set_public_key(agent->pubKeyStr());
						nametrans.set_fantasy_name(name);
						nametrans.mutable_proof()->CopyFrom(np);

					}
				}
				Transaction trans{};
				trans.set_version(Commissioner::TRANS_VERSION);
				trans.set_type(TransType::NAME);
				trans.MutableExtension(NameTrans::name_trans)->CopyFrom(nametrans);
				SignedTransaction sn = agent->makeSigned(trans);
				agent->onSignedTransaction(sn);
				sender_trans.send(sn);
            }
            break;
        case FantasyAgent::NOTAVAILABLE:
            mfn.set_status(MyNameStatus::notavil);
            o.mutable_myfantasyname()->CopyFrom(mfn);
            sender.send(o);
            break;
        case FantasyAgent::OWNED:
            mfn.set_status(MyNameStatus::confirmed);
            o.mutable_myfantasyname()->CopyFrom(mfn);
            sender.send(o);
            break;
        default:
            break;
    }
}


}
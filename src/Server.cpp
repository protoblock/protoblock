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
#include "boostLog.h"

//#define LOG(logger, severity) LOGIT(logger, severity,  __FILE__, __FUNCTION__)
#define LOG(logger, severity) LOGIT(logger, severity,  __FILE__, __LINE__, __FUNCTION__)


using namespace std;
using namespace nn;
namespace fantasybit
{

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
        LOG(lg,info) << "read from file" << secret.DebugString();
        if ( secret.has_myfantasyname() )
        {           
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

    for(auto p :bestsecret)
		LOG(lg, trace) << "Bestsecret: " << p.first << " "  << p.second.DebugString();
#ifdef TRACE

#endif

    OutData o;
    o.set_type( OutData_Type::OutData_Type_MYFANTASYNAME);
    
    if ( priv_key != "")
		agent.reset(new FantasyAgent{ fc::sha256{ priv_key }, secret.myfantasyname().name() });

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
      
		LOG(lg, trace) << "indata: " << InData_Type_Name(indata.type());

        switch (indata.type())
        {
            case InData_Type_QUIT:
                stop();
                break;
			case InData_Type_NEWNAME: 
			{
				claimName(indata.data());

				/*
				agent->resetPrivateKey();
				NameTrans nt{};
				nt.set_fantasy_name(indata.data());
				nt.set_public_key(agent->pubKeyStr());
				auto ret = agent->signPlayer(nt.fantasy_name());
				if (ret == MyNameStatus::notavil) {
					LOG(lg, info) << " name not avail " << nt.fantasy_name();
				}
				auto st = agent->toSignedTransaction(nt);
				agent->onSignedTransaction(st);
				sender_trans.send(st);

				OutData o;
				o.set_type(OutData_Type::OutData_Type_MYFANTASYNAME);
				MyFantasyName mfn;
				mfn.set_name(nt.fantasy_name());
				mfn.set_status(ret);
				*/

			}
			break;
			case InData_Type_PROJ:
				{
				FantasyPlayerPoints fpp{};
				fpp.set_playerid(indata.data2());
				fpp.set_points(indata.num());

				//todo: set season, week 

				ProjectionTrans pj{};
				pj.mutable_fpp()->CopyFrom(fpp);

				Transaction trans{};
				trans.set_version(Commissioner::TRANS_VERSION);
				trans.set_type(TransType::PROJECTION);
				trans.MutableExtension(ProjectionTrans::proj_trans)->CopyFrom(pj);
				SignedTransaction sn = agent->makeSigned(trans);
				agent->onSignedTransaction(sn);
				sender_trans.send(sn);

				}
				break;
			case InData_Type_DATA:
			{

				/*
				//todo: results as dataagent 
				Transaction trans{};
				trans.set_version(Commissioner::TRANS_VERSION);
				trans.set_type(TransType::DATA);
				trans.MutableExtension(DataTransition::data_trans)->CopyFrom(indata.data_trans());
				SignedTransaction sn = agent->makeSigned(trans);
				agent->onSignedTransaction(sn);
				*/
				Transaction trans{};
				trans.set_version(Commissioner::TRANS_VERSION);
				trans.set_type(TransType::DATA);
				trans.MutableExtension(DataTransition::data_trans)->CopyFrom(indata.data_trans());
				SignedTransaction sn = agent->makeSigned(trans);

				auto block = agent->makeNewBlockAsDataAgent(sn);
				sender_blocks.send(block);
				break;

			}
            default:
                break;
        }
        //cout << "server " << indata.DebugString() << "\n";
    }
}

void Server::claimName(const std::string &name)
{

    auto ret = agent->signPlayer(name);

	LOG(lg, info) << "claimName(" << name << ") " << to_string(ret);

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
				np.set_type(NameProof_Type_ORACLE);
				nametrans.set_public_key(agent->pubKeyStr());
				nametrans.set_fantasy_name(name);
				nametrans.mutable_proof()->CopyFrom(np);

				Transaction trans{};
				trans.set_version(Commissioner::TRANS_VERSION);
				trans.set_type(TransType::NAME);
				trans.MutableExtension(NameTrans::name_trans)->CopyFrom(nametrans);
				SignedTransaction sn = agent->makeSigned(trans);
				agent->onSignedTransaction(sn);
				sender_trans.send(sn);

				Writer<Secret> writer{ ROOT_DIR + "secret.out", ios::app };
				Secret secret{};
				secret.set_private_key(agent->getSecret());
				secret.mutable_myfantasyname()->CopyFrom(mfn);
				secret.mutable_nametran()->CopyFrom(nametrans);
				writer(secret);

				LOG(lg, info) << "saving secret to file " << secret.DebugString();

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
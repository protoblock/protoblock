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
    std::copy(begin(nt.public_key()),end(nt.public_key()),begin(nt2.pubkey));
    nt2.nonce = nt.nonce();
    nt2.utc_sec = fc::time_point_sec{nt.utc_sec()};
    std::copy(begin(nt.sig()),end(nt.sig()),begin(nt2.sig));
    bool ver = pk.verify(nt2.digest(), nt2.sig);

    cout << " verfied " << ver <<  "\n";
    if ( ver )
    {
        cout << " hit " << difficulty(nt2.id()) << " target " << Commissioner::target(difficulty(nt2.prev)) << "\n";
    }
}

void Server::init()
{
    Reader<Secret> read{"secret.out"};

    map<string,MyFantasyName> bestsecret{};
    Secret secret{};
    string priv_key{""};
    if ( read.good() )
    while (read.ReadNext(secret))
    {
        if ( secret.has_myfantasyname() )
        {
            if ( secret.private_key() == "" && secret.myfantasyname().has_nametransaction() && priv_key != "")
                verify(priv_key,secret.myfantasyname().nametransaction());
            
            if ( bestsecret.find(secret.myfantasyname().name()) == end(bestsecret))
                bestsecret[secret.myfantasyname().name()] = secret.myfantasyname();
            else if ( secret.myfantasyname().status() > bestsecret[secret.myfantasyname().name()].status())
            {
                bestsecret[secret.myfantasyname().name()] = secret.myfantasyname();
            }
        }
        else if (priv_key == "")
            priv_key = secret.private_key();
    }

    OutData o;
    o.set_type( OutData_Type::OutData_Type_MYFANTASYNAME);
   
    if ( bestsecret.size() == 0)
    {
        agent.reset(new FantasyAgent{});
        secret.set_private_key(agent->getSecret());
        sender.send(o);
        
        Writer<Secret> writer{"secret.out",ios::trunc};
        writer(secret);
    }
    else for ( const auto& pair : bestsecret)
    {
        o.mutable_myfantasyname()->CopyFrom(pair.second);
        sender.send(o);
    
        //MyFantasyName *m = o.MutableExtension(MyFantasyName_ext);
        //o.SetExtension(myfantasyname_ext,pair.second);
        //, <#typename _proto_TypeTraits::ConstType value#>)
        //MyFantasyName *m = o.MutableExtension(myfantasyname_ext);
        //*m=pair.second;
        //o.set_my
        //o.mutable_myfantasyname();
        //mfn->set_allocated_fantasyname(<#::fantasybit::NameTransaction *fantasyname#>)
        //o.set_allocated_myfantasyname(pair.sec)

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
            case InData_Type_MineName:
                mine(indata.data());
                break;
            default:
                break;
        }
        cout << "server " << indata.DebugString() << "\n";
    }
}

void Server::mine(const std::string &name)
{
    //FantasyName me;
    //agent fa{true};
    auto ret = agent->signPlayer(name);
    
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
            
                name_transaction nt = agent->getRequested();
                fc::ecc::public_key pk{nt.pubkey};
                if ( !pk.verify(nt.digest(),nt.sig) )
                    cerr << "error key not verfied \n";
                else {
                    mfn.set_status(MyNameStatus::found);
                    NameTransaction nt2;
                    nt2.set_hash(nt.name_hash);
                    nt2.set_public_key(&nt.pubkey,nt.pubkey.size());
                    nt2.set_nonce(nt.nonce);
                    nt2.set_utc_sec(nt.utc_sec.sec_since_epoch());
                    nt2.set_prev_id(nt.prev.str());
                    nt2.set_sig(&nt.sig, nt.sig.size());
                    mfn.mutable_nametransaction()->CopyFrom(nt2);
                    
                    Writer<Secret> writer{"secret.out",ios::app};
                    Secret secret{};
                    secret.set_private_key("");
                    secret.mutable_myfantasyname()->CopyFrom(mfn);
                    writer(secret);
                    
                    o.mutable_myfantasyname()->CopyFrom(mfn);
                    sender.send(o);
                }
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

/*
    const int bsize = 256;
    char buf[bsize];
    int flags=0;
    
            int size = sock.recv(buf, bsize, flags);
        outdata.ParseFromArray(buf, size);
*/





}
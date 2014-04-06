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

using namespace std;
using namespace nn;
namespace fantasybit
{

void Server::init()
{
    Reader<Secret> read{"secret.out",ios::in|ios::app};

    map<string,MyFantasyName> bestsecret{};
    Secret secret{};
    while (read.ReadNext(secret))
    {
        if ( secret.has_myfantasyname() )
        {
            if ( bestsecret.find(secret.myfantasyname().name()) == end(bestsecret))
                bestsecret[secret.myfantasyname().name()] = secret.myfantasyname();
            else if ( secret.myfantasyname().status() > bestsecret[secret.myfantasyname().name()].status())
            {
                bestsecret[secret.myfantasyname().name()] = secret.myfantasyname();
            }
        }
    }

    OutData o;
    o.set_type(Type::MYFANTASYNAME);

    for ( const auto& pair : bestsecret)
    {
        //MyFantasyName *m = o.MutableExtension(MyFantasyName_ext);
        //o.SetExtension(myfantasyname_ext,pair.second);
        //o.SetExtension(<#const ::google::protobuf::internal::ExtensionIdentifier<OutData, _proto_TypeTraits, _field_type, _is_packed> &id#>, <#typename _proto_TypeTraits::ConstType value#>)
        MyFantasyName *m = o.MutableExtension(myfantasyname_ext);
        *m=pair.second;
        size_t sz = o.ByteSize();
        void *buf = allocmsg(sz,0);
        o.SerializeWithCachedSizesToArray((::google::protobuf::uint8_t *)buf);
        sock.send(buf,sz,0);
    }
}



}
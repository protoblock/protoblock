//
//  MsgSock.cpp
//  fantasybit
//
//  Created by Jay Berg on 4/6/14.
//
//

#include "MsgSock.h"

namespace fantasybit
{

int Sender::Send(nn::socket &s, const google::protobuf::Message &msg, int flags)
{
    size_t sz = msg.ByteSize();
    void *buf = nn::allocmsg(sz,0);
    msg.SerializeWithCachedSizesToArray((::google::protobuf::uint8 *)buf);
    return s.send(buf,sz,flags);
}

 

}
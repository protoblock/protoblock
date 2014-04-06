//
//  FanatsyAgent.cpp
//  fantasybit
//
//  Created by Jay Berg on 4/6/14.
//
//

#include "FantasyAgent.h"

namespace fantasybit {

FantasyAgent::status FantasyAgent::signPlayer(alias_t name)
{
    status ret = NOTAVAILABLE;
   
    if ( Commissioner::isAliasAvailable(name) )
    {
        fut = std::async(&Commissioner::generateName, &comish, name, m_priv.get_public_key() );
        ret = REQUESTED;
    }
    else if ( auto p = Commissioner::getName(m_priv.get_public_key()) )
    {
        if ( p->isAlias(name) )
        {
            ret = OWNED;
            client = *p;
        }
    }
        
    return ret;
}

}
//
//  FantasyName.h
//  fantasybit
//
//  Created by Jay Berg on 3/19/14.
//
//

#ifndef __fantasybit__FantasyName__
#define __fantasybit__FantasyName__

#include <string>
#include <unordered_map>
#include <map>

namespace fantasybit
{

struct FantasyName
{
    using Pubkey = std::string;
    using Alias = std::string;
    
    Pubkey pubkey;
    Alias  alias;
    
    static std::unordered_map<Pubkey,std::shared_ptr<FantasyName>> FantasyNames;
    static std::map<Alias,Pubkey> Aliases;
    
    
};


}

#endif /* defined(__fantasybit__FantasyName__) */

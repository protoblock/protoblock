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
#include "fbutils.h"

namespace fantasybit
{

class Bits
{
    Int64 base;
public:
    Bits(Int64 b) : base(b) {}
    
    double points() { return base * .00000001; }
    double bits() { return points() * 100; }
    Int64 amount() { return base; }
    void add(Bits b) { base+=b.amount(); }
};

struct FantasyName
{
    using Pubkey = std::string;
    using Alias = std::string;
    
    Pubkey pubkey;
    Alias  alias;
    
    Bits getBalance() { return 0; }
     
    static std::unordered_map<Pubkey,std::shared_ptr<FantasyName>> FantasyNames;
    static std::map<Alias,Pubkey> Aliases;
};


}

#endif /* defined(__fantasybit__FantasyName__) */

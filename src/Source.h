//
//  Source.h
//  fantasybit
//
//  Created by Jay Berg on 3/22/14.
//
//

#ifndef __fantasybit__Source__
#define __fantasybit__Source__

#include <unordered_map>
#include "fbutils.h"

namespace fantasybit
{

class Source
{
    Uid event;
    std::unordered_map<Uid,Int> results; //player,points

public:
    Uid eventId() const { return event; }
    decltype(results) & getResults() { return results; }
};

}
#endif /* defined(__fantasybit__Source__) */

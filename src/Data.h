//
//  Data.h
//  fantasybit
//
//  Created by Jay Berg on 3/25/14.
//
//

#ifndef __fantasybit__Data__
#define __fantasybit__Data__

#include "FantasyName.h"

namespace fantasybit
{

template<class T>
class Data
{
    T data;
    FantasyName::Alias alias;
public:
    Data(T t) : data(t) {}
    
    Bits getStake() {
        Bits stake{0};

        do {
            stake.add(FantasyName::FantasyNames[FantasyName::Aliases[alias]].getBalance());
        } while ( !decrypt());
            
        return stake;
    }
    
    bool verify(Bits min) {
        return getStake() >= min;
    }
    
private:
    bool decrypt() {
        if ( data.isRaw() ) return true;
        
        //ToDo:
        //decript data
        //get alias
        //set data
        return false;
    }
};

}

#endif /* defined(__fantasybit__Data__) */

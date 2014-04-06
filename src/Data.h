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
#include "Commissioner.h"

namespace fantasybit
{

template<class T>
class Data
{
    T data;
    alias_t alias;
public:
    Data(T t) : data(t) {}
    
    Bits getStake() {
        Bits stake{0};

        do {
            stake.add(Commissioner::FantasyNames[Commissioner::Aliases[alias]].getBalance());
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


struct predict
{
    alias_t id;
    Uid player;
    int8_t weak;
    int8_t projection;
    Int nonce;

    Signature sig;
    void sign()
    {
    }
    
};

}

#endif /* defined(__fantasybit__Data__) */

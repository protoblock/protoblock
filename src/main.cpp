//
//  main.cpp
//  satoshifantasy
//
//  Created by User on 3/17/14.
//
#include "Comissioner.h"
#include "FantasyName.h"
#include <iostream>

using namespace fantasybit;
using namespace std;



int main(int argc, const char * argv[])
{
    FantasyName me;
    FantasyAgent fa{true};
    auto ret = fa.signPlayer("SatoshiFantasy");
    if ( ret == FantasyAgent::REQUESTED ) {
        me = fa.getRequested();
    
        cout << me.alias << "\n";
    }
    return 0;
};


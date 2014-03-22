//
//  main.cpp
//  satoshifantasy
//
//  Created by User on 3/17/14.
//  Copyright (c) 2014 User. All rights reserved.
//

#include <iostream>
#include <string>
#include <memory>
#include "FantasyName.h"
#include "DistributionAlgo.h"
#include "fbutils.h"
#include <numeric>

using namespace fantasybit;
using namespace std;

int main(int argc, const char * argv[])
{
    NameValuePairs<Int> proj {
       make_pair(shared_ptr<FantasyName>(new FantasyName{"key1","alias1"}), 10),
       make_pair(shared_ptr<FantasyName>(new FantasyName{"key2","alias2"}), 15),
       make_pair(shared_ptr<FantasyName>(new FantasyName{"key3","alias3"}), 16),
       make_pair(shared_ptr<FantasyName>(new FantasyName{"key4","alias4"}), 19),
       make_pair(shared_ptr<FantasyName>(new FantasyName{"key5","alias5"}), 1),
       make_pair(shared_ptr<FantasyName>(new FantasyName{"key6","alias6"}), 33),
       make_pair(shared_ptr<FantasyName>(new FantasyName{"key7","alias7"}), 11)
    };

    Int points = 14;
    DistribuePointsAvg d{proj};
    auto res = d.distribute(points);

    double sum = accumulate(begin(res), end(res), 0.0,
        [](const double rsum, decltype(res)::const_reference pair){return rsum + pair.second;});
    
    if ( sum > points )
        throw exception();
    
    return 0;
};


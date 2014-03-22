//
//  DistributionAlgo.cpp
//  fantasybit
//
//  Created by Jay Berg on 3/19/14.
//
//

#include "DistributionAlgo.h"
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <cmath>
#include <algorithm>
#include <numeric>


namespace fantasybit {

using namespace std;

NameValuePairs<double> DistribuePointsAvg::distribute(const Int result) const
{
    double mean = 0;
    vector<Int> diffs;
    diffs.reserve(projections.size());
    
    for(const auto& pair : projections) {
        Int diff = abs(result-pair.second);
        mean+=diff;
        diffs.emplace_back(diff);
    }
  
    mean /= projections.size();
    
    Int maxdiff = min((Int)lround(mean),result);
    
    Int sum = accumulate(begin(diffs), end(diffs), 0,
        [maxdiff,result](const Int sum,const Int val) {return sum + ((val < maxdiff) ? result-val : 0);});
        
    double payout = static_cast<double>(result) / sum;
    NameValuePairs<double> award{};
<<<<<<< HEAD

    for (const auto& pair : projections) {
        Int diff = abs(result-pair.second);
        if ( diff < maxdiff )
            award.emplace_back(pair.first,(result-diff)*payout);
    }
=======
    for_each(begin(projections), end(projections),
        [&](decltype(projections)::const_reference pair)
        {
            Int diff = abs(result-pair.second);
            if ( diff < maxdiff )
                award.emplace_back(pair.first,(result-diff)*payout);
        });
>>>>>>> FETCH_HEAD
    
    return award;
}

  
}

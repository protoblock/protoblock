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
#include <assert.h>
 
namespace fantasybit {

using namespace std;

NameValuePairs<double> DistribuePointsAvg::distribute(const Int result , const std::string &agent) const
{
	NameValuePairs<double> award{};
	if (projections.size() == 0) {
		award.emplace(agent, result);
		return award;
	}
	
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
        [maxdiff,result](const Int sum,const Int val)
        {
            return sum + ((val < maxdiff) ? result-val : 0);
        });
        
    double payout = static_cast<double>(result) / sum;

	double total = 0.0;
    for (const auto& pair : projections) {
        Int diff = abs(result-pair.second);
		if (diff < maxdiff)
		{
			double amount = (result - diff)*payout;
			award.emplace(pair.first, amount);
			total += amount;
		}
    }

	assert(result >= Int(total));
	double leftover = result - total;
	if (leftover > 0.0)
		award.emplace(agent, leftover);
    return award;
}
}

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
#include "boostLog.h"

#define LOG(logger, severity) LOGIT(logger, severity,  __FILE__, __LINE__, __FUNCTION__)

namespace fantasybit {

using namespace std;

NameValuePairs<double> DistribuePointsAvg::distribute(const Int64 result, const std::string &agent) const
{
	NameValuePairs<double> award{};
	if (projections.size() == 0) {
		LOG(lg, info) << "no projections agent " << agent << " get balance " << result;
		award.emplace(agent, result);
		return award;
	}
	
    double mean = 0;
    vector<Int> diffs;
    diffs.reserve(projections.size());
    
    for(const auto& pair : projections) {

		Int diff = abs((int64_t)result - (int64_t)pair.second);
        mean+=diff;
        diffs.emplace_back(diff);

		LOG(lg, trace) << pair.first << " projection " << pair.second << " diff " << diff;
    }
  
    mean /= projections.size();
	LOG(lg, trace) << "mean " << mean;

    
	Int maxdiff = min((Int64)lround(mean), result);
    
    Int sum = accumulate(begin(diffs), end(diffs), 0,
        [maxdiff,result](const Int sum,const Int val)
        {
            return sum + ((val <= maxdiff) ? result-val : 0);
        });
        
    double payout = static_cast<double>(result) / sum;

	double total = 0.0;
    for (const auto& pair : projections) {
		Int diff = abs((int64_t)result - (int64_t)pair.second);
		if (diff <= maxdiff)
		{
			double amount = (result - diff)*payout;
			award.emplace(pair.first, amount);
			total += amount;
			LOG(lg, trace) << pair.first << " projection " << pair.second << " award " << amount;
		}
		else 
			LOG(lg, trace) << pair.first << " projection " << pair.second << "no award ";
    }

	assert(result >= Int(total));
	double leftover = result - total;
	if (leftover > 0.0) {
		award.emplace(agent, leftover);
		LOG(lg, trace) << "agent " << agent << " leftovers " << leftover;

	}
    return award;
}
}

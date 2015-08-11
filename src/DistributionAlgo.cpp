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

NameValuePairs<int>
    DistribuePointsAvg::distribute(const double result, const std::string &agent) const
{
    NameValuePairs<int> award{};
	if (projections.size() == 0) {
        LOG(lg, info) << "no projections agent " << agent << " gets balance " << result;
        award[agent] = result * 100.0;
		return award;
	}
	
    double mean = 0;
    vector<double> diffs;
    diffs.reserve(projections.size());
    
    for(const auto& pair : projections) {
        double diff = result - pair.second;
        diff = fabs(diff);
        mean+=diff;
        diffs.emplace_back(diff);

		LOG(lg, trace) << pair.first << " projection " << pair.second << " diff " << diff;
    }
  
    mean /= projections.size();
	LOG(lg, trace) << "mean " << mean;

    
    double maxdiff = min(mean, result);
    LOG(lg, trace) << " maxdiff " << maxdiff;

    
    double sum = accumulate(begin(diffs), end(diffs), 0,
        [maxdiff,result](const double sum,const double val)
        {
            return sum + ((val <= maxdiff) ? result-val : 0.0);
        });


        
    double payout = result / sum;

    LOG(lg, trace) << " sum " << sum << " payout " << payout;

	double total = 0.0;
    for (const auto& pair : projections) {
        double diff = fabs(result - pair.second);
		if (diff <= maxdiff)
		{
			double amount = (result - diff)*payout;
            award[pair.first] = amount * 100.0;
			total += amount;
			LOG(lg, trace) << pair.first << " projection " << pair.second << " award " << amount;
		}
		else 
			LOG(lg, trace) << pair.first << " projection " << pair.second << "no award ";
    }

    if (result < total) {
        LOG(lg,error) << "gave out to much";
    }
    else {
        double leftover = result - total;
        if (leftover > 0.00001) {
            award[agent] = leftover;
            LOG(lg, trace) << "agent " << agent << " leftovers " << leftover;

        }
    }
    return award;
}
}

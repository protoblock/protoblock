//
//  DistributionAlgo.h
//  fantasybit
//
//  Created by Jay Berg on 3/19/14.
//
//

#ifndef __fantasybit__DistributionAlgo__
#define __fantasybit__DistributionAlgo__

#include "fbutils.h"

namespace fantasybit
{
    
class DistributionAlgo {};

	
class DistributePoints : public DistributionAlgo
{
public:
	DistributePoints(const NameValuePairs<Int64> &p) : projections(p) {}
    
protected:
	NameValuePairs<Int64> projections;
};

struct DistribuePointsAvg : DistributePoints
{
	DistribuePointsAvg(const NameValuePairs<Int64> &p) : DistributePoints(p) {}
	NameValuePairs<double> distribute(const Int64 result, const std::string &agent) const;
};


}

#endif /* defined(__fantasybit__DistributionAlgo__) */

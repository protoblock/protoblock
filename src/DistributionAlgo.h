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
    DistributePoints(const NameValuePairs<Int> &p) : projections(p) {}
    
protected:
    NameValuePairs<Int> projections;
};

struct DistribuePointsAvg : DistributePoints
{
    DistribuePointsAvg(const NameValuePairs<Int> &p) : DistributePoints(p) {}
    NameValuePairs<double> distribute(const Int result) const;
};



}

#endif /* defined(__fantasybit__DistributionAlgo__) */

/*
template <class IMP>
class DistributePoints : public IMP
{
public:
    //using input = Int;
    //using output = MapS2I;
    DistributePoints(const Projections &p) : projections(p) {}
    
protected:
    Projections projections;
};

struct DistribuePointsAvg
{
    MapNameValue distribute(const Int result) const;
};
*/
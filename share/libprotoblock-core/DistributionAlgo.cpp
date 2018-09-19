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
//#include "core.h"

namespace fantasybit {

using namespace std;

NameValuePairs<int>
    DistribuePointsAvg::distribute(const double result, const std::string &agent) const
{
    NameValuePairs<int> award{};
    if (projections.size() == 0 || result <= 0.0001 ) {
        if ( result > 0.0001 ) {
            double aw = result * 100.0;
            award[agent] =  std::lround(aw);
            qInfo() << "no projections agent " << agent << " gets balance " << result << aw;

        }
		return award;
	}
	
    double mean = 0;
    vector<double> diffs;
    diffs.reserve(projections.size());
    
    double maxdiff = result;
    if ( maxdiff < 1.0 ) {
        maxdiff = .99;
    }

    for(const auto& pair : projections) {
        double diff = result - pair.second;
        diff = fabs(diff);
        if ( diff > maxdiff )
            continue;
        mean+=diff;
        diffs.emplace_back(diff);

//        qDebug() << pair.first << " projection " << pair.second << " diff " << diff;
    }
  
    if ( diffs.size() > 1  )
        mean /= diffs.size();

    //qInfo() << "mean " << mean;

    
    maxdiff = mean;
    //qInfo() << " maxdiff " << maxdiff;


    double sum = 0.0;
    for ( double d : diffs) {
        if ( d <= maxdiff ) {
            sum = sum + (result - d);
        }
    }

    if (sum == 0.0) {
        qInfo() << "no projections within 100% " << agent << " gets balance " << result;
        award[agent] = std::lround(result * 100.0);
        return award;
    }

    /*
    
    double sum2 = accumulate(begin(diffs), end(diffs), 0,
        [maxdiff,result](const double insum,const double val)
        {
            return insum + ((val <= maxdiff) ? (result-val) : 0.0);
        });

    //qInfo() << " sum " << sum << " sum2" << sum2;
    */

    double payout = result / sum;

//    qInfo() << " sum " << sum << " payout " << payout;

	double total = 0.0;
    for (const auto& pair : projections) {
        double diff = fabs(result - pair.second);
		if (diff <= maxdiff)
		{
			double amount = (result - diff)*payout;
            award[pair.first] = std::lround(amount * 100.0);
			total += amount;
//            qInfo() << pair.first << " projection " << pair.second << " award " << amount;
		}
        else;
//            qInfo() << pair.first << " projection " << pair.second << "no award ";
    }

    double leftover = result - total;
    if (leftover > 0.00001) {
        int hold = award[agent];
        award[agent] = hold + std::lround(leftover);;
        qDebug() << "agent " << agent << " leftovers " << leftover;

    }
    else if ( leftover < -0.00001 )
        qCritical() << "gave out to much" << result << total;

    return award;
}

//#include "ExchangeData.h"

PnlResults SettlePositionsRawStake::
            settle(
                const double result,
                const std::string &agent
            ) const {

    PnlResults pnl{};
    if (positions.positions().size() == 0)
        return pnl;

    //double mean = 0;
    //vector<double> diffs;
    //diffs.reserve(positions.size());
    int intresult = floor((result * 100.0) + 0.5);

    for(const auto& settlepos : positions.positions()) {
        int hispnl = (settlepos.qty() * intresult) + settlepos.price() * 100;
        pnl[settlepos.pk()] = make_pair(settlepos,hispnl);
#ifdef TRACE
        qDebug() << settlepos.DebugString().data();
#endif
//        " pnl " <<  hispnl << " result " << result <<
//                    " pos " << settlepos.second.first << " " << settlepos.second.second;

    }

    return pnl;
}

PnlResults SettleROWPositionsRawStake::
            settle(
                const double result,
                const std::string &agent,
                bool iswk16
            ) const {

    PnlResults pnl{};
    if (positions.positions().size() == 0)
        return pnl;

    //double mean = 0;
    //vector<double> diffs;
    //diffs.reserve(positions.size());
//    int intresult = floor((result * 100.0) + 0.5);

    for(const auto& settlepos : positions.positions()) {
        double qty = settlepos.qty();
        double myresult = result;
        if ( iswk16 )
            myresult *= 2.0;
        double dpnl = qty * myresult;
        if ( iswk16 ) {
//            double remainpnl =  + dpnl;
//            if ( remainpnl < 0.0001)
//                remainpnl = 0.0;

            dpnl += (double)settlepos.price();
        }
        int intpnl = floor((dpnl) + 0.5);
        pnl[settlepos.pk()] = make_pair(settlepos,intpnl);
#ifdef TRACE
        qDebug() << settlepos.DebugString().data();
#endif
//        " pnl " <<  hispnl << " result " << result <<
//                    " pos " << settlepos.second.first << " " << settlepos.second.second;

    }

    return pnl;
}

}

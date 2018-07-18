//
//  boostLog.h
//  fantasybit
//
//  Created by Jay Berg on 5/9/15.
//
// copied from: https://gist.github.com/gaultier/f5083ce5ead46f02264f
//
//

#ifndef __fantasybit__boostLog__
#define __fantasybit__boostLog__
//Link: -lpthread -lboost_log -lboost_system -lboost_thread
 
//#define BOOST_ALL_DYN_LINK
#include <boost/log/trivial.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/date_time/posix_time/posix_time_types.hpp>
#include <boost/log/support/date_time.hpp>
 
//using namespace std;
 
namespace logging = boost::log;
namespace attrs = boost::log::attributes;
namespace sinks = boost::log::sinks;
namespace expr = boost::log::expressions;
namespace trivial = boost::log::trivial;
namespace src = boost::log::sources;
namespace keywords = boost::log::keywords;
 
 
#define LOGIT(logger, severity, logfile, logline, logfunction) \
    BOOST_LOG_SEV(logger, severity) << "(" << logfile << ":" << logline << ":" << logfunction << ") \n"
//BOOST_LOG_SEV(logger, severity) << "(" << __FILE__ << ":" << __LINE__ << ":" << __FUNCTION__ << ") "

namespace fantasybit
{
	using namespace logging::trivial;
	static src::severity_logger< severity_level > lg;

	void initBoostLog();
}

#endif
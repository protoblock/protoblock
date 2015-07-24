//
//  boostLog.h
//  fantasybit
//
//  Created by Jay Berg on 5/9/15.
//
// copied from: https://gist.github.com/gaultier/f5083ce5ead46f02264f
//
//
#include "boostLog.h"
#include "fbutils.h"
namespace fantasybit
{
	//decltype(lg) lg{};

	void initBoostLog()
	{
		logging::add_file_log
			(
			keywords::file_name = ROOT_DIR + "boostlog.log",
			// This makes the sink to write log records that look like this:
			// 1: <normal> A normal severity message
			// 2: <error> An error severity message
            keywords::auto_flush = true,

			keywords::format =
			(
			expr::stream
			<< expr::attr< unsigned int >("LineID") << " |"
			<< " [ " << expr::format_date_time< boost::posix_time::ptime >("TimeStamp", "%Y-%m-%d %H:%M:%S") << "]"
			<< ": <" << logging::trivial::severity << "> "
			<< expr::smessage
			)
			);

		logging::add_console_log
			(
			std::clog,
			keywords::auto_flush = true,
			keywords::format =
			(
			expr::stream
			<< expr::attr< unsigned int >("LineID") << " |"
			<< " [ " << expr::format_date_time< boost::posix_time::ptime >("TimeStamp", "%Y-%m-%d %H:%M:%S") << "]"
			<< ": <" << logging::trivial::severity << "> "
			<< expr::smessage
			)

			);

		logging::add_common_attributes();

		//using namespace logging::trivial;
		//src::severity_logger< severity_level > lg;
		//LOGIT(lg, warning, __FILE__, __LINE__, __FUNCTION__) << "Hello world";
	}
}

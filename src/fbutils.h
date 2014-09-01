//
//  fbutils.h
//  fantasybit
//
//  Created by Jay Berg on 3/19/14.
//
//

#ifndef fantasybit_fbutils_h
#define fantasybit_fbutils_h

#include <cstdint>
#include <map>
#include <vector>
#include <memory>
#include <utility>
#include <iostream>
#include <string>
#include <unordered_map>

namespace fantasybit {

//#define NO_ORACLE_CHECK_TESTING 
//forwards
struct FantasyName;

using Int = int_fast32_t;
using Uid = std::string;// uint_least64_t;
using Int64 = uint64_t;
using MapS2I = std::map<std::string,Int>;
template<class T>
using NameValuePairs =
	std::unordered_map < std::string, T > ;
	//	std::vector<std::pair<std::shared_ptr<FantasyName>, T>>;
    
using Signature = std::string;

#ifdef FBWIN
static std::string ROOT_DIR("C:/fantasybit/");
#endif



static struct fbutils
{
	static bool LogFalse(std::string &in)
	{
		std::cout << in << "\n";
		return false;
	}

};
}

#endif

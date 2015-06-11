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
	using MapS2I = std::map < std::string, Int > ;
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

	template < typename T >
	std::map<int, std::string> proto_enum_map() {
		std::map<int, std::string> ret;

		for (int i = T::Type_MIN; i < T::Type_ARRAYSIZE; i++) {

			if (!T::Type_IsValid(i)) continue;
			ret[i] = T::Type_Name(T::Type(i));
		}

		return ret;
	}

	template < typename T >
	typename T::Type input_proto_enum_map() {
		auto in = proto_enum_map<T>();
		int ii;

		do {
			for (auto &i : in) {
				std::cout << i.first << ": " << i.second << endl;
			}

			std::cin >> ii;

		} while (in.find(ii) == end(in));

		return static_cast<T::Type>(ii);
	}

}

#endif

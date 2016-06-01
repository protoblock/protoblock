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
#include "platform.h"
#include<sstream>


#define QTD QString::fromStdString


namespace fantasybit {

#ifndef PRODFOOTBALL
    static std::string PB_WS_LITE_AGENT = "app.trading.football";
    static int PB_WS_LITE_AGENT_PORT = 4001;
    static std::string PB_WS_TX = "app.trading.football";
    static int PB_WS_TX_PORT = 4000;
    static std::string PB_WS_CHAT = "app.trading.football";
    static int PB_WS_CHAT_PORT = 4002;


    static std::string PAPIURL = "https://stagingapi.trading.football:4545";
    static std::string LAPIURL = "https://stagingapi.trading.football:9854";
    static std::string DBIP = "158.222.102.21";
    static std::string DBNAME = "tfprod";

#else
    static std::string PAPIURL = "https://app.trading.football:4545";
    static std::string LAPIURL = "https://app.trading.football:9854";
    //static std::string DBIP = "192.96.159.216";
    static std::string DBIP = "158.222.102.83";
    static std::string DBNAME = "tfprod";
#endif

    //static std::string LAPIURL = "158.222.102.83:9854";

    //#define NO_ORACLE_CHECK_TESTING
    //forwards
    struct FantasyName;

    using Int = int;
    using Uid = std::string;// uint_least64_t;
    using UInt64 = uint64_t;
    using Int64 = int64_t;

    using MapS2I = std::map < std::string, Int > ;
    template<class T>
    using NameValuePairs =
        std::unordered_map < std::string, T > ;
    //	std::vector<std::pair<std::shared_ptr<FantasyName>, T>>;

    using Signature = std::string;

#if defined(Jay2015PrePreSeasonTestDemo) || defined(DATAAGENTGUIJay2015PrePreSeasonTestDemo)
//#ifdef FBWIN
//    static std::string ROOT_DIR_("C:/fantasybit-internal2/");
//#endif

    static int PORT_HAND = 10130;
    static int PORT_SYNC_SERV = 10125;
    static int PORT_LIVE_BLOCK = 10126;
    static int PORT_LIVE_TX = 10127;
    static int PORT_LIVE_TX_NAT = 10128;
#else

//#ifdef FBWIN
//    static std::string ROOT_DIR_("C:/fantasybit/");
//#endif

//    static int PORT_HAND = 8130;
//    static int PORT_SYNC_SERV = 8125;
//    static int PORT_LIVE_BLOCK = 8126;
//    static int PORT_LIVE_TX = 8127;
//    static int PORT_LIVE_TX_NAT = 8128;

#endif

    template <typename T>
    std::string To_String(T val)
    {
        std::stringstream stream;
        stream << val;
        return stream.str();
    }


    static std::string GET_ROOT_DIR() {
        return Platform::instance()->getRootDir();
    }

    static std::string FB_PORT(int port) {
        return std::string(":").append( To_String( port ) );
    }

    /*
    static void Jay2015PrePreSeasonTestDemo()
    {
        ROOT_DIR_ = "C:/fantasybit-internal/";
        PORT_HAND += 1000;
        PORT_SYNC_SERV += 1000;
        PORT_LIVE_BLOCK += 1000;
        PORT_LIVE_TX += 1000;
        PORT_LIVE_TX_NAT += 1000;
    }
*/

    template < typename T >
    std::map<int, std::string> proto_enum_map() {
        std::map<int, std::string> ret;

        for (int i = T::Type_MIN; i < T::Type_ARRAYSIZE; i++) {

            if (!T::Type_IsValid(i)) continue;
            ret[i] = T::Type_Name(T::Type(i));
        }

        return ret;
    }

//	template < typename T >
//	typename T::Type input_proto_enum_map() {
//		auto in = proto_enum_map<T>();
//		int ii;

//		do {
//			for (auto &i : in) {
//				std::cout << i.first << ": " << i.second << endl;
//			}

//			std::cin >> ii;

//		} while (in.find(ii) == end(in));

//		return static_cast<T::Type>(ii);
//	}

}

#endif

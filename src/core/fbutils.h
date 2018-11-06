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
#include "ProtoData.pb.h"
#include "platform.h"
#include "appsettings.h"


#define QTD QString::fromStdString


namespace fantasybit {

#if !defined(PRODFOOTBALL)

#ifdef USE_LOCALNETWORKHOST_SERVER
    static std::string PB_WS_LITE_AGENT = "192.168.42.80";
    static std::string PB_WS_CHAT = "192.168.42.80";
    static std::string PB_WS_TX = "192.168.42.80";
#elif USE_LOCALHOST_SERVER
    static std::string PB_WS_LITE_AGENT = "localhost";
    static std::string PB_WS_CHAT = "localhost";
    static std::string PB_WS_TX = "localhost";
#else
    static std::string PB_WS_LITE_AGENT = "app.trading.football";
    static std::string PB_WS_CHAT = "app.trading.football";
    static std::string PB_WS_TX = "app.trading.football";
#endif
//    static int PB_WS_LITE_AGENT_PORT = 4001;
//    static int PB_WS_TX_PORT = 4000;
//    static int PB_WS_CHAT_PORT = 4002;

    static int PB_WS_LITE_AGENT_PORT = 9111;

    static std::string PAPIURL = "https://stagingapi.trading.football:4545";
    static std::string LAPIURL = "https://stagingapi.trading.football:9854";
    static std::string DBIP = "208.94.37.244";
    static std::string DBNAME = "tfprod";

    static bool IS_TEST_NET = true;
#else
    static bool IS_TEST_NET = false;
    static int PB_WS_LITE_AGENT_PORT = 9111;
    static int PB_WS_TX_PORT = 9110;
    static int PB_WS_CHAT_PORT = 9112;

#ifdef USE_LOCALNETWORKHOST_SERVER
    static std::string PB_WS_LITE_AGENT = "192.168.42.80";
    static std::string PB_WS_CHAT = "192.168.42.80";
    static std::string PB_WS_TX = "192.168.42.80";
#elif USE_LOCALHOST_SERVER
    static std::string PB_WS_LITE_AGENT = "localhost";
    static std::string PB_WS_CHAT = "localhost";
    static std::string PB_WS_TX = "localhost";
#else
    static std::string PB_WS_LITE_AGENT = "app.trading.football";
    static std::string PB_WS_CHAT = "app.trading.football";
    static std::string PB_WS_TX = "app.trading.football";
#endif

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


    static std::string GET_ROOT_DIR(bool is_resource = false) {
#ifdef Q_OS_MAC
        if ( is_resource ) {
            QString thefile =
                    Platform::instance()->settings()->getSetting(AppSettings::ResourceLocation).toString();

            return thefile.toStdString ();
        }
#endif
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


    static std::string PID2Symbol(uint64_t pid) {
        /*
        // digits:

        1-2: first initial, last initial
        3-4: seq (0a,0b...0z,1a,1b,1c,9z,Aa,Ab,....Za)
        4-5: year (16,17,18,19)
        6: (contract type, W - weekly, S - Season)
        7: week# ( hexadecimal 0=season  1=week1,2,3,4,5,6,7,8,9,a,b,c,d,e,f=15)

        Arizona Cardinals 2016 Season contract = AC0a16S0
        AC = Arizona Cardinals
        0a = the first sequece of "AC"
        16 = 2016
        S = season long
        0 = week 0 = season long


        Adrian Peterson 2016 Week 14 Contract
        AP0a16We
        AP = initials
        0a = first one
        16 = year
        W = weekly
        e = 14 in hex


        to generate symbols from player ids - start with player id 1 , and keep track of seqence for initals

        so keep a count of all initals

        SS0a - steve smith 1a - 1st one
        SS0b - steve smith 1b - 2nd one
        SS0


        AC0a16S0 can be shotened to  AC16
        AP0a16We can be shotened to  AP16We  - or APe for on the run - current year - weeklys  APS - season
        */

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

    struct PlayerDetail {
        PlayerBase base;
        PlayerStatus::Status team_status;
        PlayerGameStatus game_status;
        std::string symbol;
        std::string team;
    };

    struct GameRoster {
        GameInfo info;
        GameStatus::Status  status;
        std::unordered_map<std::string,PlayerDetail> homeroster;
        std::unordered_map<std::string,PlayerDetail> awayroster;

    };

    static bool isWeekly(const QString &symbol) {
        return !symbol.endsWith('s');
    }

    static bool isWeekly(const std::string &symbol) {
        return !(symbol.back() == 's');
    }

    static std::string stripSymbol(const std::string &symbol) {
        if  (symbol.size() < 6 )
            return symbol;

        return symbol.substr(0,(symbol.at(4) > '9') ? 5 : 4);
    }

    static std::string contractSymbolSuffix(int season, int week, bool isweekly) {
        std::string ret = std::to_string(season-2000);
        if ( isweekly) {
            ret += "w";
            if ( week <= 9)
                ret += "0";

            ret += std::to_string(week);
        }
        else
            ret += "s";

        return ret;
    }

    static int getSymbolWeek(const std::string &symbol) {
        int ret = 0;

        size_t pos = symbol.rfind('w');
        if ( pos != std::string::npos && pos+1 == symbol.size ()-2) {
            try {
                ret = std::stoi(symbol.substr (pos+1));
            } catch(const std::invalid_argument& e) {
                std::cout << "getSymbolWeek invalid_argument";
                ret = 0;
            }
        }
        return ret;
    }

    static uint64_t time_since_epoch() {
        return std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    }
}

#endif

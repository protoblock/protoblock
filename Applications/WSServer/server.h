#ifndef SERVER_H
#define SERVER_H

#include "StateData.pb.h"
#include "TxServer.h"
#include "LiteServer.h"
#include "ExchangeData.h"
#include "Data.h"

using namespace fantasybit;

class Server {
public:
    static fantasybit::GetAllNamesRep AllNamesRep;
    static fantasybit::ExchangeData TheExchange;
//    static fantasybit::GetROWMarketRep ROWMarketRep;
    static NFLStateData  NFLData;
};


#endif //SERVER_H

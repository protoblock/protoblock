#include "server.h"

fantasybit::GetAllNamesRep Server::AllNamesRep{};
fantasybit::ExchangeData Server::TheExchange{};
//fantasybit::GetROWMarketRep Server::ROWMarketRep{};
fantasybit::NFLStateData Server::NFLData;

decltype(Server::Pk2Bal) Server::Pk2Bal{};

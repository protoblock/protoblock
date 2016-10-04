#ifndef SERVER_H
#define SERVER_H

#include "StateData.pb.h"
#include "TxServer.h"
#include "LiteServer.h"
#include "ExchangeData.h"
#include "Data.h"
#include "pbgateways.h"
#include "dataservice.h"

using namespace fantasybit;

class Server : public QObject {
    Q_OBJECT

    explicit Server(QObject *parent = 0) :  QObject(parent) {}

    static Server *myInstance;
    void setupConnection(pb::IPBGateway *ingateway);
public:
    static Server *instance();
    static fantasybit::GetAllNamesRep AllNamesRep;
    static fantasybit::ExchangeData TheExchange;
//    static fantasybit::GetROWMarketRep ROWMarketRep;
//    static NFLStateData  NFLData;
    static std::unordered_map<std::string,FantasyNameBal *> Pk2Bal;

    static void AddNames(const FantasyNameBal &fnb) {
        Server::AllNamesRep.add_names(fnb.name());
        FantasyNameBal *p = Server::AllNamesRep.add_fnb();
        p->CopyFrom(fnb);
        Pk2Bal.insert({fnb.public_key(),p});
    }

    static void AddNames(FantasyNameBal *pFn) {
        Server::AllNamesRep.add_names(pFn->name());
        Server::AllNamesRep.mutable_fnb()->AddAllocated(pFn);
        Pk2Bal.insert({pFn->public_key(),pFn});

    }

    static bool goodPid(const std::string &pid) {
        auto pb = DataService::instance()->GetPlayerBase(pid);
        return pb.has_last() && pb.last() != "";
    }

    pb::IPBGateway *mGateway = nullptr;
    void setContext(pb::IPBGateway *ingateway) {
        mGateway = ingateway;
        setupConnection(mGateway);
    }

protected slots:
    void OnNewFantasyName(fantasybit::FantasyNameBal fnb) {
        AddNames(fnb);
    }

    void OnAnyFantasyNameBalance(fantasybit::FantasyNameBal fnb) {

    }


};


#endif //SERVER_H

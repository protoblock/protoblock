#include "server.h"

fantasybit::GetAllNamesRep Server::AllNamesRep{};
fantasybit::ExchangeData Server::TheExchange{};
//fantasybit::GetROWMarketRep Server::ROWMarketRep{};
//fantasybit::NFLStateData Server::NFLData;

decltype(Server::Pk2Bal) Server::Pk2Bal{};

Server *Server::instance() {
    if (myInstance == NULL) {
        myInstance = new Server();
    }
    return myInstance;
}

void Server::setupConnection(pb::IPBGateway *ingateway) {

    QObject* that = dynamic_cast<QObject*>(ingateway);
    connect( that, SIGNAL   ( LiveGui(fantasybit::GlobalState)     ),
            this,      SLOT     (  LiveGui(fantasybit::GlobalState)     ));

    connect(that, SIGNAL(NameStatus(fantasybit::MyFantasyName)),
            this, SLOT(NameStatus(fantasybit::MyFantasyName)));

    connect( that, SIGNAL(LiveProj(fantasybit::FantasyBitProj)),
            this, SLOT(LiveProj(fantasybit::FantasyBitProj )));

    connect( that, SIGNAL(MyNames(vector<fantasybit::MyFantasyName>)),
            this, SLOT(MyNames(vector<fantasybit::MyFantasyName> )));
    connect( that, SIGNAL(NameBal(fantasybit::FantasyNameBal)),
            this, SLOT(NameBal(fantasybit::FantasyNameBal )));
    connect( that, SIGNAL( PlayerStatusChange(pair<string,fantasybit::PlayerStatus>) ),
            this, SLOT(PlayerStatusChange(pair<string,fantasybit::PlayerStatus> )));


    connect( that, SIGNAL(  GlobalStateChange(fantasybit::GlobalState)  ),
            this,      SLOT(    GlobalStateChange(fantasybit::GlobalState)        ));


    connect( that, SIGNAL   (  NewWeek(int)    ),
            this,      SLOT     (  NewWeek(int)     ));
    connect( that, SIGNAL   (  GameStart(string)    ),
            this,      SLOT     (  GameStart(string)     ));
    connect( that, SIGNAL   (  GameOver(string)    ),
            this,      SLOT     (  GameOver(string)     ));
    connect( that, SIGNAL   (  onControlMessage(QString)    ),
            this,      SLOT     (  onControlMessage(QString)     ));


    connect( that, SIGNAL   (  NewFantasyName(fantasybit::FantasyNameBal)    ),
            this,  SLOT     (  OnNewFantasyName(fantasybit::FantasyNameBal)      ));

    connect( that, SIGNAL   (  AnyFantasyNameBalance(fantasybit::FantasyNameBal)    ),
            this,  SLOT     (  OnAnyFantasyNameBalance(fantasybit::FantasyNameBal)      ));


//    QObject::connect(this,SIGNAL(OnClaimName(QString)),that,SLOT(OnClaimName(QString)));

//    connect( this, SIGNAL(OnUseName(QString)),
//             that, SLOT(UseName(QString)));

//    connect( this, SIGNAL(doNameCheck(QString)),
//             that, SLOT(nameCheck(QString)));

//    connect( that, SIGNAL(nameAvail(QString &,bool)),
//             this, SLOT(nameAvail(QString &,bool)));


//    return that;
}


Server *Server::myInstance;

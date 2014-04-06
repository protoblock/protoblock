//
//  main.cpp
//  satoshifantasy
//
//  Created by User on 3/17/14.
//
#include "Commissioner.h"
#include "FantasyName.h"
#include <iostream>
#include <fc/crypto/hex.hpp>

#include "nn.hpp"

#include <nanomsg/pair.h>

#include "DataPersist.h"
#include "ProtoData.pb.h"

#include "Server.h"
#include "ClientUI.h"

using namespace fantasybit;
using namespace std;

int main(int argc, const char * argv[])
{
    string address{"inproc://test"};
    Server server{address};
    ClientUI client{address};
    
    
    thread servert{&Server::run,&server};
    thread clientt{&ClientUI::run,&client};
    
    string command;
    cin >> command;
    if ( command == "exit" )
    {
        server.stop();
        client.stop();
        nn_term();
    }
    
    servert.join();
    clientt.join();
    
}
int main2(int argc, const char * argv[])
{

{
    nn::socket s1 (AF_SP, NN_PAIR);
    s1.bind ("inproc://a");
    nn::socket s2 (AF_SP, NN_PAIR);
    s2.connect ("inproc://a");

    s2.send ("ABC", 3, 0);
    char buf [3];
    int rc = s1.recv (buf, sizeof (buf), 0);
    assert (rc == 3);
}

fc::ecc::private_key p = fc::ecc::private_key::generate();

fc::sha256 secret = p.get_secret();
std::string str1 = secret.str();
std::string str2 = fc::to_hex((char*)&secret,sizeof(secret));
fc::sha256 s1{str1};
fc::sha256 s2{str2};
fc::ecc::private_key p1 = fc::ecc::private_key::regenerate(s1);
fc::ecc::private_key p2 = fc::ecc::private_key::regenerate(s2);
fc::sha256 secret1 = p1.get_secret();
fc::sha256 secret2 = p2.get_secret();
std::string str3 = secret1.str();
std::string str4 = fc::to_hex((char*)&secret2,sizeof(secret2));

cout << p.get_secret().str() << "\n";


Secret secretdata;

secretdata.set_private_key(str4);

cout << "(" << secretdata.private_key() << ")\n";

Writer<Secret> write{"secret.out",std::ios::app};
write(secretdata);


Secret secretdata2;
Reader<Secret> read{"secret.out"};

read.ReadNext(secretdata2);

/*
    FantasyName me;
    FantasyAgent fa{true};
    auto ret = fa.signPlayer("SatoshiFantasy");
    if ( ret == FantasyAgent::REQUESTED ) {
        me = fa.getRequested();
    
        cout << me.alias << "\n";
    }
  */  
    return 0;
};


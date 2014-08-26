//
//  main.cpp
//  satoshifantasy
//
//  Created by Jay Berg  on 4/6/14.
//

#include <iostream>
#include <string>
#include "nn.hpp"
#include <nanomsg/pair.h>
#include "MsgSock.h"
#include "ProtoData.pb.h"
#include "tclap/CmdLine.h"
 
using namespace std;
using namespace fantasybit;
using namespace TCLAP;
string input(const std::string &in,char c=' ')
{
	cout << in << c << endl;
	string s;
	cin >> s;
	return s;
};

int main(int argc, const char * argv[])
{
    std::string address{};

    CmdLine cmd("fantasybit ", ' ', "0.1");

    ValueArg<string> tcpPort("p","port","use tcp for GUI",false,"31200","port number");
    cmd.add( tcpPort );

    SwitchArg tcp("t","tcp","Use Tcp -p port", false);
    cmd.add( tcp );
    
    ValueArg<string> ipcSuf("i","ipc","use ipc for GUI",false,"temp","address suffix");
    cmd.add( ipcSuf );

    cmd.parse( argc, argv );

    if ( tcp.isSet() )
        address = "tcp://127.0.0.1:" + tcpPort.getValue();
    else
        address = "ipc:///tmp/" + ipcSuf.getValue() + ".ipc";

    cout << " using " << address << endl;

    string in;
    
    nn::socket sock{AF_SP, NN_PAIR};
    sock.connect(address.c_str());
    Receiver ui{sock};
    
    InData indata{};
    OutData outdata{};
    

    
    string commands =
                    "1: \t\tconnect\n" \
                    "2:\t\treceive" \
                    "3:\t\tmine" \
                    "4:\t\tquit" \
                    "\nexit:\tquit";
    while ( "exit" != (in = input(commands)) )
    { 
        if ( in == "1")
        {
            indata.set_type(InData_Type_CONNECT);
            Sender::Send(sock,indata);

        }
        if ( in == "2") {

            indata.set_type(InData_Type_HEARTBEAT);
            Sender::Send(sock,indata);
            
            outdata.Clear();
            while ( ui.receive(outdata,NN_DONTWAIT) )
                cout << outdata.DebugString() << " \n";
        }
        
        if ( in == "3") {
            if ( "exit" == (in = input("name"))) break;
            indata.Clear();
            indata.set_type(InData_Type_MINENAME);
            indata.set_data(in);
            Sender::Send(sock,indata);
        }
        if ( in == "4")
        {
            indata.set_type(InData_Type_QUIT);
            Sender::Send(sock,indata);
            break;
        }
    }
    
    nn_term();
}


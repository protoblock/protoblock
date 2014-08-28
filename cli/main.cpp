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
#include <fb/MsgSock.h>
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

int input_int(const std::string &in, char c = ' ')
{
	cout << in << c << endl;
	int i;
	cin >> i;
	return i;
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
    

	stringstream ss{};
	ss << "1" << "\t" << "connect" << "\n";
	ss << "2" << "\t" << "receive" << "\n";
	ss << "3" << "\t" << "mine" << "\n";
	ss << "4" << "\t" << "block" << "\n";
	ss << "5" << "\t" << "new name" << "\n";
	ss << "6" << "\t" << "project" << "\n";
	ss << "7" << "\t" << "result" << "\n";
	//ss << "8" << "\t" << "connect" << "\n";
	//ss << "9" << "\t" << "connect" << "\n";

	ss << "99" << "\t" << "disonnect" << "\n";
	ss << "exit" << "\t" << "quit" << "\n";

	string commands = ss.str();
    while ( "exit" != (in = input(commands)) )
    { 
		indata.Clear();
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
        if ( in == "99")
        {
            indata.set_type(InData_Type_QUIT);
            Sender::Send(sock,indata);
            break;
        }
		if (in == "4")
		{
			indata.set_type(InData_Type_MAKE_BLOCK);
			Sender::Send(sock, indata);
		}

		if (in == "5")
		{
			if ("exit" == (in = input("name"))) break;
			indata.Clear();
			indata.set_type(InData_Type_NEWNAME);
			indata.set_data(in);
			Sender::Send(sock, indata);
		}

		if (in == "6")
		{
			indata.Clear();
			indata.set_type(InData_Type_PROJ);
			indata.set_data(input("game:"));
			indata.set_data2(input("player:"));
			indata.set_num(input_int("proj:"));
			Sender::Send(sock, indata);
		}
		if (in == "7")
		{
			indata.Clear();
			indata.set_type(InData_Type_RESULT);
			indata.set_data(input("game:"));
			indata.set_data2(input("player:"));
			indata.set_num(input_int("result:"));
			Sender::Send(sock, indata);
		}


    }
    
    nn_term();
}


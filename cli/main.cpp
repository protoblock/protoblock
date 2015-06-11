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
#include <nanomsg\reqrep.h>
#include "fb/boostLog.h"
#include <functional>
#include <fbutils.h>

#define LOG(logger, severity) LOGIT(logger, severity,  __FILE__, __LINE__, __FUNCTION__)

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

int input_int(std::map<int, string> in) {
	int ii;

	do {
		for (auto &i : in) {
			cout << i.first << ": " << i.second << endl;
		}

		cin >> ii;

	} while (in.find(ii) == end(in));

	return ii;
}

int input_int(const std::string &in, char c = ' ')
{
	cout << in << c << endl;
	int i;
	cin >> i;
	return i;
};

bool yn(const std::string &in) {
	cout << in << " ? y/n: ";
	string s;
	cin >> s;
	return (s == "y");
}

std::map<int, string> input_DataTransition_Type() {
	std::map<int, string> ret;

	for (int i = DataTransition::Type_MIN; i < DataTransition::Type_ARRAYSIZE; i++) {

		if (!DataTransition::Type_IsValid(i)) continue;
		ret[i] = DataTransition::Type_Name(DataTransition::Type(i));
	}

	return ret;
}


/*
std::map<int, string> input_Type(
							int min, int size, 
							function<bool(int)> isvalid, 
							function<string(int)> name
							) {
	std::map<int, string> ret;

	for (int i = min; i < size; i++) {

		if (!isvalid(i)) continue;
		ret[i] = name(i);
	}

	return ret;
}
*/


int main(int argc, const char * argv[])
{
	initBoostLog();

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


	address = "ipc:///tmp/fantasygui.ipc";
    cout << " using " << address << endl;
	LOG(lg, trace) << " address " << address;

    string in;

	/*
	nn::socket sock2{ AF_SP, NN_REP };
	sock2.bind("tcp://127.0.0.1:30000");
	NodeRequest reqhs{};
	Receiver rec{ sock2 };
	rec.receive( reqhs);
	cout << reqhs.DebugString();
	NodeReply reply{};
	reply.set_hight(0);
	Sender::Send(sock2,reply);
	*/
    nn::socket sock{AF_SP, NN_PAIR};
	int con = sock.connect(address.c_str());
	//"ipc:///tmp/fantasygui.ipc");

	LOG(lg, trace) << " connect " << con;

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
	ss << "7" << "\t" << "data" << "\n";
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

			LOG(lg, trace) << " send " << indata.DebugString();

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
			indata.set_type(InData_Type_DATA);

			DataTransition dt{};
			//	DataTransition dt{}
			//DataTransition::Type_Parse
			//dt.set_type(static_cast<DataTransition_Type>(input_int(proto_enum_map<DataTransition>())));
			dt.set_type(input_proto_enum_map<DataTransition>());
			
			dt.set_season(input_int("season:"));
			dt.set_week(input_int("week:"));

			while(true) {
				string team{ input("team or na:") };
				if (team == "na") break;
				dt.add_teamid(team);
			}

			while (yn("add data")) {
				Data d{};
				d.set_type(input_proto_enum_map<Data>());
				switch (d.type()) {
					case Data::PLAYER:
					{
						PlayerData pd{};
						pd.set_playerid(input("playerid:"));
						pd.set_teamid(input("teamid:"));
						d.MutableExtension(PlayerData::player_data)->CopyFrom(pd);
					}
					break;
					
					case Data::TEAM:
					{
						TeamData pd{};
						pd.set_teamid(input("teamid:"));
						d.MutableExtension(TeamData::team_data)->CopyFrom(pd);
					}
					break;

					case Data::RESULT:
					{
						FantasyPlayerPoints fpp{};
						fpp.set_season(dt.season());
						fpp.set_week(dt.week());
						fpp.set_playerid(input("playerid"));
						fpp.set_points(input_int("points:"));

						ResultData rd{};
						rd.mutable_fpp()->CopyFrom(fpp);

						d.MutableExtension(ResultData::result_data)->CopyFrom(rd);
					}
					break;

					default:
					break;

				}

				Data *d2 = dt.add_data();
				d2->CopyFrom(d);
			}

			indata.mutable_data_trans()->CopyFrom(dt);
			Sender::Send(sock, indata);
		}


    }
    
    nn_term();
}



    //
//  main.cpp
//  satoshifantasy
//
//  Created by Jay Berg on 3/17/14.
//
#include "Commissioner.h"
#include "FantasyName.h"
#include <iostream>
#include <fc/crypto/hex.hpp>

#include "nn.hpp"

#include <nanomsg/pair.h>
#include <nanomsg/pubsub.h>
#include <nanomsg/tcp.h>

#include "DataPersist.h"
#include "ProtoData.pb.h"

#include "Server.h"
#include "ClientUI.h"

#include "tclap/CmdLine.h"

#include <leveldb/db.h>
#include "Node.h"
#include "Processor.h"

#include "boostLog.h"

/*
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>

namespace logging = boost::log;
*/
using namespace fantasybit;
using namespace std;
using namespace TCLAP;

/*
void init()
{
	logging::core::get()->set_filter
		(
		logging::trivial::severity >= logging::trivial::info
		);

	/ *
	BOOST_LOG_TRIVIAL(trace) << "A trace severity message";
	BOOST_LOG_TRIVIAL(debug) << "A debug severity message";
	BOOST_LOG_TRIVIAL(info) << "An informational severity message";
	BOOST_LOG_TRIVIAL(warning) << "A warning severity message";
	BOOST_LOG_TRIVIAL(error) << "An error severity message";
	BOOST_LOG_TRIVIAL(fatal) << "A fatal severity message";
	* /
}
*/


#define LOG(logger, severity) LOGIT(logger, severity,  __FILE__, __LINE__, __FUNCTION__)


void makegenesistransblock() {
	/*
	FantasyAgent agent{};
	agent.beOracle();

	Block b{}; 

	auto time = fc::time_point_sec(fc::time_point::from_iso_string("20150601T000000")).sec_since_epoch();

	auto gtrans = Commissioner::makeGenesisName();
	//merkle root - single transaction 
	auto root = fc::sha256::hash(gtrans.id() + gtrans.id()).str();

	BlockHeader bh{};
	bh.set_version(1);
	bh.set_num(1);
	bh.set_prev_id("0000000000000000000000000000000000000000000000000000000000000000");
	bh.set_timestamp(time);

	bh.set_generator_pk(std::string("mT1M2MeDjA1RsWkwT7cjE6bbjprcNi84cWyWNvWU1iBa"));
	bh.set_generating_sig(fc::sha256::hash(bh.generator_pk()).str());
	bh.set_basetarget(0); //todo
	bh.set_blocktype(BlockHeader_Type_NORMAL);
	bh.set_transaction_id(root); //todo: merkle root of tx 

	SignedBlockHeader sbh{};
	sbh.mutable_head()->CopyFrom(bh);

	auto p = agent.getIdSig(sbh.head().SerializeAsString());
	sbh.set_sig(p.second);

	auto genesisblockhash = p.first;

	b.mutable_signedhead()->CopyFrom(sbh);

	b.add_signed_transactions()->CopyFrom(gtrans);


	/*
	cout << "===============================\n"
		<< sn.SerializeAsString()
		<< "\n=============================\n";
/**/
}
int main(int argc, const char * argv[])
{
	initBoostLog(); 

	CmdLine cmd("fantasybit ", ' ', "0.1");

	SwitchArg reset("r", "reset", "clear all data and start from scratch", false);
	cmd.add(reset);

	SwitchArg Reset("R", "resetexit", "clear all data and start from scratch", false);
	cmd.add(Reset);

	cmd.parse(argc, argv);

	if (reset.isSet() || Reset.isSet()) {
		fc::remove_all(ROOT_DIR + "index/");
		fc::remove_all(ROOT_DIR + "block/");
		fc::remove_all(ROOT_DIR + "node/");
		fc::remove_all(ROOT_DIR + "tx/");

		fc::remove(ROOT_DIR + "secret.out");

		if (Reset.isSet())
			return 0;
	}

	/*
	nn::socket s1(AF_SP, NN_PAIR);

	if (push.isSet()) {
		auto eid = s1.connect("tcp://162.254.25.193:8125");
		cout << " connect " << eid << endl;

		char buf[3];
		int rc = s1.recv(buf, sizeof(buf), 0);
		cout << " received " << buf << endl;
		assert(rc == 3);
	}
	else {
		//auto eid = s1.bind("tcp://*:8125");
		auto eid = s1.bind("tcp://162.254.25.193:8125");

		cout << " bind " << eid << endl;

		auto i = s1.send("ABC", 3, 0);
		cout << " sent";

		assert(i == 3);

	}
	*/

	Node node{};
	//node.getMyIp();
	node.init();
	node.runHandShake();

	string delta_address{ "inproc://deltaserver" };

	string server_address{ "inproc://rcpserver" };
	Server server{ server_address };;

	string gui_address{ "ipc:///tmp/fantasygui.ipc" };
	ClientUI client{ server_address, gui_address, delta_address };

	thread servert{ &Server::run, &server };
	thread clientt{ &ClientUI::run, &client };

	thread syncRequest_{ &Node::syncRequest, &node };
	thread syncService_{ &Node::syncService, &node };
	
	BlockProcessor processor{ delta_address };
	thread processor_{ &BlockProcessor::run, &processor };

	thread runLive_{ &Node::runLive, &node };
	thread pendingTransactions_{ &Node::pendingTransactions, &node };

	clientt.join();
	server.stop();
	node.stop();
	processor.stop();
	nn_term();
	servert.join();
	syncRequest_.join();
	syncService_.join();
	runLive_.join();
	pendingTransactions_.join();
	processor_.join();

	return 0;
}

int main2(int argc, const char * argv[])
{
	/*
	leveldb::DB *db;
	leveldb::Options options;
	options.create_if_missing = true;
	leveldb::Status status = leveldb::DB::Open(options, "./leveldb_dir", &db);
	assert(status.ok());
	*/
    std::string address{};
	/*
	nn::socket ssso{ AF_SP_RAW, NN_PUB };
	int rc = ssso.connect("tcp://192.168.1.127:3000");
	std::string str("XXXsddfs");
	char buf[4];// = nn::allocmsg(5, 0);
	memcpy(buf, str.c_str(), 3);
	int opt;
	opt = 1;
	//ssso.setsockopt(NN_SOL_SOCKET, NN_TCP_NODELAY, &opt, sizeof(opt));

	rc = ssso.send(str.c_str(), str.length()+1, 0);
	//ssso.send
	rc = ssso.send(buf, 3, 0);
	//ssso.setsockopt
	*/
    CmdLine cmd("fantasybit ", ' ', "0.1");

    ValueArg<string> tcpPort("p","port","use tcp for GUI",false,"31200","port number");
    cmd.add( tcpPort );

	SwitchArg tcp("t", "tcp", "Use Tcp -p port", false);
    cmd.add( tcp );

	//ValueArg<string> ip("a", "ip", "Use Tcp -p port", false, "192.168.1.27", "ip");
	//cmd.add(ip);

    SwitchArg mine("m","mine","Just mine", false);
    cmd.add( mine );

    ValueArg<int> minecount("c","minecount","mine count for test", false,1,"number of times");
    cmd.add( minecount );

    ValueArg<string> mineName("n","name","mine name",false,"satohi fantasy","name to mine");
    cmd.add( mineName );
    
    ValueArg<string> ipcSuf("i","ipc","use ipc for GUI",false,"temp","address suffix");
    cmd.add( ipcSuf );

    cmd.parse( argc, argv );

	if (tcp.isSet())
		address = "tcp://192.168.1.27:3000";// +ip.getValue() + ":" + tcpPort.getValue();
    else
        address = "ipc:///tmp/" + ipcSuf.getValue() + ".ipc";

    cout << " using " << address << endl;
    /*
    std::string ipc{"ipc:///tmp/"};
    ipc += ( argc > 1) ? argv[1] :  "test";
    ipc+= ".ipc";
    
    ipc = "tcp://127.0.0.1:";
    ipc += ( argc > 1) ? argv[1] :  "56633";
    */
    
    
    string server_address{"inproc://test"};
	Server server{ server_address} ;;
    
    string gui_address{address};
    ClientUI client{server_address,gui_address,""};
    
    
    thread servert{&Server::run,&server};
    thread clientt{&ClientUI::run,&client};

    clientt.join();
    server.stop();
    nn_term();
    servert.join();
     
	return 0;
}


int main3(int argc, const char * argv[])
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


//
//  Node.cpp
//  fantasybit
//
//  Created by Jay Berg on 8/31/14
//
//

#include "Node.h"
#include "ProtoData.pb.h"
#include <iostream>
#include "MsgSock.h"
#include <thread>

#include <iostream>
#include <istream>
#include <ostream>
#include <string>
#include <boost/asio.hpp>
#include "ProtoData.pb.h"
#include "nn.hpp"
#include <nanomsg\reqrep.h>
#include <set>
#include <fc/time.hpp>
#include "Processor.h"
#include <nanomsg\pubsub.h>
#include "boostLog.h"

using boost::asio::ip::tcp;

using namespace std;
using namespace nn;
	
#define LOG(logger, severity) LOGIT(logger, severity,  __FILE__, __LINE__, __FUNCTION__)

namespace fantasybit
{
int Node::init()
{
	LOG(lg, trace) << "init node";
	leveldb::Options options;
	options.create_if_missing = true;
	leveldb::Status status;
	leveldb::DB *db;
	status = leveldb::DB::Open(options, filedir("node/peers"), &db);
	peers.reset(db);

	leveldb::DB *db2;
	status = leveldb::DB::Open(options, filedir("block/blockchain"), &db2);
	Node::blockchain.reset(db2);

	leveldb::DB *db3;
	status = leveldb::DB::Open(options, filedir("block/holdblock"), &db3);
	holdblocks.reset(db3);

	leveldb::DB *db4;
	status = leveldb::DB::Open(options, filedir("tx/txpool"), &db4);
	Node::txpool.reset(db4);

	current_hight = getLastBlockNum();
	if (current_hight == 0)
	{	
        LOG(lg, info) << "no blocks - makeing Genesis";

		current_hight++;
		Block sb{Commissioner::makeGenesisBlock()};
		leveldb::Slice value((char*)&current_hight, sizeof(int));
		{
            //std::lock_guard<std::mutex> lockg{ blockchain_mutex };
			blockchain->Put(leveldb::WriteOptions(), value, sb.SerializeAsString());
		}
		current_hight = getLastBlockNum();
	}

	assert(getLastBlockNum() > 0);

	LOG(lg, info) << " current_hight " << current_hight;

	if (getMyIp())
	{
		reqhs.set_myip(myip);
		reqhs.set_myhost(myhost);
	}
    else {
        LOG(lg, error) << " cant find myip ";
        return -1;
    }


	LOG(lg, info) << " my ip " << myip;
	LOG(lg, info) << " my host" << myhost << "\n";

	auto *it = peers->NewIterator(leveldb::ReadOptions());
	it->SeekToFirst();
	if (!it->Valid())
	{
		if (SEED_NODE != myip)
			peers->Put(leveldb::WriteOptions(), SEED_NODE, "0");
        //if (SEED_HOST != myhost)
        //	peers->Put(leveldb::WriteOptions(), SEED_HOST, "0");
	}
	delete it;

	return 0;
}

void Node::runHandShake()
{
	bool triedseed = false;
	int numconnected = 0;

	auto *it = peers->NewIterator(leveldb::ReadOptions());
	for (it->SeekToFirst(); it->Valid(); it->Next()) {
		peerlist.insert(it->key().ToString());
		LOG(lg, info) << "peer" << it->key().ToString() << " seen " << it->value().ToString();
	}

	if (peerlist.find(SEED_NODE) == peerlist.end()) {
		LOG(lg, info) << "add seed node";
		//if (SEED_NODE != myip)
		peers->Put(leveldb::WriteOptions(), SEED_NODE, "0");
		//else if (peerlist.find(SEED_HOST) == peerlist.end()) {
		//	peers->Put(leveldb::WriteOptions(), SEED_HOST, "0");
		//}
	}

	if (peerlist.find(SEED_HOST) == peerlist.end()) {
		LOG(lg, info) << "add seed host";
		//if (SEED_NODE != myip)
        //peers->Put(leveldb::WriteOptions(), SEED_HOST, "0");
		//else if (peerlist.find(SEED_HOST) == peerlist.end()) {
		//	peers->Put(leveldb::WriteOptions(), SEED_HOST, "0");
		//}
	}

    LOG(lg, info) << "handshake known hosts";
    for (it->SeekToFirst(); it->Valid() && connected.size() < 20; it->Next())
	{
		if (it->key().ToString() == myip) continue;
		if (it->key().ToString() == myhost) continue;
        LOG(lg, info) << "handshake " << it->key().ToString();
		if (doHandshake(it->key().ToString())) numconnected++;
	}
	delete it;
    LOG(lg, info) << "finished handshake known hosts";

    LOG(lg, info) << "handshake newpeers";
	while (numconnected < 20 && newpeers.size() > 0)
	{
        LOG(lg, info) << "numconnected: " << numconnected << "newpeers " << newpeers.size();
		vector<std::string> copy{ newpeers };
		newpeers.clear();
		for (const auto &p : copy)
		{
            LOG(lg, info) << "handshake " << p;
			//peers->Put(leveldb::WriteOptions(), p,"0");
            if (doHandshake(p)) {
                numconnected++;
                LOG(lg, info) << "handshake success";
            }
            else
                LOG(lg, info) << "handshake fail";
		}
	}
    LOG(lg, info) << "finished handshake newpeers";

	std::string stime(fc::time_point::now());
	{
		std::lock_guard<std::mutex> lockg{ connected_mutex };
		for (auto &p : connected)
		{
			peers->Put(leveldb::WriteOptions(), p, stime);
			LOG(lg, info) << "connected to peer " << p << " at time " << stime;
		}
	}

	return;
}

bool Node::doHandshake(const std::string &inp)
{
    LOG(lg, info) << " dohandshake " << inp;
	nn::socket natbind{ AF_SP, NN_PAIR };
    natbind.bind(("tcp://*" + FB_PORT(PORT_HAND)).c_str());
	int timeout = 6000;
	natbind.setsockopt(NN_SOL_SOCKET, NN_RCVTIMEO, &timeout, sizeof(timeout));

	std::string pre{ "tcp://" };
    std::string po{ FB_PORT(PORT_SYNC_SERV) };
	NodeReply reply{};
	reqhs.set_type(NodeRequest_Type_HANDSHAKE);
	bool isconnected = false;
	nn::socket peer{ AF_SP, NN_REQ };
	timeout = 5000;
	peer.setsockopt(NN_SOL_SOCKET, NN_RCVTIMEO, &timeout, sizeof(timeout));
	peer.setsockopt(NN_SOL_SOCKET, NN_SNDTIMEO, &timeout, sizeof(timeout));

	int id = peer.connect((pre + inp + po).c_str());
	if (id < 0) {
        LOG(lg, error) << " connect error " << nn_errno();
        return false;
	}
    else LOG(lg, info) << " connect peer id " << id;

    LOG(lg, info) << "sending " << reqhs.DebugString();
    if (Sender::Send(peer, reqhs)<= 0 )
        LOG(lg, warning) << "send error/timeout";
    else {
        LOG(lg, info) << "sent ";
        if (!Receiver::Receive(peer, reply))//, NN_DONTWAIT))
            LOG(lg, warning) << "receive error/timeout";
        else  {
            LOG(lg, info) << "received " << reply.DebugString();
			{
				std::lock_guard<std::mutex> lockg{ connected_mutex };
				connected.push_back(inp);
			}

			isconnected = true;
            LOG(lg, info) << "connected! " << inp;

            if (reply.hight() > maxhi) {
				maxhi = reply.hight();
                LOG(lg, info) << inp << "new high! " << maxhi;
            }

            if (reply.hight() > current_hight) {
				higherpeers.push_back(inp);
                LOG(lg, info) << inp << " " << reply.hight() << "higher than me " << current_hight;
            }

			for (auto i : reply.ips())
			{
				if (i == myip) continue;
				if (peerlist.find(i) == end(peerlist))
				{
					//peerlist.insert(i);
                    LOG(lg, info) << i << "new peer. thank! " << inp;
					newpeers.push_back(i);
				}
			}

            LOG(lg, info) << "behind_nat " << behind_nat;
            if (behind_nat) {
                reqhs.set_type(NodeRequest_Type_NAT_TEST);
                LOG(lg, info) << "out NAT test. sending" << reqhs.DebugString();
                if (Sender::Send(peer, reqhs) <= 0)
                    LOG(lg, warning) << "send error/timeout";
                else {
                    if (!Receiver::Receive(peer, reply))
                        LOG(lg, warning) << "receive error/timeout";
                    else {
                        LOG(lg, info) << "received " << reply.DebugString();
                        LOG(lg, info) << "try recive NAT bind" ;

						if (Receiver::Receive(natbind, reqhs)) {
                            LOG(lg, info) << "!behind_nat! received: " << reqhs.DebugString();
							behind_nat = false;
						}
                        else
                            LOG(lg, info) << "receive error/timeout,  behind_nat";
					}
				}
				LOG(lg, info) << "after behind_nat " << behind_nat;
			}
		}
    }

    peer.shutdown(id);

	return isconnected;
}

bool Node::getMyIp()
{
	WSAData wsaData;
	if (WSAStartup(MAKEWORD(1, 1), &wsaData) != 0) {
		return false;// 255;
	}

	char ac[80];
	if (gethostname(ac, sizeof(ac)) == SOCKET_ERROR) {
		LOG(lg, error) << "Error " << WSAGetLastError() <<
			" when getting local host name." << endl;
		return 1;
	}
	LOG(lg, info) << "Host name is " << ac << "." << endl;

	myhost = ac;
	WSACleanup();;
	
	bool ret = false;
	try
	{  
		boost::asio::io_service io_service;

		LOG(lg, info) << ".";

		// Get a list of endpoints corresponding to the server name.
		tcp::resolver resolver(io_service);
		tcp::resolver::query query("checkip.dyndns.org", "http");
		tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
		LOG(lg, info) << ".";

		// Try each endpoint until we successfully establish a connection.
		tcp::socket socket(io_service);
		boost::asio::connect(socket, endpoint_iterator);
		LOG(lg, info) << ".";

		// Form the request. We specify the "Connection: close" header so that the
		// server will close the socket after transmitting the response. This will
		// allow us to treat all data up until the EOF as the content.
		boost::asio::streambuf request;
		std::ostream request_stream(&request);
		request_stream << "GET / HTTP/1.0\r\n";
		request_stream << "Host: checkip.dns.org\r\n";
		//request_stream << "User-Agent: Mozilla/4.0 (compatible; MSIE 7.0; Windows NT 5.1)\r\n";
		request_stream << "Accept: */*\r\n";
		request_stream << "Connection: close\r\n\r\n";
		LOG(lg, info) << ".";

		// Send the request.
		boost::asio::write(socket, request);
		LOG(lg, info) << ".";

		// Read the response status line. The response streambuf will automatically
		// grow to accommodate the entire line. The growth may be limited by passing
		// a maximum size to the streambuf constructor.
		boost::asio::streambuf response;
		boost::asio::read_until(socket, response, "\r\n");
		LOG(lg, info) << ".";

		// Check that response is OK.
		std::istream response_stream(&response);
		std::string http_version;
		response_stream >> http_version;
		unsigned int status_code;
		response_stream >> status_code;
		std::string status_message;
		std::getline(response_stream, status_message);
		if (!response_stream || http_version.substr(0, 5) != "HTTP/")
		{
			LOG(lg, error) << "Invalid response\n";
			return 1;
		}
		if (status_code != 200)
		{
			LOG(lg, info) << "Response returned with status code " << status_code << "\n";
			return 1;
		}

		LOG(lg, info) << ".";

		// Read the response headers, which are terminated by a blank line.
		boost::asio::read_until(socket, response, "\r\n\r\n");

		// Process the response headers.
		std::string header;
		while (std::getline(response_stream, header) && header != "\r")
			LOG(lg, info) << header << "\n";
		LOG(lg, info) << "\n";

		LOG(lg, info) << ".";

		std::string body;
		std::getline(response_stream, body);

		std::string add("Address: ");
		auto result = body.substr(body.find(add) + add.length());
		result = result.substr(0, result.find("<"));
		myip = result;
		ret = true;

		// Write whatever content we already have to output.
		if (response.size() > 0)
			std::cout << &response;
		LOG(lg, info) << ".";

		// Read until EOF, writing data to output as we go.
		boost::system::error_code error;
		while (boost::asio::read(socket, response,
			boost::asio::transfer_at_least(1), error))
			std::cout << &response;
		if (error != boost::asio::error::eof)
			throw boost::system::system_error(error);
		LOG(lg, info) << ".";

	}
	catch (...)
	{
		LOG(lg, error) << ".";


	}
	return ret;
}


int Node::getLastBlockNum()
{
	std::string value;
	auto *it = blockchain->NewIterator(leveldb::ReadOptions());
	it->SeekToLast();

	if (!it->Valid()) return 0;

	auto slice = it->key();

	int num = *(reinterpret_cast<const int *>(slice.data()));
	delete it;
	return num;
}

void Node::syncService()
{
    try {
        dosyncService();
    }
    catch (std::exception &e)
    {
        LOG(lg,fatal) <<  e.what();
        return;
    }
}

void Node::dosyncService() {
	int myhight = 0;
	{
        std::lock_guard<std::mutex> lockg{ blockchain_mutex };
		myhight = getLastBlockNum();
	}

	//nn::socket blockreply{ AF_SP, NN_PAIR };
	nn::socket blockreply{ AF_SP, NN_REP };

	auto lid = blockreply.bind("inproc://syncserv");
    auto eid = blockreply.bind(("tcp://*" + FB_PORT(PORT_SYNC_SERV)).c_str()  );
	Receiver rec{ blockreply };
	Sender snd{ blockreply };
	NodeRequest nodereq{};
	NodeReply noderep{};
	int badcount = 0;
	while (sync_running)
	{
		LOG(lg, trace) << "syncService try receive";

		if (!rec.receive(nodereq)) {
			LOG(lg, error) << "!syncService receive";
			break;
		}

		LOG(lg, info) << "received " << nodereq.DebugString() << "\n";

		switch (nodereq.type())
		{
			case NodeRequest_Type_BLOCK_REQUEST:
			{
				if (nodereq.num() > myhight)
				{
                    LOG(lg, warning) << "requesting higher then my height!";
					{
                        std::lock_guard<std::mutex> lockg{ blockchain_mutex };
						myhight = getLastBlockNum();
					}
					if (nodereq.num() > myhight)
                    {
                        CheckOrphanBlocks();

                        {
                            std::lock_guard<std::mutex> lockg{ blockchain_mutex };
                            myhight = getLastBlockNum();
                        }
                        if (nodereq.num() > myhight) {
                            badcount++;
                            snd.send(Block{});
                            if (badcount == 100) {
                                LOG(lg, error) << "bad height shutdown external sync service";
                                blockreply.shutdown(eid);
                            }
                            if (badcount == 120)
                            {
                                LOG(lg, error) << "bad height shutdown local sync service";
                                blockreply.shutdown(lid);
                                sync_running = false;
                            }
                            continue;
                        }
                        else
                            LOG(lg, info) << "OK - have good height now!";
					}
                    else
                        LOG(lg, info) << "OK - have good height now!";
				}

				Block sb{};
				std::string value;
                int num = nodereq.num();
				leveldb::Slice snum((char*)&num, sizeof(int));
				{
                    std::lock_guard<std::mutex> lockg{ blockchain_mutex };
					blockchain->Get(leveldb::ReadOptions(), snum, &value);
				}

				sb.ParseFromString(value);
                LOG(lg, info) << "sending block with height " << num;
                LOG(lg, info) << "sent " << snd.send(sb);
				break;
			}

			case NodeRequest_Type_HIGHT_REQUEST:
				{
                    std::lock_guard<std::mutex> lockg{ blockchain_mutex };
					myhight = getLastBlockNum();
				}

                if ( nodereq.num() > myhight ) {
                    LOG(lg, info) << "client has higher num " << nodereq.num();
                    CheckOrphanBlocks();
                    {
                        std::lock_guard<std::mutex> lockg{ blockchain_mutex };
                        myhight = getLastBlockNum();
                    }
                }
				noderep.Clear();
				noderep.set_hight(myhight);
                LOG(lg, info) << "myheight " << myhight;
                LOG(lg, info) << "sent " << snd.send(noderep);
				break;

			case NodeRequest_Type_HANDSHAKE:
			{
				std::string stime(fc::time_point::now());

				if (nodereq.has_myip() && nodereq.myip() != myip && nodereq.myip() != "") {
                    LOG(lg, info) << "new conneced IP node! ";
					peers->Put(leveldb::WriteOptions(), nodereq.myip(), stime);
					std::lock_guard<std::mutex> lockg{ connected_mutex };
					newconnected.push_back(nodereq.myip());
				}
				
                else if (nodereq.has_myhost() && nodereq.myhost() != myhost &&
                         nodereq.myhost() != "") {
                    LOG(lg, info) << "new conneced Host node! ";
                    peers->Put(leveldb::WriteOptions(), nodereq.myhost(), stime);
					std::lock_guard<std::mutex> lockg{ connected_mutex };
					newconnected.push_back(nodereq.myhost());
				}

				{
                    std::lock_guard<std::mutex> lg{ blockchain_mutex };
					myhight = getLastBlockNum();
				}

				noderep.Clear();
				noderep.set_hight(myhight);
				{
					std::lock_guard<std::mutex> lockg{ connected_mutex };
					for (auto &s : connected)
						noderep.add_ips(s);
				}
                LOG(lg, info) << "sending " << noderep.DebugString();
                LOG(lg, info) << "sent " << snd.send(noderep);
				break;
			}
			case NodeRequest_Type_NAT_TEST:
			{
				LOG(lg, info) << "NAT test for: " << nodereq.myip();
				snd.send(noderep);
				std::this_thread::sleep_for(std::chrono::seconds{ 1 });

                if ( nodereq.myip() == "") {
                    LOG(lg, warning) << "no ip - skip NAT test send";
                    break;
                }

                std::string pre{ "tcp://" };
                std::string po{ FB_PORT(PORT_HAND) };
				nn::socket peer{ AF_SP, NN_PAIR };
                std::string natpeertest = (pre + nodereq.myip() + po);
                int id = peer.connect(natpeertest.c_str());
				int timeout = 10000;
				peer.setsockopt(NN_SOL_SOCKET, NN_SNDTIMEO, &timeout, sizeof(timeout));
                LOG(lg, info) << "NAT send " << natpeertest;
                LOG(lg, info) << "sent " << Sender::Send(peer, nodereq);
				break;
			}
			default:
                LOG(lg, warning) << "Unkown request type";
				break;
		}
	}
	LOG(lg, info) << "syncService receive outloop \n";

}

void Node::syncRequest()
{
    try {
        dosyncRequest();
    }
    catch (std::exception &e)
    {
        LOG(lg,fatal) <<  e.what();
        return;
    }
}

void Node::dosyncRequest() {
	std::string pre{ "tcp://" };
    std::string po{ FB_PORT(PORT_SYNC_SERV) };

	nn::socket blockrequest{ AF_SP, NN_REQ };
	for (auto &p : higherpeers)
		blockrequest.connect((pre + p + po).c_str());

	NodeRequest req{};
	req.set_type(NodeRequest_Type_BLOCK_REQUEST);
	Sender snd{ blockrequest };
	Receiver rec{ blockrequest };
	Block sb{};
	while (current_hight < maxhi && sync_req_running)
	{
        LOG(lg, info) << " current_hight " << current_hight << " maxhi " << maxhi;

		req.set_num(current_hight + 1);
		snd.send(req);
        LOG(lg, info) << "send " << req.DebugString();

		if (!rec.receive(sb)) break;
        LOG(lg, info) << "receive " << sb.DebugString();

        if (!BlockProcessor::verifySignedBlock(sb)) {
            LOG(lg, error) << "!block";
            continue;
        }

        if (sb.signedhead().head().num() <= current_hight) {
            LOG(lg, error) << "received wrong block num";
			continue;
        }

        if (sb.signedhead().head().num() > current_hight + 1) {
            LOG(lg, error) << "received wrong block num";
            continue; //TODO save for later
        }


		current_hight++;
        LOG(lg, info) << "received block num: current_hight " << current_hight;
		leveldb::Slice snum((char*)&current_hight, sizeof(int));
		{
            LOG(lg, trace) << "lock";
			std::lock_guard<std::mutex> lg{ blockchain_mutex };
			blockchain->Put(leveldb::WriteOptions(), snum, sb.SerializeAsString());
		}
        LOG(lg, trace) << "put";

	}
}

void Node::runLive()
{
    try {
        dorunLive();
    }
    catch (std::exception &e)
    {
        LOG(lg,fatal) <<  e.what();
        return;
    }
}

void Node::dorunLive() {
	int myhight = 0;
	{
		std::lock_guard<std::mutex> lockg{ blockchain_mutex };
		myhight = getLastBlockNum();
	}
	std::string pre{ "tcp://" };
    std::string po{ FB_PORT(PORT_LIVE_BLOCK) };
	std::set<int> published{};
    nn::socket blockslivesub{ AF_SP, NN_SUB };
	blockslivesub.setsockopt(NN_SUB, NN_SUB_SUBSCRIBE, "", 0);
    int timeout = 11000;
    blockslivesub.setsockopt(NN_SOL_SOCKET, NN_RCVTIMEO, &timeout, sizeof(timeout));
	{
		std::lock_guard<std::mutex> lockg{ connected_mutex };
		for (auto &p : connected)
			blockslivesub.connect((pre + p + po).c_str());
	}
	blockslivesub.connect("inproc://newlocalblock");

	nn::socket blockslivepub{ AF_SP, NN_PUB };
	blockslivepub.bind((pre + "*" + po).c_str());
	blockslivepub.bind("inproc://pubblock");

	Sender snd{ blockslivepub };
	Receiver rec{ blockslivesub };
	Block sb{};

	while (running_live)
	{
        if (!rec.receive(sb)) {
            std::this_thread::sleep_for(std::chrono::seconds{ 1 });
            std::lock_guard<std::mutex> lockg{ connected_mutex };
            for (auto &p : newconnected2) {
                blockslivesub.connect((pre + p + po).c_str());
            }
            newconnected2.clear();
            continue;
        }

		LOG(lg, info) << "running_live rec " << sb.DebugString() << "\n";

        if (!BlockProcessor::verifySignedBlock(sb)) {
            LOG(lg,error) << " !BlockProcessor::verifySignedBlock(sb) ";
			continue;
        }

        if (published.find(sb.signedhead().head().num()) != end(published)) {
            LOG(lg,warning) << "already seen num " << sb.signedhead().head().num();
			continue;
        }

        if (sb.signedhead().head().num() > myhight + 1)
        {
            std::lock_guard<std::mutex> lockg{ blockchain_mutex };
            myhight = getLastBlockNum();
        }

        if (sb.signedhead().head().num() == myhight + 1)
		{
            LOG(lg,info) << "Received next " << myhight+1;
			myhight++;
			leveldb::Slice snum((char*)&myhight, sizeof(int));
			{
				std::lock_guard<std::mutex> lockg{ blockchain_mutex };
				blockchain->Put(leveldb::WriteOptions(), snum, sb.SerializeAsString());
			}

			Node::ClearTx(sb);

            CheckOrphanBlocks();
/*            int next = myhight;
/*
            while(true) {
                next++;
                leveldb::Slice snum((char*)&next, sizeof(int));
                {
                    std::string value;
                    std::lock_guard<std::mutex> lockg{ blockchain_mutex };
                    if (holdblocks->Get(leveldb::ReadOptions(), snum, &value).IsNotFound())
                        break;

                    LOG(lg, info) << "found next block in hold" << next;

                    blockchain->Put(leveldb::WriteOptions(), snum, value);
                    holdblocks->Delete(leveldb::WriteOptions(),snum);
                }
            }
*/
		}
        else if ( sb.signedhead().head().num() > myhight+1){
            LOG(lg,warning) << "Received gap in block " << sb.signedhead().head().num();
            int num = sb.signedhead().head().num();
            leveldb::Slice snum((char*)&num, sizeof(int));
            {
                std::lock_guard<std::mutex> lockg{ blockchain_mutex };
                holdblocks->Put(leveldb::WriteOptions(), snum, sb.SerializeAsString());
            }
            Node::ClearTx(sb);
        }

		snd.send(sb);
		published.insert(sb.signedhead().head().num());
	}
}

void Node::pendingTransactions()
{
    try {
        dopendingTransactions();
    }
    catch (std::exception &e)
    {
        LOG(lg,fatal) <<  e.what();
        return;
    }
}

void Node::dopendingTransactions() {
	bool nat = behind_nat && connected.size() > 0;
	std::string pre{ "tcp://" };
    std::string po{ FB_PORT(PORT_LIVE_TX) };
    std::string po2{ FB_PORT(PORT_LIVE_TX_NAT) };

	std::set<std::string> published{};
	nn::socket translivepub{ AF_SP, NN_PUB };
    nn::socket translivesub{ AF_SP, NN_SUB};
	translivesub.setsockopt(NN_SUB, NN_SUB_SUBSCRIBE, "", 0);
	int timeout = 10000;
	translivesub.setsockopt(NN_SOL_SOCKET, NN_RCVTIMEO, &timeout, sizeof(timeout));
	{
		std::lock_guard<std::mutex> lockg{ connected_mutex };
		for (auto &p : connected) {
			translivesub.connect((pre + p + po).c_str());
			if (nat) translivepub.connect((pre + p + po2).c_str());
		}
	}
	translivesub.connect("inproc://newlocaltrans");

	if (!nat)	{
		translivesub.bind((pre + "*" + po2).c_str());
		translivepub.bind((pre + "*" + po).c_str());
	}
	translivepub.bind("inproc://pubtrans");

	Sender snd{ translivepub };
	Receiver rec{ translivesub };
	SignedTransaction st{};

	while (running_live)
	{
		if (!rec.receive(st)) {
			std::this_thread::sleep_for(std::chrono::seconds{ 1 });
			std::lock_guard<std::mutex> lockg{ connected_mutex };
            if ( newconnected.size() <= 0 )
                continue;
			for (auto &p : newconnected) {
                LOG(lg, info) << "newconnected! " << p;
				translivesub.connect((pre + p + po).c_str());
			}
            newconnected2 = newconnected;
			newconnected.clear();
			continue;
		}
		LOG(lg, info) << "rec tx hash " << st.id();

		//todo: send to transaction pool
        if ( !BlockProcessor::verifySignedTransaction(st)) {
            LOG(lg, warning) << "ignore bad tx";
            continue;
        }
		//  only send to peers if transaction is verified 
		if (published.find(st.id()) != end(published))
			continue;

		Node::txpool->Put(leveldb::WriteOptions(), st.id(), st.SerializeAsString());
		snd.send(st);
		published.insert(st.id());
		LOG(lg, info) << "put txpool " << st.DebugString();
	}
}

void Node::ClearTx(const Block &b) {
	for (const auto &st : b.signed_transactions()) {
		Node::txpool->Delete(leveldb::WriteOptions(), st.id());
	}
}

void Node::CheckOrphanBlocks()
{
    int myhight = 0;
    {
        std::lock_guard<std::mutex> lockg{ blockchain_mutex };
        myhight = getLastBlockNum();
    }

    while(true) {
        myhight++;
        leveldb::Slice snum((char*)&myhight, sizeof(int));
        {
            std::string value;
            std::lock_guard<std::mutex> lockg{ blockchain_mutex };
            if (holdblocks->Get(leveldb::ReadOptions(), snum, &value).IsNotFound())
                break;

            LOG(lg, info) << "found next block in hold" << myhight;

            blockchain->Put(leveldb::WriteOptions(), snum, value);
            holdblocks->Delete(leveldb::WriteOptions(),snum);
        }
    }
}

decltype(Node::blockchain) Node::blockchain{};
decltype(Node::txpool) Node::txpool{};


}	

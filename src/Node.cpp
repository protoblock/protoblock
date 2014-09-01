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

using boost::asio::ip::tcp;

using namespace std;
using namespace nn;
	
namespace fantasybit
{
int Node::init()
{
	leveldb::Options options;
	options.create_if_missing = true;
	leveldb::Status status;
	leveldb::DB *db;
	status = leveldb::DB::Open(options, filedir("node/peers"), &db);
	peers.reset(db);

	leveldb::DB *db2;
	status = leveldb::DB::Open(options, filedir("block/blockchain"), &db2);
	blockchain.reset(db2);

	leveldb::DB *db3;
	status = leveldb::DB::Open(options, filedir("block/holdblock"), &db2);
	holdblocks.reset(db3);

	current_hight = getLastBlockNum();

	if (getMyIp())
		reqhs.set_myip(myip);

	auto *it = peers->NewIterator(leveldb::ReadOptions());
	it->SeekToFirst();
	if (!it->Valid())
		peers->Put(leveldb::WriteOptions(), SEED_NODE, "0");
	delete it;

	return 0;
}

void Node::runHandShake()
{
	int numconnected = 0;

	auto *it = peers->NewIterator(leveldb::ReadOptions());
	for (it->SeekToFirst(); it->Valid(); it->Next())
		peerlist.insert(it->key().ToString());
	
	for (it->SeekToFirst(); it->Valid() && connected.size() < 20; it->Next())
	{
		if (doHandshake(it->key().ToString())) numconnected++;
	}
	delete it;

	while (numconnected < 20 && newpeers.size() > 0)
	{
		vector<std::string> copy{ newpeers };
		newpeers.clear();
		for (const auto &p : copy)
		{
			//peers->Put(leveldb::WriteOptions(), p,"0");
			if (doHandshake(p)) numconnected++;
		}
	}

	std::string stime(fc::time_point::now());
	{
		std::lock_guard<std::mutex> lg{ connected_mutex };
		for (auto &p : connected)
			peers->Put(leveldb::WriteOptions(), p, stime);
	}

	return;
}

bool Node::doHandshake(const std::string &inp)
{
	std::string pre{ "tcp://" };
	std::string po{ ":8125" };
	NodeReply reply{};
	reqhs.set_type(NodeRequest_Type_HANDSHAKE);
	bool isconnected = false;
	nn::socket peer{ AF_SP, NN_REQ };
	int timeout = 10000;
	peer.setsockopt(NN_SOL_SOCKET, NN_RCVTIMEO, &timeout, sizeof(timeout));
	if (peer.connect((pre + inp + po).c_str()))
	{
		if (Sender::Send(peer, reqhs))
		{
			if (Receiver::Receive(peer, reply))//, NN_DONTWAIT))
			{
				{
					std::lock_guard<std::mutex> lg{ connected_mutex };
					connected.push_back(inp);
				}

				isconnected = true;
				if (reply.hight() > maxhi)
					maxhi = reply.hight();

				if (reply.hight() > current_hight)
					higherpeers.push_back(inp);

				for (auto i : reply.ips())
				{
					if (i == myip) continue;
					if (peerlist.find(i) == end(peerlist))
					{
						//peerlist.insert(i);
						newpeers.push_back(i);
					}
				}
			}
		}
	}
	return isconnected;
}

bool Node::getMyIp()
{
	bool ret = false;
	try
	{  
		boost::asio::io_service io_service;

		// Get a list of endpoints corresponding to the server name.
		tcp::resolver resolver(io_service);
		tcp::resolver::query query("checkip.dyndns.org", "http");
		tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);

		// Try each endpoint until we successfully establish a connection.
		tcp::socket socket(io_service);
		boost::asio::connect(socket, endpoint_iterator);

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

		// Send the request.
		boost::asio::write(socket, request);

		// Read the response status line. The response streambuf will automatically
		// grow to accommodate the entire line. The growth may be limited by passing
		// a maximum size to the streambuf constructor.
		boost::asio::streambuf response;
		boost::asio::read_until(socket, response, "\r\n");

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
			std::cout << "Invalid response\n";
			return 1;
		}
		if (status_code != 200)
		{
			std::cout << "Response returned with status code " << status_code << "\n";
			return 1;
		}

		// Read the response headers, which are terminated by a blank line.
		boost::asio::read_until(socket, response, "\r\n\r\n");

		// Process the response headers.
		std::string header;
		while (std::getline(response_stream, header) && header != "\r")
			std::cout << header << "\n";
		std::cout << "\n";


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

		// Read until EOF, writing data to output as we go.
		boost::system::error_code error;
		while (boost::asio::read(socket, response,
			boost::asio::transfer_at_least(1), error))
			std::cout << &response;
		if (error != boost::asio::error::eof)
			throw boost::system::system_error(error);
	}
	catch (...)
	{

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
	int myhight = 0;
	{
		std::lock_guard<std::mutex> lg{ blockchain_mutex };
		myhight = getLastBlockNum();
	}

	nn::socket blockreply{ AF_SP, NN_REP };
	auto lid = blockreply.bind("inproc:://syncserv");
	auto eid = blockreply.bind("tcp://*:8125");
	Receiver rec{ blockreply };
	Sender snd{ blockreply };
	NodeRequest nodereq{};
	NodeReply noderep{};
	int badcount = 0;
	while (sync_running)
	{
		if (!rec.receive(nodereq))
			break;

		switch (nodereq.type())
		{
			case NodeRequest_Type_BLOCK_REQUEST:
			{
				if (nodereq.num() > myhight)
				{
					{
						std::lock_guard<std::mutex> lg{ blockchain_mutex };
						myhight = getLastBlockNum();
					}
					if (nodereq.num() > myhight)
					{
						badcount++;
						snd.send(nodereq);
						if (badcount == 100)
							blockreply.shutdown(eid);
						if (badcount == 120)
						{
							blockreply.shutdown(lid);
							sync_running = false;
						}
						continue;
					}
				}

				SignedBlock sb{};
				std::string value;
				int num = nodereq.num();
				leveldb::Slice snum((char*)&num, sizeof(int));
				{
					std::lock_guard<std::mutex> lg{ blockchain_mutex };
					blockchain->Get(leveldb::ReadOptions(), snum, &value);
				}

				sb.ParseFromString(value);
				snd.send(sb);
				break;
			}

			case NodeRequest_Type_HIGHT_REQUEST:
				{
					std::lock_guard<std::mutex> lg{ blockchain_mutex };
					myhight = getLastBlockNum();
				}

				noderep.Clear();
				noderep.set_hight(myhight);
				snd.send(noderep);
				break;

			case NodeRequest_Type_HANDSHAKE:
				{
					std::lock_guard<std::mutex> lg{ blockchain_mutex };
					myhight = getLastBlockNum();
				}

				noderep.Clear();
				noderep.set_hight(myhight);
				{
					std::lock_guard<std::mutex> lg{ connected_mutex };
					for (auto &s : connected)
						noderep.add_ips(s);
				}
				snd.send(noderep);
				break;
			default:
				break;
		}
	}
}

void Node::syncRequest() 
{
	std::string pre{ "tcp://" };
	std::string po{ ":8125" };

	nn::socket blockrequest{ AF_SP, NN_REQ };
	for (auto &p : higherpeers)
		blockrequest.connect((pre + p + po).c_str());

	NodeRequest req{};
	req.set_type(NodeRequest_Type_BLOCK_REQUEST);
	Sender snd{ blockrequest };
	Receiver rec{ blockrequest };
	SignedBlock sb{};
	while (current_hight < maxhi && sync_req_running)
	{
		req.set_num(current_hight + 1);
		snd.send(req);

		if (!rec.receive(sb)) break;

		if (!BlockProcessor::verifySignedBlock(sb))
			continue;

		if (sb.block().head().num() < current_hight)
			continue;

		if (sb.block().head().num() > current_hight)
			continue; //TODO save for later

		current_hight++;
		leveldb::Slice snum((char*)&current_hight, sizeof(int));
		{
			std::lock_guard<std::mutex> lg{ blockchain_mutex };
			blockchain->Put(leveldb::WriteOptions(), snum, sb.SerializeAsString());
		}

	}
}

void Node::runLive()
{
	int myhight = 0;
	{
		std::lock_guard<std::mutex> lg{ blockchain_mutex };
		myhight = getLastBlockNum();
	}
	std::string pre{ "tcp://" };
	std::string po{ ":8126" };
	std::set<int> published{};
	nn::socket blockslivesub{ AF_SP, NN_SUB };
	{
		std::lock_guard<std::mutex> lg{ connected_mutex };
		for (auto &p : connected)
			blockslivesub.connect((pre + p + po).c_str());
	}
	blockslivesub.connect("inproc:://newlocalblock");

	nn::socket blockslivepub{ AF_SP, NN_PUB };
	blockslivepub.bind((pre + "*" + po).c_str());
	blockslivepub.bind("inproc:://pubblock");

	Sender snd{ blockslivepub };
	Receiver rec{ blockslivesub };
	SignedBlock sb{};

	while (running_live)
	{
		if (!rec.receive(sb)) continue;

		if (!BlockProcessor::verifySignedBlock(sb))
			continue;

		if (published.find(sb.block().head().num()) != end(published))
			continue;

		if (sb.block().head().num() == myhight + 1)
		{
			myhight++;
			leveldb::Slice snum((char*)&myhight, sizeof(int));
			{
				std::lock_guard<std::mutex> lg{ blockchain_mutex };
				blockchain->Put(leveldb::WriteOptions(), snum, sb.SerializeAsString());
			}
		}

		snd.send(sb);
		published.insert(sb.block().head().num());
	}
}

void Node::pendingTransactions()
{
	std::string pre{ "tcp://" };
	std::string po{ ":8127" };
	std::set<std::string> published{};
	nn::socket translivesub{ AF_SP, NN_SUB };
	{
		std::lock_guard<std::mutex> lg{ connected_mutex };
		for (auto &p : connected)
			translivesub.connect((pre + p + po).c_str());
	}
	translivesub.connect("inproc:://newlocaltrans");

	nn::socket translivepub{ AF_SP, NN_PUB };
	translivepub.bind((pre + "*" + po).c_str());
	translivepub.bind("inproc:://pubtrans");

	Sender snd{ translivepub };
	Receiver rec{ translivesub };
	SignedTransaction st{};

	while (running_live)
	{
		if (!rec.receive(st)) continue;

		if (published.find(st.id()) != end(published))
			continue;

		snd.send(st);
		published.insert(st.id());
	}
}
}	
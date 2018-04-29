Peer to peer - design notes 
===========================

Protoblock is a network of peers. Clients send and receive transactions from peers. Network is a distributed mesh. Data is flood filled, where everyone sends everyone everything! 

### p2p operations
* Node discovery 

* Peer chain comparison

* Blockchain Sync  

* Handshake

* Mempool sync 

* Transaction gossip 

* Block gossip

* Peer list gossip 

### Design goals and issues
All clients should be able to run as a full-node, with zero config, even if running "from a laptop in starbucks"

* NAT - network address translation 
The major blocking issues for running full nodes, is peer discovery and identification.  Many home and public networks are “Natted”, where a clients public ip address, cannot be used by other peers to find him. This is a result of running out of ipv4 ip addresses. Bottom line, a natted client cannot bind to a port, share his IP address and wait for a connection!  See [The State of NAT Traversal](https://www.zerotier.com/blog/state-of-nat-traversal.shtml)

* ZeroTier solution  - [ZeroTierOne](https://github.com/zerotier/ZeroTierOne) [libzt](https://github.com/zerotier/libzt)
ZeroTier replaces ip addresses with  _ZeroTier addresses_ , this puts a whole new layer on top of the internet, where all peers can be discovered and identified. All the low level work of dealing with clients behind difficult networks is abstracted away!  

### Implementation 

##### Fantasybit/Protoblock Project - past attempts  
* p2p code was previously developed within the fantasybit project. Project was stalled in [2014](https://github.com/protoblock/fantasybit-2014/blob/osx/src/Commissioner.h) due to NAT blocking issue (_Genesis block: Monday, August 25, 2014 6:01:17 PM_). 

* p2p code was then successfully tested in a [2015](https://github.com/protoblock/fantasybit-2014/blob/windows/) live demo. However it never made it into prooduction due to unstable _nanomsg_ library. [Fantasybit-2014](https://github.com/protoblock/fantasybit-2014)
[Node.h](https://github.com/protoblock/fantasybit-2014/blob/windows/src/Node.h)
[Node.cpp](https://github.com/protoblock/fantasybit-2014/blob/windows/src/Node.cpp)
[MsgSock.h](https://github.com/protoblock/fantasybit-2014/blob/windows/src/MsgSock.h)
[Processor.h](https://github.com/protoblock/fantasybit-2014/blob/windows/src/Processor.h)
[ClientUI.h](https://github.com/protoblock/fantasybit-2014/blob/windows/src/ClientUI.h)
[Server.h](https://github.com/protoblock/fantasybit-2014/blob/windows/src/Server.h)

##### Scalability Protocols 
The communication patterns, also called "scalability protocols", are basic blocks for building distributed systems. By combining them you can create a vast array of distributed applications. [nanomsg.org](http://nanomsg.org)

Scalability protocols are layered on top of the transport layer in the network stack. 
##### SP implimentations - [Garrett D’Amore](https://staysail.github.io/nng_presentation/nng_presentation.html) 
[nanomsg](http://nanomsg.org/) (original),
[mangos](https://github.com/go-mangos/mangos) (golang),
[nng](https://nanomsg.github.com/nng) (nanomsg-next-generation),
[scaproust](https://github.com/blabaere/scaproust) (rust)

The following scalability protocols are currently available:
* Req/Rep - allows to build clusters of stateless services to process user requests
* Pub/Sub - distributes messages to large sets of interested subscribers
* Bus - simple many-to-many communication
* Pipeline (Push/Pull) - aggregates messages from multiple sources and load balances them among many destinations
* Survey - allows to query state of multiple applications in a single go
* Pair - simple one-to-one communication
* Polyamorous Pair (nng only) - a peer can maintain multiple partnerships 
* Star (mangos only) – communication with all members of the topology

At the moment, the nng library supports the following transports mechanisms:
* TCP - network transport via TCP
* IPC - transport between processes on a single machine
* INPROC - transport within a process (between threads, modules etc.)
* WS - websockets over TCP (with TLS on mangos and nng)
* TLS - Transport Layer Security (mangos and nng only)
* QUIC - Quick UDP Internet Connections (mangos only)
* **ZeroTier** (nng only) [http://nanomsg.org/rfcs/sp-zerotier-v0.html]

[See NNG Reference Manual](https://nanomsg.github.io/nng/man/v0.8.0/index.html)

##### Example use in fantasybit 

_doHandshake_ from [Node.cpp](https://github.com/protoblock/fantasybit-2014/blob/windows/src/Node.cpp)

_Node::doHandhsake_ function attemts to connect to a peer and check its current block height. A peer is a nanomsg socket connection to "tcp://x.x.x.x:8125", and it is a Requestor (NN_REQ) from the Request/Reply Scalability Protocol. _Sender::Send_ is used to send "reqhs" - a request for block height, _Receiver::Receive_ is used to process the response. 


	#include <nanomsg\reqrep.h>
    ...
	bool Node::doHandshake(const std::string &inp)
    ...
		std::string pre{ "tcp://" };
		std::string po{ ":8125" };
		...
		nn::socket peer{ AF_SP, NN_REQ };
		int id = peer.connect((pre + inp + po).c_str());
		...
		if (Sender::Send(peer, reqhs) > 0 )
		...
		if (Receiver::Receive(peer, reply))//, NN_DONTWAIT))
		...	
			isconnected = true;
			if (reply.hight() > maxhi)
				maxhi = reply.hight();

_Sender::Send_ from [MsgSock.cpp](https://github.com/protoblock/fantasybit-2014/blob/windows/src/MsgSock.cpp)

	int Sender::Send(nn::socket &s, const google::protobuf::Message &msg, int flags)
	...
	    return s.send(buf,sz,flags);

_Receiver::receive_ from [MsgSock.h](https://github.com/protoblock/fantasybit-2014/blob/windows/src/MsgSock.h)

	template <class T>
	static bool Receive(nn::socket &rsock,T &t, int flags = 0)
	...
		int size = rsock.recv(&sbuf, NN_MSG, flags);

_Node::syncService()_ creates a **blockreply** socket connection as a Replier (NN_REP), it then binds both a tcp port and a inprocess address **_"inproc://syncserv"_** to the socket. Calling receive, will wait until it gets a request for a BLOCK by number, or block HEIGHT. This request can be from a connected peer or from within the same client on a seperate thread. It then processes the request by getting the BLOCK or HEIGHT from its local (leveldb) database and sends it with _send()_. 

_syncService_ from [Node.cpp](https://github.com/protoblock/fantasybit-2014/blob/windows/src/Node.cpp)

	void Node::syncService()
	...
		nn::socket blockreply{ AF_SP, NN_REP };
		...
		auto lid = blockreply.bind("inproc://syncserv");
		auto eid = blockreply.bind("tcp://*:8125");
		Receiver rec{ blockreply };
		Sender snd{ blockreply };
		...
		rec.receive(nodereq))
	        ...
			switch (nodereq.type())
			...
				case NodeRequest_Type_BLOCK_REQUEST:
				...
					Block sb{};
					...
					blockchain->Get(leveldb::ReadOptions(), snum, &value);
					...
					snd.send(sb);
				...

				case NodeRequest_Type_HIGHT_REQUEST:
				    ...
				    myhight = getLastBlockNum();
					...
					noderep.set_hight(myhight);
					snd.send(noderep);

_Node::syncRequest_ creates a blockrequest socket connection as a Requestor, it then connects to all peers with higher block height. It then does a single _send.send(req)_ call which sends this request to all peers. When it receives the new block it vaerifies it and then adds it to the local (leveldb) database with _blockchain->Put()_

_syncRequest_ from [Node.cpp](https://github.com/protoblock/fantasybit-2014/blob/windows/src/Node.cpp)

	void Node::syncRequest() 
	...
		nn::socket blockrequest{ AF_SP, NN_REQ };
		for (auto &p : higherpeers)
			blockrequest.connect((pre + p + po).c_str());

        ...
		req.set_type(NodeRequest_Type_BLOCK_REQUEST);
		Sender snd{ blockrequest };
		Receiver rec{ blockrequest };
		...
		while (current_hight < maxhi && sync_req_running)
		...
			snd.send(req);
			...
			if (!rec.receive(sb)) break;
			...
			BlockProcessor::verifySignedBlock(sb);
			...
			if (sb.signedhead().head().num() <= current_hight)
				continue;

			if (sb.signedhead().head().num() > current_hight + 1)
				continue;

			current_hight++;
			...
				blockchain->Put(leveldb::WriteOptions(), snum, sb.SerializeAsString());

_Node::runLive_ creates a blockslivesub socket connection as a Subscriber (NN_SUB) from Pub/Sub protocol, and subscribes to all messages, it then connects (subsscribes) to all connected peers as well as **_"inproc://newlocalblock"_**, the local miner/block producer. 

_runLive_ from [Node.cpp](https://github.com/protoblock/fantasybit-2014/blob/windows/src/Node.cpp)

	void Node::runLive()
	...
		std::set<int> published{};
		nn::socket blockslivesub{ AF_SP, NN_SUB };
		blockslivesub.setsockopt(NN_SUB, NN_SUB_SUBSCRIBE, "", 0);
	...
		for (auto &p : connected)
			blockslivesub.connect((pre + p + po).c_str());
	...
		blockslivesub.connect("inproc://newlocalblock");

A blockslivepub socket connection is then setup as a Publisher (NN_PUB) and binds to tcp port and **_"inproc://pubblock"_** locally. 

	nn::socket blockslivepub{ AF_SP, NN_PUB };
	blockslivepub.bind((pre + "*" + po).c_str());
	blockslivepub.bind("inproc://pubblock");

A Sender class is for publishing and a Receiver class for subscribers 

	Sender snd{ blockslivepub };
	Receiver rec{ blockslivesub };
	Block sb{};

On receive of an unpublished valid block, write block to local (leveldb) database, clear transactions from mempool, and send (publish) block to all peer and local subscribers.  

	while (running_live)
	...
		if (!rec.receive(sb)) continue;
	...
		if (published.find(sb.signedhead().head().num()) != end(published))
			continue;

		if (sb.signedhead().head().num() == myhight + 1)
		...
			myhight++;
	        ...
			blockchain->Put(leveldb::WriteOptions(), snum, sb.SerializeAsString());
	        ...
			Node::ClearTx(sb);
	    ...
		snd.send(sb);
		published.insert(sb.signedhead().head().num());

_ClearTx_ from [Node.cpp](https://github.com/protoblock/fantasybit-2014/blob/windows/src/Node.cpp) to remove transaction from new block from mempool 

	void Node::ClearTx(const Block &b) 
		for (const auto &st : b.signed_transactions()) 
			Node::txpool->Delete(leveldb::WriteOptions(), st.id());

_class BlockProcessor_ from [Processor.h](https://github.com/protoblock/fantasybit-2014/blob/windows/src/Processor.h)

_BlockProcessor_ sets up _syncserv_ as a Request (NN_REQ) socket connection to **_"inproc://syncserv"_**, and sets _syncradio_ as a sender/receiver pair to _syncserv_.  A _subblock_ (NN_SUB) is setup as a subscriber to **_"inproc://pubblock"_**  

	class BlockProcessor
	...
		nn::socket syncserv, delasrv;
		std::pair<Sender, Receiver> syncradio;
	..
		Receiver rec_block;
		nn::socket subblock;
	...
		BlockProcessor(std::string deltaserveraddr) :
			    ...
				syncserv{ AF_SP, NN_REQ },
				syncradio{ std::make_pair(Sender(syncserv), Receiver(syncserv)) } ,
				subblock{ AF_SP, NN_SUB },
				rec_block{ Receiver(subblock) } 
		...
			syncservid = syncserv.connect("inproc://syncserv");
			subblock.setsockopt(NN_SUB, NN_SUB_SUBSCRIBE, "", 0);
			subblock.connect("inproc://pubblock");

_BlockProcessor::run_ first checks if the blockchain is in sync (_isInSync_), and will get in sync if not (_GetInSync_). It then receives blocks as a subscriber to **_"pubblock"_** (see [_Node::runLive::blockslivepub.bind()_](https://github.com/protoblock/fantasybit-2014/blob/windows/src/Node.cpp#L571) ) , and finally calls process() to process the block and update state in _BlockProcessor::process()_

	void run()
		...
		while (running && !isInSync())
			GetInSync(lastidprocessed + 1, realHeight);
		...
		while (running)
			if (!rec_block.receive(sb)) continue;

			if (sb.signedhead().head().num() > lastidprocessed + 1)
				GetInSync(lastidprocessed + 1, sb.signedhead().head().num());
			
			if (sb.signedhead().head().num() == lastidprocessed + 1)
				process(sb);

_BlockProcessor::isInSync_ sends a request to get block_HEIGHT from **_"inproc://syncserv"_** , which is waiting from the bind call seen above in _Node::syncService()_. It then calls receive to get the reponse of the request, which is also sent from _Node::syncService()_

	bool isInSync()
		...
		nrq.set_type(NodeRequest_Type_HIGHT_REQUEST);
		...
		syncradio.first.send(nrq);
		...
		syncradio.second.receive(nrp)
		...
		realHeight = nrp.hight();
     	return (realHeight == lastidprocessed);

_BlockProcessor::GetInSync_ sends a request to get a BLOCK from **_"inproc://syncserv"_**, which is waiting from the bind call seen above in _Node::syncService()_. It then calls receive to get the Block from reponse of the request, which is also sent from _Node::syncService()_, the block is then processed with _BlockProcessor::process()_

	void GetInSync(int start,int end)
		...
		while (true)
		...
			nrq.set_type(NodeRequest_Type_BLOCK_REQUEST);
			nrq.set_num(lastid);
			syncradio.first.send(nrq);
			Block sb{};
			if (!syncradio.second.receive(sb)) 
				...
				break;
		    ...
			if (sb.signedhead().head().num() != lastid) 
			    ...
				break;

			if (!process(sb)) 
			    ...
				break;

			if (end == lastid) 
			    break;
			lastid++;


#### Pre-Design Notes
A collection of notes on p2p network design from other blockchain projects. 

[From EOS project](https://github.com/EOSIO/eos/blob/8e723fda01d3e5ab49a4edeec7898a290ef13476/plugins/net_plugin/include/eosio/net_plugin/protocol.hpp)

````/**
 *
Goals of Network Code
1. low latency to minimize missed blocks and potentially reduce block interval
2. minimize redundant data between blocks and transactions.
3. enable rapid sync of a new node
4. update to new boost / fc
State:
   All nodes know which blocks and transactions they have
   All nodes know which blocks and transactions their peers have
   A node knows which blocks and transactions it has requested
   All nodes know when they learned of a transaction
   send hello message
   write loop (true)
      if peer knows the last irreversible block {
         if peer does not know you know a block or transactions
            send the ids you know (so they don't send it to you)
            yield continue
         if peer does not know about a block
            send transactions in block peer doesn't know then send block summary
            yield continue
         if peer does not know about new public endpoints that you have verified
            relay new endpoints to peer
            yield continue
         if peer does not know about transactions
            sends the oldest transactions that is not known by the remote peer
            yield continue
         wait for new validated block, transaction, or peer signal from network fiber
      } else {
         we assume peer is in sync mode in which case it is operating on a
         request / response basis
         wait for notice of sync from the read loop
      }
    read loop
      if hello message
         verify that peers Last Ir Block is in our state or disconnect, they are on fork
         verify peer network protocol
      if notice message update list of transactions known by remote peer
      if trx message then insert into global state as unvalidated
      if blk summary message then insert into global state *if* we know of all dependent transactions
         else close connection
    if my head block < the LIB of a peer and my head block age > block interval * round_size/2 then
    enter sync mode...
        divide the block numbers you need to fetch among peers and send fetch request
        if peer does not respond to request in a timely manner then make request to another peer
        ensure that there is a constant queue of requests in flight and everytime a request is filled
        send of another request.
     Once you have caught up to all peers, notify all peers of your head block so they know that you
     know the LIB and will start sending you real time transactions
parallel fetches, request in groups
only relay transactions to peers if we don't already know about it.
send a notification rather than a transaction if the txn is > 3mtu size.
*/
````

[Satoshi Client Operations from Bitcointalk](https://bitcointalk.org/index.php?topic=41718.0)
````
The client is oriented around several major operations, including: 

    Initialization and Startup
        Upon startup, the client performs various initilization routines
        including starting multiple threads to handle concurrent operations.

    Node Discovery
        The client uses various techniques find out about other bitcoin
        nodes that may exist.

    Node Connectivity
        The client initiates and maintains connections to other nodes.

    Sockets and Messages
        The client processes messages from other nodes and sends
        messages to other nodes using socket connections.
    
    Block Exchange
        Nodes advertise their inventory of blocks to each other and
        exchange blocks to build block chains.

    Transaction Exchange
        Nodes exchange and relay transactions with each other.
        The client associates transactions with bitcoin addresses in the
        local wallet.

    Wallet Services
        The client can create transactions using the local wallet.
        The client associates transactions with bitcoin addresses in the
        local wallet. The client provides a service for managing
        the local wallet.

    RPC Interface
        The client offers an JSON-RPC interface over HTTP over sockets
        to perform various operational functions and to manage the local
        wallet.

    User Interface
        The user interface code is scheduled to be superseded by bitcoin-qt.
        Therefore, it is not covered in further detail.
````

[Tendermint Peers Notes](https://github.com/tendermint/tendermint/blob/master/docs/specification/new-spec/p2p/peer.md)

[Tendermint Peer Strategy and Exchange](https://github.com/tendermint/tendermint/blob/master/docs/specification/new-spec/reactors/pex/pex.md)

[From Graphene](https://github.com/cryptonomex/graphene/blob/master/libraries/p2p/design.md)

    Each node implements the following protocol:

    onReceiveTransaction( from_peer, transaction )
        if( isKnown( transaction.id() ) ) 
            return

        markKnown( transaction.id() )

        if( !validate( transaction ) ) 
           return

        for( peer : peers )
          if( peer != from_peer )
             send( peer, transaction )


    onReceiveBlock( from_peer, block_summary )
        if( isKnown( block_summary ) 
            return

        full_block = reconstructFullBlcok( from_peer, block_summary )
        if( !full_block ) disconnect from_peer 

        markKnown( block_summary )

        if( !pushBlock( full_block ) ) disconnect from_peer 

        for( peer : peers )
           if( peer != from_peer )
             send( peer, block_summary )
             

     onHello( new_peer, new_peer_head_block_num )

        replyHello( new_peer ) // ack the hello message with our timestamp to measure latency

        if( peers.size() >= max_peers )
           send( new_peer, peers )
           disconnect( new_peer )
           return
          
        while( new_peer_head_block_num < our_head_block_num )
           sendFullBlock( new_peer, ++new_peer_head_block_num )

        new_peer.synced = true
        for( peer : peers )
            send( peer, new_peer )

     onHelloReply( from_peer, hello_reply )
         update_latency_measure, disconnect if too slow
    
     onReceivePeers( from_peer, peers )
        addToPotentialPeers( peers )

     onUpdateConnectionsTimer
        if( peers.size() < desired_peers )
          connect( random_potential_peer )

     onFullBlock( from_peer, full_block )
        if( !pushBlock( full_block ) ) disconnect from_peer 

     onStartup
        init_potential_peers from config
        start onUpdateConnectionsTimer
     



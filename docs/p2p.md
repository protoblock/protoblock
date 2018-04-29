Peer to peer - design notes 
===========================

Protoblock is a network of peers. Clients send and receive transactions from peers. Network is a distributed mesh. Data is flood filled, where everyone send everyone everything! 

### p2p operations
* Node discovery 

* Peer chain comparison

* Blockchain Sync  

* Handshake

* Mempool sync 

* Transaction gossip 

* Block gossip

* Peer list gossip 

#### Pre-Design Notes
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



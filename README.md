#Satoshi Fantasy

##Fantasy Name
Fantasy Names are mined into existence in a separate proof-of-work blockchain similar to bitnames for [BitShares](https://github.com/InvictusInnovations/BitShares). A fantasy name is an alias to your public key that corresponds to your private "wallet" key. 

##Fantasy Bits

Fantasy Bits are tokens that get created based on NFL player events.  Each fantsybit is owned by a single Fantasy Name. Fantsybits can be transferred by transactions in the underlying Proof-of-skill block chain.    

Fantsybits are awarded based on:

1. weekly fantasy point projections 
2. player meta-data projections
3. stats and data contributors.
4. transactions fees for block forging

###Proof-of-skill
Proof-of-skill is essentially a [Proof-of-stake](http://en.wikipedia.org/wiki/Proof-of-stake) system, where the stakes are earned based on the skill of projecting weekly fantasy football results. The main downside in other proof-of-stake networks, such as [NXT](http://wiki.nxtcrypto.org/wiki/Whitepaper:Nxt#Proof-of-Stake), is that all tokens must be pre-distributed, creating an unfair advantage to early stake holders. Fantasybits are created by NFL players playing football, so there is no pre-distribution needed.  


###Fantasy point projections
100 fantasybits are generated for each real fantasy point earned by an NFL player during the fantasy season. A generation and distribution algorithm determines the number of bits to generate and the names to distribute them to.

The algorithm awards more bits to those with projections that were closest to actual results. 

####[Distribution Algorithm](https://github.com/jaybny/fantasybit/blob/master/src/DistributionAlgo.cpp) 
1. Calculate average difference of all projections from actual result. 
2. Filter out projections that are below average or have a difference > result.
3. unitpayout = result / (sum of (result-projection))
4. award = 100 * (result - difference) * unitpayout 


###Consensus 
All data in the network, such as fantasy point results, will be determined by the consensus. At least 51% of total Stake Values must agree before data can be included in block. This is done by recursively signing the data payload. 

####Stake Value
Each fantasy name will have four different Stake Values

1. Skill - total historical bits earned from projections.
2. Stake - total bits owned. 
2. Data-feed - sum of Skill of all fantasy names that designate you as their feed provider.
3. Time-sync - sum of Stake of all fantasy names that designate you as their sync provider.  


##State Machine
System has a built in state machine, and will act differently depending on its current state. A new block with specific data can trigger a transition to a new state. 

1. **Season**
 	- **PreGame**(week[1-16])  
	- **InGame**(week[1-16]) 
	- **TradingSession** - Wed 6pm - 11:59pm EDT 
4. **PreDraft** 
5. **PreSeason**
6. **53ManRoster**

##BlockChain
Satoshi Fantasy uses a Proof-of-Stake(type) system to determine who can mine the next block. Special state transition blocks require 51% Consensus from Data-feed or Time-sync stake values 

###Transactions

Type | Description
---- | -----------
xxxx | xxxxxxxx

State transition transactions must be signed by 51% Data-feed-Stake consensus, and any block containing these transactions is Proof-of-Skill or Proof-of-Data-feed.

TradingSession state transition transactions must be signed by 51% Time-sync-Stake consensus, and any block containing these transactions is Proof-of-Stake or Proof-of-Time-sync




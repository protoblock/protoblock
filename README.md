#Fantasy Bits

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
All data in the network, such as fantasy point results, will be determined by the consensus. At least 51% of total fantasybits must agree before data can be included in block. This is done by recursively signing the data payload. 

##Fantasy Name
Fantasy Names are mined into existence in a separate proof-of-work blockchain similar to bitnames for [BitShares](https://github.com/InvictusInnovations/BitShares). A fantasy name is an alias to your public key that corresponds to your private "wallet" key. 



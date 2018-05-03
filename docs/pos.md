Proof-of-Skill Consensus protocol design notes 
==============================================

##Theory

### Emergent Centralization 
In decentralize systems, centralization tends to eventually emerge due to the efficiency gains. In Bitcoin, mining has centralization issues around ASIC producers and mining pools.  

By explicitly choosing centralization points, when designing new blockchains, you alleviate the pressure of natural future centralization points

**Protoblock uses NFL statistics within the blockchain. Many will call this centralized data, however each week millions of Americans come to distributed consensus on these results, and billions of dollars are settled without any central authority. **

### Blockchain Consensus 
*Theorem1:* distributed consensus can be reached when these two properties are met:

1. a random process is known, but the result is random and not yet known  
2. once the result is revealed it can be independently identified by each node

 blockchain | Random | Result Verifiable
------------| ------ | -------------
bitcoin | block nonce | sha256 hash
protoblock | NFL stats| watch and calculate

### Proof-of-Stake 

Using coin stake to determine the next block signer has a major issue of "stake grinding" or "long range attacks" [..][2]

A solution was eluded to by Vitalik Buterin. 
```` nevertheless the existence of the algorithm is encouraging because it suggests that long-range-nothing-at-stake might be fundamentally resolvable ```` [..][2]

*Theorem2:* Distributed Consensus can reached via “proof-of-stake” if the following conditions are met:

1. Stake is static and is immutably tied to a specific public-key
2. Block signer selection algorithm is independent of the contents of any block data.
3. The set of public-keys in the selection algorithm was chosen with a decentralized protocol.  
 
This solution has been described with various names. 

* "Proof-of-Skill" 
* "Virtual Perpetual Mining Rig" 
* ["Bergstake"](https://bitcointalk.org/index.php?topic=572454.msg6241082#msg6241082) 
* "Proof-of-Proof-of-Work" 


*Even Bitcoin can switch to POS, with Proof-of-POW*

````The idea, described by others before, is to use POW from previous blocks to build a virtual mining rig. Miners who solved POW would secure the private-keys that controls the outputs from the coinbase transaction in each block. The set of all public-keys from coinbase outputs is used as the “Stake” in POS. Even when the coinbase coins are transferred, the “Stake” keys do not change. ````


 blockchain | stake | decentralized process
------------| ------ | -------------
bitcoin | coinbase tx | POW
breakoutcoin | bergstake | ICO 
protoblock | FantasySkill| Projections 

### Proof-Of-Skill 
````Intuitively, it seems impossible to obtain distributed consensus without provably consuming some resource outside of the system ```` [..][1]

Proof-of-Skill uses the external resource of time and effort needed to obtain skill of predicting football stats. 



#### References

A Treatise on Altcoins, Andrew Poelstra
<https://download.wpsoftware.net/bitcoin/alts.pdf>

On Stake - Vitalik Buterin, July 5th, 2014.
<https://blog.ethereum.org/2014/07/05/stake/>

Bergstake – distributed consensus, jaybny
<https://bitcointalk.org/index.php?topic=572454.msg6241082#msg6241082>

Distributed Engineered Autonomous Agents : Satoshi Fantasy - Jay Berg
<http://protoblock.com/Protoblock_Original_Whitepaper-SatoshiFantasy.pdf>

[1]: https://download.wpsoftware.net/bitcoin/alts.pdf
[2]: https://blog.ethereum.org/2014/07/05/stake/



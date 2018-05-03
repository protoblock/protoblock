Proof-of-Skill Consensus protocol design notes 
==============================================

## Theory

### Emergent Centralization 
In decentralize systems, centralization tends to eventually emerge due to the efficiency gains. In Bitcoin, mining has centralized around ASIC producers and mining pools.  

By explicitly choosing centralization points, when designing new blockchains, you alleviate the pressure of natural future centralization points

**Protoblock uses NFL statistics within the blockchain. Many will call this centralized data, however each week millions of Americans come to distributed consensus on these results, and billions of dollars are settled without any central authority.**

### Blockchain Consensus 
*Theorem1:* distributed consensus can be reached when these two properties are met:

1. A random process is known, but the result is random and not yet known  
2. Once the result is revealed it can be independently identified by each node

 blockchain | Random | Result Verifiable
------------| ------ | -------------
bitcoin | block nonce | sha256 hash
protoblock | NFL stats| watch and calculate

### Proof-of-Stake 

Using coin stake to determine the next block signer has a major issue of "stake grinding" or "long range attacks" [..][2]

A solution was alluded to by Vitalik Buterin. 
```` nevertheless the existence of the algorithm is encouraging because it suggests that long-range-nothing-at-stake might be fundamentally resolvable ```` [..][2]


~~*Theorem2:* Distributed Consensus can reached via “proof-of-stake” if the following conditions are met:~~

~~1. Stake is static and is immutably tied to a specific public-key~~
~~2. Block signer selection algorithm is independent of the contents of any block data~~
~~3. The set of public-keys in the selection algorithm was chosen with a decentralized protocol~~
 
~~This solution has been described with various names.~~

 ~~*"Proof-of-Skill"~~
 ~~*"Virtual Perpetual Mining Rig"~~
 ~~*["Bergstake"](https://bitcointalk.org/index.php?topic=572454.msg6241082#msg6241082)~~
 ~~*"Proof-of-Proof-of-Work"~~ 

~~*Even Bitcoin can switch to POS, with Proof-of-POW*~~

~~_The idea, described by others before, is to use POW from previous blocks to build a virtual mining rig. Miners who solved POW would secure the private-keys that controls the outputs from the coinbase transaction in each block. The set of all public-keys from coinbase outputs is used as the “Stake” in POS. Even when the coinbase coins are transferred, the “Stake” keys do not change._~~


~~blockchain|static stake|decentralized process~~
~~----------|------------|---------------------~~
~~bitcoin|coinbase tx|POW~~
~~breakoutcoin|bergstake|ICO~~
~~protoblock|FantasySkill|Projections~~

### Proof-Of-Skill 
_"Intuitively, it seems impossible to obtain distributed consensus without provably consuming some resource outside of the system"_ [..][1]

Proof-of-Skill uses the external resource of "time and effort" needed to obtain the skill of predicting football stats. 

* *"Nothing-At-Stake"* solved with slasher  
* *"Stake Grinding"* solved with static FantasySkill 

## Implementation 

Proof-of-Skill borrows from NXT Proof-of-Stake "Forging"  

* [wiki](http://nxtwiki.org/wiki/Forging)
* [code](https://github.com/Blackcomb/nxt/blob/master/src/java/nxt/Generator.java)


**The POSkill algorithm revolves around two variables: Hit and Target.**

Each account generates one 'hit' value between 0 and 2^64 - 1 for each new block. While hit is deterministic, it's produced through hashing previous blocks generating signature you’re your public_key, so it can be regarded as a uniform random variable.

From the code, hit is defined as:

````
int Hit = getHit(generator_public_key, generating_signature);
````

The getHit function, takes the users public key and the previous blocks generating_signature. 

````
int64 getHit(string generator_public_key, string generating_signature) {
    
    generating_signature = hashit(generating_signature + generator_public_key);
    
    int64 hit;
    memcpy(&hit, *generating_signature, sizeof hit);
    
    return hit;
}
````

The target is realtive to your skill balance and increases with each second.  
````
Target = BaseTarget * EffectiveSkillBalance * TimeSinceLastBlock
````

Where:

* _BaseTarget_ - This parameter is the same for everybody. It gets adjusted from block to block to obtain, roughly, a generation time between blocks of 40s. If two successive blocks get generated too quickly, the new BaseTarget gets reduced. If instead they take too long, the value gets increased. This adjustment is capped, it can go up at most twice as much (2 * currentBaseTarget) and down at most by half (0.5 * currentBaseTarget).

* _EffectiveSkillBalance_ - This is your FantasySkill balance, as of the end of previous NFL week or season. 

* _TimeSinceLastBlock_ - How long, in seconds, since the last block has been generated. This parameter is also the same for everybody.

To determine if you get sign the next block: 

````
if ( Hit < Target )
   Generate and Sign New Block 
````

Each new block header has: 

* the block signers public key (generator_public_key)
* the generating signature 
* base target
* time stamp

````
    BlockHeader {
	    bytes generator_public_key
	    bytes generating_signature //hash(prev_generating_signature + generator_public_key);
	    int32 basetarget
	    int32 timestamp
	    ...
	}
````

Each generating_signature is the hash of block signers publick_key and the previous generating signature. The Hit is the first 8 bytes of the generating_signature, so the main data that determines the next block signer is the previous block signer, your keys, your balance and time in seconds.  

###  Longest Chain

Competing Blockchain forks are measured based on their cummulativeDifficulty.  

````cummulativeDifficulty = sum(1/baseTarget)````

baseTarget is set to
````baseTarget = previous_baseTarget * TimeSinceLastBlock / 40````

This may open a "tail switching" attack, which comes from N@S (nothing-at-stake) that is, within the long-run, an arbitrary chain can win the competition in term of the _Block Measure Function_, which leads to very long history re-writes. [..][3]  

### Forks
When two block signers near simultaneously win the right to sign the next block, a natural fork will occur. 

If signerA wins at time T and signerB wins at time T+1, then what if nodeA gets signerB's block before signerA's block? Does nodeA build on the first block it received (signerB), or the stronger of the two (signerA)? If the signerB knows that signerA is active and live on the network, then maybe signerA should not even create/sign/send a block? This is knows as ["Transparent Forging"](https://nxtwiki.org/wiki/Transparent_Forging) in NXT. 

However, transparent forging requires nodes that are "unlocked" to sign blocks when it’s their turn, otherwise they temporarily lose their right to sign. This solves "Selfish mining" and reduces "nothing at stake" incentive. The downside is, signers lose their anonymity, and can be traced to an IP address, which open up DDOS and other attack vectors. 

_Transparent Forging_ idea is that an account will decide to either generate a new block or skip their turn for the benfit of the network. This works because accounts are able to predict when a different forger will sign this block at a technically early timestamp. [..][6]

_Multibranch Forging_ is an alternate approuch - "If you can generate - do it and tell others". So every since node will generate a sign every block, and each node will create a local _Blocktree_ a tree of blockchains, and then they will all come to consensus on the stringest chain with the BlockMeasureFunction. Blocktree turn N@S problem upside down, where eveyone explicitly works on all forks by design. [..][4]

### Data Agents 
To solve N@S and grinding issues, NXT and other POS systems use developer checkpoints and/or time delay rules, where your stake can only be used 1000 blocks in the future. Other bockchains such as EOS have rules as to how many blocks back a fork can occur. 

Protoblock has special _dataTransitions_ events related to NFL data and stats, where _DataAgents_ will sign blocks with consensus of a majority of the network. These events create natural checkpoints, as these trnsaction have a reference to a recent block, and these events have consensus of the network. This will mitigate these POS attack vectors. [..][5] 

## ToDo
* Simulate POS for natural forks 
* Test _Block Measure Function_ 

#### References
Bergstake <https://drive.google.com/file/d/0B4APw_r5QuqTOWJhWDlOOENlaEk/view?usp=sharing>

A Treatise on Altcoins, Andrew Poelstra
<https://download.wpsoftware.net/bitcoin/alts.pdf>

On Stake - Vitalik Buterin, July 5th, 2014.
<https://blog.ethereum.org/2014/07/05/stake/>

Bergstake – distributed consensus, jaybny
<https://bitcointalk.org/index.php?topic=572454.msg6241082#msg6241082>

Distributed Engineered Autonomous Agents : Satoshi Fantasy - Jay Berg
[Protoblock Original Whitepaper SatoshiFantasy.pdf](http://protoblock.com/Protoblock_Original_Whitepaper-SatoshiFantasy.pdf)

Multibranch forging algorithms: tailsswitching eﬀect and chain measures - andruiman
<https://www.scribd.com/document/256073121/Multibranch-forging-algorithms-tails-switching-effect-and-chain-measures>

PoS forging algorithms: multi-strategy forgingand related security issues - andruiman
<https://www.scribd.com/document/256072839/PoS-forging-algorithms-multi-strategy-forging-and-related-security-issues>

[1]: https://download.wpsoftware.net/bitcoin/alts.pdf
[2]: https://blog.ethereum.org/2014/07/05/stake/
[3]: https://www.scribd.com/document/256073121/Multibranch-forging-algorithms-tails-switching-effect-and-chain-measures
[4]: https://www.scribd.com/document/256072839/PoS-forging-algorithms-multi-strategy-forging-and-related-security-issues
[5]: http://protoblock.com/Protoblock_Original_Whitepaper-SatoshiFantasy.pdf
[6]: https://nxtwiki.org/wiki/Transparent_Forging

<https://github.com/ConsensusResearch/articles-papers/blob/master/articles/inside-pos-2.md>

https://www.jelurida.com/sites/default/files/NxtForging.pdf




Fantasybit Accounts (not UTXO)  
==============================

## Account vs UTXO
Fantasybits balance are assigned to an "Account". This is different that bitcoin, which uses UTXO.

UTXO creates a history of each coin as it transfers from wallet to wallet.  

Accounts keep a balance of each account and credit/debit accont for each transfer transaction. 

### Replay Issues  
Account based systems have an issue with replay. What if someone recycles an old transaction? With UTXOs this is not a problem, beacuse the old transaction inputs ar eno longer valid. 

Etherum solves this with a nonce. Each transaction for each account must inciment the nonce by 1. Howver this causes various problems, when ansyncounous processing in wallets are trying to create trsnactions on the same account at the same time.  

Fantasybit uses a timeout solution similar to bitshares/steem/eos. Where a transaction is only valid for 48 hours, and each transaction must be unique. So the soution is to keep the last 48 hours of transactions in memory to check for duplicates, otherwise the transaction is invalid due to timeouts. 

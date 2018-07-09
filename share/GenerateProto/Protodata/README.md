# Protodata
Wire Format for Fantasybit Coin Blockchain 

Google Protobuf Version 2

```
message Transaction {
    optional int32 version = 1;
    optional TransType type = 2;

    extensions 100 to max;
}

message SignedTransaction {
    optional Transaction trans = 10;
    optional bytes id = 20;                 // to_hex ( google::protobuf::SerializeAsString( Transaction{} ) 
    optional bytes sig = 30;                // secp256k1_ecdsa_sign ( id ) 
    optional string fantasy_name = 40;
}
```

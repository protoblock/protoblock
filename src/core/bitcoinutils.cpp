/*
//  bitcoinutils.cpp.h
//
//  Protoblock
//
//  Created by Jay Berg on 11/17/2018
//
*/
#include "bitcoinutils.h"
#include <utils/utils.h>
//Input 1 script length: 0x17 (23 bytes)
//Input 1 signature script: 16001427c106013c0042da165c082b3870c31fb3ab4683

namespace pb {

fc::optional<Bitcoin_UTXO> BitcoinUtils::getUtxo(const std::string &btcaddress, uint64_t max, uint64_t min) {

    fc::optional<Bitcoin_UTXO> ret;
    utxoData use;
    uint64_t diff = max+1;
    for ( const auto &utxo : BitcoinApi::getUtxo(btcaddress) ) {
        if ( utxo.in_value < min || utxo.in_value > max)
            continue;

        if ( diff > (max - utxo.in_value) ) {
            diff = max - utxo.in_value;
            use = utxo;
        }
    }

    if ( diff < max ) { //found it
        ret = Bitcoin_UTXO{};
        ret->set_txid(use.tx_hash.toStdString());
        ret->set_tx_output_n(use.tx_output_n);
        ret->set_locking_script(use.script.toStdString());
        ret->set_in_value(use.in_value);
    }

    return ret;
}

std::string BitcoinUtils::createInputsFromUTXO(const Bitcoin_UTXO &iutxo,
                                               std::string &input, std::string &in_script) {

    input += iutxo.txid();
    auto reversetxoun = pb::toReverseHexFromDecimal (iutxo.tx_output_n());
    input += reversetxoun;

    {
        auto ssize = static_cast<unsigned char>( iutxo.locking_script().size() / 2 );
        auto sstr = pb::to_hex ( &ssize, sizeof( unsigned char ) );
        in_script = sstr + iutxo.locking_script();
    }

    return "";
}
std::string BitcoinUtils::createTX(const Bitcoin_UTXO &iutxo,
                                   std::string &input, std::string &in_script) {
    std::string OP_DUP = "76";
    std::string OP_HASH160 = "a9";
    std::string OP_EQUALVERIFY = "88";
    std::string OP_CHECKSIG = "ac";
    std::string OP_RETURN = "6a";
    std::string OP_EQUAL = "87";
    std::string sighash_all = "01000000";


    uint64_t amount = iutxo.in_value();
    pb::public_key_data publick_key;
    std::string to_address = "1CKo57EJuBjJ1Sdioqed4mLrWGoKGn4dzS";

    int numinputs = 1;
    uint64_t satoshifee = 5000;

    //output
    std::string raw_transaction_out;
    int numoutputs = 1;
    {
        auto size = ( unsigned char )numoutputs;
        auto sstr = pb::to_hex ( &size, sizeof( unsigned char ) );
        raw_transaction_out += sstr;
    }
    auto amstr = pb::toReverseHexFromDecimal(uint64_t(amount - satoshifee));
    raw_transaction_out += amstr;


    //locking script
    std::string p2pkh  = OP_DUP + OP_HASH160;
    std::string pubkeyHash = pb::fromBtcAddress (to_address);
    {
        auto size = ( unsigned char )( pubkeyHash.size( ) / 2 );
        auto sstr = pb::to_hex ( &size, sizeof( unsigned char ) );
        p2pkh += sstr;
    }
    p2pkh += pubkeyHash;
    p2pkh += OP_EQUALVERIFY + OP_CHECKSIG;

    {
        auto size = ( unsigned char )( p2pkh.size( ) / 2 );
        auto sstr = pb::to_hex ( &size, sizeof( unsigned char ) );
        raw_transaction_out += sstr;
    }
    raw_transaction_out += p2pkh;

    std::string locktime = "00000000";
    raw_transaction_out += locktime;


    //to_sign
    std::string to_sign = input + in_script;
    to_sign +=  "ffffffff";
    to_sign += raw_transaction_out;
    to_sign += sighash_all;



    //create tx
    std::string raw_transaction_pre = "01000000"; // i.e. version
    {
        auto size = ( unsigned char )numinputs;
        auto sstr = pb::to_hex ( &size, sizeof( unsigned char ) );
        raw_transaction_pre += sstr;
    }
    /**/
    return raw_transaction_pre + to_sign;



}

}


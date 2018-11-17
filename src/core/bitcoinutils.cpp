/*
//  bitcoinutils.cpp.h
//
//  Protoblock
//
//  Created by Jay Berg on 11/17/2018
//
*/
#include "bitcoinutils.h"

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

}

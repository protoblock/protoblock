/*
//  bitcoinutils.h.h
//
//  Protoblock
//
//  Created by Jay Berg on 11/17/2018
//
*/
#ifndef BITCOINUTILS_H
#define BITCOINUTILS_H
#include <string>

#include <QString>
#include <vector>
//#include <RestfullService.h>
//#include <utils.h>
#include <QByteArray>
#include <ProtoData.pb.h>
#include <bitcoinapi.h>
#include <optional.hpp>



namespace pb {
using namespace fantasybit;

class BitcoinUtils
{
public:

    static bool getUtxos(Bitcoin_UTXOS &utxos, const std::string &btcaddress, uint64_t max, uint64_t min);

    static fc::optional<Bitcoin_UTXO> getUtxo(const std::string &btcaddress,
                     uint64_t max = 999000000000,
                     uint64_t min = 200);



    static std::string createTX(const Bitcoin_UTXO &iutxo,
                         std::string &input, std::string &in_script);

    static std::string createInputsFromUTXO(const Bitcoin_UTXO &iutxo,
                                     std::string &input, std::string &in_script);

    static uint64_t getBTCbalance(const std::string &btcaddress) {
        return BitcoinRestfullService::getBtcAddressBalance(btcaddress.data());
    }

    static uint64_t getBTCbalance(const QString &btcaddress) {
        return BitcoinRestfullService::getBtcAddressBalance(btcaddress);
    }


};

}
#endif // BITCOINUTILS_H










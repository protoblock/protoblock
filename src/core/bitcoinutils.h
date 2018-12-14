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

const std::string SIGHASH_ALL_1 = "01";
const std::string SIGHASH_ALL_4 = "01000000";

class BitcoinUtils
{
public:

    static bool getUtxos(Bitcoin_UTXOS &utxos, const std::string &btcaddress, uint64_t max, uint64_t min);

    static fc::optional<Bitcoin_UTXO> getUtxo(const std::string &btcaddress,
                     uint64_t max = 999000000000,
                     uint64_t min = 200);

    static int checkUtxo(const Bitcoin_UTXO &iutxo, const std::string &btcaddress);

    static std::string createTX(const Bitcoin_UTXO &iutxo,
                                       const std::string &input,
                                       const std::string &in_script,
                                       const std::string &to_address,
                                       uint64_t amount,
                                       uint64_t btyefee,
                                       const std::string &change_address,
                                    std::string &pre_script,
                                    std::string &post_script);

    static void createInputsFromUTXO(const Bitcoin_UTXO &iutxo,
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










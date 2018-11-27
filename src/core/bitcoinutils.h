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

    fc::optional<Bitcoin_UTXO> getUtxo(const std::string &btcaddress,
                     uint64_t max = 999000000000,
                     uint64_t min = 200);



    std::string createTX(const Bitcoin_UTXO &iutxo,
                         std::string &input, std::string &in_script);

    std::string createInputsFromUTXO(const Bitcoin_UTXO &iutxo,
                                     std::string &input, std::string &in_script);

    uint64_t getBTCbalance(const std::string &btcaddress);

};

}
#endif // BITCOINUTILS_H










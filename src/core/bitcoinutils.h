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
#include <QDataStream>


namespace pb {
using namespace fantasybit;

const std::string SIGHASH_ALL_1 = "01";
const std::string SIGHASH_ALL_4 = "01000000";

const std::string VERSION_SEGWIT = "02000000";
const std::string VERSION_NORMAL = "01000000";

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

    static void parseRawTx(const QByteArray &rawtx) {
        QByteArray data;
        QDataStream stream(rawtx);
//        std::string version;
        int size = VERSION_NORMAL.size();
        std::vector<char> version(size+1);
        size = stream.readRawData(&version[0],size);
        version[size] = '\0';

        if ( strcmp (version.data(),VERSION_NORMAL.c_str()) == 0)
            qDebug() << "jayversion yes VERSION_NORMAL ";
        else
            qDebug() << "jayversion no VERSION_NORMAL ";

//        version.resize(11);
//        stream.readRawData(&version[0],10);
//        version[10] = '\0';
//        qDebug() << version;

        uint8_t numinputs = 0;
        {
            std::vector<char> vsz(3);
            stream.readRawData(&vsz[0],2);
            vsz[2] = '\0';
            std::string sz(&vsz[0]);
            pb::from_hex(sz,(char *)&numinputs,sizeof( uint8_t ));
            qDebug() << sz.data() << "jay size" << numinputs << "size jay";
        }

        for ( int i =0; i < numinputs; i++) {
            std::vector<char> prevhash_reverse(33);
            stream.readRawData(&prevhash_reverse[0],32);
            prevhash_reverse[32] = '\0';



        }


    }

    /*
     *
    01000000
    01
    0123449fad6289dda5365a197fa822e320cdfc106bed243bf773cac64cfdb237
    05000000
    6a
    1333183ddf384da83ed49296136c70d206ad2b19331bf25d390e69b222165e37
    473044022036be6403aeb4e0e6fd54720b328d9d81bea32fb79684da0288743668fb5ef3ee02202023a71ef7217061fb9b4f35a05143de71447032e5a35b39c3d14b3210bad10b0121032725846bb7bc2e47b7b5a50670d77c8268f4d7f3243bdcf1b22174a67faaf528
    feffffff
    0200213900000000001976a914659042e01e864e2f29641ea3a213c51a956d33c788ac288c0f00000000001976a9144fc238bcda3f884ff6ce8d9feeb89b50dfd3da8888ac2c480700
 *
    const MyKey32 Transaction::getDigest ( const int n, const QByteArray& scr ) const
    {
      MyByteArray data;                                 // create empty array
      MyStream stream ( s );                            // source transaction is a stream
      data.putInt32 ( stream.readU32 ( ) );             // version
      data.putVarInt ( stream.readVar ( ) );            // input count
      for ( int i ( 0 ); i < inputs; i++ )              // copy all inputs
      {
        data.putArray ( stream.readAdvance ( 36 ), 36 );// copy 32 byte hash as is + copy 4 bytes index
        data.nop ( stream.skipVarData ( ) );            // skip original script and do nothing
        data.putPrefixedCond ( i ^ n, scr );            // script replacement: empty or given in param
        data.putInt32 ( stream.readU32 ( ) );           // sequence
      }
      data.putVarInt ( stream.readVar ( ) );            // output count
      for ( int i ( 0 ); i < outputs; i++ )             // copy all outputs byte-by-byte
      {
        data.putInt64 ( stream.readU64 ( ) );
        data.putPrefixed ( stream.readVarData ( ) );
      }
      return data
          .putInt32 ( stream.readU32 ( ) )              // lock
          .putInt32 ( SIGHASH_ALL )                     // append hashcode
          .sha256d ( );                                 // double-sha256
    }
    */

};

}
#endif // BITCOINUTILS_H










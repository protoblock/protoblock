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

struct var_data {
    std::string size;
    std::string data;
    operator std::string() const {
        return size + data;
    }

};

struct btx_input {
    std::string rtxid; //64
    std::string txindex; //8
    var_data script;
    std::string sequence; //8

    operator std::string() const {
        return rtxid + txindex + std::string(script) + sequence;
    }
};

struct btx_output {
    std::string amount; //16 - reverse hex
    var_data script;
    operator std::string() const {
        return amount + std::string(script);
    }

};

struct bitcoin_hex_tx {
    std::string version; //8
    std::string input_count; // var
    std::vector<btx_input> inputs;
    std::string output_count; // var
    std::vector<btx_output> outputs;
    std::string locktime; //8 - reverse hex
};

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

    static bool createProofOfUtxoSpend(const bitcoin_hex_tx &ref,
                                       const Bitcoin_UTXO &spent,
                                       std::string &pre, std::string &post,
                                       std::string &sigscript) {
        pre = ref.version + ref.input_count;

        bool seen = false;
        for ( auto &r : ref.inputs ) {
            if ( r.rtxid == spent.txid () &&
                 getInt8(r.txindex) == spent.tx_output_n() ) {
                if ( seen ) {
                    qCritical() << " bad seen";
                    return false;
                }

                std::string input, script;
                createInputsFromUTXO(spent,input,script);

                //ToDo: verify
                if ( input+script+r.sequence != std::string(r)) {
                    qCritical() << " bad " << std::string(r).data ();
                }
                //pre += r.rtxid + r.txindex + r.script;


                seen = true;
                post = r.sequence;
            }
            else {
                btx_input bin = r;
                bin.script.size = "00";
                bin.script.data = "";

                if ( !seen )
                    pre += bin;
                else
                    post += bin;
            }
        }

        return seen;
    }

    static bitcoin_hex_tx parseRawHexTx(const QByteArray &rawtx);

    static std::string toRawHexTx(const bitcoin_hex_tx &tx);

    template<int T>
    static std::string getStr(QDataStream &stream) {
        char c[T+1];
        stream.readRawData(&c[0],T);
        c[T] = '\0';
        return c;
    }

    static std::string getStr(uint size, QDataStream &stream) {
        std::vector<char> c(size+1);
        stream.readRawData(&c[0],size);
        c[size] = '\0';
        return c.data();
    }

    static var_data getVarStr(QDataStream &stream) {
        var_data vd;
        vd.size = getStr<2>(stream);

        uint8_t len;
        pb::from_hex(vd.size,(char *)&len,sizeof( uint8_t ));

        vd.data = getStr(len*2,stream);
        return vd;
    }

    static uint8_t getInt8(const std::string &in) {
        uint8_t num = 0;
        pb::from_hex(in,(char *)&num,sizeof( uint8_t ));
        return num;
    }

    static uint8_t getInt8Str(std::string &out, QDataStream &stream) {
        char c[3];
        stream.readRawData(&c[0],2);
        c[2] = '\0';
        out = c;

        return getInt8(c);
    }



    //01000000010123449fad6289dda5365a197fa822e320cdfc106bed243bf773cac64cfdb237050000006a473044022036be6403aeb4e0e6fd54720b328d9d81bea32fb79684da0288743668fb5ef3ee02202023a71ef7217061fb9b4f35a05143de71447032e5a35b39c3d14b3210bad10b0121032725846bb7bc2e47b7b5a50670d77c8268f4d7f3243bdcf1b22174a67faaf528feffffff0200213900000000001976a914659042e01e864e2f29641ea3a213c51a956d33c788ac288c0f00000000001976a9144fc238bcda3f884ff6ce8d9feeb89b50dfd3da8888ac2c480700

/*      49
        30460221009e0339f72c793a89e664a8a932df073962a3f84eda0bd9e02084a6a9567f75aa022100bd9cbaca2e5ec195751efdfac164b76250b1e21302e51ca86dd7ebd7020cdc0601410450863ad64a87ae8a2fe83c1af1a8403cb53f53e486d8511dad8a04887e5b23522cd470243453a299fa9e77237716103abc11a1df38855ed6f2ee187e9c582ba6
    /*
    01000000
    01
    0123449fad6289dda5365a197fa822e320cdfc106bed243bf773cac64cfdb237
    05000000
    6a
        <sig>
        47
        3044022036be6403aeb4e0e6fd54720b328d9d81bea32fb79684da0288743668fb5ef3ee02202023a71ef7217061fb9b4f35a05143de71447032e5a35b39c3d14b3210bad10b01
        <PubKey>
        21
        032725846bb7bc2e47b7b5a50670d77c8268f4d7f3243bdcf1b22174a67faaf528
    feffffff
    02  // num outs
    0021390000000000 // amount
    19               // len
        76a9
        14
        659042e01e864e2f29641ea3a213c51a956d33c7
        88ac
    288c0f0000000000  / amount
    19
        76a9
        14
        4fc238bcda3f884ff6ce8d9feeb89b50dfd3da88
        88ac
    2c480700  //locktime

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










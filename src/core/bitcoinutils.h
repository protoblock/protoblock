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

    template<int T>
    static std::string getStr(QDataStream &stream) {
        char c[T+1];
        stream.readRawData(&c[0],T);
        c[T] = '\0';
        return c;
    }

    static bitcoin_hex_tx parseRawHexTx(const QByteArray &rawtx) {
        QByteArray data;
        QDataStream stream(rawtx);
        bitcoin_hex_tx btx;
        {
            int size = VERSION_NORMAL.size();
            std::vector<char> version(size+1);
            size = stream.readRawData(&version[0],size);
            version[size] = '\0';
            btx.version = version.data();


            if ( strcmp (version.data(),VERSION_NORMAL.c_str()) == 0)
                qDebug() << "jayversion yes VERSION_NORMAL ";
            else
                qDebug() << "jayversion no VERSION_NORMAL ";
        }
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
            btx.input_count = sz;
            qDebug() << sz.data() << "jay size" << numinputs << "size jay";
        }

        std::vector<btx_input> &inputs = btx.inputs;
        for ( int i =0; i < numinputs; i++) {
            btx_input in;

            in.rtxid = getStr<64>(stream);
            if ( false ){
                std::vector<char> prevhash_reverse(65);
                stream.readRawData(&prevhash_reverse[0],64);
                prevhash_reverse[64] = '\0';
                in.rtxid = prevhash_reverse.data();
            }

            {
                std::vector<char> ti(9);
                stream.readRawData(&ti[0],8);
                ti[8] = '\0';
                in.txindex = ti.data();
            }

            {
                std::vector<char> vsz(3);
                stream.readRawData(&vsz[0],2);
                vsz[2] = '\0';
                in.script.size = vsz.data();

                uint8_t len;
                pb::from_hex(in.script.size,(char *)&len,sizeof( uint8_t ));

                len*=2;
                std::vector<char> sdata(len + 1);
                stream.readRawData(&sdata[0],len);
                sdata[len] = '\0';
                in.script.data = sdata.data();
            }

            {
                std::vector<char> seq(9);
                stream.readRawData(&seq[0],8);
                seq[8] = '\0';
                in.sequence = seq.data();
            }

            inputs.push_back(in);
        }

        uint8_t numout = 0;
        {
            std::vector<char> vsz(3);
            stream.readRawData(&vsz[0],2);
            vsz[2] = '\0';
            //std::string sz(&vsz[0]);
            pb::from_hex(&vsz[0],(char *)&numout,sizeof( uint8_t ));
            btx.output_count = vsz.data();
        }

        std::vector<btx_output> &outs = btx.outputs;
        for ( int i =0; i < numout; i++) {
            btx_output out;

            {
                const int _size = 16;
                std::vector<char> amount(_size+1);
                stream.readRawData(&amount[0],_size);
                amount[_size] = '\0';
                out.amount = amount.data();
            }

            {
                int _size = 2;
                std::vector<char> vsz(_size+1);
                stream.readRawData(&vsz[0],_size);
                vsz[_size] = '\0';
                out.script.size = vsz.data();

                uint8_t len;
                pb::from_hex(out.script.size,(char *)&len,sizeof( uint8_t ));

                _size = len*2;
                std::vector<char> sdata(_size + 1);
                stream.readRawData(&sdata[0],_size);
                sdata[_size] = '\0';
                out.script.data = sdata.data();
            }

            outs.push_back(out);
        }

        {
            const int _size = 8;
            std::vector<char> lock(_size+1);
            stream.readRawData(&lock[0],_size);
            lock[_size] = '\0';
            btx.locktime = lock.data();
        }

        return btx;
    }

    static std::string toRawHexTx(const bitcoin_hex_tx &tx) {
       auto ret = tx.version + tx.input_count;
       for ( const auto &v : tx.inputs )
           ret += v;

       ret += tx.output_count;
       for ( const auto &ov : tx.outputs )
           ret += ov;


       ret += tx.locktime;

       return ret;
    }



    /*
     *
    01000000
    01
    0123449fad6289dda5365a197fa822e320cdfc106bed243bf773cac64cfdb237
    05000000
    6a
        47
        3044022036be6403aeb4e0e6fd54720b328d9d81bea32fb79684da0288743668fb5ef3ee02202023a71ef7217061fb9b4f35a05143de71447032e5a35b39c3d14b3210bad10b01
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










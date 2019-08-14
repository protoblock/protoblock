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
#include "Commissioner.h"
//Input 1 script length: 0x17 (23 bytes)
//Input 1 signature script: 16001427c106013c0042da165c082b3870c31fb3ab4683

namespace pb {
using namespace std;
using namespace fantasybit;


bool BitcoinUtils::getUtxos(Bitcoin_UTXOS &utxos, const std::string &btcaddress, uint64_t max, uint64_t min) {
    utxos.set_total_value(0);
    for ( const auto &utxo : BitcoinApi::getUtxo(btcaddress) ) {
        auto *bu = utxos.add_utxo();
        bu->set_txid(utxo.tx_hash.toStdString());
        bu->set_tx_output_n(utxo.tx_output_n);
        bu->set_locking_script(utxo.script.toStdString());
        bu->set_in_value(utxo.in_value);

#ifdef TRACE
    qDebug() << " new utxo " << bu->DebugString().data();
#endif

        utxos.set_total_value(utxos.total_value()+utxo.in_value);
        if ( utxos.total_value() >= max)
            return true;
    }

#ifdef TRACE
    qDebug() << " utxos.total_value() " << utxos.total_value() << " min " << min;
#endif

    return utxos.total_value() > min;
}

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

int BitcoinUtils::checkUtxo(const Bitcoin_UTXO &iutxo, const std::string &btcaddress) {
    for ( const auto &utxo : BitcoinApi::getUtxo(btcaddress) ) {
        if ( utxo.tx_hash.toStdString() != iutxo.txid() )
            continue;

        if ( utxo.tx_output_n != iutxo.tx_output_n() )
            continue;

        if ( utxo.script.toStdString() != iutxo.locking_script() )
            continue;

        if ( utxo.in_value != iutxo.in_value())
            continue;

        return utxo.confirms;
    }

    return -1;
}

void BitcoinUtils::createInputsFromUTXO(const Bitcoin_UTXO &iutxo,
                                        std::string &input,
                                        std::string &in_script) {

    input += iutxo.txid();
    auto reversetxoun = pb::toReverseHexFromDecimal (iutxo.tx_output_n());
    input += reversetxoun;

    {
        auto ssize = static_cast<unsigned char>( iutxo.locking_script().size() / 2 );
        auto sstr = pb::to_hex ( &ssize, sizeof( unsigned char ) );
        in_script = sstr + iutxo.locking_script();
    }
}

std::string BitcoinUtils::toRawHexTx(const bitcoin_hex_tx &tx) {
    auto ret = tx.version + tx.input_count;
    for ( const auto &v : tx.inputs )
        ret += v;

    ret += tx.output_count;
    for ( const auto &ov : tx.outputs )
        ret += ov;

    ret += tx.locktime;
    return ret;
}

bitcoin_hex_tx BitcoinUtils::parseRawHexTx(const QByteArray &rawtx) {
    QByteArray data;
    QDataStream stream(rawtx);
    bitcoin_hex_tx btx;
    btx.version = getStr<8>(stream);

    if ( btx.version == VERSION_NORMAL )
        qDebug() << "podp yes VERSION_NORMAL ";
    else
        qDebug() << "podp no VERSION_NORMAL ";

    uint64_t numinputs;
    btx.input_count = getVarSize(stream,numinputs);
    std::vector<btx_input> &inputs = btx.inputs;
    for ( int i =0; i < numinputs; i++) {
        btx_input in;
        in.rtxid = getStr<64>(stream);
        in.txindex = getStr<8>(stream);
        in.script = getVarStr(stream);
        in.sequence = getStr<8>(stream);
        inputs.push_back(in);
    }

    uint64_t numout;
    btx.output_count = getVarSize(stream,numout);
    std::vector<btx_output> &outs = btx.outputs;
    for ( int i =0; i < numout; i++) {
        btx_output out;
        out.amount = getStr<16>(stream);
        uint64_t amount = toDecimalReverseHex<uint64_t>(out.amount);
        out.script = getVarStr(stream);
        outs.push_back(out);
    }
    btx.locktime = getStr<8>(stream);

    return btx;
}

std::string BitcoinUtils::createTX(const Bitcoin_UTXO &iutxo,
                                   const std::string &input,
                                   const std::string &in_script,
                                   const std::string &to_address,
                                   uint64_t amount,
                                   uint64_t btyefee,
                                   const std::string &change_address,
                                   std::string &pre_script,
                                   std::string &post_script) {

    qDebug() << "createTX" << input.data() << in_script.data() << to_address.data() << amount << btyefee;

    const std::string OP_DUP = "76";
    const std::string OP_HASH160 = "a9";
    const std::string OP_EQUALVERIFY = "88";
    const std::string OP_CHECKSIG = "ac";
    const std::string OP_RETURN = "6a";
    const std::string OP_EQUAL = "87";
    //const std::string SIGHASH_ALL = "01000000";
    const std::string SEQUENCE = "ffffffff";
    const std::string LOCKTIME = "00000000";
    const std::string VERSION = "01000000";

    int numinputs = 1;
    pb::public_key_data publick_key;
    uint64_t satoshifee = btyefee * 150;
    uint64_t change = iutxo.in_value() - amount;
    if ( change > 0 ) {
        if ( change < satoshifee ) {
            satoshifee -= change;
            change = 0;
        }
    }

    if ( change > 0 )
        change -= (btyefee*50);

    //output
    std::string raw_transaction_out;
    int numoutputs = 1;
    if ( change > 0 ) numoutputs++;
    {
        auto size = ( unsigned char )numoutputs;
        auto sstr = pb::to_hex ( &size, sizeof( unsigned char ) );
        raw_transaction_out += sstr;
    }

    {  //main output
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
    }

    if ( change > 0 ) {  //change output
        auto amstr = pb::toReverseHexFromDecimal(uint64_t(change));
        raw_transaction_out += amstr;

        //locking script
        std::string p2pkh  = OP_DUP + OP_HASH160;
        std::string pubkeyHash = pb::fromBtcAddress (change_address);
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
    }

    std::string locktime = LOCKTIME;
    raw_transaction_out += locktime;

    //create tx pre
    std::string raw_transaction_pre = VERSION; // i.e. version
    {
        auto size = ( unsigned char )numinputs;
        auto sstr = pb::to_hex ( &size, sizeof( unsigned char ) );
        raw_transaction_pre += sstr;
    }

    //to_sign
    std::string to_sign;
    to_sign += raw_transaction_pre;
    to_sign += input;
    to_sign += in_script;
    to_sign += SEQUENCE;
    to_sign += raw_transaction_out;
    to_sign += pb::SIGHASH_ALL_4;

    /*create tx
    std::string final_tx;
    final_tx += raw_transaction_pre;
    final_tx += input;
    final_tx += "%1"; // sigout_len + sigout
    final_tx += SEQUENCE;
    final_tx += raw_transaction_out;
    */

    pre_script = raw_transaction_pre;// + input;
    post_script = SEQUENCE + raw_transaction_out;
    /**/
    return to_sign;
}

bool BitcoinUtils::createProofOfUtxoSpend(const bitcoin_hex_tx &ref, const Bitcoin_UTXO &spent,
                                          std::string &pre, std::string &post, std::string &sigscript) {
    pre = ref.version + ref.input_count;

    bool seen = false;
    for ( auto &r : ref.inputs ) {
        if ( r.rtxid == spent.txid () &&
            toDecimalReverseHex<uint32_t>(r.txindex) == spent.tx_output_n() ) {
            if ( seen ) {
                qCritical() << " bad seen";
                return false;
            }

            std::string input, script;
            createInputsFromUTXO(spent,input,script);

            seen = true;
            post = r.sequence;
            sigscript = std::string(r.script);
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

    post += ref.output_count;
    for ( const auto &o : ref.outputs)
        post += o;

    post += ref.locktime;
    return seen;
}


/*
 *  verifyProofOfUtxoSpend - verify a sign tx - that will revert a Swap
 *
 */
bool BitcoinUtils::verifyProofOfUtxoSpend(const ProofOfDoubleSpend &podp) {

    const SwapFill &sf = podp.swapsent().swapfill();

    auto sigscript = getVarStr(podp.sig());
    auto dersig = getVarStr(sigscript.data);
    auto hexsig = dersig.data.substr(0,dersig.data.size()-2);
    if (dersig.data.substr(dersig.data.size()-2) != pb::SIGHASH_ALL_1) {
        qDebug() << "nogood verifyProofOfUtxoSpend dersig  !SIGHASH_ALL_1 ";
    }

    std::vector<unsigned char> sigv( (hexsig.size()/2) + 1 );
    auto rsz = from_hex(hexsig,(char *)(&sigv[0]),sigv.size());
    if (rsz != 71) {
        qDebug() << "nogood verifyProofOfUtxoSpend dersig size !70 " << rsz;
    }

    auto sig = pb::parse_der(&sigv[0],rsz);
    sig = pb::signature_normalize(sig);

    bool found = false;
    for ( const auto &theutxo : sf.swapbid().utxos().utxo() ) {
        if ( theutxo.txid() == podp.utxo().txid() &&
             theutxo.tx_output_n() == podp.utxo().tx_output_n() ) {
            found = true;
            break;
        }
    }

    if ( !found ) {
        qDebug() << "nogood verifyProofOfUtxoSpend !found utxo";
    }

    string input, inputscript;
    createInputsFromUTXO(podp.utxo(),input,inputscript);

    auto tosigntraw = podp.pre() + input + inputscript + podp.post() + SIGHASH_ALL_4;
    auto dblhash = pb::hashit(pb::hashfromhex(tosigntraw));
    auto hash_signed = dblhash.str();
    if ( hash_signed == podp.swapsent().swapfill().hash_to_sign() ) {
        qDebug() << "nogood verifyProofOfUtxoSpend hash match! ";
    }

    if ( !Commissioner::verifyByName(sig, dblhash, sf.counterparty()) ) {
        qDebug() << "nogood verifyProofOfUtxoSpend !verifyByName " << sf.counterparty().data();
        return false;
    }

    return true;
}


}


#ifndef BITCOINAPI_H
#define BITCOINAPI_H

#include <QString>
#include <vector>
#include <RestFullCall.h>
#include <utils/utils.h>
#include <QByteArray>

namespace fantasybit {

struct utxoData {
    QString tx_hash;
    QString script;
    uint64_t in_value;
    uint32_t tx_output_n;
    int     confirms;
};

struct txData {
    QString tx_hash;
    uint64_t out_value;
};

class BitcoinApi {
public:
    BitcoinApi();

    static std::vector<utxoData> getUtxo(const std::string &btcaddress) {
        std::vector<utxoData> ret;

        auto json = BitcoinRestfullService::getBlockchainBtcAddressUnspent (btcaddress);
        if ( json != "Service Unavailable") {
            if ( json == "No free outputs to spend") {
                qDebug() << json;
                return ret;
            }

            QJsonParseError * error = NULL;
            QJsonDocument doc = QJsonDocument::fromJson(json,error);
            if (error != NULL || doc.isEmpty()){
                    qDebug() << " error parsing json:" << json;
                if ( error != NULL ) qDebug() << error->errorString();
                    return ret;
            }

            qDebug() << json;
            qDebug() << doc.isNull() << doc.isEmpty() << doc.isArray() << doc.isObject();
            QJsonObject jo = doc.object();
            QJsonArray utxos = jo.value("unspent_outputs").toArray();
            for(QJsonValueRef ut : utxos) {
                ret.push_back({});
                utxoData &utd = ret.back();
                QJsonObject vo = ut.toObject();
                utd.tx_hash = vo.value("tx_hash").toString();
                utd.script = vo.value("script").toString();
                utd.in_value = vo.value("value").toInt();
                utd.tx_output_n = vo.value("tx_output_n").toInt();
                utd.confirms = vo.value("confirmations").toInt();
            }

            return ret;
        }

        auto json2 = BitcoinRestfullService::getChainsoBtcAddressUnspent (btcaddress);
        QJsonValue data = getChainData(json2);
        if ( !data.isNull() ) {
            QJsonObject jo2 = data.toObject();
            QJsonArray utxos = jo2.value("txs").toArray();
            for(QJsonValueRef ut : utxos) {
                ret.push_back({});
                utxoData &utd = ret.back();
                QJsonObject vo = ut.toObject();
                auto txid = vo.value("txid").toString();
                pb::sha256 txhash(txid.toStdString());
//                auto reverserhash = pb::toReverseHexFromDecimal (txhash.begin());
                utd.tx_hash = txhash.reversestr().data();
                utd.script = vo.value("script_hex").toString();
                auto strval = vo.value("value").toString();
                utd.in_value = strval.remove('.').toULongLong();
                utd.tx_output_n = vo.value("output_no").toInt();
                utd.confirms = vo.value("confirmations").toInt();
            }

            return ret;
        }
        else {
            qDebug() << " error getChainsoBtcAddressUnspent ";
        }

        return ret;

    }
/*
    static std::string sendrawTx(const std::string &rawin) {
        auto json = BitcoinRestfullService::pushBitcoinTx(rawin);
        auto strret = json.toStdString ();
        qDebug() << json << strret.data ();
        if ( std::string::npos == strret.find ("Transaction Submitted") ) {
            auto json2 = BitcoinRestfullService::pushChainsoBitcoinTx(rawin);

            QJsonParseError * error = NULL;
            QJsonDocument doc = QJsonDocument::fromJson(json2,error);
            if (error != NULL || doc.isEmpty()){
                    qDebug() << " error parsing json";
                if ( error != NULL )
                    qDebug() << error->errorString();

               return "";
            }

            qDebug() << json2;
            qDebug() << doc.isNull() << doc.isEmpty() << doc.isArray() << doc.isObject();
            QJsonObject jo = doc.object();
            if (jo.value("status") == "success") {
                QJsonValue data = jo.value("data");
                QJsonObject jo2 = data.toObject();
                auto txidin = jo2.value("txid").toString();

                return txidin.toStdString();
            }
            else
                return "";
        }
        else {
            return pb::hashit(pb::hashfromhex(rawin)).reversestr();
        }
    }

    static std::vector<txData> getSpentTx( const std::string &from,
                                           const std::string &to) {

        std::vector<txData> ret;
        auto json2 = RestfullService::getChainsoBtcAddress (from);
        QJsonValue data = getChainData(json2);
        if ( data.isNull() )
            return ret;

        QJsonObject jo2 = data.toObject();
        QJsonArray txs = jo2.value("txs").toArray();
        for(QJsonValueRef ut : txs) {
            QJsonObject vo = ut.toObject();
            if ( !vo.contains("outgoing") )
                continue;

            QJsonObject out = vo.value("outgoing").toObject();
            QJsonArray outs = out.value("outputs").toArray();
            txData txd;
            txd.out_value = 0;
            txd.tx_hash = vo.value("txid").toString();
            auto strval = out.value("value").toString();
            long long change = strval.remove('.').toULongLong();
            for(QJsonValueRef o : outs) {
                QJsonObject oo = o.toObject();
                auto outadd = oo.value("address").toString();
                strval = oo.value("value").toString();
                long long lval = strval.remove('.').toULongLong();
                if ( outadd.compare(to.data()) == 0)
                    txd.out_value += lval;
                change -= lval;
            }
            if ( txd.out_value > 0 ) {
                if ( change > 0 )
                    txd.out_value += change;
                ret.push_back(txd);
            }
        }

        return ret;
    }
*/
    static QJsonValue getChainData(const QByteArray &json2) {
        QJsonParseError * error = NULL;
        QJsonDocument doc = QJsonDocument::fromJson(json2,error);
        if (error != NULL || doc.isEmpty()){
                qDebug() << " error parsing json";
            if ( error != NULL )
                qDebug() << error->errorString();
            return QJsonValue();
        }

        qDebug() << json2;
        qDebug() << doc.isNull() << doc.isEmpty() << doc.isArray() << doc.isObject();
        QJsonObject jo = doc.object();
        if (jo.value("status") != "success")
            return QJsonValue();

        return jo.value("data");


    }
};

}
#endif // BITCOINAPI_H

/*
//  swapstatedata.h
//
//  Protoblock
//
//  Created by Jay Berg on 11/23/18
//
*/
#include <swapstatedata.h>
#include <fbutils.h>
#include <mutex>
#include <pbutils.h>

namespace fantasybit {

using std::string;

SwapStateData::SwapStateData() {}

void SwapStateData::init() {
//    return;
    write_sync.sync = true;
    leveldb::Options options;
    options.create_if_missing = true;
    leveldb::Status status;

    leveldb::DB *db2;
    status = leveldb::DB::Open(options, filedir("sellers"), &db2);
    swapasks.reset(db2);
    if ( !status.ok() ) {
        qCritical() << " cant open " << filedir("sellers").data();
        return;
    }
    else {
        std::lock_guard<std::recursive_mutex> lockg{ data_mutex };
        auto *it = swapasks->NewIterator(leveldb::ReadOptions());
        SwapAsk sa;
        for (it->SeekToFirst(); it->Valid(); it->Next()) {
            sa.ParseFromString(it->value().ToString());

            SwapSeller ss(it->key().ToString(), sa);
            mOrderBook.Add( ss );
            qDebug() << "twitch111" << sa.DebugString().data();
        }

        delete it;
    }

    leveldb::DB *db1;
    status = leveldb::DB::Open(options, filedir("buyers"), &db1);
    swapbids.reset(db1);
    if ( !status.ok() ) {
        qCritical() << " cant open " << filedir("buyers").data();
        return;
    }
    else {
        std::lock_guard<std::recursive_mutex> lockg{ data_mutex };
        auto *it = swapbids->NewIterator(leveldb::ReadOptions());
        SwapBid sb;
        SwapFill sf;
        SwapSent ss;
        ProofOfDoubleSpend pods;
        std::vector<std::pair<std::string,SwapFill>> vsf;
        std::vector<std::pair<std::string,SwapSent>> vss;
        std::vector<std::pair<std::string,ProofOfDoubleSpend>> vds;
        for (it->SeekToFirst(); it->Valid(); it->Next()) {
            auto str = it->key().ToString();
            auto pos =  str.find_last_of(':');
            if ( pos != string::npos) {
                sf.ParseFromString(it->value().ToString());
                vsf.push_back({str,sf});
                qDebug() << "twitch111" << sf.DebugString().data();

            }
            else if ( str.at(0) == '^' ) {
                if ( str.at(1) == '^' ) {
                    pods.ParseFromString(it->value().ToString());
                    vds.push_back({str.substr(2),pods});
                    qDebug() << "twitch111" << pods.DebugString().data();
                }
                else {
                    ss.ParseFromString(it->value().ToString());
                    vss.push_back({str.substr(1),ss});
                    qDebug() << "twitch111" << ss.DebugString().data();
                }
            }
            else {
                sb.ParseFromString(it->value().ToString());
                if ( sb.counteroffer() != "" ) {
                    if ( !mOrderBook.Buy( sb, it->key().ToString() ) ) {
                        qDebug() << "init !mOrderBook.Buy( inbid )  " << sb.DebugString().data();
                    }
                }

                SwapBuyer sb2(it->key().ToString(), sb);
                mOrderBook.Add( sb2 );
                qDebug() << "twitch111" << sb.DebugString().data();

            }
        }

        for ( auto ssf : vsf) {
            auto pos =  ssf.first.find_last_of(':');
            auto fnamebuyer = ssf.first.substr(0, pos); //Alice
            auto fnameseller = ssf.first.substr(pos+1); //Bob

            if ( !mOrderBook.Fill( ssf.second, fnameseller ) ) {
                qDebug() << "init !mOrderBook.Fill( infill )  " << ssf.second.fb_qty();
                continue;
            }
        }

        for ( auto sss : vss) {
            if ( mOrderBook.Sent(sss.second, sss.first ) == "") {
                qDebug() << "init !mOrderBook.Sent(  )  " << sss.first.data();
                continue;
            }
        }

        for ( auto sds : vds) {
            if ( !mOrderBook.doubleSpent(sds.second, sds.first )) {
                qDebug() << "init !mOrderBook.doubleSpent(  )  " << sds.first.data();
                continue;
            }
        }



        delete it;
    }
}

void SwapStateData::closeAll() {
    std::lock_guard<std::recursive_mutex> lockg{ data_mutex };
    mOrderBook.clear();
    swapasks.reset();
    swapbids.reset();
}

void SwapStateData::removeAll() {
    swapasks.reset();
    swapbids.reset();
    pb::remove_all(GET_ROOT_DIR() + "swap/");
}

void SwapStateData::OnNewSwapTx(const SwapAsk &inoffer, const string &fname, const string & ) {
    //store local in memory
    {
        std::lock_guard<std::recursive_mutex> lockg{ data_mutex };
        if ( inoffer.rate() <= 0 ) {
            qDebug() << "invalid offer rate " << inoffer.rate();
            return;
        }

        SwapSeller so(fname, inoffer);
        mOrderBook.Add( so );
    }

    AddNewSwapOrder(inoffer,fname);
    if ( amlive ) {
        SwapOrder so;
        so.set_isask(true);
        so.set_satoshi_min(inoffer.satoshi_min());
        so.set_satoshi_max(inoffer.satoshi_max());
        so.set_rate(inoffer.rate());
        so.set_fname(fname);
        emit NewSwapData(so);
    }
}

void SwapStateData::OnNewSwapTx(const SwapBid &inbid, const string &fname,const string & ) {
    //store local in memory
    {
        std::lock_guard<std::recursive_mutex> lockg{ data_mutex };
        //agressive buy
        if ( inbid.counteroffer() != "" ) {
            if ( !mOrderBook.Buy( inbid, fname ) ) {
                qDebug() << "!mOrderBook.Buy( inbid )  " << inbid.DebugString().data();
            }
        }
        //passive buy
        else if ( inbid.rate() <= 0 ) {
            qDebug() << "invalid bid rate " << inbid.rate();
            return;
        }

        SwapBuyer sb(fname, inbid);
        mOrderBook.Add( sb );
    }

    AddNewSwapOrder(inbid,fname);
    if ( amlive ) {
        SwapOrder so;
        so.set_isask(false);
        so.set_satoshi_min(inbid.satoshi_min());
        so.set_satoshi_max(inbid.satoshi_max());
        so.set_rate(inbid.rate());
        so.set_directed(inbid.counteroffer());
        so.set_fname(fname);
        emit NewSwapData(so);
    }
}

void SwapStateData::OnNewSwapTx(const SwapFill &infill, const string &fname, const string & ) {
    //store local in memory
    {
        std::lock_guard<std::recursive_mutex> lockg{ data_mutex };
        //todo: check balance
        if ( infill.fb_qty() <= 0 ) {
            qDebug() << "invalid SwapFill qty " << infill.fb_qty();
            return;
        }
        if ( !mOrderBook.Fill( infill, fname ) ) {
            qDebug() << "!mOrderBook.Fill( infill )  " << infill.fb_qty();
            return;
        }
    }

//    SwapBuyer sb = GetSwapBid(infill.counterparty().data());
//    if ( sb.is_pending ) {
//        qDebug() << "Invalid SwapFill - SwapBid already pending " << infill.DebugString().data();
//        return;
//    }
//    sb.is_pending = true;

    AddNewSwapOrder(infill,fname);
    if ( amlive ) {
        SwapOrder so;
        so.set_isask(false);
        so.set_fname(infill.counterparty());
        so.set_pendq(infill.fb_qty());
        so.set_ref(fname);
        emit NewSwapData(so);
    }
}

void SwapStateData::OnNewSwapTx(const SwapSent &insent, const string &fname, const string & ) {
    std::string counterparty = "";
    {
        std::lock_guard<std::recursive_mutex> lockg{ data_mutex };
        counterparty = mOrderBook.Sent(insent, fname );
        if ( counterparty == "" ) {
            qDebug() << "!mOrderBook.Sent( insent )  " << fname.data();
            return;
        }
    }

    AddNewSwapOrder(insent,fname);
    if ( amlive ) {
        SwapOrder so;
        so.set_isask(false);
        so.set_fname(fname);
//        so.set_fillq(insent.swapfill().fb_qty());
        so.set_ref(counterparty);
        so.set_msg(insent.sig());
        emit NewSwapData(so);
    }
}

void SwapStateData::OnNewSwapTx(const ProofOfDoubleSpend &inpods, const std::string &fname,const std::string &txid) {
    {
        std::lock_guard<std::recursive_mutex> lockg{ data_mutex };
        if ( !mOrderBook.doubleSpent(inpods, fname ) )  {
            qDebug() << "!mOrderBook.doubleSpent( )  " << inpods.DebugString().data();
            return;
        }
    }

    AddNewSwapOrder(inpods,fname);
    if ( amlive ) {
        SwapOrder so;
        so.set_isask(false);
        so.set_fname(inpods.swapsent().swapfill().counterparty());
        so.set_pendq(inpods.swapsent().swapfill().fb_qty());
        so.set_ref(fname);
        so.set_msg(inpods.swapsent().sig());
        so.set_double_spent(true);
        emit NewSwapData(so);
    }
}

std::vector<SwapOrder> SwapStateData::GetCurrentSwapSnaps() {
    std::lock_guard<std::recursive_mutex> lockg{ data_mutex };

    qDebug() << "get GetCurrentSwapSnaps" ;
    std::vector<SwapOrder> vso{};

    for (auto &it : mOrderBook.bids) {
        for ( auto &b  : it.second.sbuyers ) {
            SwapOrder so;
            so.set_isask(false);
            so.set_rate(it.first);
            so.set_satoshi_min(b.bid.satoshi_min());
            so.set_satoshi_max(b.bid.satoshi_max());
            so.set_rate(b.bid.rate());
            so.set_fname(b.fname);
            so.set_ref(b.counterparty);
            so.set_msg(b.signature);
            so.set_directed(b.counteroffer);
            so.set_double_spent(b.podp.has_sig());
            vso.emplace_back(so);
        }
    }

    for (auto &it : mOrderBook.asks) {
        for ( auto &a  : it.second.ssellers ) {
            SwapOrder so;
            so.set_isask(true);
            so.set_rate(it.first);
            so.set_satoshi_min(a.offer.satoshi_min());
            so.set_satoshi_max(a.offer.satoshi_max());
            so.set_rate(a.offer.rate());
            so.set_fname(a.fname);
            so.set_pendq(a.pending_fill);
            so.set_fillq(a.filled);
            so.set_openq(a.open);
            vso.emplace_back(so);
        }
    }

    return vso;
}

SwapBuyer SwapStateData::GetSwapBid(const QString &fname) {
    return mOrderBook.GetBuyer(fname.toStdString());

    /*
    SwapBuyer sb;
    sb.fname  = fname.toStdString();

    string temp;
    if ( !swapbids->Get(leveldb::ReadOptions(), sb.fname, &temp).ok() ) {
        qWarning() << "cant read swapbid for" << fname;
        sb.fname = "";
        return sb;
    }

    sb.bid.ParseFromString(temp);
    return sb;
    */

}

SwapSeller SwapStateData::GetSwapAsk(const QString &fname) {
    return mOrderBook.GetSeller(fname.toStdString());
    /*
    auto it =  mOrderBook.theSellers.find(fname.toStdString());
    if ( it != end(mOrderBook.theSellers))
        return it->second;

    SwapSeller sa;
    return sa;

    sa.fname  = fname.toStdString();

    string temp;
    if ( !swapasks->Get(leveldb::ReadOptions(), sa.fname, &temp).ok() ) {
        qWarning() << "cant read swapask for" << fname;
        sa.fname = "";
        return sa;
    }

    sa.offer.ParseFromString(temp);
    return sa;
    */
}

SwapFill SwapStateData::GetSwapFill(const QString &buyer, const QString &seller_ref) {
    SwapFill sf;

    QString qkey("%1:%2");
    std::string key = qkey.arg(buyer).arg(seller_ref).toStdString();
    string temp;
    if ( !swapbids->Get(leveldb::ReadOptions(), key, &temp).ok() ) {
        qWarning() << "cant read swapbids for" << key.data() << buyer << seller_ref;
        return sf;
    }

    sf.ParseFromString(temp);
    return sf;
}

uint64_t SwapStateData::GetLocked(const std::string &fname)  {
    std::lock_guard<std::recursive_mutex> lockg{ data_mutex };
    auto it = mTotLocked.find(fname);
    if ( it == end(mTotLocked) )
        return 0;

    return it->second;
}

void SwapStateData::AddNewSwapOrder(const SwapBid &inbid, const string &fname ) {
    qDebug() << " AddNewSwapBid " << inbid.DebugString().data();

    swapbids->Put(write_sync, fname, inbid.SerializeAsString());
}

void SwapStateData::AddNewSwapOrder(const SwapAsk &inask, const string &fname ) {
    qDebug() << " AddNewSwapAsk " << inask.DebugString().data();

    swapasks->Put(write_sync, fname, inask.SerializeAsString());
}

void SwapStateData::AddNewSwapOrder(const SwapFill &infill, const string &fname ) {
    qDebug() << " AddNewSwapFill " << infill.DebugString().data();

    std::string key = infill.counterparty() + ":" +  fname;
    swapbids->Put(write_sync, key, infill.SerializeAsString());
}

void SwapStateData::AddNewSwapOrder(const SwapSent &insent, const string &fname ) {
    qDebug() << " AddNewSwapSent " << insent.DebugString().data();

    std::string key =  "^" +  fname;
    swapbids->Put(write_sync, key, insent.SerializeAsString());
}

void SwapStateData::AddNewSwapOrder(const ProofOfDoubleSpend &inpodp, const string &fname ) {
    qDebug() << " AddNewSwapOrder inpodp ";

    std::string key =  "^^" +  fname;
    swapbids->Put(write_sync, key, inpodp.SerializeAsString());
}


}




















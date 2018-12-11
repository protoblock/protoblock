/*
//  swapstatedata.h
//
//  Protoblock
//
//  Created by Jay Berg on 11/23/18
//
*/
#include <SwapStateData.h>
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
        for (it->SeekToFirst(); it->Valid(); it->Next()) {
            sb.ParseFromString(it->value().ToString());

            SwapBuyer sb2(it->key().ToString(), sb);
            mOrderBook.Add( sb2 );
        }

        delete it;
    }

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


void SwapStateData::OnNewSwapTx(const SwapBid &inbid, const string &fname,const string & ) {
    //store local in memory
    {
        std::lock_guard<std::recursive_mutex> lockg{ data_mutex };
        if ( inbid.rate() <= 0 ) {
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
        so.set_fname(fname);
        emit NewSwapData(so);
    }
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
            vso.emplace_back(so);
        }
    }

    return vso;
}

SwapBuyer SwapStateData::GetSwapBid(const QString &fname) {
    SwapBuyer sb;
    sb.fname  = fname.toStdString();

    string temp;
    if ( !swapbids->Get(leveldb::ReadOptions(), sb.fname, &temp).ok() ) {
        qWarning() << "cant rad swapbid for" << fname;
        return sb;
    }

    sb.bid.ParseFromString(temp);
    return sb;
}

SwapSeller SwapStateData::GetSwapAsk(const QString &fname) {
    SwapSeller sa;
    sa.fname  = fname.toStdString();

    string temp;
    if ( !swapasks->Get(leveldb::ReadOptions(), sa.fname, &temp).ok() ) {
        qWarning() << "cant rad swapask for" << fname;
        return sa;
    }

    sa.offer.ParseFromString(temp);
    return sa;
}

void SwapStateData::AddNewSwapOrder(const SwapBid &inbid, const string &fname ) {
    qDebug() << " AddNewSwapBid " << inbid.DebugString().data();

    swapbids->Put(write_sync, fname, inbid.SerializeAsString());
}

void SwapStateData::AddNewSwapOrder(const SwapAsk &inask, const string &fname ) {
    qDebug() << " AddNewSwapAsk " << inask.DebugString().data();

    swapasks->Put(write_sync, fname, inask.SerializeAsString());
}

}




















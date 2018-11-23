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

void SwapStateData::OnNewSwapTx(const SwapBid &inbid, const string &fname ) {
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
}

void SwapStateData::OnNewSwapTx(const SwapAsk &inoffer, const string &fname ) {
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




















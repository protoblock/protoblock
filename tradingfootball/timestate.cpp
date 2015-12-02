//
//  timestate.h
//  fantasybit
//
//  Created by Jay Berg on 11/28/15
//
//
#include "timestate.h"
#include "qdebug.h"

//#include <vector>

using namespace fantasybit;

/*
vector<SignedTransaction>
TimeState::onStampedTrans(const StampedTrans &inst) {
    int32_t doprocess = 0;
    bool newmapenntry = false;
    bool isorphan = false;
    for ( auto it : mBuckets) {
        if ( it->first < inst.timestamp() ) {
            if ( it->second.second < inst.seqnum())
                continue; //new seq# > last seq#

            if ( it->second.first+1 == inst.seqnum() ) {
                //it->second.first = inst.seqnum();
                doprocess = it->first;
                break;
            }
            if ( it->second.first < inst.seqnum() ) {
                isorphan = true;
                break;
            }
            if ( inst.seqnum() == 0) {
                it->second.second = inst.prevseq;
                if ( inst.prevseq == it->second.first) {
                    doprocess = it->first;
                    //new map entry?
                    newmapentry = true;
                    break;
                }
            }
        }
        else {
            if ( inst.seqnum() == 0) {
                newmapentry = true;
                break;
            }
            isorphan = true;
            break;
        }
    }

    vector<SignedTransaction> ret;
    if ( isorphan ) {
        AddNewMsg(inst);
        return ret;
    }

    if ( newmapentry )
        mBuckets[inst.timestamp()] =
                make_pair(inst.seqnum(),std::numeric_limits<int32_t>::max());

    if ( doprocess > 0 )
        ;//AddNewMsg(inst);
    else
        return ret;

    if ( )
    auto *it = msgstore->NewIterator(leveldb::ReadOptions());
    Slice prev;
    int32_t process = mBuckets.begin()->first;
    auto pp = mBuckets.begin().second;
    for (it->SeekToFirst(); it->Valid(); it->Next()) {
        if ( !prev.empty() )
            msgstore->Delete(leveldb::WriteOptions(), prev);

        string temp;
        if ( !msgstore->Get(leveldb::ReadOptions(), it->key(), &temp).ok() )
            continue;
        StampedTrans st;
        if (!st.ParseFromString(temp) )
            continue;

        if ( st.seqnum() != pp.first+1 ) {
            if ( st.)
        }
        ret.push_back(st.signed_orig());
        prev = it->key();
    }
    if ( !prev.empty() )
        msgstore->Delete(leveldb::WriteOptions(), prev);

    return ret;
    //auto iter = mBuckets.upper_bound(inst.timestamp());
    //if ( iter == mBuckets)
}
*/
void TimeState::AddNewMsg(const StampedTrans &inst) {
    int32_t mynum = inst.timestamp();
    leveldb::Slice snum((char*)&mynum, sizeof(int32_t));
    if ( !msgstore->Put(write_sync, snum, inst.SerializeAsString()).ok() )
        qWarning() << " error writing msgstore";

}

vector<SignedTransaction>
TimeState::onStampedTrans(const StampedTrans &inst) {
    vector<SignedTransaction> ret;
    ret.push_back(inst.signed_orig());
    return ret;
}


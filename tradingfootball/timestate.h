//
//  timestate.h
//  fantasybit
//
//  Created by Jay Berg on 11/28/15
//
//
#ifndef __fantasybit__TimeState__
#define __fantasybit__TimeState__

#include <leveldb/db.h>
#include "ProtoData.pb.h"
#include <memory>
#include <vector>

namespace fantasybit {

using namespace std;

class TimeState {
    leveldb::WriteOptions write_sync{};

    std::shared_ptr<leveldb::DB> msgstore;
    std::map<int32_t,std::pair<int32_t,int32_t>> mBuckets; //timestamp -> prev_seq , last_seq
public:
    TimeState() {
        write_sync.sync = true;
    }

    vector<SignedTransaction>
    onStampedTrans(const StampedTrans &inst);
    void AddNewMsg(const StampedTrans &inst);

};

}

#endif // __fantasybit__TimeState__


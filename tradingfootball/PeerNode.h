//
//  Node.h
//  fantasybit
//
//  Created by Jay Berg on 8/31/14
//
//

#ifndef __fantasybit__Node__
#define __fantasybit__Node__
/*
#include <assert.h>
#include <string>
#include <iostream>
#include <set>
#include <vector>
*/

#include <mutex>
#include "fbutils.h"

#include <fc/optional.hpp>
//#include "boostLog.h"
#include <leveldb/db.h>
#include "ProtoData.pb.h"
#include <memory>
#include <leveldb/comparator.h>

namespace fantasybit
{

class Node
{
    int32_t current_hight = 0;
    int32_t global_height = 0;
public:
    Node();
    void init();
    bool Sync();
    bool SyncTo(int32_t);
    void BackSync(int32_t to);
    bool BackFork(const std::string &goodid, int32_t num);

    leveldb::WriteOptions write_sync{};

    fc::optional<int32_t> getLastGlobalBlockNum();

    std::string filedir(const std::string &in);
    static leveldb::Slice i2slice(int32_t i);

    static Block getlastLocalBlock() ;

    static fc::optional<Block> getLocalBlock(int32_t num, bool = false) ;

    static fc::optional<Block> getGlobalBlock(int32_t num);

    static std::vector<Block> getGlobalBlock(int32_t num, int32_t bend) ;

    static int32_t getLastLocalBlockNum();
    static int32_t myLastGlobalBlockNum();
    static void setLastGlobalBlockNum(int32_t num);
    static std::mutex blockchain_mutex;

    static void ClearTx(const Block &);
	static std::shared_ptr<leveldb::DB> blockchain;
    static std::shared_ptr<leveldb::DB> txpool;
    static int32_t GlobalHeight;
    static bool addTxPool(const std::string &id, std::string &txstr)  {
        return (txpool->Put(leveldb::WriteOptions(), id, txstr ).ok());

    }

    static bool forking;

    void Cleaner();
    bool Cleanit(Block *b) ;
};


class Int32Comparator : public leveldb::Comparator {
 public:
  // Three-way comparison function:
  //   if a < b: negative result
  //   if a > b: positive result
  //   else: zero result
  int Compare(const leveldb::Slice& a, const leveldb::Slice& b) const {
    int32_t ia = *(reinterpret_cast<const int32_t *>(a.data()));
    int32_t ib = *(reinterpret_cast<const int32_t *>(b.data()));

    //qDebug() << ia << ib << "yoyo";
    if (ia < ib) return -1;
    if (ia > ib) return +1;
    return 0;
  }

  // Ignore the following methods for now:
  const char* Name() const { return "Int32Comparator"; }
  void FindShortestSeparator(std::string*, const leveldb::Slice&) const { }
  void FindShortSuccessor(std::string*) const { }
};
//static std::string SEED_NODE("162.254.27.226");
//static std::string SEED_HOST("Jets");

}

#endif /* defined(__fantasybit__Node__) */

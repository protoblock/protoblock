#ifndef fantasybit_pbutils_h
#define fantasybit_pbutils_h

#include <QDir>
#include "StateData.pb.h"
#include "utils/utils.h"
#include <queue>
#include <unordered_map>
#include "StateData.pb.h"
#include "ldbwriter.h"

namespace pb {

using namespace fantasybit;

static void remove_all(const std::string &indir) {
    QDir dir(indir.data());
    dir.removeRecursively();
}

static void make_all(const std::string &indir) {
    QDir dir(indir.data());
    if (!dir.exists())
        dir.mkpath(".");
}

static std::string makeMerkleRoot(decltype(fantasybit::MerkleTree::default_instance().leaves()) &in) {
    std::queue<pb::sha256> merkle;

    for (const auto &elem : in ) {
        pb::sha256 digest = pb::hashit(elem);

        merkle.push(digest);
    }

    if (merkle.empty())
        return "";//(fc::sha256) NULL;

    int rows = 0;
    while (merkle.size() > 1) {
       /*
        if (merkle.size() % 2 != 0) {
            merkle.push(merkle.back());
        }
       */
        std::queue<pb::sha256> new_merkle;

        rows++;
        int j =  merkle.size();

        while (merkle.size() > 1) {
            j =  merkle.size();

            pb::sha256 first = merkle.front();
            merkle.pop();

            j =  merkle.size();

            pb::sha256 second = merkle.front();
            merkle.pop();

            j =  merkle.size();

            std::string concat;

            concat = (std::string) first + (std::string) second;

            new_merkle.push(pb::hashit(concat));
        }
        if ( merkle.size() == 1) {
            new_merkle.push(merkle.front());
            merkle.pop();
        }

        j =  merkle.size();

        merkle = new_merkle;
    }

    return merkle.front();

    return "";
}

static std::string makeMerkleRoot(decltype(fantasybit::Block::default_instance().signed_transactions()) &in) {
    std::queue<pb::sha256> merkle;

    for (const auto &elem : in ) {
        merkle.push(elem.id());
    }

    if (merkle.empty())
        return "";//(fc::sha256) NULL;

    int rows = 0;
    while (merkle.size() > 1) {
       /*
        if (merkle.size() % 2 != 0) {
            merkle.push(merkle.back());
        }
       */
        std::queue<pb::sha256> new_merkle;

        rows++;
        int j =  merkle.size();

        while (merkle.size() > 1) {
            j =  merkle.size();

            pb::sha256 first = merkle.front();
            merkle.pop();

            j =  merkle.size();

            pb::sha256 second = merkle.front();
            merkle.pop();

            j =  merkle.size();

            std::string concat;

            concat = (std::string) first + (std::string) second;

            new_merkle.push(pb::hashit(concat));
        }
        if ( merkle.size() == 1) {
            new_merkle.push(merkle.front());
            merkle.pop();
        }

        j =  merkle.size();

        merkle = new_merkle;
    }

    return merkle.front();

    return "";
}

/*
template<class T>
std::string loadMerkleMap(const LdbWriter &ldb,
              const std::string &in, MerkleTree &mtree,
              std::unordered_map<std::string,  T> &mapt) {

    ldb.read(in,mtree);
    for ( auto nodestr : mtree.leaves()) {
        T nodet;
        ldb.read(nodestr,nodet);
        mapt[nodestr] = nodet;
    }

    return mtree.root();
}
*/
template<class T>
std::string loadMerkleMap(LdbWriter &ldb,
                          const std::string &in, MerkleTree &mtree,
              std::vector< std::pair<std::string,  T> > &mapt) {

    ldb.read(in,mtree);
    for ( auto nodestr : mtree.leaves()) {
        T nodet;
        ldb.read(nodestr,nodet);
        mapt.push_back(make_pair(nodestr,nodet));
    }

    return mtree.root();
}
/*
template<class T>
std::string setNewMerkelTree(const LdbWriter &ldb,,
                             const std::unordered_map<std::string,  T>  &mapt,
                             MerkleTree &mtree) {
    for ( auto &leaf : mapt) {
        mtree.add_leaves(leaf.first);
    }
    mtree.set_root(makeMerkleRoot(mtree.leaves()));
    ldb.write(mtree.root(),mtree.SerializeAsString());
    return mtree.root();
}

template<class T>
std::string loadMerkleMap(const LdbWriter &ldb,,
                          const std::string &in,std::vector<T> &vect) {

    MerkleTree mtree;
    ldb.read(in,mtree);
    for ( int i = 0; i < mtree.leaves_size(); i++) {
        auto nodestr = mtree.leaves(i);
        T nodet;
        ldb.read(nodestr,nodet);
        vect.push_back(nodet);
    }

    return mtree.root();
}
*/

}
#endif

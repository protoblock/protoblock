#ifndef fantasybit_pbutils_h
#define fantasybit_pbutils_h

#include <QDir>
#include "StateData.pb.h"
#include "utils/utils.h"
#include <queue>

namespace pb {

static void remove_all(const std::string &indir) {
    QDir dir(indir.data());
    dir.removeRecursively();
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

}
#endif

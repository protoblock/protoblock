#ifndef fantasybit_pbutils_h
#define fantasybit_pbutils_h

#include <QDir>

namespace pb {

static void remove_all(const std::string &indir) {
    QDir dir(indir.data());
    dir.removeRecursively();
}

}
#endif

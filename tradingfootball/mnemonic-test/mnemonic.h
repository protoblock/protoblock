#ifndef MNEMONIC_H
#define MNEMONIC_H

#include <vector>
#include <cstdint>

typedef std::vector<uint8_t> bytes;

#define SEED_SIZE (32)
#define MNEMONIC_SALT ("fantasy bit")

bytes mnemonicToSeed(std::string mnemonic);

#include <QByteArray>
#include <QString>

class Mnemonic
{
public:
    Mnemonic(){}
    ~Mnemonic(){}
    static QByteArray test(QString & mnemonic);

};

#endif // MNEMONIC_H

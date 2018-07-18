#include "mnemonic.h"
#include <openssl/sha.h>
#include <openssl/evp.h>
#include <fc/crypto/sha256.hpp>


QByteArray Mnemonic::test(QString & mnemonic){
    bytes results = mnemonicToSeed(mnemonic.toStdString());
    QByteArray buffer;
    for (std::vector<uint8_t>::iterator it = results.begin() ; it != results.end(); ++it)
        buffer.append(*it);
    return buffer;
}

bytes mnemonicToSeed(std::string mnemonic) {
    bytes seed(SEED_SIZE);

    // TODO check mnemonic validity and set the iterations count
    int iterations = 131072;

    if (!PKCS5_PBKDF2_HMAC(mnemonic.c_str(), mnemonic.length(),
            (const unsigned char*)MNEMONIC_SALT, strlen(MNEMONIC_SALT),
            iterations, EVP_sha512(), seed.size(), &seed[0])) {
        puts("Error deriving seed from mnemonic");
        exit(EXIT_FAILURE);
    }

    return seed;
}

bool verify() {
    std::string in = "2db65ce84d518618bb2a0c256bab624223c8e57d92a97e83629b7e2d651cbe1b";
    fc::ecc::private_key::regenerate(fc::sha256{ in });
    fc::ecc::private_key::generate_from_seed(fc::sha256{ in });


}



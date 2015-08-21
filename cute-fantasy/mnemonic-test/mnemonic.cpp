#include "mnemonic.h"
#include <openssl/sha.h>
#include <openssl/evp.h>


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




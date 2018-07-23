#include <cstring>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <openssl/sha.h>
#include <openssl/evp.h>
#include "mnemonic.h"
#include "utils/utils.h"

using namespace std;

// Helper functions
string encodeToMnemonic(bytes& data);
bytes decodeMnemonic(const string& mnemonic);
bits bytesToBits(const bytes& data);

string createMnemonic(const uint8_t *randomData, uint length) {
    bytes entropy(length);
    memcpy(entropy.data(), randomData, length);

    return encodeToMnemonic(entropy);
}

bytes mnemonicToSeed(const string& mnemonic) {
    bytes entropy = decodeMnemonic(mnemonic);

    bytes seed(SEED_SIZE);
    if (!PKCS5_PBKDF2_HMAC(mnemonic.c_str(), mnemonic.length(),
            (const unsigned char*) MNEMONIC_SALT, strlen(MNEMONIC_SALT),
            MNEMONIC_PBKDF2_ITERATIONS, EVP_sha512(), seed.size(), seed.data())) {
        throw MnemonicException("Error deriving seed from mnemonic");
    }

    return seed;
}

bits bytesToBits(const bytes& data) {
    bits bitsData(data.size() * 8);
    for (uint i = 0; i < data.size(); ++i) {
        for (int j = 0; j < 8; ++j) {
            bitsData[(i * 8) + j] = (data[i] & (1 << (7 - j))) != 0;
        }
    }
    return bitsData;
}

bytes decodeMnemonic(const string& mnemonic) {
    // Split mnemonic to words
    istringstream iss(mnemonic);
    mnemonic_words words { istream_iterator<string> { iss }, istream_iterator<string> { } };

    if (words.size() % 3 != 0) {
        throw MnemonicLengthException("Mnemonic words must be multiple of 3");
    }

    if (words.size() == 0) {
        throw MnemonicLengthException("Mnemonic is empty");
    }

    int concatLenBits = words.size() * 11;
    vector<bool> concatBits(concatLenBits);
    int wordindex = 0;
    for (string word : words) {
        // Find the word index
        if ( word == "infacnt") word = "infant";
        else if ( word == "accross") word = "across";
        else if ( word == "acheive") word = "achieve";
        else if ( word == "emply") word = "employ";
        else if ( word == "visibile") word = "visible";

        uint index = std::distance(wordlist.begin(),
                std::find(wordlist.begin(), wordlist.end(), word));
        if (index == wordlist.size()) {
            throw MnemonicWordException("Invalid mnemonic word: " + word);
        }

        // Set the next 11 bits to the value of the index.
        for (int ii = 0; ii < 11; ++ii) {
            concatBits[(wordindex * 11) + ii] = (index & (1 << (10 - ii))) != 0;
        }
        ++wordindex;
    }

    int checksumLengthBits = concatLenBits / 33;
    int entropyLengthBits = concatLenBits - checksumLengthBits;

    // Extract original entropy as bytes.
    bytes entropy(entropyLengthBits / 8);
    for (uint ii = 0; ii < entropy.size(); ++ii) {
        for (int jj = 0; jj < 8; ++jj) {
            if (concatBits[(ii * 8) + jj]) {
                entropy[ii] |= 1 << (7 - jj);
            }
        }
    }

    //unsigned char out[32];


    // Take the digest of the entropy.
    bytes hash(SHA256_DIGEST_LENGTH);

//    secp256k1_sha256_t hasher;
//    secp256k1_sha256_initialize(&hasher);
//    secp256k1_sha256_write(&hasher, entropy.data(), entropy.size());
//    secp256k1_sha256_finalize(&hasher, hash.data());

    pb::hashc(entropy.data(),entropy.size(),hash.data());

    //SHA256(entropy.data(), entropy.size(), hash.data());
    bits hashBits = bytesToBits(hash);

    // Check all the checksum bits.
    for (int i = 0; i < checksumLengthBits; ++i) {
        if (concatBits[entropyLengthBits + i] != hashBits[i]) {
            throw MnemonicChecksumException("Invalid mnemonic checksum");
        }
    }

    return entropy;
}

string encodeToMnemonic(bytes& data) {
    if (data.size() % 4 != 0) {
        throw MnemonicLengthException("Entropy length not multiple of 32 bits.");
    }

    if (data.size() == 0) {
        throw MnemonicLengthException("Entropy is empty.");
    }

    // We take initial entropy of ENT bits and compute its
    // checksum by taking first ENT / 32 bits of its SHA256 hash.

    bytes hash(SHA256_DIGEST_LENGTH);
    SHA256(data.data(), data.size(), hash.data());
    bits hashBits = bytesToBits(hash);

    bits dataBits = bytesToBits(data);
    int checksumLengthBits = dataBits.size() / 32;

    // We append these bits to the end of the initial entropy.
    bits concatBits(dataBits.size() + checksumLengthBits);
    memcpy(concatBits.data(), dataBits.data(), dataBits.size());
    memcpy(&concatBits[dataBits.size()], hashBits.data(), checksumLengthBits);

    // Next we take these concatenated bits and split them into
    // groups of 11 bits. Each group encodes number from 0-2047
    // which is a position in a wordlist.  We convert numbers into
    // words and use joined words as mnemonic sentence.

    mnemonic_words words;
    int nwords = concatBits.size() / 11;
    for (int i = 0; i < nwords; ++i) {
        int index = 0;
        for (int j = 0; j < 11; ++j) {
            index <<= 1;
            if (concatBits[(i * 11) + j]) {
                index |= 0x1;
            }
        }
        words.push_back(wordlist[index]);
    }

    ostringstream os;
    for (mnemonic_words::const_iterator i = words.begin(); i != words.end(); i++){
        if (i != words.begin()) os << " ";
        os << *i;
    }
    return os.str();

}

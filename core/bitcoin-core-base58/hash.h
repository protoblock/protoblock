// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2013 The Bitcoin developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef BITCOIN_HASH_H
#define BITCOIN_HASH_H

#include <vector>

#include <openssl/ripemd.h>
#include <openssl/sha.h>

template<typename T1>
inline unsigned char* Hash(const T1 pbegin, const T1 pend)
{
    static unsigned char pblank[1];
    unsigned char hash1[32];
    SHA256((pbegin == pend ? pblank : (unsigned char*)&pbegin[0]), (pend - pbegin) * sizeof(pbegin[0]), hash1);
    unsigned char *hash2 = new unsigned char[32];
    SHA256(hash1, sizeof(hash1), hash2);
    return hash2;
}

template<typename T1>
inline unsigned char* Hash160(const T1 pbegin, const T1 pend)
{
    static unsigned char pblank[1];
    unsigned char hash1[32];
    SHA256((pbegin == pend ? pblank : (unsigned char*)&pbegin[0]), (pend - pbegin) * sizeof(pbegin[0]), hash1);
    unsigned char *hash2 = new unsigned char[20];
    RIPEMD160(hash1, sizeof(hash1), hash2);
    return hash2;
}

inline unsigned char* Hash160(const std::vector<unsigned char>& vch)
{
    return Hash160(vch.begin(), vch.end());
}


#endif

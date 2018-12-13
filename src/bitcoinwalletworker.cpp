/*
//  bitcoinwalletworker.cpp.h
//
//  Protoblock
//
//  Created by Jay Berg on 12/10/2018
//
*/
#include "bitcoinwalletworker.h"

BitcoinWalletWorker::BitcoinWalletWorker(QObject *parent) : QObject(parent) {
    timer = new QTimer(this);



}

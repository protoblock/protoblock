/*
//  bitcoinwalletworker.h.h
//
//  Protoblock
//
//  Created by Jay Berg on 12
//
*/
#ifndef BITCOINWALLETWORKER_H
#define BITCOINWALLETWORKER_H

#include <QObject>

class BitcoinWalletWorker : public QObject
{
    Q_OBJECT
public:
    explicit BitcoinWalletWorker(QObject *parent = nullptr);

signals:

public slots:
};

#endif // BITCOINWALLETWORKER_H
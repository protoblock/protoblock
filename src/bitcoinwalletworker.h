/*
//  bitcoinwalletworker.h.h
//
//  Protoblock
//
//  Created by Jay Berg on 12/10/2018
//
*/
#ifndef BITCOINWALLETWORKER_H
#define BITCOINWALLETWORKER_H

#include <QObject>
#include <QTimer>

class BitcoinWalletWorker : public QObject
{
    Q_OBJECT

    QTimer * timer;

public:
    explicit BitcoinWalletWorker(QObject *parent = nullptr);

signals:
    void NameBitcoinBalance(const QString &name, quint64 satoshis);


public slots:
    void onUsingFantasyName(const QString &name, const QString &btcaddress);

};

#endif // BITCOINWALLETWORKER_H

/*
//  console.h.h
//
//  Protoblock
//
//  Created by Jay Berg on 5/11/2018
//
*/
#ifndef CONSOLE_H
#define CONSOLE_H

#include <QObject>
#ifdef Q_OS_WIN
#include <QWinEventNotifier>
#include <windows.h>
#else
#include <QSocketNotifier>
#endif


class Console : public QObject
{
    Q_OBJECT

public:
    Console();

public slots:
    void run();

signals:
    void quit();
    void command(std::string);
    void sendto();


private:
#ifdef Q_OS_WIN
    QWinEventNotifier *m_notifier;
#else
    QSocketNotifier *m_notifier;
#endif

private slots:
    void readCommand(HANDLE hEvent);
};

inline Console::Console() {}

#endif // CONSOLE_H

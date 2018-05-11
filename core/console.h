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

    void run();

signals:
    void quit();
    void command(std::string);

private:
#ifdef Q_OS_WIN
    QWinEventNotifier *m_notifier;
#else
    QSocketNotifier *m_notifier;
#endif

private slots:
    void readCommand(HANDLE hEvent);
};

inline Console::Console()
{
#ifdef Q_OS_WIN
    m_notifier = new QWinEventNotifier(GetStdHandle(STD_INPUT_HANDLE));
    connect(m_notifier, &QWinEventNotifier::activated,this, &Console::readCommand);
#else
    m_notifier = new QSocketNotifier(fileno(stdin), QSocketNotifier::Read, this);
    connect(m_notifier, &QSocketNotifier::activated,this, SLOT(readCommand()));
#endif
}

#endif // CONSOLE_H

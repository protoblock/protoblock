/*
//  console.cpp.h
//
//  Protoblock
//
//  Created by Jay Berg on 5/11/2018
//
*/
#include <iostream>
#include "console.h"

void Console::run()
{
#ifdef Q_OS_WIN
    m_notifier = new QWinEventNotifier(GetStdHandle(STD_INPUT_HANDLE));
    connect(m_notifier, &QWinEventNotifier::activated,this, &Console::readCommand);
#else
    m_notifier = new QSocketNotifier(fileno(stdin), QSocketNotifier::Read, this);
    connect(m_notifier, &QSocketNotifier::activated,this, SLOT(readCommand()));
#endif

    std::cout << "First message" << std::endl;
    std::cout << "> " << std::flush;
}

void Console::readCommand(HANDLE hEvent)
{
    std::string line;
    std::getline(std::cin, line);
    if (std::cin.eof() || line == "quit") {
        std::cout << "Good bye!" << std::endl;
        emit quit();
    }
    else if (line == "send") {
        std::cout << "send" << std::endl;
        emit sendto();
    }
    else if (line == "x") {
        std::cout << "> " << std::flush;
    }
    else {
        std::cout << "Echo: " << line << std::endl;
        std::cout << "> " << std::flush;
        emit command(line);
    }
}

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
    } else {
        std::cout << "Echo: " << line << std::endl;
        std::cout << "> " << std::flush;
        emit command(line);
    }
}

/**
 * @file server.cpp
 * @headerfile server.h
 * @author lqy
 * @brief Wrapper for server classes.
 * @date 2024-10-10
 */

#include "../include/server.h"
#include <string>
#include <iostream>

/* Public methods */

Server::Server(std::string type)
{
    if (type == "TCP") {
        server = new Server_TCP(SERVER_IP, SERVER_PORT);
    } else if (type == "UDP") {
        server = new Server_UDP(SERVER_IP, SERVER_PORT);
    } else {
        server = nullptr;
    }
}

Server::~Server()
{
    delete server;
}

void Server::cmds()
{
    std::string command;
    while (true) {
        std::cin >> command;
        if (command == "init") {
            server->init();
        } else if (command == "run" || command == "start") {
            server->run();
        } else if (command == "stop") {
            server->stop();
        } else if (command == "help") {
            help();
        } else if (command == "exit" || command == "quit") {
            server->quit();
            break;
        } else {
            std::cout << "Invalid server commands!!! Try again." << std::endl;
        }
    }
    return ;
}

void Server::help()
{
    std::cout << "*--------------------------------------*" << std::endl;
    std::cout << "|           SERVER HELP MENU           |" << std::endl;
    std::cout << "*--------------------------------------*" << std::endl;
    std::cout << "|           AUTHOR: !EEExp3rt          |" << std::endl;
    std::cout << "*--------------------------------------*" << std::endl;
    std::cout << "|   CMDS   |           USAGE           |" << std::endl;
    std::cout << "*----------*---------------------------*" << std::endl;
    std::cout << "|   init   | Initialize the server     |" << std::endl;
    std::cout << "*----------*---------------------------*" << std::endl;
    std::cout << "|   run    |                           |" << std::endl;
    std::cout << "*----------* Start the server and run  |" << std::endl;
    std::cout << "|   start  |                           |" << std::endl;
    std::cout << "*----------*---------------------------*" << std::endl;
    std::cout << "|   stop   | Stop the server           |" << std::endl;
    std::cout << "*----------*---------------------------*" << std::endl;
    std::cout << "|   help   | Show this help message    |" << std::endl;
    std::cout << "*----------*---------------------------*" << std::endl;
    std::cout << "|   exit   |                           |" << std::endl;
    std::cout << "*----------* Exit the server program   |" << std::endl;
    std::cout << "|   quit   |                           |" << std::endl;
    std::cout << "*----------*---------------------------*" << std::endl;
}
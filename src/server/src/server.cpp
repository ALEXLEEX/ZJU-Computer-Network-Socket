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

Server::Server(std::string type, std::string ip, int port, int queueSize, int bufferSize, double timeout)
{
    if (type == "TCP") {
        server = new Server_TCP(ip, port, queueSize, bufferSize, timeout);
    } else if (type == "UDP") {
        server = new Server_UDP(ip, port, queueSize, bufferSize, timeout);
    } else {
        server = nullptr;
    }
}

Server::Server(int argc, char *argv[])
{
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <server_type> [ip] [port] [max_connections] [message_buffer_size] [wait_timeout]" << std::endl;
        exit(EXIT_FAILURE);
    }
    std::string type = argv[1];
    std::string ip = (argc > 2) ? argv[2] : SERVER_IP;
    int port = (argc > 3) ? std::stoi(argv[3]) : SERVER_PORT;
    int queueSize = (argc > 4) ? std::stoi(argv[4]) : SERVER_QUEUE_SIZE;
    int bufferSize = (argc > 5) ? std::stoi(argv[5]) : SERVER_BUFFER_SIZE;
    double timeout = (argc > 6) ? std::stod(argv[6]) : SERVER_TIMEOUT;
    if (type == "TCP") {
        server = new Server_TCP(ip, port, queueSize, bufferSize, timeout);
    } else if (type == "UDP") {
        server = new Server_UDP(ip, port, queueSize, bufferSize, timeout);
    } else {
        std::cerr << "Invalid server type: " << type << "!" << std::endl;
        server = nullptr;
        exit(EXIT_FAILURE);
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
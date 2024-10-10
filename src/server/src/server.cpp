/**
 * @file server.cpp
 * @headerfile server.h
 * @author lqy
 * @brief Server's base class implementation.
 * @date 2024-10-08
 */

#include "../include/server.h"
#include <iostream>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <thread>
#include <system_error>

/* Public methods */

Server::Server(std::string ip, int port, int queueSize, int bufferSize, double timeout):
    serverSocket(-1), serverIp(ip), serverPort(port), queueSize(queueSize), bufferSize(bufferSize), timeout(timeout) {}

Server::~Server()
{
    printMessage(ServerMsgType::WELCOME, "Server closed, GOODBYE!!!");
}

void Server::init()
{
    printMessage(ServerMsgType::WELCOME, "Server is initializing...");
    getSocket();
    setOptions(timeout);
    bindAddress(serverIp, serverPort);
    printMessage(ServerMsgType::WELCOME, "Server initialization completed, WELCOME!!!");
}

/* Utility functions */

void Server::printMessage(ServerMsgType msgType, std::string msg)
{
    switch (msgType) {
        case ServerMsgType::MSG:
            std::cout << "\033[37m" << msg << "\033[0m" << std::endl;
            break;
        case ServerMsgType::ERROR:
            std::cerr << "\033[31m[Server] " << msg << "\033[0m" << std::endl;
            exit(-1);
        case ServerMsgType::INFO:
            std::cout << "\033[32m[Server] " << msg << "\033[0m" << std::endl;
            break;
        case ServerMsgType::WELCOME:
            std::cout << "\033[34m[Server] " << msg << "\033[0m" << std::endl;
            break;
    }
    return ;
}

void Server::setOptions(double timeout)
{
    // Set reuse address.
    int on = 1;
    setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, (char *)&on, sizeof(on));

    // Set timeout.
    struct timeval serverTimeout;
    serverTimeout.tv_sec = static_cast<long>(timeout); // Seconds.
    serverTimeout.tv_usec = static_cast<long>((timeout - serverTimeout.tv_sec) * 1000000); // Microseconds.
    setsockopt(serverSocket, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(timeout));
    printMessage(ServerMsgType::INFO, "Set server waiting timeout: " + std::to_string(timeout) + ".");
}

void Server::bindAddress(std::string serverIp, int serverPort)
{
    struct sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = serverIp.size() ? inet_addr(serverIp.c_str()) : INADDR_ANY;
    serverAddress.sin_port = htons(serverPort);

    if (bind(serverSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0) {
        printMessage(ServerMsgType::ERROR, "Failed to bind address.");
    }
    printMessage(ServerMsgType::INFO, "Bind server to IP: " + serverIp + ", port: " + std::to_string(serverPort) + ".");
}

void Server::startSocketThread()
{
    try {
        std::thread thread(&Server::worker, this);
        thread.join();
    } catch (const std::system_error& e) {
        printMessage(ServerMsgType::ERROR, e.what());
    }
}
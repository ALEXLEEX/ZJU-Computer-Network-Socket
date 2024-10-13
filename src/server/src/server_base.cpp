/**
 * @file server.cpp
 * @headerfile server.h
 * @author lqy
 * @brief Server's base class implementation.
 * @date 2024-10-08
 */

#include "../include/server_base.h"
#include "../include/client_info.h"
#include <iostream>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <thread>
#include <system_error>
#include <stdlib.h>

/* Public methods */

Server_Base::Server_Base(std::string ip, int port, int queueSize, int bufferSize, double timeout):
    serverSocket(-1), serverIp(ip), serverPort(port), queueSize(queueSize), bufferSize(bufferSize), timeout(timeout)
{
    clientQueue = std::vector<ClientInfo>(queueSize, ClientInfo());
}

Server_Base::~Server_Base()
{
    // Disconnect all clients.
    for (ClientInfo& client: clientQueue) {
        if (client.getStatus()) {
            client.setStatus(0);
        }
    }
    clientQueue.clear();
    close(serverSocket); // Close the server socket.
    printMessage(ServerMsgType::WELCOME, "Server deconstructed, GOODBYE!!!");
}

void Server_Base::init()
{
    printMessage(ServerMsgType::WELCOME, "Server is initializing...");
    getSocket();
    setOptions(timeout);
    bindAddress(serverIp, serverPort);
    printMessage(ServerMsgType::WELCOME, "Server initialization completed, WELCOME!!!");
}

/* Utility functions */

void Server_Base::printMessage(ServerMsgType msgType, std::string msg)
{
    switch (msgType) {
        case ServerMsgType::MSG:
            std::cout << "\033[37m" << msg << "\033[0m" << std::endl;
            break;
        case ServerMsgType::ERROR:
            std::cerr << "\033[31m[Server] " << msg << "\033[0m" << std::endl;
            exit(EXIT_FAILURE);
        case ServerMsgType::INFO:
            std::cout << "\033[32m[Server] " << msg << "\033[0m" << std::endl;
            break;
        case ServerMsgType::WELCOME:
            std::cout << "\033[34m[Server] " << msg << "\033[0m" << std::endl;
            break;
    }
    return ;
}

void Server_Base::setOptions(double timeout)
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

void Server_Base::bindAddress(std::string serverIp, int serverPort)
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

void Server_Base::startSocketThread()
{
    try {
        printMessage(ServerMsgType::INFO, "The server is now running...");
        std::thread thread(&Server_Base::worker, this);
        thread.join();
        printMessage(ServerMsgType::INFO, "Server shuts down.");
    } catch (const std::system_error& e) {
        printMessage(ServerMsgType::ERROR, e.what());
    }
}

void Server_Base::closeClient(ClientInfo& client)
{
    sendResponse(client, "Disconnected from server(" + serverIp + ":" + std::to_string(serverPort) + ").");
    close(client.getSocket());
    client.setStatus(0); // Shut down the sub thread.
    client.setID(-1);
    printMessage(ServerMsgType::INFO, "Client " + std::to_string(client.getID()) + " disconnected from server.");
}
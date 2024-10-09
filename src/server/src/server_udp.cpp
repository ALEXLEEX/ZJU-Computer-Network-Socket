/**
 * @file server_udp.h
 * @author lqy
 * @brief Server's UDP class implementation.
 * @date 2024-10-10
 */

#include "../include/server_udp.h"
#include <vector>

/* Public methods */

Server_UDP::Server_UDP(std::string ip, int port, int queueSize, int bufferSize, double timeout):
    Server(ip, port, queueSize, bufferSize, timeout)
{
    clientQueue = std::vector<ClientInfo>(queueSize);
}

Server_UDP::~Server_UDP()
{
    clientQueue.clear();
}

void Server_UDP::run()
{
    printMessage(ServerMsgType::INFO, "The server is now running...");
    startSocketThread();
}

/* Utility functions */

void Server_UDP::getSocket()
{
    serverSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (serverSocket < 0) printMessage(ServerMsgType::ERROR, "Failed to create server socket.");
    printMessage(ServerMsgType::INFO, "Get server socket: " + std::to_string(serverSocket));
}
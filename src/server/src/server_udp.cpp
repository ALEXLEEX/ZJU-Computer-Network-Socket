/**
 * @file server_udp.h
 * @headerfile server_udp.h
 * @author lqy
 * @brief Server's UDP class implementation.
 * @date 2024-10-10
 */

#include "../include/server_udp.h"
#include <vector>
#include <unistd.h>
#include <arpa/inet.h>

/* Public methods */

Server_UDP::Server_UDP(std::string ip, int port, int queueSize, int bufferSize, double timeout):
    Server_Base(ip, port, queueSize, bufferSize, timeout) {}

void Server_UDP::run()
{
    startSocketThread();
    printMessage(ServerMsgType::INFO, "The server is now running...");
    // TODO: add server cmds(quit, ...) here
}

/* Utility functions */

void Server_UDP::getSocket()
{
    serverSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (serverSocket < 0) printMessage(ServerMsgType::ERROR, "Failed to create server socket.");
    printMessage(ServerMsgType::INFO, "Get server socket: " + std::to_string(serverSocket) + ".");
}

void Server_UDP::worker()
{
    char buffer[bufferSize];
    struct sockaddr_in clientAddr;
    socklen_t len = sizeof(clientAddr);

    while (1) {
        ssize_t rc = recvfrom(serverSocket, buffer, sizeof(buffer), 0, (struct sockaddr *)&clientAddr, &len);
        if (rc <= 0) break;
        std::string message(buffer, rc);
        printMessage(ServerMsgType::MSG, "[Client|" + std::string(inet_ntoa(clientAddr.sin_addr)) + ":" + std::to_string(ntohs(clientAddr.sin_port)) + "] " + message);
        // TODO: add server cmds: while(1) -> while(serverStatus) { ... }
        // TODO: add saveConnectInfo.
    }
}

void Server_UDP::saveConnectInfo(ClientAddr clientAddr, int clientStatus)
{
    // TODO: fix whole func
    int id = 0;
    if (clientStatus) { // New connection.
        for (ClientInfo& client: clientQueue) {
            if (!client.getStatus()) {
                client.setStatus(1);
                client.setAddr(clientAddr);
                client.setID(id);
                break;
            }
            id++;
        }
    } else id = queueSize; // Close connection.

    // Close the client's other connections.
    for (ClientInfo& other: clientQueue) {
        if (clientQueue[id] == other) {
            other.setStatus(0); // Clear the status.
        }
    }
}
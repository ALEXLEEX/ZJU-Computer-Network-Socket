/**
 * @file server_udp.h
 * @headerfile server_udp.h
 * @author lqy
 * @brief Server's UDP class implementation.
 * @date 2024-10-10
 */

#include "../include/server_base.h"
#include "../include/server_udp.h"
#include "../include/client_info.h"
#include "../include/utils.h"
#include <vector>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>

/* Public methods */

Server_UDP::Server_UDP(std::string ip, int port, int queueSize, int bufferSize, double timeout):
    Server_Base(ip, port, queueSize, bufferSize, timeout) {}

void Server_UDP::run()
{
    startSocketThread();
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
    ClientAddr clientAddr;
    ClientAddrLen len = sizeof(clientAddr);

    serverStatus = ServerStatus::RUN;
    while (serverStatus == ServerStatus::RUN) {
        ssize_t rc = recvfrom(serverSocket, buffer, sizeof(buffer), 0, (struct sockaddr *)&clientAddr, &len);
        if (rc <= 0) break;
        // TODO: send response to client.
        std::string message(buffer, rc);
        printMessage(ServerMsgType::MSG, "[Client|" + std::string(inet_ntoa(clientAddr.sin_addr)) + ":" + std::to_string(ntohs(clientAddr.sin_port)) + "] " + message);
        // TODO: add handle client request with saveConnectInfo.
    }
}

void Server_UDP::sendResponse(ClientInfo client, std::string message)
{
    message = "[Server] " + message;
    sendto(client.getSocket(), message.c_str(), message.size(), 0, (struct sockaddr *)&client.getAddr(), sizeof(client.getAddr()));
}

void Server_UDP::saveConnectInfo(ClientAddr clientAddr, int clientStatus)
{
    int id = 0;
    if (clientStatus) { // New connection.
        for (ClientInfo& client: clientQueue) {
            if (!client.getStatus()) {
                client.setStatus(1);
                client.setAddr(clientAddr);
                client.setID(id);
                activeClients.insert(id);
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

void Server_UDP::handleRequest(ClientInfo& client, std::string message)
{
    /**
     * @todo Handle client request.
     * @brief 1. Send response to client of receiving message.
     *        2. Decode packet and print to server console.
     *        3. Parse request.
     *        4. Handle request and response.
     *        5. set return code: if the client quits, set return code to -1.
     */
}
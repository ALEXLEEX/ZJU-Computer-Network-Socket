/**
 * @file server_tcp.cpp
 * @headerfile server_tcp.h
 * @author lqy
 * @brief Server's TCP class implementation.
 * @date 2024-10-09
 */

#include "../include/server_base.h"
#include "../include/server_tcp.h"
#include "../include/client_info.h"
#include "../include/utils.h"
#include <vector>
#include <thread>
#include <system_error>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

/* Public methods */

Server_TCP::Server_TCP(std::string ip, int port, int queueSize, int bufferSize, double timeout):
    Server_Base(ip, port, queueSize, bufferSize, timeout) {}

void Server_TCP::run()
{
    startListen();
    startSocketThread();
}

/* Utility functions */

void Server_TCP::getSocket()
{
    serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (serverSocket < 0) printMessage(ServerMsgType::ERROR, "Failed to create server socket.");
    printMessage(ServerMsgType::INFO, "Get server socket: " + std::to_string(serverSocket) + ".");
}

void Server_TCP::startListen()
{
    int rc = listen(serverSocket, queueSize);
    if (rc < 0) printMessage(ServerMsgType::ERROR, "Failed to listen.");
    printMessage(ServerMsgType::INFO, "Start listening on " + serverIp + ":" + std::to_string(serverPort) + ".");
}

void Server_TCP::worker()
{
    // Server starts running, wait and accept.
    while (1) {
        int clientSocket = accept(serverSocket, nullptr, nullptr);
        if (clientSocket < 0) printMessage(ServerMsgType::ERROR, "Failed to accept client.");
        startClientThread(clientSocket);
        // TODO: add server cmds: while(1) -> while(serverStatus) { ... }
    }

    // Server shuts down.
    closeServer();
}

void Server_TCP::startClientThread(int clientSocket)
{
    try {
        std::thread clientThread(&Server_TCP::process, this, clientSocket);
        clientThread.detach();
    } catch (const std::system_error& e) {
        printMessage(ServerMsgType::ERROR, e.what());
    }
}

void Server_TCP::process(int clientSocket)
{
    // Save client connection information.
    ClientInfo& client = saveConnectInfo(clientSocket, std::this_thread::get_id());
    sendResponse(client, "Connected to server(" + serverIp + ":" + std::to_string(serverPort) + ").");
    printMessage(ServerMsgType::INFO, "New client (socketFd = " + std::to_string(clientSocket) + ") from " + client.getIP() + ":" + std::to_string(client.getPort()) + " connected, assigned id = " + std::to_string(client.getID()) + ".");

    // Receive and handle client requests.
    char buffer[bufferSize];
    while (client.getStatus()) {
        int rc = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (rc <= 0) break;
        std::string message(buffer, rc);
        handleRequest(client, message);
    }

    // Close connection.
    closeClient(client);
}

void Server_TCP::sendResponse(ClientInfo client, std::string message)
{
    message = "[Server] " + message;
    send(client.getSocket(), message.c_str(), message.size(), 0);
}

ClientInfo& Server_TCP::saveConnectInfo(int clientSocket, std::thread::id thread)
{
    struct sockaddr_in clientAddr;
    socklen_t len = sizeof(clientAddr);

    // Get client address.
    getpeername(clientSocket, (struct sockaddr *)&clientAddr, &len);

    // Enqueue client information.
    int id = 0;
    for (ClientInfo& thisClient: clientQueue) {
        if (!thisClient.getStatus()) {
            thisClient.setStatus(1);
            thisClient.setSocket(clientSocket);
            thisClient.setAddr(clientAddr);
            thisClient.setThread(thread);
            thisClient.setID(id);
            break;
        }
        id++;
        // TODO: if (id == queueSize + 1) {...}
    }
    ClientInfo& client = clientQueue.at(id);

    // Close the client's other connections.
    for (ClientInfo& other: clientQueue) {
        if (client == other) other.setStatus(0); // Disconnect.
    }
    return client;
}

void Server_TCP::handleRequest(ClientInfo& client, std::string message)
{
    /**
     * @todo Handle client request.
     * @brief 1. Send response to client of receiving message.
     *        2. Decode packet and print to server console.
     *        3. Parse request.
     *        4. Handle request and response.
     *        5. set return code: if the client quits, set return code to -1.
     * @note Maybe can be a base class method.
     */
}

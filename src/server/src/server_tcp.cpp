/**
 * @file server_tcp.cpp
 * @headerfile server_tcp.h
 * @author lqy
 * @brief Server's TCP class implementation.
 * @date 2024-10-09
 */

#include "../include/server_tcp.h"
#include "../include/client_info.h"
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
    printMessage(ServerMsgType::INFO, "The server is now running...");
    // TODO: add server cmds(quit, ...) here
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
    // Wait and accept.
    while (1) {
        int clientSocket = accept(serverSocket, nullptr, nullptr);
        if (clientSocket < 0) printMessage(ServerMsgType::ERROR, "Failed to accept client.");
        startClientThread(clientSocket);
        // TODO: add server cmds: while(1) -> while(serverStatus) { ... }
    }
}

void Server_TCP::startClientThread(int clientSocket)
{
    try {
        std::thread clientThread(&Server_TCP::process, this, clientSocket);
        clientThread.join();
    } catch (const std::system_error& e) {
        printMessage(ServerMsgType::ERROR, e.what());
    }
}

void Server_TCP::process(int clientSocket)
{
    // Save client connection information.
    ClientInfo& client = saveConnectInfo(clientSocket, std::this_thread::get_id());
    sendResponse(client, "Connected to server(" + serverIp + ":" + std::to_string(serverPort) + ").");
    printMessage(ServerMsgType::INFO, "New client(socket=" + std::to_string(clientSocket) + ") " + client.getIP() + ":" + std::to_string(client.getPort()) + "connected, assigned id=" + std::to_string(client.getID()) + ".");

    // Receive and handle client requests.
    char buffer[bufferSize];
    while (client.getStatus()) {
        int rc = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (rc <= 0) break;
        std::string message(buffer, rc);
        printMessage(ServerMsgType::MSG, "[Client|" + std::to_string(client.getID()) + "]" + message);
        handleRequest(client, message);
    }

    // Close connection.
    sendResponse(client, "Disconnected from server(" + serverIp + ":" + std::to_string(serverPort) + ").");
    close(clientSocket); // Close the socket.
    client.setSocket(-1); 
    client.setStatus(0);
    client.setID(-1); // Disconnect.
    printMessage(ServerMsgType::INFO, "Client " + std::to_string(client.getID()) + " disconnected.");
}

ClientInfo& Server_TCP::saveConnectInfo(int clientSocket, std::thread::id thread)
{
    struct sockaddr_in clientAddr;
    socklen_t len = sizeof(clientAddr);

    // Get client address.
    getpeername(clientSocket, (struct sockaddr *)&clientAddr, &len);

    // Enqueue client information.
    int id = 1;
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

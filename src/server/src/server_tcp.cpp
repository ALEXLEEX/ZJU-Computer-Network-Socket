/**
 * @file server_tcp.cpp
 * @author lqy
 * @brief Server's TCP class implementation.
 * @date 2024-10-09
 */

#include "../include/server_tcp.h"
#include <vector>
#include <thread>
#include <system_error>
#include <netinet/in.h>
#include <unistd.h>

/* Public methods */

Server_TCP::Server_TCP(std::string ip, int port, int queueSize, int bufferSize, double timeout):
    Server(ip, port, queueSize, bufferSize, timeout)
{
    clientQueue = std::vector<ClientInfo>(queueSize, ClientInfo());
}

Server_TCP::~Server_TCP()
{
    for (ClientInfo& client: clientQueue) {
        if (client.getStatus()) {
            client.setStatus(0); // Disconnect.
        }
    }
    close(serverSocket);
    clientQueue.clear();
}

void Server_TCP::run()
{
    printMessage(ServerMsgType::INFO, "The server is now running...");
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
    printMessage(ServerMsgType::INFO, "Start listening on " + serverIp + std::to_string(serverPort) + ".");
}

void Server_TCP::worker()
{
    // Wait and accept.
    while (1) {
        int clientSocket = accept(serverSocket, nullptr, nullptr);
        if (clientSocket < 0) printMessage(ServerMsgType::ERROR, "Failed to accept client.");
        printMessage(ServerMsgType::INFO, "New client connected, socket = " + std::to_string(clientSocket));
        startClientThread(clientSocket);
        // TODO: add cmds.
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
    char buffer[bufferSize];
    ClientInfo& client = saveConnectInfo(clientSocket, std::this_thread::get_id());
    while (client.getStatus()) {
        int rc = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (rc <= 0) break; // Receive message.
        std::string message(buffer, rc);
        printMessage(ServerMsgType::MSG, "[Client|" + std::to_string(clientSocket) + "]" + message);
        // TODO: Feedback.
    }
    close(clientSocket); // Close the socket.
    client.setSocket(-1); 
    client.setStatus(0); // Disconnect.
    printMessage(ServerMsgType::INFO, "Client closed, socket = " + std::to_string(clientSocket));
}

ClientInfo& Server_TCP::saveConnectInfo(int clientSocket, std::thread::id thread)
{
    struct sockaddr_in clientAddr;
    socklen_t len = sizeof(clientAddr);

    // Get client address.
    getpeername(clientSocket, (struct sockaddr *)&clientAddr, &len);

    in_addr_t clientIp = clientAddr.sin_addr.s_addr;
    in_port_t clientPort = clientAddr.sin_port;

    // Enqueue client information.
    int index1 = 0, index2 = 0;
    for (ClientInfo& client: clientQueue) {
        if (!client.getStatus()) {
            client.setStatus(1);
            client.setSocket(clientSocket);
            client.setIP(clientIp);
            client.setPort(clientPort);
            client.setThread(thread);
            break;
        }
        index1++;
    }

    // Close the client's other connections.
    for (ClientInfo& client: clientQueue) {
        if (index1 != index2 && client.getStatus() && client.getIP() == clientQueue[index1].getIP() && client.getPort() == clientQueue[index1].getPort()) {
            client.setStatus(0); // Disconnect.
        }
        index2++;
    }

    return clientQueue.at(index1);
}
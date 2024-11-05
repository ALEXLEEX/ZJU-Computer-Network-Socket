/**
 * @file server.cpp
 * @headerfile server.h
 * @author lqy
 * @brief Server's base class implementation.
 * @date 2024-10-08
 */

#include "../include/server_base.h"
#include "../include/client_info.h"
#include "../include/utils.h"
#include "../../packet/include/packet.h"
#include "../../packet/include/utils.h"
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
    serverSocket(-1), serverIp(ip), serverPort(port), serverStatus(ServerStatus::UNINIT),
    queueSize(queueSize), bufferSize(bufferSize), timeout(timeout), packetID(PacketID(0))
{
    clientQueue = std::vector<ClientInfo>(queueSize, ClientInfo());
    activeClients = ActiveClients();
    printMessage(ServerMsgType::WELCOME, "Welcome to the server!!!");
}

Server_Base::~Server_Base()
{
    closeServer();
    clientQueue.clear();
    activeClients.clear();
    printMessage(ServerMsgType::GOODBYE, "Server exited, GOODBYE!!!");
}

void Server_Base::init()
{
    printMessage(ServerMsgType::NOTE, "Server is initializing...");
    getSocket();
    setOptions();
    bindAddress();
    serverStatus = ServerStatus::READY;
    printMessage(ServerMsgType::NOTE, "Server initialization completed.");
}

void Server_Base::stop()
{
    switch (serverStatus) {
        case ServerStatus::RUN:
            closeServer();
            printMessage(ServerMsgType::NOTE, "Server shuts down.");
            break;
        default:
            printMessage(ServerMsgType::WARNING, "Server is not running.");
    }
    return ;
}

void Server_Base::quit()
{
    serverStatus = ServerStatus::EXIT;
    printMessage(ServerMsgType::NOTE, "Exitting the server...");
}

/* Utility functions */

void Server_Base::printMessage(ServerMsgType msgType, std::string msg)
{
    switch (msgType) {
        case ServerMsgType::MSG:
            std::cout << "\033[37m" << msg << "\033[0m" << std::endl;
            break;
        case ServerMsgType::WELCOME:
            std::cout << "\033[1;31m[WELCOME] " << msg << "\033[0m" << std::endl;
            break;
        case ServerMsgType::INFO:
            std::cout << "\033[32m[Server] " << msg << "\033[0m" << std::endl;
            break;
        case ServerMsgType::NOTE:
            std::cout << "\033[34m[Server] " << msg << "\033[0m" << std::endl;
            break;
        case ServerMsgType::WARNING:
            std::cout << "\033[33m[WARNING] " << msg << "\033[0m" << std::endl;
            break;
        case ServerMsgType::ERROR:
            std::cerr << "\033[31m[ERROR] " << msg << "\033[0m" << std::endl;
            closeServer();
            exit(EXIT_FAILURE);
        case ServerMsgType::GOODBYE:
            std::cout << "\033[1;31m[GOODBYE] " << msg << "\033[0m" << std::endl;
            break;
    }
    return ;
}

void Server_Base::setOptions()
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

void Server_Base::bindAddress()
{
    struct sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = serverIp.size() ? inet_addr(serverIp.c_str()) : INADDR_ANY;
    serverAddress.sin_port = htons(serverPort);

    if (bind(serverSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0) {
        close(serverSocket);
        printMessage(ServerMsgType::ERROR, "Failed to bind address.");
    }
    printMessage(ServerMsgType::INFO, "Bind server to IP: " + serverIp + ", port: " + std::to_string(serverPort) + ".");
}

void Server_Base::startSocketThread()
{
    printMessage(ServerMsgType::NOTE, "The server is now running...");
    try {
        std::thread thread(&Server_Base::worker, this); // Main thread.
        thread.detach();
    } catch (const std::system_error& e) {
        printMessage(ServerMsgType::ERROR, e.what());
    }
}

void Server_Base::sendAssignment(ClientInfo& client, ContentType type, std::string message)
{
    Packet packet(SERVER_INFO, PacketType::ASSIGNMENT, packetID++, type);
    packet.addArg(message);
    send2Client(client, packet.encode());
}

void Server_Base::broadcastMessage(ContentType type, std::string message)
{
    for (ClientID id: activeClients) {
        ClientInfo& client = clientQueue.at(id);
        sendAssignment(client, type, message);
    }
}

void Server_Base::closeClient(ClientInfo& client)
{
    if (client.getStatus()) {
        ClientID id = client.getID();
        send2Client(client, "Disconnected from server (" + serverIp + ":" + std::to_string(serverPort) + ").");
        close(client.getSocket());
        client.setStatus(0); // Shut down the sub thread.
        activeClients.erase(id);
        if (serverStatus == ServerStatus::RUN)
            broadcastMessage(ContentType::AssignmentClientLogout, "Client " + std::to_string(id) + " logged out.");
        printMessage(ServerMsgType::INFO, "Client " + std::to_string(client.getID()) + " disconnected from server.");
    }
    return ;
}

void Server_Base::closeServer()
{
    if (serverStatus == ServerStatus::RUN) {
        serverStatus = ServerStatus::STOP; // Set server status.
        // Disconnect all clients.
        for (ClientInfo& client: clientQueue) closeClient(client);
    }
    // Close the server socket.
    if (serverSocket >= 0) {
        close(serverSocket);
        serverSocket = -1;
    }
}
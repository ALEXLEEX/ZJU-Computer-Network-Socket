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
#include "../include/config.h"
#include "../../packet/include/packet.h"
#include "../../packet/include/utils.h"
#include <vector>
#include <thread>
#include <system_error>
#include <algorithm>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

/* Public methods */

Server_TCP::Server_TCP(std::string ip, int port, int queueSize, int bufferSize, double timeout):
    Server_Base(ip, port, queueSize, bufferSize, timeout) {}

void Server_TCP::run()
{
    if (serverStatus != ServerStatus::READY) {
        printMessage(ServerMsgType::WARNING, "Server is not ready, type `init` to get ready.");
        return ;
    }
    startListen();
    startSocketThread();
}

/* Utility functions */

void Server_TCP::getSocket()
{
    if (serverSocket >= 0) close(serverSocket);
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
    serverStatus = ServerStatus::RUN;
    while (serverStatus == ServerStatus::RUN) {
        int clientSocket = accept(serverSocket, nullptr, nullptr);
        if (clientSocket < 0) printMessage(ServerMsgType::ERROR, "Failed to accept client.");
        startClientThread(clientSocket);
    }
    return ;
}

void Server_TCP::startClientThread(int clientSocket)
{
    try {
        std::thread clientThread(&Server_TCP::process, this, clientSocket);
        clientThread.detach();
    } catch (const std::system_error& e) {
        printMessage(ServerMsgType::ERROR, e.what());
    }
    return ;
}

void Server_TCP::process(int clientSocket)
{
    // Save client connection information.
    ClientInfo& client = saveConnectInfo(clientSocket, std::this_thread::get_id());
    // TODO: Send connection message to this client.
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

void Server_TCP::sendAssignment(ClientInfo& client, ContentType type, std::string message)
{
    Packet packet(SERVER_INFO, PacketType::ASSIGNMENT, packetID++, type);
    packet.addArg(message);
    send2Client(client, packet.encode());
}

void Server_TCP::broadcastMessage(ContentType type, std::string message)
{
    for (ClientID id: activeClients) {
        ClientInfo& client = clientQueue.at(id);
        sendAssignment(client, type, message);
    }
}

void Server_TCP::send2Client(ClientInfo client, std::string message)
{
    send(client.getSocket(), message.c_str(), message.size(), 0);
}

ClientInfo& Server_TCP::saveConnectInfo(int clientSocket, std::thread::id thread)
{
    struct sockaddr_in clientAddr;
    socklen_t len = sizeof(clientAddr);

    // Get client address.
    getpeername(clientSocket, (struct sockaddr *)&clientAddr, &len);

    // Enqueue client information.
    ClientID id = 0;
    for (ClientInfo& thisClient: clientQueue) {
        if (!thisClient.getStatus()) {
            thisClient.setStatus(1);
            thisClient.setSocket(clientSocket);
            thisClient.setAddr(clientAddr);
            thisClient.setThread(thread);
            thisClient.setID(id);
            broadcastMessage(ContentType::AssignmentClientLogin, "Client " + std::to_string(id) + " logged in.");
            activeClients.insert(id);
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
    Packet request(CLIENT_INFO);
    if (request.decode(message)) {
        std::vector<std::string> args = request.getArgs();
        Packet response(SERVER_INFO, PacketType::RESPONSE, packetID++);
        switch (request.getContent()) {
            case ContentType::RequestCityName: {
                response.setContent(ContentType::ResponseCityName);
                int cityID = std::stoi(args[0]);
                if (cityID <= CityNums) {
                    response.addArg("1"); // Arg 1: success.
                    response.addArg(CityNames.at(cityID)); // Arg 2: city name.
                } else {
                    response.addArg("0"); // Arg 1: failure.
                    response.addArg("No matching city for ID " + args[0] + "."); // Arg 2: error message.
                }
                printMessage(ServerMsgType::INFO, "Client " + std::to_string(client.getID()) + " requested city name for ID " + args[0] + ".");
                break;
            }
            case ContentType::RequestWeatherInfo: {
                response.setContent(ContentType::ResponseWeatherInfo);
                int cityID = std::stoi(args[0]);
                std::string date = args[1] + "-" + args[2] + "-" + args[3];
                if (cityID <= WeatherInfo.size()) {
                    if (WeatherInfo.at(cityID).count(date)) {
                        response.addArg("1"); // Arg 1: success.
                        response.addArg(CityNames.at(cityID)); // Arg 2: city name.
                        response.addArg(WeatherInfo.at(cityID).at(date)); // Arg 3: weather information.
                    } else {
                        response.addArg("0"); // Arg 1: failure.
                        response.addArg("No weather information for date " + date + "."); // Arg 2: error message.
                    }
                } else {
                    response.addArg("0"); // Arg 1: failure.
                    response.addArg("No matching city for ID " + args[0] + "."); // Arg 2: error message.
                }
                printMessage(ServerMsgType::INFO, "Client " + std::to_string(client.getID()) + " requested weather information for city ID " + args[0] + " and date " + date + ".");
                break;
            }
            case ContentType::RequestClientList: {
                response.setContent(ContentType::ResponseClientList);
                response.addArg("1"); // Arg 1: success.
                response.addArg(std::to_string(activeClients.size())); // Arg 2: Active client number.
                std::vector<ClientID> clientIDs(activeClients.begin(), activeClients.end());
                std::sort(clientIDs.begin(), clientIDs.end());
                for (ClientID id: clientIDs) {
                    response.addArg(std::to_string(id) + "," + clientQueue.at(id).getIP() + ":" + std::to_string(clientQueue.at(id).getPort())); // Arg n: client info.
                }
                printMessage(ServerMsgType::INFO, "Client " + std::to_string(client.getID()) + " requested client list.");
                break;
            }
            case ContentType::RequestSendMessage: {
                response.setContent(ContentType::ResponseSendMessage);
                ClientID targetID = std::stoi(args[0]);
                if (targetID <= queueSize) {
                    ClientInfo& target = clientQueue.at(targetID);
                    if (target.getStatus()) {
                        std::string message = "Message from client " + std::to_string(client.getID()) + ": " + args[1];
                        sendAssignment(target, ContentType::AssignmentSendMessage, message);
                        response.addArg("1"); // Arg 1: success.
                    } else {
                        response.addArg("0"); // Arg 1: failure.
                        response.addArg("Client " + std::to_string(targetID) + " not exists."); // Arg 2: error message.
                    }
                } else {
                    response.addArg("0"); // Arg 1: failure.
                    response.addArg("Invalid client ID " + args[0] + "."); // Arg 2: error message.
                }
                printMessage(ServerMsgType::INFO, "Client " + std::to_string(client.getID()) + " requested to send message to client " + args[0] + ".");
                break;
            }
            default:
                response.setContent(ContentType::ResponseUnknown);
                response.addArg("Unknown request type.");
                printMessage(ServerMsgType::INFO, "Client " + std::to_string(client.getID()) + " requested unknown request type.");
        }
        send2Client(client, response.encode());
    }
    return ;
}

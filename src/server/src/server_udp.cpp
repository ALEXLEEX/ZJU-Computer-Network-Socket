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
#include "../include/config.h"
#include "../../packet/include/packet.h"
#include "../../packet/include/utils.h"
#include <vector>
#include <algorithm>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>

/* Public methods */

Server_UDP::Server_UDP(std::string ip, int port, int queueSize, int bufferSize, double timeout):
    Server_Base(ip, port, queueSize, bufferSize, timeout) {}

void Server_UDP::run()
{
    if (serverStatus != ServerStatus::READY) {
        printMessage(ServerMsgType::WARNING, "Server is not ready, type `init` to get ready.");
        return ;
    }
    startSocketThread();
}

/* Utility functions */

void Server_UDP::getSocket()
{
    if (serverSocket >= 0) close(serverSocket);
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
        std::string message(buffer, rc);
        handleRequest(clientAddr, message);
    }
}

void Server_UDP::sendAssignment(ClientAddr clientAddr, ContentType type, std::string message)
{
    Packet packet(SERVER_INFO, PacketType::ASSIGNMENT, packetID++, type);
    packet.addArg(message);
    send2Client(clientAddr, packet.encode());
}

void Server_UDP::broadcastMessage(ContentType type, std::string message)
{
    for (ClientID id: activeClients) {
        ClientInfo& client = clientQueue.at(id);
        sendAssignment(client.getAddr(), type, message);
    }
}

void Server_UDP::send2Client(ClientAddr clientAddr, std::string message)
{
    sendto(serverSocket, message.c_str(), message.size(), 0, (struct sockaddr *)&clientAddr, sizeof(clientAddr));
}

bool Server_UDP::saveConnectInfo(ClientAddr clientAddr, int clientStatus)
{
    switch (clientStatus) {
        case 0: { // Close connection.
            ActiveClients activeClientsCopy = activeClients;
            // Close the client's all connections.
            for (ClientID id: activeClientsCopy) {
                ClientInfo& client = clientQueue.at(id);
                if (client == clientAddr) {
                    closeClient(client);
                    return true;
                }
            }
            break;
        }
        case 1: { // New connection.
            ClientID id = 0;
            bool isSuccess = false;
            // Enqueue.
            for (ClientInfo& client: clientQueue) {
                if (!client.getStatus()) {
                    client.setStatus(1);
                    client.setAddr(clientAddr);
                    client.setID(id);
                    activeClients.insert(id);
                    isSuccess = true;
                    broadcastMessage(ContentType::AssignmentClientLogin, "Client " + std::to_string(id) + " logged in.");
                    printMessage(ServerMsgType::INFO, "Client from " + std::string(inet_ntoa(clientAddr.sin_addr)) + ":" + std::to_string(ntohs(clientAddr.sin_port)) + " connected.");
                    break;
                }
                id++;
                // TODO: if (id == queueSize + 1) {...}
            }

            // Close the client's other connections.
            for (ClientInfo& other: clientQueue) {
                if (clientQueue[id] == other) closeClient(other);
            }
            return isSuccess;
        }
    }
    return false;
}

bool Server_UDP::isConnected(ClientAddr clientAddr)
{
    for (ClientID id: activeClients)
        if (clientQueue.at(id) == clientAddr) return true;
    return false;
}

void Server_UDP::handleRequest(ClientAddr clientAddr, std::string message)
{
    Packet request(CLIENT_INFO);
    if (request.decode(message)) {
        std::vector<std::string> args = request.getArgs();
        Packet response(SERVER_INFO, PacketType::RESPONSE, packetID++);
        switch (request.getContent()) {
            case ContentType::RequestCityName: {
                response.setContent(ContentType::ResponseCityName);
                if (isConnected(clientAddr)) {
                    int cityID = std::stoi(args[0]);
                    if (cityID <= CityNums) {
                        response.addArg("1"); // Arg 1: success.
                        response.addArg(CityNames.at(cityID)); // Arg 2: city name.
                    } else {
                        response.addArg("0"); // Arg 1: failure.
                        response.addArg("No matching city for ID " + args[0] + "."); // Arg 2: error message.
                    }
                } else {
                    response.addArg("0"); // Arg 1: failure.
                    response.addArg("Client not connected."); // Arg 2: error message.
                }
                break;
            }
            case ContentType::RequestWeatherInfo: {
                response.setContent(ContentType::ResponseWeatherInfo);
                if (isConnected(clientAddr)) {
                    int cityID = std::stoi(args[0]);
                    if (cityID <= WeatherInfo.size()) {
                        std::string date = args[1] + "-" + args[2] + "-" + args[3];
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
                } else {
                    response.addArg("0"); // Arg 1: failure.
                    response.addArg("Client not connected."); // Arg 2: error message.
                }
                break;
            }
            case ContentType::RequestClientList: {
                response.setContent(ContentType::ResponseClientList);
                if (isConnected(clientAddr)) {
                    response.addArg("1"); // Arg 1: success.
                    response.addArg(std::to_string(activeClients.size())); // Arg 2: Active client number.
                    std::vector<ClientID> clientIDs(activeClients.begin(), activeClients.end());
                    std::sort(clientIDs.begin(), clientIDs.end());
                    for (ClientID id: clientIDs) {
                        response.addArg(std::to_string(id) + "," + clientQueue.at(id).getIP() + ":" + std::to_string(clientQueue.at(id).getPort())); // Arg n: client info.
                    }
                } else {
                    response.addArg("0"); // Arg 1: failure.
                    response.addArg("Client not connected."); // Arg 2: error message.
                }
                break;
            }
            case ContentType::RequestSendMessage: {
                response.setContent(ContentType::ResponseSendMessage);
                if (isConnected(clientAddr)) {
                    ClientID targetID = std::stoi(args[0]);
                    if (targetID <= queueSize) {
                        ClientInfo& target = clientQueue.at(targetID);
                        if (target.getStatus()) {
                            std::string message = "Message received: " + args[1];
                            sendAssignment(target.getAddr(), ContentType::AssignmentSendMessage, message);
                            response.addArg("1"); // Arg 1: success.
                        } else {
                            response.addArg("0"); // Arg 1: failure.
                            response.addArg("Client " + std::to_string(targetID) + " not exists."); // Arg 2: error message.
                        }
                    } else {
                        response.addArg("0"); // Arg 1: failure.
                        response.addArg("Invalid client ID " + args[0] + "."); // Arg 2: error message.
                    }
                } else {
                    response.addArg("0"); // Arg 1: failure.
                    response.addArg("Client not connected."); // Arg 2: error message.
                }
                break;
            }
            case ContentType::RequestMakeConnection: {
                response.setContent(ContentType::ResponseMakeConnection);
                response.addArg(saveConnectInfo(clientAddr, 1) ? "1" : "0");
                break;
            }
            case ContentType::RequestCloseConnection: {
                response.setContent(ContentType::ResponseCloseConnection);
                response.addArg(saveConnectInfo(clientAddr, 0) ? "1" : "0");
                break;
            }
            default: {
                response.setContent(ContentType::ResponseUnknown);
                response.addArg("Unknown request type.");
            }
        }
        send2Client(clientAddr, response.encode());
    }
    return ;
}
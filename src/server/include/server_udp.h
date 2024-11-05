/**
 * @file server_tcp.h
 * @author lqy
 * @brief Server's TCP class interface.
 * @date 2024-10-10
 */

#ifndef _SERVER_UDP_H
#define _SERVER_UDP_H

/* Header files */
#include "server_base.h"
#include "client_info.h"
#include "../../packet/include/utils.h"
#include <string>
#include <netinet/in.h>

/* Main class */

/**
 * @class Server_UDP
 * @brief UDP server class
 */
class Server_UDP : public Server_Base {
public:

    /**
     * Constructor of `Server_UDP` class to initialize the server.
     * @param ip Server ip
     * @param port Server port
     * @param queueSize Server queue waiting clients for connection
     * @param bufferSize Server buffer for receiving data
     * @param timeout Server waiting connection timeout
     */
    Server_UDP(std::string ip, int port, int queueSize, int bufferSize, double timeout);

    /**
     * Destructor of `Server_UDP` class.
     */
    virtual ~Server_UDP() = default;

    /**
     * Server runner.
     */
    virtual void run();

private:

    /* Get server socket */
    virtual void getSocket();

    /* Run the server by this worker. */
    virtual void worker();

    /* Send assignment message to client. */
    void sendAssignment(ClientAddr clientAddr, ContentType type, std::string message);

    /* Broadcast message to all active clients. */
    virtual void broadcastMessage(ContentType type, std::string message);

    /* Send encoded packet to client. */
    void send2Client(ClientAddr clientAddr, std::string message);

    /* Save client connection info. */
    bool saveConnectInfo(ClientAddr clientAddr, int clientStatus);

    /* Whether the client is connected. */
    bool isConnected(ClientAddr clientAddr);

    /* Handle client request. */
    void handleRequest(ClientAddr clientAddr, std::string message);

};

#endif /* _SERVER_UDP_H */
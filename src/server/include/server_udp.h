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
    Server_UDP(std::string ip, int port, int queueSize = SERVER_QUEUE_SIZE, int bufferSize = SERVER_BUFFER_SIZE, double timeout = SERVER_TIMEOUT);

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

    /* Send response to client. */
    virtual void sendResponse(ClientInfo client, std::string message);

    /* Save client connection info. */
    void saveConnectInfo(ClientAddr clientAddr, int clientStatus);

    /* Handle client request. */
    void handleRequest(ClientInfo& client, std::string message);

};

#endif /* _SERVER_UDP_H */
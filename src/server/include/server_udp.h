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
    ~Server_UDP() = default;

    /**
     * Server runner.
     */
    virtual void run();

private:

    virtual void getSocket(); // Get server socket.

    virtual void worker(); // Server worker.

    void saveConnectInfo(ClientAddr clientAddr, int clientStatus); // Save client connection info.

};

#endif /* _SERVER_UDP_H */
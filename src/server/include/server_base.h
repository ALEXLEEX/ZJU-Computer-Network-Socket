/**
 * @file server_base.h
 * @author lqy
 * @brief Server's base class interface.
 * @date 2024-10-01
 */

#ifndef _SERVER_BASE_H
#define _SERVER_BASE_H

/* Header files */

#include "utils.h"
#include "client_info.h"
#include <string>
#include <vector>

/* Main class */

/**
 * @class Server_Base
 * @brief Server's base class for TCP type server and UDP type server
 */
class Server_Base {
public:

    /**
     * Constructor of `Server_Base` class to initialize the server.
     * @param ip Server ip
     * @param port Server port
     * @param queueSize Server queue waiting clients for connection
     * @param bufferSize Server buffer for receiving data
     * @param timeout Server waiting connection timeout
     */
    Server_Base(std::string ip, int port, int queueSize = SERVER_QUEUE_SIZE, int bufferSize = SERVER_BUFFER_SIZE, double timeout = SERVER_TIMEOUT);

    /**
     * Destructor of `Server_Base` class.
     */
    virtual ~Server_Base();

    /**
     * Server initializor by creating a socket, setting socket options, and binding it to the specified IP address and port.
     */
    void init();

    /**
     * Server runner.
     */
    virtual void run() = 0;

protected:

    int serverSocket; // Server socket.
    std::string serverIp; // Server IP.
    int serverPort; // Server port.
    int queueSize; // Server waiting client queue size.
    int bufferSize; // Server received message buffer size.
    double timeout; // Server waiting timeout.
    std::vector<ClientInfo> clientQueue; // Client queue.

    /* Print messages with different types. */
    void printMessage(ServerMsgType msgType, std::string msg);

    /* Get server socket. */
    virtual void getSocket() = 0;

    /* Set server socket options. */
    void setOptions(double timeout);

    /* Bind server IP address and port. */
    void bindAddress(std::string serverIp, int serverPort);

    /* Start server main thread. */
    void startSocketThread();

    /* Running the server by this worker. */
    virtual void worker() = 0;

    /* Send response to client. */
    virtual void sendResponse(ClientInfo client, std::string message) = 0;

    /* Close the connection from the client. */
    void closeClient(ClientInfo& client);

};

#endif /* _SERVER_BASE_H */
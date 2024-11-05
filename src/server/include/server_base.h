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
#include "../../packet/include/utils.h"
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
    Server_Base(std::string ip, int port, int queueSize, int bufferSize, double timeout);

    /**
     * Destructor of `Server_Base` class.
     */
    virtual ~Server_Base();

    /**
     * Server initializor by creating a socket, setting socket options,
     * and binding it to the specified IP address and port.
     */
    void init();

    /**
     * Server runner.
     */
    virtual void run() = 0;

    /**
     * Stop the server.
     */
    void stop();

    /**
     * Stop and exit the server.
     */
    void quit();

    /**
     * Broadcast weather warning message to all active clients.
     */
    void weatherWarning();

protected:

    int serverSocket; // Server socket.
    std::string serverIp; // Server IP.
    int serverPort; // Server port.
    ServerStatus serverStatus; // Server status.
    int queueSize; // Server waiting client queue size.
    int bufferSize; // Server received message buffer size.
    double timeout; // Server waiting timeout.
    std::vector<ClientInfo> clientQueue; // Client queue.
    ActiveClients activeClients; // Active clients.
    PacketID packetID; // Packet ID.

    /* Print messages with different types. */
    void printMessage(ServerMsgType msgType, std::string msg);

    /* Get server socket. */
    virtual void getSocket() = 0;

    /* Set server socket options. */
    void setOptions();

    /* Bind server IP address and port. */
    void bindAddress();

    /* Start server main thread. */
    void startSocketThread();

    /* Running the server by this worker. */
    virtual void worker() = 0;

    /* Broadcast message to all active clients. */
    virtual void broadcastMessage(ContentType type, std::string message) = 0;

    /* Close the connection from the client. */
    void closeClient(ClientInfo& client);

    /* Close the server. */
    void closeServer();

};

#endif /* _SERVER_BASE_H */
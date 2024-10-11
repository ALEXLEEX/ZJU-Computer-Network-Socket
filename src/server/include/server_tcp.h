/**
 * @file server_tcp.h
 * @author lqy
 * @brief Server's TCP class interface.
 * @version 0.1.0
 * @date 2024-10-09
 */

#ifndef _SERVER_TCP_H
#define _SERVER_TCP_H

/* Header files */

#include "server_base.h"
#include "client_info.h"
#include <string>

/* Main class */

/**
 * @class Server_TCP
 * @brief TCP server class
 */
class Server_TCP : public Server_Base {
public:

    /**
     * Constructor of `Server_TCP` class to initialize the server.
     * @param ip Server ip
     * @param port Server port
     * @param queueSize Server queue waiting clients for connection
     * @param bufferSize Server buffer for receiving data
     * @param timeout Server waiting connection timeout
     */
    Server_TCP(std::string ip, int port, int queueSize = SERVER_QUEUE_SIZE, int bufferSize = SERVER_BUFFER_SIZE, double timeout = SERVER_TIMEOUT);

    /**
     * Destructor of `Server_TCP` class.
     * @note It will close all TCP sockets and threads.
     */
    ~Server_TCP() = default;

    /**
     * Server runner.
     */
    virtual void run();

private:

    virtual void getSocket(); // Get server socket.

    void startListen(); // Start listening.

    virtual void worker(); // Server worker.

    void startClientThread(int clientSocket); // Start a client thread.

    void process(int clientSocket); // Process client message.

    ClientInfo& saveConnectInfo(int clientSocket, std::thread::id thread); // Save client connection info.

    void handleRequest(ClientInfo& client, std::string message); // Handle client request.

};

#endif /* _SERVER_TCP_H */
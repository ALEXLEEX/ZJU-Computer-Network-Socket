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
    virtual ~Server_TCP() = default;

    /**
     * Server runner.
     */
    virtual void run();

private:

    /* Get server socket. */
    virtual void getSocket();

    /* Start listening. */
    void startListen();

    /* Run the server by this worker. */
    virtual void worker();

    /* Start a client thread. */
    void startClientThread(int clientSocket);

    /* Run the client thread by this worker. */
    void process(int clientSocket);

    /* Send response to client. */
    virtual void sendResponse(ClientInfo client, std::string message);

    /* Save client connection info. */
    ClientInfo& saveConnectInfo(int clientSocket, std::thread::id thread);

    /* Handle client request. */
    void handleRequest(ClientInfo& client, std::string message);

};

#endif /* _SERVER_TCP_H */
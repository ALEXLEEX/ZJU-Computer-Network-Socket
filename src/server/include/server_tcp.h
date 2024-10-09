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

#include "server.h"
#include <vector>
#include <netinet/in.h>
#include <thread>

/* Utilitis */

class ClientInfo {
public:
    ClientInfo(): status(0), socket(-1), ip(0), port(0), thread(0) {};
    int getStatus() const { return status; }
    int getSocket() const { return socket; }
    in_addr_t getIP() const { return ip; }
    in_port_t getPort() const { return port; }
    std::thread::id getThread() const { return thread; }
    void setStatus(int status) { this->status = status; }
    void setSocket(int socket) { this->socket = socket; }
    void setIP(in_addr_t ip) { this->ip = ip; }
    void setPort(in_port_t port) { this->port = port; }
    void setThread(std::thread::id thread) { this->thread = thread; }
private:
    int status; // 0: unconnected; 1: connected.
    int socket; // TCP socket.
    in_addr_t ip;
    in_port_t port;
    std::thread::id thread;
};

/* Main class */

/**
 * @class Server_TCP
 * @brief TCP server class
 */
class Server_TCP : public Server {
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
    ~Server_TCP();

    /**
     * Server runner.
     */
    virtual void run();

    void cmds();

private:

    std::vector<ClientInfo> clientQueue; // Client queue.

    virtual void getSocket(); // Get server socket.

    void startListen(); // Start listening.

    virtual void worker(); // Server worker.

    void startClientThread(int clientSocket); // Start a client thread.

    void process(int clientSocket); // Process client message.

    ClientInfo& saveConnectInfo(int clientSocket, std::thread::id thread); // Save client connection info.

};

#endif /* _SERVER_TCP_H */
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
#include <vector>
#include <thread>
#include <netinet/in.h>

/* Utilties */

class ClientInfo {
public:
    ClientInfo(): status(0), ip(0), port(0) {};
    int getStatus() const { return status; }
    in_addr_t getIP() const { return ip; }
    in_port_t getPort() const { return port; }
    void setStatus(int status) { this->status = status; }
    void setIP(in_addr_t ip) { this->ip = ip; }
    void setPort(in_port_t port) { this->port = port; }
private:
    int status; // 0: unconnected; 1: connected.
    in_addr_t ip;
    in_port_t port;
};

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
    ~Server_UDP();

    /**
     * Server runner.
     */
    virtual void run();

    void cmds();

private:

    std::vector<ClientInfo> clientQueue; // Client queue.

    virtual void getSocket(); // Get server socket.

    virtual void worker(); // Server worker.

    void saveConnectInfo(in_addr_t clientIp, in_port_t clientPort, int clientStatus); // Save client connection info.

};

#endif /* _SERVER_UDP_H */
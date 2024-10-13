/**
 * @file server.h
 * @author lqy
 * @brief Wrapper for server classes.
 * @date 2024-10-10
 * @note This class provides the interface for users.
 */

#ifndef _SERVER_H
#define _SERVER_H

/* Header files */

#include "server_base.h"
#include "server_tcp.h"
#include "server_udp.h"
#include <string>

/* Main class */

/**
 * @class Server
 * @brief Wrapper for server classes.
 */
class Server {
public:

    /**
     * Constructor of `Server` class to initialize the server.
     * @param type Server type in string format: `TCP` or `UDP`
     */
    Server(std::string type);

    /**
     * Destructor of `Server` class.
     */
    ~Server();

    /**
     * Server initializor by creating a socket, setting socket options, and binding it to the specified IP address and port.
     */
    void init();

    /**
     * Server runner.
     */
    void run();

private:

    Server_Base* server;

};

#endif /* _SERVER_H */
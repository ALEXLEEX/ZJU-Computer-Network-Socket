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
     * Read server commands and parse and pass to server.
     * @note Supported commands:
     * @note - `init`: initialize the server.
     * @note - `run`/`start`: run the server.
     * @note - `stop`: stop the server running.
     * @note - `quit`/`exit`: quit the server.
     * @note - `help`: show help information.
     */
    void cmds();

    /**
     * Server helping menu.
     * @note Show the supported commands and their usage.
     */
    void help();

private:

    Server_Base* server;
    std::string ip;
    int port;
    int queueSize;

};

#endif /* _SERVER_H */
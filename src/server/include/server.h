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
     * @param ip Server ip address in string format.
     * @param port Server port number.
     * @param queueSize Maximum number of messages in the queue.
     * @param bufferSize Maximum size of each message in bytes.
     * @param timeout Timeout for receiving messages in seconds.
     */
    Server(std::string type, std::string ip = SERVER_IP, int port = SERVER_PORT, int queueSize = SERVER_QUEUE_SIZE, int bufferSize = SERVER_BUFFER_SIZE, double timeout = SERVER_TIMEOUT);

    /**
     * Constructor of `Server` class to initialize the server.
     * @param argc Number of command line arguments.
     * @param argv Command line arguments.
     * @note This constructor provides the interface for CLIs.
     */
    Server(int argc, char *argv[]);

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
     * @note - `broadcast`: broadcast weather warning message to all clients.
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

};

#endif /* _SERVER_H */
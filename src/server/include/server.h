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

    Server(std::string type);

    ~Server();

    void init();

    void run();

private:

    Server_Base* server;

};

#endif /* _SERVER_H */
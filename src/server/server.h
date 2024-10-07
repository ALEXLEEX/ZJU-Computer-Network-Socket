/**
 * @file server.h
 * @author lqy
 * @brief Server header file.
 * @version 0.1.0
 * @date 2024-10-01
 */

#ifndef _SERVER_H
#define _SERVER_H

#include "micros.h"

class Server {
public:
    Server();
    ~Server();
    int init(int type);
    void run();
private:
    int serverSocket; // Server socket.
};

#endif /* _SERVER_H */
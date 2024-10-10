/**
 * @file server.cpp
 * @headerfile server.h
 * @author lqy
 * @brief Wrapper for server classes.
 * @date 2024-10-10
 */

#include "../include/server.h"

/* Public methods */
Server::Server(std::string type)
{
    if (type == "TCP") {
        server = new Server_TCP(SERVER_IP, SERVER_PORT);
    } else if (type == "UDP") {
        server = new Server_UDP(SERVER_IP, SERVER_PORT);
    } else {
        server = nullptr;
    }
}

Server::~Server()
{
    delete server;
}

/* Utility functions */
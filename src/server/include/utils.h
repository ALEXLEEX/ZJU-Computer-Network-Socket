/**
 * @file utils.h
 * @author lqy
 * @brief Utilities for server.
 * @date 2024-10-07
 */

#ifndef _SERVER_UTILS_H
#define _SERVER_UTILS_H

#include <sys/socket.h>
#include <vector>
#include <unordered_set>

/* Client ID and address type. */
using ClientID = int;
using ClientAddr = struct sockaddr_in;
using ClientAddrLen = socklen_t;
using ActiveClients = std::unordered_set<ClientID>;

/* Server type. */
enum class ServerType {
    TCP,
    UDP
};

/* Server status type. */
enum class ServerStatus {
    UNINIT, // Uninitialized.
    READY, // Initialized and ready.
    RUN, // Running.
    STOP, // Stopped.
    EXIT 
};

/* Server message type. */
enum class ServerMsgType {
    MSG, // Message.
    WELCOME, // Welcome message.
    INFO, // Information.
    NOTE, // Note.
    WARNING, // Warning.
    ERROR, // Server error.
    GOODBYE // Goodbye message.
};

/* Client && Server info. */
#define CLIENT_INFO "2682"
#define SERVER_INFO "3373"

/* Server queue size. */
#define SERVER_QUEUE_SIZE 256 // Maximum number of waiting clients in server queue.

/* Server IP and port. */
#define SERVER_IP "127.0.0.1" // Server ip.
#define SERVER_PORT 3373 // Server port.

/* Server buffer size. */
#define SERVER_BUFFER_SIZE 4096 // Maximum size of received data in server buffer.

/* Server timeout. */
#define SERVER_TIMEOUT 10.0 // Maximum time to wait for client response.

/* Server debugger. */
#define LOG(msg) std::cout << msg << std::endl // Server debugger.

#endif /* _SERVER_UTILS_H */
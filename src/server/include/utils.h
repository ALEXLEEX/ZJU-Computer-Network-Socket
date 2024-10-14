/**
 * @file utils.h
 * @author lqy
 * @brief Utilities for server.
 * @date 2024-10-07
 */

#ifndef _SERVER_UTILS_H
#define _SERVER_UTILS_H

/* Server type. */
enum class ServerType {
    TCP,
    UDP
};

/* Server message type. */
enum class ServerMsgType {
    MSG,
    ERROR,
    INFO,
    WELCOME
};

/* Server queue size. */
#define SERVER_QUEUE_SIZE 256 // Maximum number of waiting clients in server queue.

/* Server IP and port. */
#define SERVER_IP "127.0.0.1" // Server ip.
#define SERVER_PORT 3373 // Server port.

/* Server buffer size. */
#define SERVER_BUFFER_SIZE 4096 // Maximum size of received data in server buffer.

/* Server timeout. */
#define SERVER_TIMEOUT 10.0 // Maximum time to wait for client response.

#endif /* _SERVER_UTILS_H */
/**
 * @file micros.h
 * @author lqy
 * @brief Configuration file for server.
 * @date 2024-10-07
 */

#ifndef _SERVER_MICROS_H
#define _SERVER_MICROS_H

/* Micros configuration. */

/**
 * @brief Server type.
 * @deprecated Use the enum class instead.
 */
#define SERVER_TYPE_TCP 0 // Server type: TCP.
#define SERVER_TYPE_UDP 1 // Server type: UDP.

/* Server queue size. */
#define SERVER_QUEUE_SIZE 256 // Maximum number of waiting clients in server queue.

/* Server IP and port. */
#define SERVER_IP "0.0.0.0" // Server ip.
#define SERVER_PORT 1234 // Server port.

/* Server buffer size. */
#define SERVER_BUFFER_SIZE 4096 // Maximum size of received data in server buffer.

/* Server timeout. */
#define SERVER_TIMEOUT 10.0 // Maximum time to wait for client response.

/**
 * @brief Server message types.
 * @deprecated Use the enum class instead.
 */
#define SERVER_MSG 0 // Server message.
#define SERVER_MSG_ERROR 1 // Error message.
#define SERVER_MSG_INFO 2 // Information message.

#endif /* _SERVER_MICROS_H */

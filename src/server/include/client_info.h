/**
 * @file client_info.h
 * @author lqy
 * @brief Server's client info class interface.
 * @date 2024-10-11
 */

#ifndef _SERVER_CLIENT_INFO_H
#define _SERVER_CLIENT_INFO_H

/* Header files. */

#include <netinet/in.h>
#include <string>
#include <thread>

/* Utilities */

using ClientID = int;
using ClientAddr = struct sockaddr_in;

/* Main class. */

/**
 * @class ClientInfo
 * @brief Store client informations.
 * @note This is a utility class for server.
 */
class ClientInfo {
public:

    /* Initialize the client information. */
    ClientInfo(): status(0), socket(-1), thread(0), id(-1) {
        addr.sin_family = AF_INET; // IPv4.
        addr.sin_port = htons(0); // Any port.
        addr.sin_addr.s_addr = inet_addr("0.0.0.0");
    };

    /* Destructor. */
    ~ClientInfo() = default;

    /* Get client status. */
    inline int getStatus() const { return status; }

    /* Get client socket. */
    inline int getSocket() const { return socket; }

    /* Get client IP. */
    inline std::string getIP() const { return std::string(inet_ntoa(addr.sin_addr)); }

    /* Get client port. */
    inline int getPort() const { return static_cast<int>(ntohs(addr.sin_port)); }

    /* Get client address. */
    inline ClientAddr getAddr() const { return addr; }

    /* Get client thread id. */
    inline std::thread::id getThread() const { return thread; }

    /* Get client id. */
    inline ClientID getID() const { return id; }

    /* Set client status. */
    inline void setStatus(int status) { this->status = status; }

    /* Set client socket. */
    inline void setSocket(int socket) { this->socket = socket; }

    /* Set client address. */
    inline void setAddr(ClientAddr addr) { this->addr = addr; }

    /* Set client thread id. */
    inline void setThread(std::thread::id thread) { this->thread = thread; }

    /* Set client id. */
    inline void setID(ClientID id) { this->id = id; }

    /* Operator equal: compare two clients by id, IP and port. */
    inline bool operator==(const ClientInfo& other) const {
        return this->getStatus() && other.getStatus() &&
            this->id == other.id &&
            this->addr.sin_addr.s_addr == other.addr.sin_addr.s_addr &&
            this->addr.sin_port == other.addr.sin_port;
    }

private:

    int status; // 0: unconnected; 1: connected.
    int socket; // (TCP) Client socket.
    ClientAddr addr; // Address.
    std::thread::id thread; // (TCP) Thread id.
    ClientID id; // Client id.

};

#endif /* _SERVER_CLIENT_INFO_H */
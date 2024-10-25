/**
 * @file packet.h
 * @brief Packet class definition.
 * @author lqy wzj
 * @date 2024-10-18
 */

#ifndef _PACKET_H
#define _PACKET_H

/* Header files */

#include "utils.h"
#include <string>
#include <vector>

/* Main class */

/**
 * @brief Packet class defines how data is transmitted between two endpoints.
 * @note Data between server and client will be stored in packets, and each packet will be encoded into `std::string` then transmitted. After receiving, strings will be decoded into packets and processed by the server or client.
 */
class Packet {
public:

    /**
     * @brief Constructor for creating a packet.
     * @param info Packet information (Student ID).
     * @param type Packet type.
     * @param id Packet id.
     * @param content Packet content.
     * @note Packet content should be set according to the packet type.
     */
    Packet(std::string info, PacketType type = PacketType::INVALID, PacketID id = PacketID(0), ContentType content = ContentType::None);

    /**
     * @brief Destructor.
     */
    ~Packet();

    /**
     * @brief Add 1 packet argument.
     * @param arg Argument to be added.
     */
    void addArg(std::string arg);

    /**
     * @brief Get packet arguments.
     * @return Packet arguments.
     */
    std::vector<std::string> getArgs() const;

    /**
     * @brief Encode packet into string for transmission.
     * @return Encoded string.
     */
    std::string encode();

    /**
     * @brief Decode string to packet data.
     * @param data Data to be decoded.
     * @return True if decoding is successful, false otherwise.
     */
    bool decode(std::string data);

    /**
     * @brief Print packet information.
     * @note For debugging.
     */
    void print();

private:

    /* Packet header. */
    PacketType type;               // Packet type.
    PacketID id;                   // Packet id.
    PacketLength length;           // Packet total length.

    /* Packet content. */
    ContentType content;           // Packet content.
    std::vector<std::string> args; // Packet arguments.

    /* Packet checksum. */
    std::string info;              // Packet information.
    std::string checksum;          // Packet checksum.

    /* Calculate MD5 checksum of the packet. */
    std::string getChecksum(std::string origin);

    /* Validate the packet checksum. */
    bool validateChecksum(std::string data);

};

#endif /* _PACKET_H */
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
     * @brief Constructor.
     * @param type Packet type.
     * @param id Packet id.
     * @param content Packet content.
     * @param info Packet information (Student ID).
     * @note Packet content should be set according to the packet type.
     */
    Packet(PacketType type, PacketID id, ContentType content, std::string info);

    /**
     * @brief Constructor.
     * @param data Data to be decoded.
     * @note This constructor is used when decoding data into packet.
     */
    Packet(std::string data);

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
     * @brief Encode packet into string for transmission.
     * @return Encoded string.
     */
    std::string encode();

    /**
     * @brief Decode string to packet data.
     * @param data Data to be decoded.
     */
    void decode(std::string data);

private:

    PacketType type;               // Packet type.
    PacketID id;                   // Packet id.
    PacketLength length;           // Packet total length.

    ContentType content;           // Packet content.
    std::vector<std::string> args; // Packet arguments.

    std::string info;              // Packet information.
    std::string checksum;          // Packet checksum.

    /* Calculate MD5 checksum of the packet. */
    void getChecksum();

};

#endif /* _PACKET_H */
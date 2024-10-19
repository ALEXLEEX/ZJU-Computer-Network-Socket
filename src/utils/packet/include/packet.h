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

/* Main class */

/**
 * @brief Packet class defines how data is transmitted between two endpoints.
 * @note Data between server and client will be encoded into packets, then transmitted using string. After receiving, packets will be decoded into data.
 */
class Packet {
public:

    /**
     * @brief Constructor.
     */
    Packet();

    /**
     * @brief Destructor.
     */
    ~Packet();

    /**
     * @brief Encode data into packets for transmission.
     * @param data Data to be encoded.
     */
    void encode(std::string data);

    /**
     * @brief Decode packets into data.
     * @return Decoded data.
     */
    std::string decode();

private:

    PacketType type;      // Packet type.
    PacketID id;          // Packet id.
    PacketLength length;  // Packet total length.

    std::string data;     // Packet data.
    // TODO: replace data with args

    std::string checksum; // Packet checksum.

};

#endif /* _PACKET_H */
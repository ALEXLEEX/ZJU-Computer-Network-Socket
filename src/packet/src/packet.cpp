/**
 * @file packet.cpp
 * @headerfile packet.h
 * @brief Packet class implementation.
 * @author lqy wzj
 * @date 2024-10-18
 */

#include "../include/packet.h"
#include <string>
#include <openssl/md5.h>

/* Public methods */

Packet::Packet(PacketType type, PacketID id, ContentType content, std::string info) : 
    type(type), id(id), length(0), content(content), info(info), checksum("")
{
    args = std::vector<std::string>();
}

Packet::Packet(std::string data)
{
    decode(data);
}

Packet::~Packet()
{
    args.clear();
}

void Packet::addArg(std::string arg)
{
    args.push_back(arg);
    length += arg.length();
}

std::string Packet::encode()
{
    std::string data = "";

    // Encode packet header.
    data += std::to_string(static_cast<int>(type)) + "|";
    data += std::to_string(static_cast<int>(id)) + "|";
    data += std::to_string(length) + "|";

    // Encode packet content.
    data += std::to_string(static_cast<int>(content)) + "|";
    for (const std::string& arg : args) {
        data += arg + ",";
    }
    data += "|";

    // Encode packet checksum.
    getChecksum();
    data += checksum;

    // Add end of packet.
    data += "|\n";

    return data;
}

void Packet::decode(std::string data)
{
    int pos = 0;
    // TODO
}

/* Utility functions */

void Packet::getChecksum()
{
    // Get all args.
    std::string origin = "";
    for (const std::string& arg : args) {
        origin += arg;
    }
    origin += info;

    // Calculate MD5 checksum.
    unsigned char digest[MD5_DIGEST_LENGTH];
    MD5(reinterpret_cast<const unsigned char*>(origin.c_str()), origin.size(), digest);

    char md5String[33]; // MD5 is 32 characters long + null terminator
    for (int i = 0; i < 16; ++i) {
        snprintf(&md5String[i * 2], 3, "%02x", (unsigned int)digest[i]);
    }

    checksum = std::string(md5String);
}
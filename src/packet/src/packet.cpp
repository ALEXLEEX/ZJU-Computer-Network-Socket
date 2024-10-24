/**
 * @file packet.cpp
 * @headerfile packet.h
 * @brief Packet class implementation.
 * @author lqy wzj
 * @date 2024-10-18
 */

#include "../include/packet.h"
#include <string>
#include <vector>
#include <openssl/md5.h>

/* Public methods */

Packet::Packet(std::string info, PacketType type, PacketID id, ContentType content) : 
    type(type), id(id), length(0), content(content), info(info), checksum("")
{
    args = std::vector<std::string>();
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

std::vector<std::string> Packet::getArgs()
{
    return args;
}

std::string Packet::encode()
{
    std::string data = to_string();

    // Encode packet checksum.
    checksum = getChecksum(data);
    data += checksum;

    // Add end of packet.
    data += "|\n";

    return data;
}

bool Packet::decode(std::string data)
{
    std::string sep = "|";
    size_t pos1 = 0;
    size_t pos2 = data.find(sep, 0);
    std::vector<std::string> parts;

    // Get packet elements.
    while (pos2 != std::string::npos) {
        parts.push_back(data.substr(pos1, pos2 - pos1));
        pos1 = pos2 + 1;
        pos2 = data.find(sep, pos1);
    }
    parts.push_back(data.substr(pos1));

    // Check packet format.
    if (parts.size() != 7) return false;
    if (parts[6] != "\n") return false;

    // Parse packet header.
    type = static_cast<PacketType>(std::stoi(parts[0]));
    id = static_cast<PacketID>(std::stoi(parts[1]));
    length = std::stoi(parts[2]);
    content = static_cast<ContentType>(std::stoi(parts[3]));

    // Parse packet arguments.
    std::string argStr = parts[4];
    sep = ",";
    pos1 = 0;
    pos2 = argStr.find(sep, 0);
    while (pos2 != std::string::npos) {
        args.push_back(argStr.substr(pos1, pos2 - pos1));
        pos1 = pos2 + 1;
        pos2 = argStr.find(sep, pos1);
    }

    // Parse packet checksum and validate it.
    checksum = parts[5];
    if (!validateChecksum()) return false;

    return true;
}

std::string Packet::to_string()
{
    std::string str = "";
    str += std::to_string(static_cast<int>(type)) + "|";
    str += std::to_string(static_cast<int>(id)) + "|";
    str += std::to_string(length) + "|";
    str += std::to_string(static_cast<int>(content)) + "|";
    for (const std::string& arg : args) {
        str += arg + ",";
    }
    str += "|";
    return str;
}

/* Utility functions */

std::string Packet::getChecksum(std::string origin)
{
    // Get all elements.
    origin += info;

    // Calculate MD5 checksum.
    unsigned char digest[MD5_DIGEST_LENGTH];
    MD5(reinterpret_cast<const unsigned char*>(origin.c_str()), origin.size(), digest);

    char md5String[33]; // MD5 is 32 characters long + null terminator
    for (int i = 0; i < 16; ++i) {
        snprintf(&md5String[i * 2], 3, "%02x", (unsigned int)digest[i]);
    }

    return std::string(md5String);
}

bool Packet::validateChecksum()
{
    return checksum == getChecksum(to_string());
}
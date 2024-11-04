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
#include <iostream>

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

std::vector<std::string> Packet::getArgs() const
{
    return args;
}

std::string Packet::encode()
{
    std::string data = "";
    data += std::to_string(static_cast<int>(type)) + "\n";
    data += std::to_string(static_cast<int>(id)) + "\n";
    data += std::to_string(length) + "\n";
    data += std::to_string(static_cast<int>(content)) + "\n";
    for (const std::string& arg : args) {
        data += arg + "\r";
    }
    data += "\n";
    checksum = getChecksum(data);
    return data + checksum + "\n";
}

bool Packet::decode(std::string data)
{
    // Check packet format.
    if (data[data.size() - 1] != '\n') return false;
    data.erase(data.size() - 1);

    // Validate packet checksum.
    size_t pos = data.rfind("\n");
    if (pos == std::string::npos) return false;
    checksum = data.substr(pos + 1);
    data.erase(pos + 1);
    if (!validateChecksum(data)) return false;

    // Parse packet data.
    pos = data.find("\n", 0);
    std::vector<std::string> elements;
    while (pos != std::string::npos) {
        elements.push_back(data.substr(0, pos));
        data.erase(0, pos + 1);
        pos = data.find("\n", 0);
    }
    if (elements.size() != 5) return false;

    type = static_cast<PacketType>(std::stoi(elements[0]));
    id = static_cast<PacketID>(std::stoi(elements[1]));
    length = std::stoi(elements[2]);
    content = static_cast<ContentType>(std::stoi(elements[3]));

    pos = elements[4].find("\r", 0);
    while (pos != std::string::npos) {
        args.push_back(elements[4].substr(0, pos));
        elements[4].erase(0, pos + 1);
        pos = elements[4].find("\r", 0);
    }

    return true;
}

PacketType Packet::getType() const
{
    return type;
}

PacketID Packet::getID() const
{
    return id;
}

ContentType Packet::getContent() const
{
    return content;
}

void Packet::print()
{
    std::cout << "Packet type: " << static_cast<int>(type) << std::endl;
    std::cout << "Packet id: " << static_cast<int>(id) << std::endl;
    std::cout << "Packet length: " << length << std::endl;
    std::cout << "Packet content: " << static_cast<int>(content) << std::endl;
    std::cout << "Packet arguments: ";
    for (const std::string& arg : args) {
        std::cout << arg << ", ";
    }
    std::cout << std::endl;
    std::cout << "Packet checksum: " << checksum << std::endl;
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

bool Packet::validateChecksum(std::string data)
{
    return checksum == getChecksum(data);
}
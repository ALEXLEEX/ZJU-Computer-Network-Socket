/**
 * @file packet_test.cpp
 * @brief Packet class test.
 * @author lqy
 * @date 2024-10-25
 */

/* Header files */
#include "../include/packet.h"
#include "../include/utils.h"
#include <string>
#include <iostream>

int main()
{
    Packet p1("3373", PacketType::REQUEST, PacketID(1), ContentType::RequestCityName);
    p1.addArg("Beijing");
    p1.addArg("Shanghai");
    p1.addArg("Guangzhou");
    p1.addArg("Shenzhen");
    p1.print();
    std::string data = p1.encode();
    std::cout << "Encoded data: " << data << std::endl;
    Packet p2("3373");
    std::cout << (p2.decode(data)? "Decode success." : "Decode failed.") << std::endl;
    p2.print();
    return 0;
}
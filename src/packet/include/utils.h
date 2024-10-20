/**
 * @file utils.h
 * @author lqy
 * @brief Some utilities for packet.
 * @date 2024-10-18
 */

#ifndef _PACKET_UTILS_H
#define _PACKET_UTILS_H

/* Header files */

/* Packet types */
enum class PacketType {
    REQUEST,    // Request packet.
    RESPONSE,   // Response packet.
    ASSIGNMENT  // Assignment packet.
};

/* Content types */
enum class ContentType {
    RequestCityName,         // Request city name.
    RequestWeatherInfo,      // Request weather information.
    RequestClientList,       // Request client list.
    RequestSendMessage,      // Request message.
    RequestMakeConnection,   // Request make connection.
    RequestCloseConnection,  // Request close connection.

    ResponseCityName,        // Response city name.
    ResponseWeatherInfo,     // Response weather information.
    ResponseClientList,      // Response client list.
    ResponseSendMessage,     // Response message.
    ResponseMakeConnection,  // Response make connection.
    ResponseCloseConnection, // Response close connection.

    AssignmentSendMessage,   // Assignment send message.
    AssignmentClientLogin,   // Assignment client login.
    AssignmentClientLogout,  // Assignment client logout.
    AssignmentWeatherWarning // Assignment weather warning.
};

/* Packet id */
using PacketID = unsigned char;

/* Packet length */
using PacketLength = unsigned short;

#endif /* _PACKET_UTILS_H */
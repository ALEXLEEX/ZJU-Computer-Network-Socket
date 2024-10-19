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

/* Request content types */
enum class RequestType {
    GetCityName,    // Get city name.
    GetWeatherInfo, // Get weather information.
    GetClientList,  // Get client list.
    SendMessage,    // Send message.
    MakeConnection, // Make connection (UDP only).
    CloseConnection // Close connection (UDP only).
};

/* Response content types */
enum class ResponseType {
    CityName,       // City name.
    WeatherInfo,    // Weather information.
    ClientList,     // Client list.
    SendMessage,    // Message.
    MakeConnection, // Connection information.
    CloseConnection // Error.
};

/* Assignment content types */
enum class AssignmentType {
    SendMessage,    // Send message.
    ClientLogin,    // Client login.
    ClientLogout,   // Client logout.
    WeatherWarning  // Weather warning information.
};

/* Packet id */
using PacketID = unsigned char;

/* Packet length */
using PacketLength = unsigned short;

#endif /* _PACKET_UTILS_H */
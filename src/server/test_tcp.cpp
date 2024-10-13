/**
 * @file test_tcp.cpp
 * @author lqy
 * @brief Test cases for TCP server.
 * @date 2024-10-13
 */

#include "../include/server_tcp.h"

int main()
{
    Server_TCP server("127.0.0.1", 8080);
    server.init();
    server.run();
    return 0;
}
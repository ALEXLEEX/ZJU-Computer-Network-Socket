/**
 * @file main.cpp
 * @author lqy
 * @brief Main program for server.
 * @date 2024-10-09
 */

#include "include/server.h"
#include <string>

int main(int argc, char *argv[])
{
    std::string type(argv[1]);
    Server server(type);
    server.init();
    server.run();
    return 0;
}
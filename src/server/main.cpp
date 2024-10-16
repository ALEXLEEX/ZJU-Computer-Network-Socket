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
    // TODO: judge by argc.
    Server server(argv[1]);
    server.help();
    server.cmds();
    return 0;
}
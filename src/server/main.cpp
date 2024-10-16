/**
 * @file main.cpp
 * @author lqy
 * @brief Main program for server.
 * @date 2024-10-09
 */

#include "include/server.h"

int main(int argc, char *argv[])
{
    Server server(argc, argv);
    server.help();
    server.cmds();
    return 0;
}
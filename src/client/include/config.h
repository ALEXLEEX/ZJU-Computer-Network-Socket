/**
 * include basic headers
 */

#ifndef _CONFIG_H_
#define _CONFIG_H_
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

enum PROTOCOL{
    INVALID = 0,
    TCP = 1,
    UDP = 2
};

enum STATUS{
    CONNECTED = 1,
    DISCONNECTED = 0
};

#endif
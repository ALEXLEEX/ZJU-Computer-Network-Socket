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
#include <map>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <sys/ipc.h>
#include <sys/msg.h>

using namespace std;

enum PROTOCOL{
    INVALID = 0,
    TCP = 1,
    UDP = 2
};

enum STATUS{
    CONNECTED = 1,
    DISCONNECTED = 0
};

struct serverConnection
{
    int sockfd;
    bool connected = false;
    pthread_t recvThread;
    queue<string> messageQueue;
    mutex queueMutex;
    condition_variable cv;

    // 删除复制构造函数和赋值运算符，避免不必要的复制
    serverConnection(const serverConnection&) = delete;
    serverConnection& operator=(const serverConnection&) = delete;

    // 允许移动构造函数
    serverConnection(serverConnection&& other) noexcept
        : sockfd(other.sockfd),
          recvThread(other.recvThread),
          connected(other.connected),
          messageQueue(move(other.messageQueue)) {
        // 销毁原对象
        other.sockfd = -1;
        other.connected = false;
    }

    serverConnection& operator=(serverConnection&& other) noexcept {
        if (this != &other) {
            sockfd = other.sockfd;
            recvThread = other.recvThread;
            connected = other.connected;
            messageQueue = move(other.messageQueue);
            
            other.sockfd = -1;
            other.connected = false;
        }
        return *this;
    }

    serverConnection() : sockfd(-1), connected(false) {}
};

// global variables
extern int nextID;
extern map<int, serverConnection> serverConnections;

#endif
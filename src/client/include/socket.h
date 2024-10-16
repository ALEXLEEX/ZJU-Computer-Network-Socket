/**
 * Declaration of Socket API
 */

#ifndef _SOCKET_H_
#define _SOCKET_H_
#include "config.h"
/**
 * 对 Socket API 的第一层封装
 */

    void exitWithError(const char *msg);

    int init(int type);

    void setOptions(int s);

    void startConnect(int s, char ip[], int port);
    
    void worker(int s);

    void startSocketThread(int s);

    void sendMessage(int s, char *msg);

    void receiveMessage(int s);

#endif
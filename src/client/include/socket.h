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

    void woker_UDP(int s);

    pthread_t startSocketThread(int s);

    pthread_t startSocketThread_UDP(int s);

    void handle_received_message(int choice);

    void bindAddress(int s, const char ip[], int port);

#endif
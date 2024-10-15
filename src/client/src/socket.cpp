/**
 * Implementation for the Socket class.
 */

#ifndef _SOCKET_H_
#define _SOCKET_H_
#include "config.h"
#include "socket.h"
using namespace std;

/**
 * 对 Socket API 的第一层封装
 */

    void exitWithError(const char *msg)
    {
        perror(msg);
        exit(EXIT_FAILURE);
    }

    int init(int type)
    {
        int s;
        if (type == 1) // TCP
            s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        else    // UDP
            s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

        if (s < 0) exitWithError("Unable to init socket due to unknown type.");
        return s;
    }

    void setOptions(int s)
    {
        int on = 1;
        setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));

        struct timeval timeout;
        timeout.tv_sec = 20;
        timeout.tv_usec = 0;
        setsockopt(s, SOL_SOCKET, SO_SNDTIMEO, &timeout, sizeof(timeout));
    }

    void startConnect(int s, char ip[], int port)
    {
        struct sockaddr_in channel;
        memset(&channel, 0, sizeof(channel));
        channel.sin_family = AF_INET;
        channel.sin_addr.s_addr = inet_addr(ip);
        channel.sin_port = htons(port);
        int rc = connect(s, (struct sockaddr *) &channel, sizeof(channel));
        if (rc < 0) exitWithError("connect failed");
    }
    
    void worker(int s)
    {
        char buffer[1024];
        ssize_t rc;

        while (1) {
            memset(buffer, 0, sizeof(buffer));
            rc = recv(s, buffer, sizeof(buffer), 0);

            if (rc <= 0) {
                perror("recv failed");
                close(s);
                break;
            }

            else cout << "Rx: " << buffer << endl;

            if (strcmp(buffer, "BYE") == 0) {
                close(s);
                break;
            }
        }
    }

    /**
     * 创建socket主线程
     */
    void *Thread(void *arg)
    {
        int s = (long) arg & 0xFFFFFFFF;
        worker(s);
        return NULL;
    }

    void startSocketThread(int s)
    {
        pthread_t t_id;
        pthread_create(&t_id, NULL, Thread, (void *) (long) s);
    }

#endif
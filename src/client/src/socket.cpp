/**
 * Implementation for the Socket class.
 */

#ifndef _SOCKET_H_
#define _SOCKET_H_
#include "config.h"
#include "socket.h"
using namespace std;

extern int nextID;
extern map<int, serverConnection> serverConnections;

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
        timeout.tv_sec = 1;
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
    
    void worker(int serverID)
    {
        char buffer[1024];
        serverConnection& conn = serverConnections[serverID];
        ssize_t rc;

        while (conn.connected) {

            cout << "receiver State " <<conn.connected << endl;
            memset(buffer, 0, sizeof(buffer));
            rc = recv(conn.sockfd, buffer, sizeof(buffer), 0);

            if (rc <= 0) {
                if (conn.connected) {
                    perror("recv failed");
                    cout << "Server ID " << serverID << " disconnected." << endl;
                    conn.connected = false;
                    break;
                }
                else {
                    cout << "Server ID " << serverID << " disconnected." << endl;
                }
            }
            else {                
                cout << "Server ID " << serverID << " received: " << buffer << endl;
                if (strcmp(buffer, "BYE") == 0) {
                    conn.connected = false;
                    break;
                }
                conn.cv.notify_one();
            }        
        }

        
        cout << "Thread for server ID " << serverID << " exited." << endl;
    }

    /**
     * 创建socket线程
     */
    void *Thread(void *arg)
    {
        int serverID = (long) arg & 0xFFFFFFFF;
        worker(serverID);
        return NULL;
    }

    pthread_t startSocketThread(int serverID)
    {
        pthread_t t_id;
        pthread_create(&t_id, NULL, Thread, (void *) (long) serverID);
        return t_id;
    }

#endif
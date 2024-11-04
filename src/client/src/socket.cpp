/**
 * Implementation for the Socket class.
 */

#ifndef _SOCKET_H_
#define _SOCKET_H_
#include "config.h"
#include "socket.h"
using namespace std;

// 线程共享全局变量
extern int nextID;
extern map<int, serverConnection> serverConnections;
extern mutex mtx;
extern condition_variable cv;
extern queue<string> message_queue;

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

            if (rc > 0) {
                string msg(buffer, rc);
                cout << "Received message from server ID " << serverID << ": " << msg << endl;

                {
                    lock_guard<mutex> lock(mtx);
                    message_queue.push(msg);
                }
                // 通知主线程
                cv.notify_one();
            } else {
                cout << "Server ID " << serverID << " disconnected." << endl;
                conn.connected = false;
            }
        }
        
        cout << "Thread for server ID " << serverID << " exited." << endl;
    }

    void handle_received_message(int choice)
    {
        // 从队列中取出对应的消息
        unique_lock<mutex> lock(mtx);
        // 等待消息队列出现消息
        cv.wait(lock, [] { return !message_queue.empty(); });

        cout << "Choice: " << choice << endl;
        string msg = message_queue.front();
        message_queue.pop();
        cout << "Received message: " << msg << endl;
    
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
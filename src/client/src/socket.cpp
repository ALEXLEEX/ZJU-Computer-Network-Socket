/**
 * Implementation for the Socket class.
 */

#ifndef _SOCKET_H_
#define _SOCKET_H_
#include "config.h"
#include "socket.h"
#include "packet.h"
#include "utils.h"
using namespace std;

// 线程共享全局变量
extern int nextID;
extern map<int, serverConnection> serverConnections;
extern mutex mtx;
extern condition_variable cv;
extern queue<string> message_queue;
extern bool exitFlag;
extern bool messageFlag;

/**
 * 对 Socket API 的第一层封装
 */

    void exitWithError(const char *msg)
    {
        perror(msg);
        // exitFlag = true;     
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

    int startConnect(int s, char ip[], int port)
    {
        struct sockaddr_in channel;
        memset(&channel, 0, sizeof(channel));
        channel.sin_family = AF_INET;
        channel.sin_addr.s_addr = inet_addr(ip);
        channel.sin_port = htons(port);
        int rc = connect(s, (struct sockaddr *) &channel, sizeof(channel));
        if (rc < 0) {
            exitWithError("connect failed");
            return 0;
        }
        return 1;
    }
    
    void bindAddress(int s, const char ip[], int port)
    {
        struct sockaddr_in channel;
        memset(&channel, 0, sizeof(channel));
        channel.sin_family = AF_INET;
        channel.sin_addr.s_addr = inet_addr(ip);
        channel.sin_port = htons(port);
        int rc = bind(s, (struct sockaddr *) &channel, sizeof(channel));
        if (rc < 0) exitWithError("bind failed");
    }
/**
 * TCP 通信线程，用于接收服务器消息，将消息放入消息队列
 * 使用 互斥锁 和 条件变量 实现线程同步
 * @param s socket file descriptor
 * @return void
 * 
 */
    void worker(int serverID)
    {
        char buffer[1024];
        serverConnection& conn = serverConnections[serverID];
        ssize_t rc;

        while (conn.connected) {

            memset(buffer, 0, sizeof(buffer));
            rc = recv(conn.sockfd, buffer, sizeof(buffer), 0);

            if (rc > 0) {
                string msg(buffer, rc);
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
        
        cout << "\n\033[34m[Thread] Thread for server ID " << serverID << " exited." << "\033[0m" << endl;
    }

    void woker_UDP(int serverID)
    {
        char buffer[1024];
        serverConnection& conn = serverConnections[serverID];
        ssize_t rc;
        struct sockaddr_in addr;
        socklen_t addr_len = sizeof(addr);
        
        while (1) {
            // cout << "Debug: serverID = " << serverID << endl;
            memset(buffer, 0, sizeof(buffer));
            rc = recvfrom(conn.sockfd, buffer, sizeof(buffer), 0, (struct sockaddr *) &addr, &addr_len);

            // cout << "[Debug] addr: " << inet_ntoa(addr.sin_addr) << ":" << ntohs(addr.sin_port) << endl;
            // cout << "[Debug] conn.addr: " << inet_ntoa(conn.addr.sin_addr) << ":" << ntohs(conn.addr.sin_port) << endl;

            if (rc <= 0) {
                perror("recvfrom failed");
                close(conn.sockfd);
                break;
            }

            // 判断 addr 的地址是否来自服务器, 不是则忽略
            bool found = false;
            
            if (serverConnections.find(serverID) != serverConnections.end()) {
                if (serverConnections[serverID].addr.sin_addr.s_addr == addr.sin_addr.s_addr && serverConnections[serverID].addr.sin_port == addr.sin_port) {
                    found = true;
                }
            }
            
            if (!found) {
                cout << "Received message from unknown address, ignored." << endl;
                continue;
            }

            Packet p("3373");
            if (!p.decode(buffer)) {
                cout << "\033[31m[ERROR] Failed to decode message.\033[0m" << endl;
                continue;
            }

            // cout << "[Thread] Received message from server " << serverID << ": " << '{' << buffer << '}' << endl;

            if (p.getContent() == ContentType::ResponseMakeConnection) {
                if (p.getArgs()[0] == "1") {
                    cout << "Received ACK from server ID " << serverID << endl;
                    cout << "Connection established." << endl;
                    conn.connected = true;
                    messageFlag = true;
                    cv.notify_all();
                }
                else {
                    cout << "Failed to connect to server ID " << serverID << endl;
                    conn.connected = false;
                    messageFlag = true;
                    cv.notify_all();
                }     
            }

            else if (p.getContent() == ContentType::ResponseCloseConnection) {
                if (p.getArgs()[0] == "1") {
                    cout << "Received close connection ACK from server ID " << serverID << endl;
                    cout << "Connection closed." << endl;
                    conn.connected = false;
                    messageFlag = true;
                    cv.notify_all();
                    break;
                }
                else {
                    cout << "Failed to close connection to server ID " << serverID << endl;
                    conn.connected = true;
                    messageFlag = true;
                    cv.notify_all();
                }
            }

            else if (conn.connected) {
                lock_guard<mutex> lock(mtx);
                message_queue.push(buffer);
                cv.notify_all();
            }
        }        
    }
/**
 * 消息处理线程
 */
    void handle_received_message()
    {        
        while (!exitFlag) {
            // 从队列中取出对应的消息
            unique_lock<mutex> lock(mtx);
            // 等待消息队列出现消息
            cv.wait(lock, [] { return !message_queue.empty() || exitFlag; });

            while (!message_queue.empty()) {

                string msg = message_queue.front();
                message_queue.pop();                

                Packet p("3373");
                if (!p.decode(msg)) {
                    cout << "\033[31m[ERROR] Failed to decode message.\033[0m" << endl;
                    continue;                    
                }

                switch (p.getContent())
                {
                    case ContentType::ResponseCityName:
                        if(p.getArgs()[0] == "1")
                            cout << "\033[32m[Server] " << "City name: " << p.getArgs()[1] << "\033[0m" << endl;
                        else
                            cout << "\033[31m[ERROR] " << p.getArgs()[1] << "\033[0m" << endl;
                        break;
                    case ContentType::ResponseWeatherInfo:            
                        if(p.getArgs()[0] == "1")
                            cout << "\033[32m[Server] " << "Weather info in " << p.getArgs()[1] << ": " << p.getArgs()[2] << "\033[0m" << endl;
                        else
                            cout << "\033[31m[ERROR] " << p.getArgs()[1] << "\033[0m" << endl;
                        break;
                    case ContentType::ResponseClientList:
                        if (p.getArgs()[0] == "1")
                        {
                            cout << "\033[32m[Server] Client list number: " << p.getArgs()[1] << endl;
                            for (int i = 2; i < p.getArgs().size(); i++)
                            {
                                cout << p.getArgs()[i] << endl;
                            }
                            cout << "\033[0m" << endl;
                        }
                        else
                            cout << "\033[31m[ERROR] " << p.getArgs()[1] << "\033[0m" << endl;                        
                        break;
                    case ContentType::ResponseSendMessage:
                        if (p.getArgs()[0] == "1")
                            cout << "\033[32m[Server] Message sent successfully."  << "\033[0m" << endl;
                        else
                            cout << "\033[31m[ERROR] " << p.getArgs()[1] << "\033[0m" << endl;
                        break;
                    case ContentType::AssignmentSendMessage:
                        cout << "\n\033[32m[Server] Message: " << p.getArgs()[0] << "\033[0m" << endl;
                        break;
                    case ContentType::AssignmentClientLogin:
                        cout << "\033[32m[Server] " << p.getArgs()[0]  << "\033[0m" << endl;
                        break;
                    case ContentType::AssignmentClientLogout:
                        cout << "\033[32m[Server] " << p.getArgs()[0]  << "\033[0m" << endl;
                        break;
                    case ContentType::AssignmentWeatherWarning:
                        cout << "\n\033[32m[Server] Weather warning: " << p.getArgs()[0] << "\033[0m" << endl;
                        break;
                    default:
                        cout << "\n\033[31m[ERROR] Unknown message type." << "\033[0m" << endl;
                        break;
                }                            
            }            
            // 通知消息处理线程处理完消息            
            messageFlag = true;          
            cv.notify_all();
        }
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

    void *Thread_UDP(void *arg)
    {
        int serverID = (long) arg & 0xFFFFFFFF;
        woker_UDP(serverID);
        return NULL;
    }

    pthread_t startSocketThread(int serverID)
    {
        pthread_t t_id;
        pthread_create(&t_id, NULL, Thread, (void *) (long) serverID);
        return t_id;
    }

    pthread_t startSocketThread_UDP(int serverID)
    {
        pthread_t t_id;
        pthread_create(&t_id, NULL, Thread_UDP, (void *) (long) serverID);
        return t_id;
    }

#endif
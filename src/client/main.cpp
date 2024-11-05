/**
 * 基于Socket接口实现自定义协议通信
 * interface for Client
 * By WZJ
 * 2024-10-9
 */

#include "config.h"
#include "interface.h"
#include "functions.h"
#include "socket.h"
#include <thread>

using namespace std;

// global variables
int nextID=1;
bool exitFlag = false;
map<int, serverConnection> serverConnections;

// 用于线程同步的条件变量和互斥锁
std::mutex cout_mtx;
std::mutex mtx;
std::condition_variable cv;

// 消息队列y用于线程间通信
std::queue<std::string> message_queue;
bool messageFlag = false;          

/*
 * 客户端主函数
*/
int main()
{
    // 开启消息处理线程
    thread messageHandler(handle_received_message);
    int protocol = INVALID;
    protocolInterface();
    cin >> protocol;
    if (protocol != TCP && protocol != UDP)
    {
        cout << "Invalid protocol type." << endl;
        return 0;
    }
    
    while (exitFlag == false) 
    {                              
        showConnectedServers();
        int choice, status, serverID;
        status = serverConnections.size() > 0 ? CONNECTED : DISCONNECTED;
        mainInterface(status);
        cout << "[Client] Enter your cmd: ";
        cin >> choice;
        switch (choice)
        {
            case 1:
                // 连接
                if (protocol == TCP)
                    connectToServer(protocol);
                else
                    connectToServer_UDP(protocol);            
                break;
            case 2:
                // 断开连接
                disconnectFromServer();
                break;
            case 3:
                // 获取城市名字
                getCityName();
                break;
            case 4:
                // 获取气象信息
                getWeatherInfo();
                break;
            case 5:
                // 活动连接列表
                getClientList();
                break;                
            case 6:
                // 发消息     
                sendMessage();
                break;                           
            case 7:
                // 退出
                exitFromClient();
                exitFlag = true;                
                return 0;
            default:
                // invalid choice
                choice = -1;
                cout << "Invalid choice." << endl;
                return -1;
        }    
        // 等待消息处理线程处理完消息        
        unique_lock<mutex> lock(mtx);
        cv.wait(lock, [] { return messageFlag; });
        messageFlag = false;        
    }

    messageHandler.join();
}



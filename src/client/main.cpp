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

using namespace std;

// global variables
int nextID=1;
map<int, serverConnection> serverConnections;

// 用于线程同步的条件变量和互斥锁
std::mutex mtx;
std::condition_variable cv;

// 消息队列y用于线程间通信
std::queue<std::string> message_queue;          

/*
 * 客户端主函数
*/
int main()
{
    int protocol = INVALID;
    protocolInterface();
    cin >> protocol;
    if (protocol != TCP && protocol != UDP)
    {
        cout << "Invalid protocol type." << endl;
        return 0;
    }
    
    while (1) 
    {               
        showConnectedServers();
        int choice, status, serverID;
        status = serverConnections.size() > 0 ? CONNECTED : DISCONNECTED;
        mainInterface(status);
        cin >> choice;
        switch (choice)
        {
            case 1:
                // 连接
                connectToServer(protocol);
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
                exit();
                return 0;
            default:
                cout << "Invalid choice." << endl;
                break;
        }
        handle_received_message(choice);
    }
}



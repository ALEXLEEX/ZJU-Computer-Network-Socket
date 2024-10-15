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

    
    // 初始化socket    
    Client client;    
    while (1) 
    {               
        int choice, status, serverID;
        status = client.connectionStatus();
        mainInterface(status);
        cin >> choice;
        switch (choice)
        {
            case 1:
                // 连接
                char serverip[20];
                int port;
                cout << "Please enter the server IP address: ";
                cin >> serverip;
                cout << "Please enter the server port: ";
                cin >> port;
                client.ConnectToServer(protocol, serverip, port);
                break;
            case 2:
                // 断开连接
                int serverID;
                cout << "Please enter the server ID: ";
                cin >> serverID;
                client.closeConnection(serverID);
                break;
            case 3:
                // 获取城市名字
                break;
            case 4:
                // 获取气象信息
                break;
            case 5:
                // 活动连接列表
                break;                
            case 6:
                // 发消息                
                cout << "Please enter the server ID: ";
                cin >> serverID;
                client.sendTCPMessage(serverID, "list");                            
                break;
            case 7:
                // 退出
                return 0;
            default:
                cout << "Invalid choice." << endl;
                break;
        }
    }
}



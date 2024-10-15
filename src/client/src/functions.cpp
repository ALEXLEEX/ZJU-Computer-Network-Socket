#include "functions.h"
#include "socket.h"
using namespace std;
/**
 * 调用我们封装的 Socket API 实现客户端的 7 种功能：
    a)	连接：请求连接到指定地址和端口的服务端。服务端可以运行多个实例（绑定不同的地址或端口），客户端可以同时与多个服务端保持连接，一旦连接成功，给服务端分配一个ID，用于后续操作。
    b)	断开连接：断开与指定ID的服务端的连接。
    c)	获取城市名字：向指定ID的服务端请求给出某个区号对应的中英文城市名称。
    d)	获取气象信息: 向指定ID的服务端请求给出指定日期、指定城市的气象信息，气象信息包括气温、阴/晴/雨状态、风向、风力、湿度等。
    e)	活动连接列表：向指定ID的服务端请求给出当前连接的所有客户端信息（编号、IP地址、端口等）。
    f)	发消息：请求服务端把消息转发给对应编号的客户端，该客户端收到后显示在屏幕上
    g)	退出：断开所有服务端的连接并退出客户端程序。
 */

int Client::ConnectToServer(int protocol, char *serverip, int port)
{
    int s = init(protocol);
    cout << "Connecting to server..." << endl;
    
    setOptions(s);

    startConnect(s, serverip, port);
    int serverID = serverIDCounter++;
    serverConnections[serverID] = s;    
    cout << "Connected to server " << serverip << " on port " << port << " with server ID " << serverID << endl;

    startSocketThread(s);    

    return serverID;
}

void Client::closeConnection(int serverID)
{
    if (serverConnections.find(serverID) != serverConnections.end())
    {
        int s = serverConnections[serverID];
        close(s);
        serverConnections.erase(serverID);
        cout << "Connection to server " << serverID << " closed." << endl;
    }
    else
    {
        cout << "Server ID " << serverID << " not found." << endl;
    }
}

void Client::sendTCPMessage(int serverID, const string &msg)
{
    if (serverConnections.find(serverID) != serverConnections.end())
    {
        int s = serverConnections[serverID];
        int rc = send(s, msg.c_str(), msg.length(), 0);
        if (rc < 0) exitWithError("send failed");        
    }
    else
    {
        cout << "Server ID " << serverID << " not found." << endl;
    }
}

int Client::connectionStatus()
{
    if (serverConnections.size() > 0) return CONNECTED;
    else return DISCONNECTED;
}
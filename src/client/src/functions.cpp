#include "config.h"
#include "socket.h"
#include "functions.h"
#include "packet.h"

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

int packetID = 0;
// for UDP
struct sockaddr_in addr;

void connectToServer(int protocol)
{
    int sockfd = init(protocol);

    setOptions(sockfd);

    string serverIp;
    int port;
    cout << "Please enter the server IP address: ";
    cin >> serverIp;
    cout << "Please enter the server port: ";
    cin >> port;

    startConnect(sockfd, (char *)serverIp.c_str(), port);

    int serverID = nextID++;
    serverConnection conn;
    conn.sockfd = sockfd;
    conn.connected = true;
    serverConnections[serverID] = move(conn);

    // 创建子进程
    serverConnections[serverID].recvThread = startSocketThread(serverID);

    cout << "Connected to server successfully. Server ID: " << serverID << endl;
}

void connectToServer_UDP(int protocol)
{
    int sockfd = init(protocol);

    setOptions(sockfd);

    string serverIp;
    int serverPort, localPort;

    cout << "Please enter the server IP address: ";
    cin >> serverIp;
    cout << "Please enter the server port: ";
    cin >> serverPort;
    cout << "Please enter the local port: ";
    cin >> localPort;

    bindAddress(sockfd, "0.0.0.0", localPort);

    int serverID = nextID++;
    serverConnection conn;
    conn.sockfd = sockfd;
    conn.connected = false;
    serverConnections[serverID] = move(conn);

    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(serverIp.c_str());
    addr.sin_port = htons(serverPort);

    // 创建子进程
    serverConnections[serverID].recvThread = startSocketThread_UDP(serverID);

    cout << "Connected to server successfully. Server ID: " << serverID << endl;
    
}

void disconnectFromServer()
{
    int serverID;
    cout << "Please enter the server ID you want to disconnect: ";
    cin >> serverID;
    if (serverConnections.find(serverID) == serverConnections.end() || !serverConnections[serverID].connected)
    {
        cout << "Server ID not found or server is not connected." << endl;
        return;
    }

    serverConnections[serverID].connected = false;

    shutdown(serverConnections[serverID].sockfd, SHUT_RD);
    close(serverConnections[serverID].sockfd);

    pthread_join(serverConnections[serverID].recvThread, NULL);

    serverConnections.erase(serverID);
    cout << "Connections number: " << serverConnections.size() << endl;
    cout << "Disconnected from server ID " << serverID << endl;
}

void exit()
{
    for (auto it = serverConnections.begin(); it != serverConnections.end(); it++)
    {
        it->second.connected = false;
        shutdown(it->second.sockfd, SHUT_RD);
        close(it->second.sockfd);
        pthread_join(it->second.recvThread, NULL);
    }
    serverConnections.clear();
    cout << "All connections closed." << endl;
    cout << "Exit successfully." << endl;
}

void getCityName()
{
    int serverID;
    cout << "Please enter the server ID you want to get city name: ";
    cin >> serverID;
    if (serverConnections.find(serverID) == serverConnections.end() || !serverConnections[serverID].connected)
    {
        cout << "Server ID not found or server is not connected." << endl;
        return;
    }

    string areaCode;
    cout << "Please enter the area code: ";
    cin >> areaCode;

    // 组装请求数据包
    packetID = (packetID + 1) % 256;
    Packet p("2682", PacketType::REQUEST, PacketID(packetID), ContentType::RequestCityName);
    p.addArg(areaCode);

    string msg = p.encode();

    if (send(serverConnections[serverID].sockfd, msg.c_str(), msg.length(), 0))
    {
        cout << "Request sent successfully." << endl;
    }
    else
    {
        cout << "Failed to send request." << endl;
    }

    // handle_received_message();
}

void getWeatherInfo()
{
    int serverID;
    cout << "Please enter the server ID you want to get weather info: ";
    cin >> serverID;
    if (serverConnections.find(serverID) == serverConnections.end() || !serverConnections[serverID].connected)
    {
        cout << "Server ID not found or server is not connected." << endl;
        return;
    }

    string date, areaCode;
    cout << "Please enter the area code: ";
    cin >> areaCode;
    cout << "Please enter the date (YYYY-MM-DD): ";
    cin >> date;

    // 组装请求数据包
    packetID = (packetID + 1) % 256;
    Packet p("2682", PacketType::REQUEST, PacketID(packetID), ContentType::RequestWeatherInfo);
    p.addArg(areaCode);
    p.addArg(date);

    string msg = p.encode();

    if (send(serverConnections[serverID].sockfd, msg.c_str(), msg.length(), 0))
    {
        cout << "Request sent successfully." << endl;
    }
    else
    {
        cout << "Failed to send request." << endl;
    }

    // handle_received_message();
}

void getClientList()
{
    int serverID;
    cout << "Please enter the server ID you want to get client list: ";
    cin >> serverID;
    if (serverConnections.find(serverID) == serverConnections.end() || !serverConnections[serverID].connected)
    {
        cout << "Server ID not found or server is not connected." << endl;
        return;
    }

    // 组装请求数据包
    packetID = (packetID + 1) % 256;
    Packet p("2682", PacketType::REQUEST, PacketID(packetID), ContentType::RequestClientList);

    string msg = p.encode();

    if (send(serverConnections[serverID].sockfd, msg.c_str(), msg.length(), 0))
    {
        cout << "Request sent successfully." << endl;
    }
    else
    {
        cout << "Failed to send request." << endl;
    }

    // handle_received_message();
}

void sendMessage()
{
    int serverID;
    cout << "Please enter the server ID you want to send message: ";
    cin >> serverID;
    if (serverConnections.find(serverID) == serverConnections.end() || !serverConnections[serverID].connected)
    {
        cout << "Server ID not found or server is not connected." << endl;
        return;
    }

    string receiverID, message;
    cout << "Please enter the receiver ID: ";
    cin >> receiverID;
    cout << "Please enter the message: ";
    cin >> message;

    // 组装请求数据包
    packetID = (packetID + 1) % 256;
    Packet p("2682", PacketType::REQUEST, PacketID(packetID), ContentType::RequestSendMessage);
    p.addArg(receiverID);
    p.addArg(message);

    string msg = p.encode();

    if (send(serverConnections[serverID].sockfd, msg.c_str(), msg.length(), 0))
    {
        cout << "Request sent successfully." << endl;
    }
    else
    {
        cout << "Failed to send request." << endl;
    }

    // handle_received_message();
}
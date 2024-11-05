/*客户端采用交互菜单形式，用户可以选择以下功能：
    a)	连接：请求连接到指定地址和端口的服务端。服务端可以运行多个实例（绑定不同的地址或端口），客户端可以同时与多个服务端保持连接，一旦连接成功，给服务端分配一个ID，用于后续操作。
    b)	断开连接：断开与指定ID的服务端的连接。
    c)	获取城市名字：向指定ID的服务端请求给出某个区号对应的中英文城市名称。
    d)	获取气象信息: 向指定ID的服务端请求给出指定日期、指定城市的气象信息，气象信息包括气温、阴/晴/雨状态、风向、风力、湿度等。
    e)	活动连接列表：向指定ID的服务端请求给出当前连接的所有客户端信息（编号、IP地址、端口等）。
    f)	发消息：请求服务端把消息转发给对应编号的客户端，该客户端收到后显示在屏幕上
    g)	退出：断开所有服务端的连接并退出客户端程序。
*/
#include "interface.h"

using namespace std;

extern std::mutex cout_mtx;

void protocolInterface()
{
    cout << "Please select the protocol you want to use" << endl;
    cout << "1. TCP" << endl;
    cout << "2. UDP" << endl;
}

 /**
  * 主界面，格式化输出菜单, 使用互斥锁保护输出
  * @status: 客户端连接状态
  */
void mainInterface(int status)
{    
    lock_guard<mutex> lock(cout_mtx);
    {
        cout << "\n=============== MENU ================" << endl;
        if (status == DISCONNECTED) {
            cout << "1. Connect to Server" << endl;
            cout << "2. Exit" << endl;
        }
        else {
            cout << "1. Connect to Server" << endl;
            cout << "2. Disconnect from Server" << endl;
            cout << "3. Get City Name" << endl;
            cout << "4. Get Weather Information" << endl;
            cout << "5. Get Client List" << endl;
            cout << "6. Send Message" << endl;
            cout << "7. Exit" << endl;
        }
        cout << "=====================================" << endl;
    }    
}

void showConnectedServers()
{
    cout << "\n============= SERVERS ===============" << endl;
    if (serverConnections.empty()) {
        cout << "No server connected." << endl;
        cout << "=====================================" << endl;
        return;        
    }
    cout << "Allocated Server ID: " << endl;
    for (auto it = serverConnections.begin(); it != serverConnections.end(); it++)
    {
        cout << "Server ID: " << it->first << " status: " << (it->second.connected ? "\033[32mConnected\033[0m" : "\033[31mDisconnected\033[0m") << endl;
    }
    cout << "=====================================" << endl;
}
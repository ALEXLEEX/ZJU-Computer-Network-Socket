#ifndef _FUNCTIONS_H_
#define _FUNCTIONS_H_

#include "config.h"
#include <map>

using namespace std;
// void connectToServer(int protocol);

class Client
{
    private:
        map<int, int> serverConnections;    // key: server id, value: socket descriptor
        int serverIDCounter = 0;
    public:
        int ConnectToServer(int protocol, char *serverip, int port);
        void closeConnection(int serverID);
        void sendTCPMessage(int serverID, const string &msg);
        int connectionStatus();
};

#endif
#ifndef _INTERFACE_H_
#define _INTERFACE_H_
#include "config.h"

extern map<int, serverConnection> serverConnections;

void protocolInterface();
void mainInterface(int status);
void showConnectedServers();
#endif
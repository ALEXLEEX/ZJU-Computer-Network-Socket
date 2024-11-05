#ifndef _FUNCTIONS_H_
#define _FUNCTIONS_H_

using namespace std;
// void connectToServer(int protocol);

/**
 * Connection to server
 * @sockfd: socket file descriptor
 * 
 */

void connectToServer(int protocol);
void connectToServer_UDP(int protocol);
void disconnectFromServer();
void getCityName();
void getWeatherInfo();
void getClientList();
void sendMessage();

void exitFromClient();

#endif
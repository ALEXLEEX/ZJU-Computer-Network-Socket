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
void disconnectFromServer_UDP();
void getCityName(int protocol);
void getWeatherInfo(int protocol);
void getClientList(int protocol);
void sendMessage(int protocol);
void sendBatchTestData(int protocol);

void exitFromClient();

#endif
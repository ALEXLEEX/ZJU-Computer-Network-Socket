/**
 * @file client.cpp
 * @author lqy
 * @brief Test server using this simple client.
 * @date 2024-10-14
 */

/* Header files */

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <iostream>
#include <string>
#include <thread>
#include <system_error>

/* Class definition */

class Client {
public:
    Client(std::string ip, int port);
    ~Client() = default;
    void init();
    void run();
private:
    int clientSocket;
    std::string ip;
    int port;

    void getSocket();
    void setOptions();
    void bindAddress();

    void startConnect(std::string ip, int port);
    void startSocketThread();
    void worker();
};

/* Main function */

int main(int argc, char* argv[]) 
{
    Client client("127.0.0.1", atoi(argv[1]));
    client.init();
    client.run();
}

/* Class implementation */

Client::Client(std::string ip, int port): clientSocket(-1), ip(ip), port(port) {}

void Client::init()
{
    getSocket();
    setOptions();
    bindAddress();
    std::cout << "Client initialization completed." << std::endl;
}

void Client::run()
{
    std::cout << "Client started." << std::endl;
    startConnect("127.0.0.1", 3373);
    startSocketThread();
    std::cout << "Client stopped." << std::endl;
}

void Client::getSocket()
{
    clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (clientSocket < 0) {
        std::cerr << "Failed to create socket." << std::endl;
    }
}

void Client::setOptions()
{
    int on = 1;
    setsockopt(clientSocket, SOL_SOCKET, SO_REUSEADDR, (char*)&on, sizeof(on));
    struct timeval timeout;
    timeout.tv_sec = 10;
    timeout.tv_usec = 0;
    setsockopt(clientSocket, SOL_SOCKET, SO_SNDTIMEO, (char*)&timeout, sizeof(timeout));
}

void Client::bindAddress()
{
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(ip.c_str());
    addr.sin_port = htons(port);

    if (bind(clientSocket, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
        std::cerr << "Failed to bind address." << std::endl;
    }
}

void Client::startConnect(std::string ip, int port)
{
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(ip.c_str());
    addr.sin_port = htons(port);

    if (connect(clientSocket, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
        std::cerr << "Failed to connect." << std::endl;
    }
}

void Client::startSocketThread()
{
    try {
        std::thread thread(&Client::worker, this);
        thread.join();
    } catch (const std::system_error& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

void Client::worker()
{
    char buffer[4096];
    ssize_t rc;
    while (true) {
        rc = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (rc <= 0) break;
        std::string message(buffer, rc);
        std::cout << message << std::endl;
    }
}
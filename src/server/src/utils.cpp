/**
 * @file utils.cpp
 * @author lqy
 * @brief Server program for socket programming
 * @version 0.1.0
 * @date 2024-10-01
 */

#include "../include/micros.h"
#include <iostream>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>

void serverError(std::string message)
{
    std::cerr << "\033[31m[Server] " << message << "\033[0m" << std::endl;
    exit(-1);
}

void printMessage(std::string msg)
{
    // TODO
}

int init(int type)
{
    int serverSocket = -1;
    if (type == SERVER_TYPE_TCP) { // TCP.
        serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    } else if (type == SERVER_TYPE_UDP) { // UDP.
        serverSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP); 
    }

    if (serverSocket < 0) serverError("Failed to create socket.");

    return serverSocket;
}

void setOptions(int serverSocket)
{
    // Set reuse address.
    int on = 1;
    setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, (char *)&on, sizeof(on));

    // Set timeout.
    struct timeval timeout;
    timeout.tv_sec = 5; // Seconds.
    timeout.tv_usec = 0; // Microseconds.
    setsockopt(serverSocket, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(timeout));
}

void bindAddress(int serverSocket, std::string serverIp, int serverPort)
{
    struct sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = serverIp.size() ? inet_addr(serverIp.c_str()) : INADDR_ANY;
    serverAddress.sin_port = htons(serverPort);

    int rc = bind(serverSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress));
    if (rc < 0) serverError("Failed to bind address.");
}

void startListen(int serverSocket, int queueSize)
{
    int rc = listen(serverSocket, queueSize);
    if (rc < 0) serverError("Failed to listen.");
}

// Create server main thread.

void *Thread_Socket(void *arg)
{
    int serverSocket = reinterpret_cast<int>(arg); // Get server socket from argument.
    worker(serverSocket); // Process server thread.
    return nullptr;
}

void startSocketThread(int serverSocket)
{
    pthread_t thread; // Thread id.

    // Create server thread.
    pthread_create(&thread, nullptr, Thread_Socket, reinterpret_cast<void *>(serverSocket));
}

// TCP.

void worker(int serverSocket)
{
    waitAndAccept(serverSocket);
}

void waitAndAccept(int serverSocket)
{
    while (1) {
        int clientSocket = accept(serverSocket, nullptr, nullptr);
        if (clientSocket < 0) serverError("Failed to accept.");
        std::cout << "\033[32m[Server] New client connected, socket = " << clientSocket << "\033[0m" << std::endl;
        startClientThread(clientSocket);
    }
}

void *Thread_Client(void *arg)
{
    int clientSocket = (long)arg & 0xffffffff; // Get client socket from argument.
    process(clientSocket); // Process client thread.
    return nullptr;
}

void startClientThread(int clientSocket)
{
    pthread_t thread; // Thread id.

    // Create client thread.
    pthread_create(&thread, nullptr, Thread_Client, (void *)(long)clientSocket);

    // Save client information.
    saveConnectInfo(clientSocket, thread);
}

struct ClientInfo {
    int status; // 0: unconnected; 1: connected.
    int socket; // TCP socket.
    in_addr_t ip;
    in_port_t port;
    pthread_t thread;
} clientQueue[SERVER_QUEUE_SIZE];

void saveConnectInfo(int clientSocket, pthread_t thread)
{
    struct sockaddr_in clientAddr;
    socklen_t len = sizeof(clientAddr);

    // Get client address.
    getpeername(clientSocket, (struct sockaddr *)&clientAddr, &len);

    in_addr_t clientIp = clientAddr.sin_addr.s_addr;
    in_port_t clientPort = clientAddr.sin_port;

    // Enqueue client information.
    int index1, index2;
    for (index1 = 0; index1 < SERVER_QUEUE_SIZE; index1++) {
        if (!clientQueue[index1].status) {
            clientQueue[index1].status = 1;
            clientQueue[index1].socket = clientSocket;
            clientQueue[index1].ip = clientIp;
            clientQueue[index1].port = clientPort;
            clientQueue[index1].thread = thread;
            break;
        }
    }

    // Close the client's other connections.
    for (index2 = 0; index2 < SERVER_QUEUE_SIZE; index2++) {
        if (index1 != index2 && clientQueue[index2].status && clientQueue[index2].ip == clientIp && clientQueue[index2].port == clientPort) {
            close(clientQueue[index2].socket); // Close the socket.
            pthread_cancel(clientQueue[index2].thread); // Cancel the thread.
            clientQueue[index2].status = 0; // Clear the status.
        }
    }
}

void process(int clientSocket)
{
    char buffer[SERVER_BUFFER_SIZE];
    while (1) {
        ssize_t rc = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (rc <= 0) break;
        std::string message(buffer, rc);
        std::cout << "\033[36m[Client|" << clientSocket << "] " << message << "\033[0m" << std::endl;
        // Feedback.
        // TODO
    }
    close(clientSocket); // Close the socket.
    std::cout << "\033[32m[Server] Client closed, socket = " << clientSocket << "\033[0m" << std::endl;
}

// UDP.

void worker(int serverSocket)
{
    char buffer[SERVER_BUFFER_SIZE];
    struct sockaddr_in clientAddr;
    socklen_t len = sizeof(clientAddr);

    while (1) {
        ssize_t rc = recvfrom(serverSocket, buffer, sizeof(buffer), 0, (struct sockaddr *)&clientAddr, &len);
        if (rc <= 0) break;
        std::string message(buffer, rc);
        std::cout << "\033[36m[Client|" << inet_ntoa(clientAddr.sin_addr) << ":" << ntohs(clientAddr.sin_port) << "] " << message << "\033[0m" << std::endl;
        // Feedback.
        // TODO
        // Add saveConnectInfo.
    }
    close(serverSocket); // Close the socket.
    std::cout << "\033[32m[Server] Socket closed, socket = " << serverSocket << "\033[0m" << std::endl;
}

struct ClientInfo {
    int status; // 0: unconnected; 1: connected.
    in_addr_t ip;
    in_port_t port;
} clientQueue[SERVER_QUEUE_SIZE];

void saveConnectInfo(in_addr_t clientIp, in_port_t clientPort, int clientStatus)
{
    int index1, index2;
    if (clientStatus) { // New connection.
        for (index1 = 0; index1 < SERVER_QUEUE_SIZE; index1++) {
            if (!clientQueue[index1].status) {
                clientQueue[index1].status = 1;
                clientQueue[index1].ip = clientIp;
                clientQueue[index1].port = clientPort;
                break;
            }
        }
    } else index1 = SERVER_QUEUE_SIZE; // Close connection.

    // Close the client's other connections.
    for (index2 = 0; index2 < SERVER_QUEUE_SIZE; index2++) {
        if (index2 != index1 && clientQueue[index2].status && clientQueue[index2].ip == clientIp && clientQueue[index2].port == clientPort) {
            clientQueue[index2].status = 0; // Clear the status.
        }
    }
}

// TCP_main

void server_main()
{
    // Initialization.
    int serverSocket = init(SERVER_TYPE_TCP);
    setOptions(serverSocket);

    // Bind ip and port.
    std::string serverIp = SERVER_IP;
    int serverPort = SERVER_PORT;
    bindAddress(serverSocket, serverIp, serverPort);

    // Start listening.
    startListen(serverSocket, SERVER_QUEUE_SIZE);
    std::cout << "\033[32m[Server] Listening on " << serverIp << ":" << serverPort << "\033[0m" << std::endl;

    // Main socket thread.
    startSocketThread(serverSocket);

    // User cmds.
    // TODO
}

// UDP_main

void server_main()
{
    // Initialization.
    int serverSocket = init(SERVER_TYPE_UDP);
    setOptions(serverSocket);

    // Bind ip and port.
    std::string serverIp = SERVER_IP;
    int serverPort = SERVER_PORT;
    bindAddress(serverSocket, serverIp, serverPort);

    // Main socket thread.
    startSocketThread(serverSocket);

    // User cmds.
    // TODO
}
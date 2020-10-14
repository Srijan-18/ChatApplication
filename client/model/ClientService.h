#pragma once

#include <iostream>
#include <sys/socket.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <vector>
#include <thread>
#include <mutex>
#define PORT 45000
inline char message[100];
enum SERVICE_CONSTANTS
{
    SOCKET_CREATED = 0,
    MIN_LENTGTH = 0
};

void *recv_message(void *my_sock);

class ClientService
{
private:
    char send_msg[500];
    pthread_t receiving_thread;
    int length;
    int client_socket;
    int socket_value;

    struct sockaddr_in ServerIp;
    std::string client_name;

public:
    int createConnection();
    bool getConnectionStatus();
    void getClientMessage(std::string);
    void closeConnection();
    void setClientName(std::string);
    void createThread();
    friend class ClientController;
};
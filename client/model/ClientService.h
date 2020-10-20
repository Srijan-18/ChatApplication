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

using namespace std;

#define PORT 50001
#define CHATROOM "CHATROOM"
#define EXIT "bye exit"
static char message[100];
static int client_socket;
enum SERVICE_CONSTANTS
{
    SOCKET_CREATED = 0,
    MIN_LENTGTH = 0
};


void *recv_message(void *my_sock);

class ClientService
{
private:
    //char msg[500];
    pthread_t receiving_thread;
    int length;
    //int client_socket;
    int socket_value;
    

    struct sockaddr_in ServerIp;
    std::string client_name;
    std::string delimiter = ">=";

public:
    int createConnection();
    bool getConnectionStatus();
    void chatroomMessage();
    void closeConnection();
    void registerClient(int, std::string);
    int getSocketValue();
    std::string getUserName();
    void setClientName(std::string);
    void sendToServer(int, std::string);
    std::string receiveFromServer(int);
    friend class ClientController;
    std::string getMessage();
    void exitMethod();
    static void setMessage(std::string);
    static void *message_helper(void *context)
    {
        return ((ClientService *)context)->recv_message(&client_socket);
    }
    void *recv_message(void *);
};
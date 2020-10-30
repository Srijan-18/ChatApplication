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
#include "../../utility/StringUtility.cpp"

using namespace std;

#define PORT 50001
#define CHATROOM "CHATROOM"
#define BACK "##BACK"
#define CONNECT "CONNECT"
#define INSTANT_REPLY "$"

static char message[4096];
static int client_socket;

enum SERVICE_CONSTANTS
{
    SOCKET_CREATED = 0,
    MIN_LENTGTH = 0
};

class ClientService
{
private:
    pthread_t receiving_thread;
    int length;

    struct sockaddr_in ServerIp;
    std::string client_name;
    std::string client_password;
    std::string delimiter = ">=";

public:
    int createConnection();
    bool getConnectionStatus();
    void chatroomMessage();
    void closeConnection();
    void registerClient(int, std::string, std::string);
    bool loginClient(int, string, string);
    void setClientPassword(std::string);
    int getSocketValue();
    std::string getClientName();
    std::string getClientPassword();
    void setClientName(std::string);
    void sendToServer(int, std::string);
    std::string receiveFromServer(int);
    friend class ClientController;
    std::string getMessage();
    void exitMethod();
    static void setMessage(std::string);
    vector<string> getOnlineClients();
    void oneToOneMessage(string);
    string sendToOtherClient(string);
    static void *message_helper(void *context)
    {
        return ((ClientService *)context)->recv_message(&client_socket);
    }
    void *recv_message(void *);
};
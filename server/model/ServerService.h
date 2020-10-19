#pragma once
#include <iostream>
#include <sys/socket.h>
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <unistd.h>
#include <map>
#include <sstream>
#include <vector>
#include <istream>

#define CLIENTS "CLIENTS"
#define REGISTER "REGISTER"
#define CHATROOM "CHATROOM"
#define EXIT "bye exit"

using namespace std;

static int client_socket;

enum SERVICE_CONSTANTS
{
    SOCKET_CREATED = 0,
    MIN_LENTGTH = 0
};

class ServerService
{
    pthread_mutex_t mutex;
    int clients[20];
    struct sockaddr_in ServerIp;
    pthread_t recvt;
    int sock = 0, client_sock = 0;
    int client_array_size = 0;

public:
    int createConnection();
    bool getConnectionStatus();
    void sendToClient(int, string);
    void acceptIncomingClients();
    string receiveFromClient(int);
    void sendToAllClients(string, int);
    void exitClientMethod(int, vector<string>);
    void addOnlineClient(string, int);
    vector<string> splitter(const string &, string);
    void createMessageFormat(vector<string> &, int);
    void *receiveInputFromClient(void *);
    static void *threadReferenceHelper(void *context)
    {
        return ((ServerService *)context)->receiveInputFromClient(&client_socket);
    }
};
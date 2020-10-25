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
#include "../../client/model/ClientStructure.cpp"
#include "../../utility/stringUtility.cpp"
#include "../mongodb/DBOperations.h"
#include <algorithm>

#define REGISTER "REGISTER"
#define CHATROOM "CHATROOM"
#define LOGIN "LOGIN"
#define ONLINE_CLIENTS "ONLINE"
#define BACK "##BACK"
#define QUIT "#!EXIT"

using namespace std;

static int client_socket;
static vector<int> clients;
static vector<Client> client_data;

enum SERVICE_CONSTANTS
{
    SOCKET_CREATED = 0,
    MIN_LENTGTH = 0,
    MIN_SOCKET_VALUE = 0
};

class ServerService
{
    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
    struct sockaddr_in ServerIp;
    pthread_t recvt;
    int sock = 0;
    MongoDB mongo_obj;


public:
    int createConnection();
    bool getConnectionStatus();
    void sendToClient(int, string);
    void acceptIncomingClients();
    string receiveFromClient(int);
    void sendToAllClients(string, int);
    void exitClientMethod(int, vector<string>);
    void addOnlineClient(string,int);
    bool checkOnline(string);
    void saveClientCredentials(string, string, int);
    bool checkClientsCredentials(string, string);
    bool findGivenUser(string);
    vector<string> splitter(const string &, string);
    void createMessageFormat(vector<string> &, int);
    void setChatroomStatus(int);
    void removeClientFromServer(int);
    void *receiveInputFromClient(void *);
    void getChatroomClients(string, int);
    static void *threadReferenceHelper(void *context)
    {
        return ((ServerService *)context)->receiveInputFromClient(&client_socket);
    }
};
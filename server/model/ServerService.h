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

class ServerService{
    public:
    int createConnection();
    bool getConnectionStatus();
    void sendToClient(int, string);
    string receiveFromClient(int);
    void sendToAllClients(string, int);
    void exitClientMethod(int, vector<string>);
    void addOnlineClient(string, int);
    vector<string> splitter(const string&, string);
    void createMessageFormat(vector<string>&, int);
    void* receiveInputFromClient(void*);
}
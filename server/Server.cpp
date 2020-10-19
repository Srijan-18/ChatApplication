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

pthread_mutex_t mutex;
int clients[20];

int n = 0;
int first_encounter = 0;
char client_name[100];
std::map<std::string, int> online_clients;
int flag_username_exits = 0;

void sendToClient(int sock, std::string send_msg)
{
    send(sock, send_msg.c_str(), strlen(send_msg.c_str()), 0);
}

std::string receiveFromClient(int sock)
{
    char msg[500];
    bzero(msg, sizeof(msg));
    recv(sock, msg, 500, 0);
    return std::string(msg);
}

void sendtoall(std::string msg, int curr)
{
    int i;
    pthread_mutex_lock(&mutex);
    for (i = 0; i < n; i++)
    {
        if (clients[i] > 0)
        {
            if (clients[i] != curr)
            {
                sendToClient(clients[i], std::string(msg));
                continue;
            }
        }
    }
    pthread_mutex_unlock(&mutex);
}

void sendToOne(std::string msg, int socket, string other_client_name)
{
    pthread_mutex_lock(&mutex);
    for (auto elem : online_clients)
    {
        if (elem.first == other_client_name)
            sendToClient(elem.second, msg);
    }
    pthread_mutex_unlock(&mutex);
}

void exitMethod(int sock, vector<string> messageVector)
{
    string message = messageVector[1] + messageVector[2];
    sendtoall(message, sock);
    for (int i = 0; i < n; i++)
    {
        if (clients[i] == sock)
        {
            clients[i] = -1;
        }
    }

}

void showOnlineClient(int sock)
{
    for (auto elem : online_clients)
    {
        if (sock != elem.second)
            sendToClient(sock, elem.first);
    }
}

void addOnlineClient(std::string client_name, int sock)
{
    online_clients[client_name] = sock;
}

std::vector<std::string> splitter(const std::string &client_response, std::string delimiter)
{
    size_t pos_start = 0, pos_end, delim_len = delimiter.length();
    std::string token;
    std::vector<std::string> res;

    while ((pos_end = client_response.find(delimiter, pos_start)) != std::string::npos)
    {
        token = client_response.substr(pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        res.push_back(token);
    }
    res.push_back(client_response.substr(pos_start));
    return res;
}

void createMessageFormat(vector<string> &client_message, int sock)
{
    string message;
    string client_name;
    if (client_message[0] == CHATROOM)
    {
        for (int i = 1; i < client_message.size(); i++)
        {
            message += client_message[i] + " ";
        }
        client_name = client_message[1];
        sendtoall(message, sock);
    }
    message.clear();
}

void *recvmg(void *client_sock)
{
    int sock = *((int *)client_sock);

    int len;
    char client_name[10];
    std::string delimiter = ">=";
    while (1)
    {
        std::string received_from_client = receiveFromClient(sock);
        std::vector<std::string> client_response = splitter(received_from_client, delimiter);

        if (client_response[0] == REGISTER)
        {
            addOnlineClient(client_response[1], sock);
            client_response.clear();
        }

        if (client_response[0] == EXIT)
        {
            exitMethod(sock, client_response);
        }

        if (client_response[0] == CHATROOM)
        {
            createMessageFormat(client_response, sock);
        }
        received_from_client.clear();
    }
}

int main()
{
    system("clear");
    struct sockaddr_in ServerIp;
    pthread_t recvt;
    int sock = 0, Client_sock = 0;

    ServerIp.sin_family = AF_INET;
    ServerIp.sin_port = htons(54200);
    ServerIp.sin_addr.s_addr = INADDR_ANY;
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (bind(sock, (struct sockaddr *)&ServerIp, sizeof(ServerIp)) == -1)
        printf("[-]Bind unsuccessful, error!! \n");
    else
        printf("[+]Server Started\n");

    if (listen(sock, 2) == -1)
        printf("[-]Listening failed \n");

    while (1)
    {
        if ((Client_sock = accept(sock, (struct sockaddr *)NULL, NULL)) < 0)
            printf("[-]Accept failed  \n");
        pthread_mutex_lock(&mutex);
        clients[n] = Client_sock;
        n++;
        // creating a thread for each client
        pthread_create(&recvt, NULL, &recvmg, &Client_sock);
        pthread_mutex_unlock(&mutex);
    }
    return 0;
}
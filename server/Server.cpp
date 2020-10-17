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
#define ONLINE "ONLINE"
#define CONNECT "CONNECT"
#define CHATROOM "CHATROOM"

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
    cout << "send to client function :: message : " << send_msg << endl;
    send(sock, send_msg.c_str(), strlen(send_msg.c_str()), 0);
    //memset(msg, 0, sizeof(msg));
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
    std::cout << "send to all function in server : " << std::string(msg) << std::endl;
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
    std::cout << "\033[0;34m"
              << "-----------send to one function--------------"
              << "\033[0m" << std::endl;
    pthread_mutex_lock(&mutex);
    for (auto elem : online_clients)
    {
        cout << "Name: " << elem.first << endl;
        cout << "Other Name:" << other_client_name << endl;
        if (elem.first == other_client_name)
            sendToClient(elem.second, msg);
    }
    pthread_mutex_unlock(&mutex);
}

// int getClientSocket(string client_name)
// {

// }

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
    for (auto elem : online_clients)
    {
        std::cout << "Map ----- name : " << elem.first << "\n";
    }
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
    if (client_message[0] == CONNECT)
    {
        for (int i = 2; i < client_message.size(); i++)
        {
            cout << "message format section : " << client_message[i];
            message += client_message[i] + " ";
        }
        client_name = client_message[1];
        sendToOne(message, sock, client_message[1]);
    }
    if (client_message[0] == CHATROOM)
    {
        for (int i = 1; i < client_message.size(); i++)
        {
            cout << "group message format section : " << client_message[i];
            message += client_message[i] + " ";
        }
        client_name = client_message[1];
        sendtoall(message,sock);
    }
    message.clear();
}

void *recvmg(void *client_sock)
{
    int sock = *((int *)client_sock);

    int len;
    char client_name[10];
    //bzero(msg, sizeof(msg));
    std::string delimiter = ">=";
    while (1)
    {
        //memset(msg, 0, sizeof(msg));
        //len = recv(sock, msg, 500, 0);
        std::string received_from_client = receiveFromClient(sock);
        std::vector<std::string> client_response = splitter(received_from_client, delimiter);

        if (client_response[0] == CLIENTS)
        {
            showOnlineClient(sock);
            client_response.clear();
        }

        if (client_response[0] == REGISTER)
        {
            addOnlineClient(client_response[1], sock);
            client_response.clear();
        }

        // if (client_response[0] == CONNECT)
        // {

        // }

        // if (client_name[0] == ONLINE) {

        // }

        // if (strcmp(msg, "bye") == 10)
        // {
        //     for (int i = 0; i < n; i++)
        //     {
        //         if (clients[i] == sock)
        //         {
        //             clients[i] = -1;
        //         }
        //     }
        // }

        //msg[len] = '\0';
        if (client_response[0] == CONNECT)
        {
            cout << "In connect condition:  " << received_from_client << endl;
            createMessageFormat(client_response, sock);
        }

        if (client_response[0] == CHATROOM)
        {
            cout << "In group condition: " << received_from_client << endl;
            createMessageFormat(client_response, sock);
        }

        // if (client_response[0] != REGISTER || client_response[0] != CLIENTS)
        // {
        //     string message;
        //     for (int i = 3; i < client_response.size(); i++)
        //     {
        //         if (client_response[i] != CLIENTS)
        //             message += client_response[i] + " ";
        //     }
        //     // if (client_response[0] == CONNECT)
        //     // {
        //     //     for (int i = 0; i < client_response.size(); i++)
        //     //     {
        //     //         cout << "*************" << client_response[i] << endl;
        //     //     }
        //     //     cout << "--------> name of other client : " << client_response[2] << endl;
        //     //     sendToOne(message, sock, client_response[2]);
        //     //     client_response.clear();
        //     //     message.clear();
        //     // }
        //     //sendtoall(message, sock);
        //     message.clear();
        //     //client_response.clear();
        // }
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
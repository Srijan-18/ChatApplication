#include "ServerService.h"

int ServerService::createConnection()
{
    system("clear");
    ServerIp.sin_family = AF_INET;
    ServerIp.sin_port = htons(47896);
    ServerIp.sin_addr.s_addr = htonl(INADDR_ANY);
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if ((bind(sock, (struct sockaddr *)&ServerIp, sizeof(ServerIp))) == -1)
        perror("[-]Bind unsuccessful, error!!");
    else
        cout << "[+]Server Started\n";
    if (listen(sock, 2) == -1)
        perror("[-]Listening failed \n");
    return sock;
}

bool ServerService::getConnectionStatus()
{
    int connection_status_value = createConnection();
    if (connection_status_value >= SOCKET_CREATED)
        return true;
    return false;
}

void ServerService::acceptIncomingClients()
{
    ServerService service;
    online_clients["Srijan"] = 0;
    client_credentials["Srijan"] = "1234";
    while (1)
    {
        if ((client_socket = accept(sock, (struct sockaddr *)NULL, NULL)) < 0)
            printf("[-]Accept failed  \n");
        pthread_mutex_lock(&mutex);
        clients.push_back(client_socket);
        //creating a thread for each client
        pthread_create(&recvt, NULL, &threadReferenceHelper, &service);
        pthread_mutex_unlock(&mutex);
    }
}

void ServerService::sendToClient(int sock, string send_msg)
{
    send(sock, send_msg.c_str(), strlen(send_msg.c_str()), 0);
}

void ServerService::sendToAllClients(string message, int current_sock)
{
    int client;
    pthread_mutex_lock(&mutex);
    for (client = 0; client < clients.size(); client++)
    {
        if (clients[client] > MIN_SOCKET_VALUE && clients[client] != current_sock)
        {
            {
                sendToClient(clients[client], message);
            }
        }
    }
    pthread_mutex_unlock(&mutex);
}

void ServerService::exitClientMethod(int sock, vector<string> messageVector)
{

    string message = messageVector[1] + messageVector[2];
    sendToAllClients(message, sock);
    for (int i = 0; i < clients.size(); i++)
    {
        if (clients[i] == sock)
        {
            clients[i] = -1;
        }
    }
    for (auto element : online_clients)
    {
        if (element.second == sock)
        {
            online_clients.erase(element.first);
            break;
        }
    }
}

string ServerService::receiveFromClient(int sock)
{
    char msg[500];
    bzero(msg, sizeof(msg));
    recv(sock, msg, 500, 0);
    return std::string(msg);
}

void ServerService::addOnlineClient(std::string client_name, int sock)
{
    online_clients[client_name] = sock;
    sendToClient(sock, "\nREGISTERATION SUCCESSFUL");

}

void ServerService::createMessageFormat(vector<string> &client_message, int sock)
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
        sendToAllClients(message, sock);
    }
    message.clear();
}

void ServerService::saveClientCredentials(string client_name, string client_password)
{
    client_credentials[client_name] = client_password;
}

bool ServerService::checkClientsCredentials(string user_name, string password, int sock)
{
    bool client_duplication = false;
    cout << user_name << " Logged In." << endl;
    for (auto element : client_credentials)
    {
        if (element.first == user_name && element.second == password)
        {
            online_clients[user_name] = sock;
            client_duplication = true;
            break;
        }
    }
    return client_duplication;
}

std::vector<std::string> ServerService::splitter(const std::string &client_response, std::string delimiter)
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

void *ServerService::receiveInputFromClient(void *client_sock)
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
            cout << "Registering " << client_response[1] << " ..." << endl;
            if (!checkClientsCredentials(client_response[1], client_response[2], sock))
            {
                saveClientCredentials(client_response[1], client_response[2]);
                addOnlineClient(client_response[1], sock);
            }
            else
            {
                sendToClient(sock, "\nUSER ALREADY REGISTERED");
            }
            
            client_response.clear();
        }

        if (client_response[0] == LOGIN)
        {
            checkClientsCredentials(client_response[1], client_response[2], sock) 
             ? sendToClient(sock, "LOGIN SUCCESS") 
             : sendToClient(sock, "LOGIN FAILED");
                    
            client_response.clear();
        }

        if (client_response[0] == EXIT)
        {
            exitClientMethod(sock, client_response);
        }

        if (client_response[0] == CHATROOM)
        {
            createMessageFormat(client_response, sock);
        }
        received_from_client.clear();
    }
}

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
    for (auto element : client_data)
    {
        if (element.socket == sock)
        {
            element.online_status = false;
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

void ServerService::addOnlineClient(std::string client_name)
{
    for (auto client_iterator = client_data.begin(); client_iterator != client_data.end(); client_iterator++)
    {
        if (client_iterator->name == client_name)
        {
            client_iterator->online_status = true;
            break;
        }
    }
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

void ServerService::saveClientCredentials(string client_name, string client_password, int socket)
{
    client_ref.socket = socket;
    client_ref.name = client_name;
    client_ref.password = client_password;
    client_data.push_back(client_ref);
    sendToClient(socket, "\nREGISTERATION SUCCESSFUL");
}

bool ServerService::checkOnline(string name)
{
    for (auto client_iterator = client_data.begin(); client_iterator != client_data.end(); client_iterator++)
    {
        if (client_iterator->name == name && client_iterator->online_status)
        {
            return true;
        }
    }
    return false;
}

bool ServerService::checkClientsCredentials(string user_name, string password, int sock)
{
    bool client_duplication = false;
    cout << user_name << " Logged In." << endl;
    for (auto client_iterator = client_data.begin(); client_iterator != client_data.end(); client_iterator++)
    {
        if (client_iterator->name == user_name)
        {
            client_duplication = true;
            break;
        }
    }
    return client_duplication;
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
        std::vector<std::string> client_response = StringUtility::splitter(received_from_client, delimiter);

        if (client_response[0] == REGISTER)
        {
            cout << "Registering " << client_response[1] << " ..." << endl;
            if (!checkClientsCredentials(client_response[1], client_response[2], sock))
            {
                saveClientCredentials(client_response[1], client_response[2], sock);
            }
            else
            {
                sendToClient(sock, "\nUSER ALREADY REGISTERED");
            }

            client_response.clear();
        }
        if (client_response[0] == ONLINE_CLIENTS)
        {
            string onlineUsers = "No clients online";
            for (auto client_iterator = client_data.begin(); client_iterator != client_data.end(); client_iterator++)
            {
                if (client_iterator->online_status && client_iterator->socket != sock)
                {
                    onlineUsers = onlineUsers + delimiter + client_iterator->name;
                }
            }
            sendToClient(sock, onlineUsers);
        }
        if (client_response[0] == LOGIN)
        {
            if (!checkOnline(client_response[1]))
            {
                if (checkClientsCredentials(client_response[1], client_response[2], sock))
                {
                    sendToClient(sock, "LOGIN SUCCESS");
                    addOnlineClient(client_response[1]);
                }
                else
                    sendToClient(sock, "LOGIN FAILED");
            }
            else
                sendToClient(sock, "USERONLINE");

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

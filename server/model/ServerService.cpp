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
    for (auto client_iterator = client_data.begin(); client_iterator != client_data.end(); client_iterator++)
    {
        if (client_iterator->chatroom_status && client_iterator->online_status && client_iterator->socket != current_sock)
            sendToClient(client_iterator->socket, message);
    }
    pthread_mutex_unlock(&mutex);
}

void ServerService::sendToOne(std::string msg, int socket, string other_client_name)
{
    pthread_mutex_lock(&mutex);
    for (auto client_iterator = client_data.begin(); client_iterator != client_data.end(); client_iterator++)
    {
        if (client_iterator->online_status)
            if (client_iterator->name == other_client_name && client_iterator->individual_chat)
                sendToClient(client_iterator->socket, msg);
    }
    pthread_mutex_unlock(&mutex);
}

void ServerService::exitClientMethod(int sock, vector<string> messageVector)
{
    string message = messageVector[1] + messageVector[2];
    for (auto client_iterator = client_data.begin(); client_iterator != client_data.end(); client_iterator++)
    {
        if (client_iterator->socket == sock && client_iterator->online_status)
        {
            if (messageVector[messageVector.size() - 1] == CONNECT)
            {
                client_iterator->individual_chat = false;
            }
            else
            {
                client_iterator->chatroom_status = false;
                sendToAllClients(message, sock);
            }
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
    Client client_ref;
    client_ref.socket = sock;
    client_ref.name = client_name;
    client_ref.online_status = true;
    client_data.push_back(client_ref);
}

void ServerService::createMessageFormat(vector<string> client_message, int sock)
{
    string message;
    if (client_message[0] == CHATROOM)
    {
        for (int i = 1; i < client_message.size(); i++)
        {
            message += client_message[i] + " ";
        }
        sendToAllClients(message, sock);
    }

    if (client_message[0] == CONNECT)
        if (client_message[client_message.size() - 1] == "HISTORY")
        {
            string previous_messages = mongo_obj.getChats(client_message[1], client_message[3]);
            send(sock, previous_messages.c_str(), strlen(previous_messages.c_str()), 0);
        }
        else
        {
            for (int i = 2; i < client_message.size(); i++)
            {
                message += client_message[i] + " ";
            }
            sendToOne(message, sock, client_message[1]);
        }

    if (client_message[0] == INSTANT_REPLY)
    {
        for (int i = 2; i < client_message.size(); i++)
        {
            message += client_message[i] + " ";
        }
        int pos = message.find_first_of(" ");
        mongo_obj.saveMessage(client_message[2], client_message[1], message.substr(pos + 1, message.length() - pos -1));
        sendToOne(message, sock, client_message[1]);
    }
    message.clear();
}

void ServerService::saveClientCredentials(string client_name, string client_password, int socket)
{
    mongo_obj.saveGivenUser(client_name, client_password);
    sendToClient(socket, "\n\033[32;1mREGISTERATION SUCCESSFUL\033[0m");
}

void ServerService::setChatroomStatus(int sock)
{
    for (auto client_iterator = client_data.begin(); client_iterator != client_data.end(); client_iterator++)
    {
        cout << "Name : " << client_iterator->name << endl;
        if (client_iterator->socket == sock && client_iterator->online_status)
        {
            client_iterator->chatroom_status = true;
        }
    }
}

void ServerService::setIndividualChatStatus(int sock)
{
    for (auto client_iterator = client_data.begin(); client_iterator != client_data.end(); client_iterator++)
    {
        cout << "Name : " << client_iterator->name << endl;
        if (client_iterator->socket == sock && client_iterator->online_status)
        {
            client_iterator->individual_chat = true;
            break;
        }
    }
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

bool ServerService::findGivenUser(string userID)
{
    cout << "Searching for " << userID << " in data base" << endl;
    bool result = mongo_obj.checkUserPresence(userID);
    result ? cout << userID << " found." << endl : cout << userID << " not found" << endl;

    return result;
}

bool ServerService::checkClientsCredentials(string userID, string password)
{
    cout << "Validating userID and password for " << userID << endl;
    return mongo_obj.authenticateUser(userID, password);
}

void ServerService::removeClientFromServer(int sock)
{
    for (auto client_iterator = client_data.begin(); client_iterator != client_data.end(); client_iterator++)
    {
        if (client_iterator->socket == sock)
        {
            cout << client_iterator->name << " disconnected" << endl;
            client_data.erase(client_iterator);
            break;
        }
    }
}

void ServerService::getChatroomClients(string name, int sock)
{
    for (int i = 0; i < client_data.size(); i++)
    {
        if (client_data[i].socket != sock && client_data[i].chatroom_status)
        {
            string temp = "\033[32;1m" + client_data[i].name + "\033[0m" + " joined the chat\n";
            sendToClient(sock, temp);
        }
    }
}

void *ServerService::receiveInputFromClient(void *client_sock)
{
    int sock = *((int *)client_sock);
    int flag = 0;
    int len;
    char msg[4096];
    char client_name[10];
    std::string delimiter = ">=";
    while (1)
    {
        std::string received_from_client = receiveFromClient(sock);
        std::vector<std::string> client_response = StringUtility::splitter(received_from_client, delimiter);
        if (client_response[0] == REGISTER)
        {
            if (!findGivenUser(client_response[1]))
                saveClientCredentials(client_response[1], client_response[2], sock);
            else
                sendToClient(sock, "\n\033[31;1mUSER ALREADY REGISTERED\033[0m");

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
                if (checkClientsCredentials(client_response[1], client_response[2]))
                {

                    sendToClient(sock, "LOGIN SUCCESS");
                    addOnlineClient(client_response[1], sock);
                }
                else
                    sendToClient(sock, "LOGIN FAILED");
            }
            else
                sendToClient(sock, "USERONLINE");

            client_response.clear();
        }

        if (client_response[0] == BACK)
        {
            if (flag == 1)
            {
                exitClientMethod(sock, client_response);
                flag = 0;
            }
        }

        if (client_response[0] == QUIT)
        {
            removeClientFromServer(sock);
        }

        if (client_response[0] == CHATROOM)
        {
            if (flag == 0)
            {
                flag = 1;
                string notify_others;
                notify_others = "\033[32;1m" + client_response[1] + "\033[0m joined the chat\n";
                sendToAllClients(notify_others, sock);
                setChatroomStatus(sock);
                getChatroomClients(client_response[1], sock);
            }
            else
            {
                createMessageFormat(client_response, sock);
            }
        }

        if (client_response[0] == CONNECT)
        {

            if (flag == 0)
            {
                flag = 1;
                setIndividualChatStatus(sock);
            }
            else
            {
                mongo_obj.saveMessage(client_response[2], client_response[1], client_response[3]);
            }
            createMessageFormat(client_response, sock);
        }
        if (client_response[0] == INSTANT_REPLY)
        {
            createMessageFormat(client_response, sock);
        }

        received_from_client.clear();
    }
}
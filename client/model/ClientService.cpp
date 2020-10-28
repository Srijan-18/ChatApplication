#include "ClientService.h"

int ClientService::createConnection()
{
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    ServerIp.sin_port = htons(47896);
    ServerIp.sin_family = AF_INET;
    ServerIp.sin_addr.s_addr = INADDR_ANY;
    connect(client_socket, (struct sockaddr *)&ServerIp, sizeof(ServerIp));
    return client_socket;
}

void ClientService::registerClient(int sock, std::string username, std::string password)
{
    std::string send_to_client;
    send_to_client = "REGISTER>=" + username + ">=" + password;
    sendToServer(sock, send_to_client);
    cout << receiveFromServer(sock) << endl;
}

bool ClientService::getConnectionStatus()
{
    int connection_status_value = createConnection();
    if (connection_status_value >= SOCKET_CREATED)
        return true;
    return false;
}

int ClientService::getSocketValue()
{
    return client_socket;
}

vector<string> ClientService::getOnlineClients()
{
    sleep(1);
    sendToServer(client_socket, "ONLINE>=");
    string server_respone = receiveFromServer(client_socket);
    vector<string> response = StringUtility::splitter(server_respone, ">=");
    if (response.size() > 1)
        response.erase(response.begin());
    return response;
}

void ClientService::chatroomMessage()
{
    pthread_t recvt;
    ClientService service;
    std::string send_msg;
    std::string chatroom_chat = CHATROOM + delimiter;
    int flag = 0;
    int pos = 0;
    pthread_create(&recvt, NULL, message_helper, &service);
    memset(message, 0, sizeof(message));
    while (fgets(message, sizeof(message), stdin) > 0)
    {
        if (flag == 0)
        {
            send_msg = CHATROOM + delimiter + client_name;
            flag = 1;
        }
        else
        {
            send_msg = chatroom_chat + client_name + delimiter + ":" + delimiter + std::string(message);
        }

        if (send_msg.find(BACK) != std::string::npos)
        {
            flag = 0;
            send_msg.clear();
            send_msg = BACK + delimiter + client_name + delimiter + " has left the chat\n";
            sendToServer(client_socket, send_msg);
            break;
        }

        if (send_msg.find(INSTANT_REPLY) != string::npos)
        {
            send_msg.clear();
            send_msg = sendToOtherClient(std::string(message));
            sendToServer(client_socket, send_msg);
            continue;
        }

        sendToServer(client_socket, send_msg);
        std::cout << "\n";
    }
    pthread_detach(pthread_self());
}

string ClientService::sendToOtherClient(std::string message)
{
    message.insert(1,1,'>');
    message.insert(2,1,'=');
    int position = message.find_first_of(' ');
    
    std::string new_message = message.substr(0, position) + ">=" + client_name + ":" + ">=" + message.substr(position + 1, message.length());
    return new_message;
}

void ClientService::oneToOneMessage(string other_client)
{
    std::string delimiter = ">=";
    std::string send_msg;
    pthread_t recvt;
    int flag = 0;
    ClientService service;
    pthread_create(&recvt, NULL, message_helper, &service);
    memset(message, 0, sizeof(message));
    while (fgets(message, sizeof(message), stdin) > 0)
    {
        if (flag == 0)
        {
            send_msg = CONNECT + delimiter + client_name + delimiter + "Sending Messages to : " + delimiter + other_client + "\n" + delimiter + "HISTORY";
            flag = 1;
        }
        else
        {
            send_msg = CONNECT + delimiter + other_client + delimiter + client_name + ":" + delimiter + std::string(message);
            
        }

        if (send_msg.find(BACK) != std::string::npos)
        {
            flag = 0;
            send_msg.clear();
            send_msg = BACK + delimiter + client_name + delimiter + " has left the chat\n" + delimiter + CONNECT;
            sendToServer(client_socket, send_msg);
            break;
        }

        if (send_msg.find(INSTANT_REPLY) != string::npos)
        {
            send_msg.clear();
            send_msg = sendToOtherClient(std::string(message));
            sendToServer(client_socket, send_msg);
            continue;
        }
        sendToServer(client_socket, send_msg);
        bzero(message, sizeof(message));
        send_msg.clear();
        std::cout << "\n";
    }
    pthread_detach(pthread_self());
}

void ClientService::closeConnection()
{
    close(client_socket);
}

void ClientService::sendToServer(int sock, std::string send_msg)
{
    send(sock, send_msg.c_str(), strlen(send_msg.c_str()), 0);
    memset(message, 0, sizeof(message));
}

std::string ClientService::receiveFromServer(int sock)
{
    bzero(message, sizeof(message));
    recv(sock, message, 4096, 0);
    return std::string(message);
}

void ClientService::setClientName(std::string o_client_name)
{
    client_name = o_client_name;
}

void ClientService::setClientPassword(std::string password)
{
    client_password = password;
}

std::string ClientService::getClientName()
{
    return client_name;
}

std::string ClientService::getClientPassword()
{
    return client_password;
}

bool ClientService::loginClient(int socket, string client_name, string password)
{
    std::string send_to_server;
    bool login_status = false;
    send_to_server = "LOGIN>=" + client_name + ">=" + password;
    sendToServer(socket, send_to_server);
    string server_response = receiveFromServer(socket);
    if (server_response.find("SUCCESS") != std::string::npos)
    {
        login_status = true;
    }
    else if (server_response.find("USERONLINE") != std::string::npos)
    {
        cout << "\n"
             << "\033[31;1m " << client_name << " already logged in.\033[0m" << endl;
    }
    return login_status;
}

void *ClientService::recv_message(void *my_sock)
{
    int sock = *((int *)my_sock);
    int len;
    std::string received_message;
    // client thread always ready to receive message
    while ((received_message = ClientService::receiveFromServer(sock)).size() > 0)
    {
        std::cout << received_message << std::endl;
        memset(message, 0, sizeof(message));
    }
    return NULL;
}
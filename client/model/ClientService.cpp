#include "ClientService.h"

int ClientService::createConnection()
{
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    ServerIp.sin_port = htons(47896);
    ServerIp.sin_family = AF_INET;
    ServerIp.sin_addr.s_addr = INADDR_ANY;
    socket_value = connect(client_socket, (struct sockaddr *)&ServerIp, sizeof(ServerIp));
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


void ClientService::chatroomMessage()
{
    pthread_t recvt;
    ClientService service;
    std::string send_msg;
    std::string group_chat = CHATROOM + delimiter;
    int flag = 0;
    int pos = 0;
    pthread_create(&recvt, NULL, message_helper, &service);
    memset(message, 0, sizeof(message));
    while (fgets(message, sizeof(message), stdin) > 0)
    {
        if (flag == 0)
        {
            send_msg = CHATROOM + delimiter + client_name + delimiter + "joined the chat\n";
            flag = 1;
        }
        else
        {
            send_msg = group_chat + client_name + ":" + delimiter + std::string(message);
        }

        if (send_msg.find(EXIT) != std::string::npos)
        {
            send_msg.clear();
            send_msg = EXIT + delimiter + client_name + delimiter + " has left the chat";
            sendToServer(client_socket, send_msg);
            break;
        }
        sendToServer(client_socket, send_msg);
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
    recv(sock, message, 500, 0);
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
    send_to_server = "LOGIN>=" + client_name + ">=" + password;
    sendToServer(socket, send_to_server);
    string server_response = receiveFromServer(socket);
    if (server_response.find("SUCCESS") != std::string::npos)
    {
        return true;
    }
    return false;
}

void *ClientService::recv_message(void *my_sock)
{
    int sock = *((int *)my_sock);
    int len;
    std::string st_len;
    // client thread always ready to receive message
    while ((st_len = ClientService::receiveFromServer(sock)).size() > 0)
    {
        std::cout << st_len << std::endl;
        memset(message, 0, sizeof(message));
    }
    return NULL;
}
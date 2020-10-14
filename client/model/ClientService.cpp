#include "ClientService.h"

int ClientService::createConnection()
{
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    ServerIp.sin_port = htons(PORT);
    ServerIp.sin_family = AF_INET;
    ServerIp.sin_addr.s_addr = INADDR_ANY;
    socket_value = connect(client_socket, (struct sockaddr *)&ServerIp, sizeof(ServerIp));
    return socket_value;
}

bool ClientService::getConnectionStatus()
{
    int connection_status_value = createConnection();
    if (connection_status_value >= SOCKET_CREATED)
        return true;
    return false;
}

void ClientService::createThread()
{
    pthread_create(&receiving_thread, NULL, &recv_message, &client_socket);
}

void ClientService::getClientMessage(std::string client_message)
{

    strcpy(send_msg, strdup(client_name.c_str()));
    strcat(send_msg, ":");
    strcat(send_msg, strdup(client_message.c_str()));

    length = send(client_socket, send_msg, sizeof(send_msg), MSG_OOB);
    if (length < MIN_LENTGTH)
        printf("\n message not sent \n");
}

void ClientService::closeConnection()
{
    close(client_socket);
}

void ClientService::setClientName(std::string o_client_name)
{
    client_name = o_client_name;
}

void *recv_message(void *my_sock)
{
    int sock = *((int *)my_sock);
    int len;

    while ((len = recv(sock, message, 100, 0)) > 0)
    {
        message[len] = '\0';
        std::cout << "\033[0;34m" << std::string(message) << "\033[0m" << std::endl;
        memset(message, 0, sizeof(message));
    }

    return NULL;
}
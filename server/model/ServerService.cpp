#include "ServerService.h"

int ServerService::createConnection()
{
    system("clear");
    ServerIp.sin_family = AF_INET;
    ServerIp.sin_port = htons(35789);
    ServerIp.sin_addr.s_addr = htonl(INADDR_ANY);
    sock = socket(AF_INET, SOCK_STREAM, 0);
    int value;
    int opt = 1;
    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0)
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    if (setsockopt(sock, SOL_SOCKET, SO_REUSEPORT, (char *)&opt, sizeof(opt)) < 0)
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    if ((value = bind(sock, (struct sockaddr *)&ServerIp, sizeof(ServerIp))) == -1)
        printf("[-]Bind unsuccessful, error!! %d \n", value);
    else
        printf("[+]Server Started\n");
    if (listen(sock, 2) == -1)
        printf("[-]Listening failed \n");
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
    while (1)
    {
        ServerService service;
        cout << "In accept method" << endl;
        if ((client_sock = accept(sock, (struct sockaddr *)NULL, NULL)) < 0)
            printf("[-]Accept failed  \n");
        cout << "This is the client socket : " << client_sock << endl;
        pthread_mutex_lock(&mutex);
        clients[client_array_size] = client_sock;
        client_array_size++;
        //creating a thread for each client
        pthread_create(&recvt, NULL, threadReferenceHelper, &service);
        pthread_mutex_unlock(&mutex);
    }
}
void ServerService::exitClientMethod(int sock, vector<string> message)
{
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
            addOnlineClient(client_response[1], sock);
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

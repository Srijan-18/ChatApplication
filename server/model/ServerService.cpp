#include "ServerService.h"

int ServerService::createConnection()
{
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
}

bool ServerService::getConnectionStatus()
{
    int connection_status_value = createConnection();
    if (connection_status_value >= SOCKET_CREATED)
        return true;
    return false;
}
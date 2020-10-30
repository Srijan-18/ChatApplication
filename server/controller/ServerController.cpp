#include "ServerController.h"

using namespace std;

void ServerController::startServer()
{
    if(server_service.getConnectionStatus() == true)
    {
        cout << "Connection succesfull" << endl;
    }
    else{
        cout << "Connection failed" << endl;
    }
    server_service.acceptIncomingClients();
}
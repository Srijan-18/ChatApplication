#include "server_controller.h"

using namespace std;

void ServerController::printWelcomeMessage()
{
    server_view.printWelcomeMessage();
}

void ServerController::startServer()
{
    server_service.createConnection();
    if(server_service.getConnectionStatus() == true)
    {
        cout << "Connection succesfull" << endl;
    }
    else{
        cout << "Connection failed" << endl;
    }
    server_service.acceptIncomingClients();
}
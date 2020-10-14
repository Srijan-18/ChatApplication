#include "../model/ClientService.h"
#include "../view/clientView.h"

class ClientController
{
    ClientService client_service;
    ClientView client_view;
    std::string client_message_view;

public:
    void startChat()
    {
        if (client_service.getConnectionStatus() == true)
        {
            std::cout << "Connection succesful" << std::endl;
            client_service.setClientName(client_view.getusername());
            client_service.createThread();
            while (true)
            {
                client_view.setMessage();
                client_message_view = client_view.getMessage();
                client_service.getClientMessage(client_message_view);
            }
        }
        client_service.closeConnection();
    }
};

int main()
{
    system("clear");
    ClientController client_contoller;
    client_contoller.startChat();

    return 0;
}
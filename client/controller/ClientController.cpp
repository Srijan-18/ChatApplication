#include "../model/ClientService.h"
#include "../view/clientView.h"

class ClientController
{
    ClientService client_service;
    ClientView client_view;
    std::string client_message_view;

    enum Input_Constants
    {
        TO_CHATROOM = 1,
        QUIT = 9
    };

public:
    void startChat()
    {
        bool end_key = true;
        client_view.printWelcomeMessage();
        client_service.setClientName(client_view.getInputFor("User ID"));
        if (client_service.getConnectionStatus() != true)
        {
            exit(0);
        }
        std::string name = client_service.getUserName();
        int socket = client_service.getSocketValue();
        client_service.registerClient(socket, name);
        while (end_key)
        {
            int user_choice = client_view.selectOption();
            switch (user_choice)
            {
            case TO_CHATROOM:
            {
                client_service.chatroomMessage();
                client_service.closeConnection();
            }
            break;
            case QUIT:
                end_key = false;
                break;
            default:
                client_view.displayMessage("\n##  Invalid Input  ##");
                break;
            }
        }
    }
};

int main()
{
    system("clear");
    ClientController client_contoller;
    client_contoller.startChat();

    return 0;
}
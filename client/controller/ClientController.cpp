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
        QUIT = 9,
        TO_REGISTER = 2,
        TO_LOGIN = 1
    };

public:
    void startChat()
    {
        bool end_key = true;
        bool end_key_2 = false;
        client_view.printWelcomeMessage();
        if (client_service.getConnectionStatus() != true)
        {
            exit(0);
        }

        int socket = client_service.getSocketValue();

        std::string name;

        while (!end_key_2)
        {
            int user_choice = client_view.selectAuthOption();
            switch (user_choice)
            {
            case TO_LOGIN:
                client_service.setClientName(client_view.getInputFor("User ID"));
                client_service.setClientPassword(client_view.getInputFor("Password"));
                end_key_2 = client_service.loginClient(socket, client_service.getClientName(), client_service.getClientPassword());

                end_key_2 ? cout << "\n++++++LOGGED IN++++++" << endl
                          : cout << "\nInavlid user name or password" << endl;
                break;
            case TO_REGISTER:
                client_service.setClientName(client_view.getInputFor("User ID"));
                client_service.setClientPassword(client_view.getInputFor("Password"));
                client_service.registerClient(socket, client_service.getClientName(), client_service.getClientPassword());
                break;
            case QUIT:
                client_service.sendToServer(socket, "##EXIT>=");
                exit(0);
                break;

            default:
                client_view.displayMessage("\n##  Invalid Input  ##");
                break;
            }
        }

        while (end_key)
        {
            client_view.printOnlineClients(client_service.getOnlineClients());
            int user_choice_2 = client_view.selectOption();
            switch (user_choice_2)
            {
            case TO_CHATROOM:
            {
                client_service.chatroomMessage();
            }
            break;
            case QUIT:
                end_key = false;
                client_service.closeConnection();
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
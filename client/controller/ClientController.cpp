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
        ONLINE_CLIENTS = 3,
        QUIT = 9,
        TO_REGISTER = 2,
        TO_LOGIN = 1,
        TO_ONE_CLIENT = 2,
        TO_LOGOUT = 8
    };

public:
    void startChat()
    {
        system("clear");
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

                end_key_2 ? client_view.displayMessage("\n\033[32;1mLogin Successful..\033[0m") :
                            client_view.displayMessage("\n\033[31;1mInvalid user name or password\033[0m"); 
                sleep(1);
                break;
            case TO_REGISTER:
                client_service.setClientName(client_view.getInputFor("User ID"));
                client_service.setClientPassword(client_view.getInputFor("Password"));
                client_service.registerClient(socket, client_service.getClientName(), client_service.getClientPassword());
                break;
            case QUIT:
                client_service.sendToServer(socket, "#!EXIT>=");
                exit(0);
                break;

            default:
                client_view.displayMessage("\n\033[31;1m##  Invalid Input  ##\033[0m");
                break;
            }
        }

        while (end_key)
        {   system("clear");
            client_view.displayMessage("\033[32;1mOnline : " + client_service.getClientName() + "\033[0m");
            int user_choice_2 = client_view.selectOption();
            switch (user_choice_2)
            {
                case TO_CHATROOM:
                {
                    client_service.chatroomMessage();
                }
                break;
                case TO_ONE_CLIENT:
                {
                    string other_client = client_view.getInputFor("Other User ID");
                    client_service.oneToOneMessage(other_client);
                }
                break;
                case ONLINE_CLIENTS:
                {
                    client_view.printOnlineClients(client_service.getOnlineClients());
                    cin.get();
                    client_view.displayMessage("\nPress ENTER to continue");
                    cin.get();
                }
                break;
                case QUIT:
                    client_service.sendToServer(socket, "#!EXIT>=");
                    end_key = false;
                    client_service.closeConnection();
                    break;
                case TO_LOGOUT:
                {
                    client_service.sendToServer(socket, "#!EXIT>=");
                    end_key = false;
                    client_service.closeConnection();
                    startChat();
                }
                break;
                default:
                    client_view.displayMessage("\n\033[31;1m##  Invalid Input  ##\033[0m");
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